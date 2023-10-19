#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm_brl4.h"  //address map file 

#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

// Function prototypes that handle VGA text, drawing filled rectangles, and drawing lines on the VGA monitor

void VGA_text (int, int, char *);
void VGA_box (int, int, int, int, short);

// virtual to real address pointers

volatile unsigned int * red_LED_ptr = NULL ;  // virtual - used to access memory-mapped I/O, values may change unexpectidly due to external factors
void *h2p_lw_virtual_base;  // Holds the virtual base address of a memory region

volatile unsigned int * vga_pixel_ptr = NULL ;
void *vga_pixel_virtual_base;

volatile unsigned int * vga_char_ptr = NULL ;
void *vga_char_virtual_base;

int fd;  // file descriptor 

int x1, y1, x2, y2;

int main(void)  {

	// === get FPGA addresses ==================
	
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )  {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return(1);
	}
    
    // get virtual addr that maps to physical
	// mmap maps the physical memory region defined by HW_REGS_BASE to a virtual memory space, allows the program to directly access the FPGA's memory-mapped components
	
	h2p_lw_virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );	
	if( h2p_lw_virtual_base == MAP_FAILED )  {
		printf( "ERROR: mmap1() failed...\n" );
		close(fd);
		return(1);
	}
    
    // Get the address that maps to the FPGA LED control 
	// Initializes red_LED_ptr to point to a specific memory location relative to the h2p_lw_virtual_base
	
	red_LED_ptr =(unsigned int *)(h2p_lw_virtual_base + LEDR_BASE);

	// === get VGA char addr =====================
	// get virtual addr that maps to physical
	
	vga_char_virtual_base = mmap( NULL, FPGA_CHAR_SPAN, (PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_CHAR_BASE );	
	if( vga_char_virtual_base == MAP_FAILED )  {
		printf( "ERROR: mmap2() failed...\n" );
		close(fd);
		return(1);
	}
    
    // Get the address that maps to the FPGA LED control 
	
	vga_char_ptr = (unsigned int *)(vga_char_virtual_base);

	// === get VGA pixel addr ====================
	// get virtual addr that maps to physical
	
	vga_pixel_virtual_base = mmap( NULL, FPGA_ONCHIP_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_ONCHIP_BASE);	
	if( vga_pixel_virtual_base == MAP_FAILED )  {
		printf( "ERROR: mmap3() failed...\n" );
		close(fd);
		return(1);
	}
    
    // Get the address that maps to the FPGA LED control 
	
	vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);

	// ===========================================

	// create a message to be displayed on the VGA and LCD displays 
	
	char text_top_row[40] = "Cristina\0";
	char text_bottom_row[40] = "2023\0";	

	VGA_text (34, 29, text_top_row);
	VGA_text (34, 30, text_bottom_row);
	// clear the screen
	VGA_box (0, 0, 319, 239, 0);
	// x1 y1 x2 y2	320x240					
	VGA_box (33*4, 28*4, 49*4, 32*4, 0x187F);
	VGA_box (100, 210, 300, 220, 0xffe0);

    /*
	// just a dot in the upper left corner
	*vga_pixel_ptr = 0xffff ;
	short color ;
	while(1) {
		*(red_LED_ptr) = 0x1;		// turn on LEDR_0
		VGA_box (100, 100, 200, 200, color++);
		*(red_LED_ptr) = 0x0;		// turn off LEDR_0
		VGA_box (100, 100, 200, 200, color++);					
	}  */
	
	short color ;
	while(1)  {
		*(red_LED_ptr) = 0x1;		// turn on LEDR_0
		x1 = (int)((rand() & 0x1ff));  //generates random values for the coordinates x1, y1, x2, y2
		y1 = (int)((rand() & 0xff));  //bitwise AND operations with masks ensure the generated values are within specific ranges
		x2 = (int)((rand() & 0x1ff));  //0-511 for x1 and x2 and 0-255 for y1 and y2
		y2 = (int)((rand() & 0xff)); 
		if (x1>319) x1 = 319; //checks if the generated values for x1, y1, x3 and y2 exceed the maximum coordinates
		if (y1>239) y1 = 239; //that the VGA display supports (319 for x and 238 for y)
		if (x2>319) x2 = 319; //if any of the values exceed these limits, they are capped to the maximum values
		if (y2>319) y2 = 239;
		if (x1>x2) SWAP(x1,x2); //ensures that the coordinates x1 and x2 are in increasing order and same with the y's
		if (y1>y2) SWAP(y1,y2); //This is done using the 'swap' macro, which swaps the values of its arguments if the condition inside of the macro is met
		VGA_box (x1,y1,x2,y2, rand() & 0xffff); //random colour value obtianed by performing bitwise & with the mask 0xffff on the output of the rand() fct. 
		//This draws a filled rectangle on the VGA display with random position, dimentions and colour.
		*(red_LED_ptr) = 0x0;		// turn off LEDR_0							
	}
}

/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor 
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr) {
	int offset;
  	volatile char * character_buffer = (char *) vga_char_ptr ;	// VGA character buffer

	// assume that the text string fits on one line 
	offset = (y << 7) + x;
	while ( *(text_ptr) ) {
		// write to the character buffer
		*(character_buffer + offset) = *(text_ptr);	
		++text_ptr;
		++offset;
	}
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)  {
	int *pixel_ptr, row, col;

	// assume that the box coordinates are valid 
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)  {
			pixel_ptr = (char *)vga_pixel_ptr + (row << 10) + (col << 1); 
			// set pixel color
			*(short *)pixel_ptr = pixel_color;		
		}
}

