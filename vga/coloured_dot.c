/* The following c code was taken and adapted from "DE1-SoC: University ComputerGraphics, audio, IPC Cornell ece5760". See: "One c example" https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_peripherials/univ_pgm_computer.index.html */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm_brl4.h"                        //address map file

// Declare function prototypes that handle VGA text, drawing filled rectangles, and drawing lines on the VGA monitor

void VGA_text (int, int, char *);
void VGA_box (int, int, int, int, short);
void VGA_line(int, int, int, int, short) ;
void VGA_dot(int, int, short);

// virtual to real address pointers

volatile unsigned int * red_LED_ptr = NULL ;             // virtual - used to access memory-mapped I/O, values may change unexpectidly due to external factors
void *h2p_lw_virtual_base;                               
            
volatile unsigned int * vga_pixel_ptr = NULL ;
volatile unsigned int * vga_pixel_ptr_2 = NULL ;
volatile unsigned int * vga_pixel_ptr_3 = NULL ;
volatile unsigned int * vga_pixel_ptr_4 = NULL ;
volatile unsigned int * vga_pixel_ptr_5 = NULL ;
volatile unsigned int * vga_pixel_ptr_6 = NULL ;
volatile unsigned int * vga_pixel_ptr_7 = NULL ;
volatile unsigned int * vga_pixel_ptr_8 = NULL ;
volatile unsigned int * vga_pixel_ptr_9 = NULL ;
volatile unsigned int * vga_pixel_ptr_10 = NULL ;
volatile unsigned int * vga_pixel_ptr_11 = NULL ;
volatile unsigned int * vga_pixel_ptr_12 = NULL ;
volatile unsigned int * vga_pixel_ptr_13 = NULL ;
volatile unsigned int * vga_pixel_ptr_14 = NULL ;
volatile unsigned int * vga_pixel_ptr_15 = NULL ;
volatile unsigned int * vga_pixel_ptr_16 = NULL ;
volatile unsigned int * vga_pixel_ptr_17 = NULL ;
volatile unsigned int * vga_pixel_ptr_18 = NULL ;
volatile unsigned int * vga_pixel_ptr_19 = NULL ;
volatile unsigned int * vga_pixel_ptr_20 = NULL ;
volatile unsigned int * vga_pixel_ptr_21 = NULL ;
volatile unsigned int * vga_pixel_ptr_22 = NULL ;
volatile unsigned int * vga_pixel_ptr_23 = NULL ;
volatile unsigned int * vga_pixel_ptr_24 = NULL ;
volatile unsigned int * vga_pixel_ptr_25 = NULL ;
volatile unsigned int * vga_pixel_ptr_26 = NULL ;
volatile unsigned int * vga_pixel_ptr_27 = NULL ;
volatile unsigned int * vga_pixel_ptr_28 = NULL ;
volatile unsigned int * vga_pixel_ptr_29 = NULL ;
volatile unsigned int * vga_pixel_ptr_30 = NULL ;
volatile unsigned int * vga_pixel_ptr_31 = NULL ;

volatile unsigned int * new_vga_pixel_ptr = NULL ;

void *vga_pixel_virtual_base;

volatile unsigned int * vga_char_ptr = NULL ;
void *vga_char_virtual_base;

int fd;                                                  // file descriptor 

int main(void)  {

	// ======================= get FPGA addresses ==========================
	
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )  {                                                        // attempt to open the /dev/mem device file with read and write access and syncronization
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
    
    /* 
	get virtual addr that maps to physical
	mmap maps the physical memory region defined by HW_REGS_BASE to a virtual memory space accessible throught h2p_lw_virtual_base, allows the program to directly access the FPGA's memory-mapped components
	void * mmap(void *starting_address, size_t length_bytes, int protect_access_types, int flags, int file_descriptor, off_t offset) 
	mmap maps physical memory regions into my program's virtual address space. This allows you to treat these memory locations as if they were regular variables or arrays in your program.
	*/
	
	// ======================= get red LED addr =============================
	
	h2p_lw_virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );	           //null means that kernal can place mapping anywhere it sees fit
	if( h2p_lw_virtual_base == MAP_FAILED )  {                                                                             //mmap returns 0 if successful else MAP_FAILED
		printf( "ERROR: mmap1() failed...\n" );
		close(fd);
		return(1);
	}
	
	red_LED_ptr =(unsigned int *)(h2p_lw_virtual_base + LEDR_BASE);                                                        

	// ======================= get VGA char addr =============================
	
	vga_char_virtual_base = mmap( NULL, FPGA_CHAR_SPAN, (PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_CHAR_BASE );	       
	if( vga_char_virtual_base == MAP_FAILED )  {                                                                             
		printf( "ERROR: mmap1() failed...\n" );
		close(fd);
		return(1);
	}
	
	vga_char_ptr = (unsigned int *)(vga_char_virtual_base);                                                                

	// ========================= get VGA pixel addr ===========================
	
	vga_pixel_virtual_base = mmap( NULL, FPGA_ONCHIP_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_ONCHIP_BASE);  
	if( vga_pixel_virtual_base == MAP_FAILED )  {
		printf( "ERROR: mmap3() failed...\n" );
		close(fd);
		return(1);
	}
	
	new_vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);
    
	//vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base+1024*239+6);                                                 
	vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base+1024*4);    
	vga_pixel_ptr_2 =(unsigned int *)(vga_pixel_virtual_base+1024*4+2);     
	vga_pixel_ptr_3 =(unsigned int *)(vga_pixel_virtual_base+1024*4+4);    
	vga_pixel_ptr_4 =(unsigned int *)(vga_pixel_virtual_base+1024*4+6);    
	vga_pixel_ptr_5 =(unsigned int *)(vga_pixel_virtual_base+1024*4+8);    
	vga_pixel_ptr_6 =(unsigned int *)(vga_pixel_virtual_base+1024*4+10);    
	vga_pixel_ptr_7 =(unsigned int *)(vga_pixel_virtual_base+1024*4+12);    
	vga_pixel_ptr_8 =(unsigned int *)(vga_pixel_virtual_base+1024*4+14);    
	vga_pixel_ptr_9 =(unsigned int *)(vga_pixel_virtual_base+1024*4+16);    
	vga_pixel_ptr_10 =(unsigned int *)(vga_pixel_virtual_base+1024*4+18);    
	vga_pixel_ptr_11 =(unsigned int *)(vga_pixel_virtual_base+1024*4+20);    
	vga_pixel_ptr_12 =(unsigned int *)(vga_pixel_virtual_base+1024*4+22);    
	vga_pixel_ptr_13 =(unsigned int *)(vga_pixel_virtual_base+1024*4+24);    
	vga_pixel_ptr_14 =(unsigned int *)(vga_pixel_virtual_base+1024*4+26);    
	vga_pixel_ptr_15 =(unsigned int *)(vga_pixel_virtual_base+1024*4+28);    
	vga_pixel_ptr_16 =(unsigned int *)(vga_pixel_virtual_base+1024*4+30);  
	vga_pixel_ptr_17 =(unsigned int *)(vga_pixel_virtual_base+1024*4+32);  
	vga_pixel_ptr_18 =(unsigned int *)(vga_pixel_virtual_base+1024*4+34);  
	vga_pixel_ptr_19 =(unsigned int *)(vga_pixel_virtual_base+1024*4+36);  
	vga_pixel_ptr_20 =(unsigned int *)(vga_pixel_virtual_base+1024*4+38);  
	vga_pixel_ptr_21 =(unsigned int *)(vga_pixel_virtual_base+1024*4+40);  
	vga_pixel_ptr_22 =(unsigned int *)(vga_pixel_virtual_base+1024*4+42);  
	vga_pixel_ptr_23 =(unsigned int *)(vga_pixel_virtual_base+1024*4+44);  
	vga_pixel_ptr_24 =(unsigned int *)(vga_pixel_virtual_base+1024*4+46);
	vga_pixel_ptr_25 =(unsigned int *)(vga_pixel_virtual_base+1024*4+48);  
	vga_pixel_ptr_26 =(unsigned int *)(vga_pixel_virtual_base+1024*4+50);  
	vga_pixel_ptr_27 =(unsigned int *)(vga_pixel_virtual_base+1024*4+52);  
	vga_pixel_ptr_28 =(unsigned int *)(vga_pixel_virtual_base+1024*4+54);
	vga_pixel_ptr_29 =(unsigned int *)(vga_pixel_virtual_base+1024*4+56);
	vga_pixel_ptr_30 =(unsigned int *)(vga_pixel_virtual_base+1024*4+58);
	vga_pixel_ptr_31 =(unsigned int *)(vga_pixel_virtual_base+1024*4+60);
	
	// Set vga_pixel_ptr to point to the mapped VGA pixel memory

	// ========================================================================

	// create a message to be displayed on the VGA and LCD displays 
	
	char text_top_row[40] = "Cristina\0";
	char text_bottom_row[40] = "2023\0";	

	VGA_text (34, 29, text_top_row);                
	VGA_text (34, 30, text_bottom_row);             //character coordinates 0..79 for x and 0..59 for y
	VGA_box (0, 0, 319, 239, 0);                    // calls the vga_box function to clear the screen, fills the entire area with the colour specified by 0
	// x1 y1 x2 y2	320x240					
	VGA_box (33*4, 28*4, 49*4, 32*4, 0x187F);       
	VGA_box (100, 210, 300, 220, 0xffe0);           
	VGA_line(10, 20, 100, 50, 0xf000) ;             // calls the vga_line function to draw a line. (10,20) is the starting point and (100,50) is the ending point 

	
	*vga_pixel_ptr = 0x0001 << 11;                  // sets the value at the memory location pointed to by vga_pixel_ptr to the colour
	*vga_pixel_ptr_2 = 0x0002 << 11;  
	*vga_pixel_ptr_3 = 0x0003 << 11;   
	*vga_pixel_ptr_4 = 0x0004 << 11;  
	*vga_pixel_ptr_5 = 0x0005 << 11;   
	*vga_pixel_ptr_6 = 0x0006 << 11;  
	*vga_pixel_ptr_7 = 0x0007 << 11;   
	*vga_pixel_ptr_8 = 0x0008 << 11;  
	*vga_pixel_ptr_9 = 0x0009 << 11;  
	*vga_pixel_ptr_10 = 0x000a << 11;  
	*vga_pixel_ptr_11 = 0x000b << 11;  
	*vga_pixel_ptr_12 = 0x000c << 11;  
	*vga_pixel_ptr_13 = 0x000d << 11;  
	*vga_pixel_ptr_14 = 0x000e << 11;  
	*vga_pixel_ptr_15 = 0x000f << 11; 
	*vga_pixel_ptr_16 = 0x0010 << 11; 
	*vga_pixel_ptr_17 = 0x0011 << 11; 
	*vga_pixel_ptr_18 = 0x0012 << 11; 
	*vga_pixel_ptr_19 = 0x0013 << 11; 
	*vga_pixel_ptr_20 = 0x0014 << 11; 
	*vga_pixel_ptr_21 = 0x0015 << 11; 
	*vga_pixel_ptr_22 = 0x0016 << 11; 
	*vga_pixel_ptr_23 = 0x0017 << 11;
	*vga_pixel_ptr_24 = 0x0018 << 11;
	*vga_pixel_ptr_25 = 0x0019 << 11;
	*vga_pixel_ptr_26 = 0x001a << 11;
	*vga_pixel_ptr_27 = 0x001b << 11;
	*vga_pixel_ptr_28 = 0x001c << 11;
	*vga_pixel_ptr_29 = 0x001d << 11;
	*vga_pixel_ptr_30 = 0x001e << 11;
	*vga_pixel_ptr_31 = 0x001f << 11;
	
    short color, cnt=0 ;
	while(1) {                                      // enters an infinite loop and repeatedly toggles a LED on and off while drawing coloured boxes on the FPGA
		*(red_LED_ptr) = 0x1;		                // turn on LEDR_0
		VGA_box (100, 100, 200, 200, color++);      // the colour variable in incremented, causing the colour of the boxes to change with each iteration
		*(red_LED_ptr) = 0x0;		                // turn off LEDR_0
		VGA_box (100, 100, 200, 200, color++);		// the colour variable in incremented, causing the colour of the boxes to change with each iteration
		if(cnt < 1024) {
		   cnt++;
		}
		else {
		   cnt=0;
		}
        
        if(cnt < 1024/3) { 
		   VGA_dot(7, 20, 0xf800);
        }		
		else if(cnt < 1024*2/3) {
		   VGA_dot(7, 20, 0x07e0);
		}
		else {
		   VGA_dot(7, 20, 0x001f);
		}
	}
	
}

/****************************************************************************************
 * Draw a dot on the VGA monitor 
****************************************************************************************/

void VGA_dot(int x, int y, short pixel_colour) {
    int *pixel_ptr;
	int offset = (1024 * y) + (x * 2);
	pixel_ptr = (int *)(new_vga_pixel_ptr + offset);
	*(short *)pixel_ptr = pixel_colour;
}

/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor 
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr) {                        //takes x and y as input (coordinates) and a string 
	int offset;                                                       // 1024/8=7 for next line                                              
  	volatile char * character_buffer = (char *) vga_char_ptr ;	      
	
	// assume that the text string fits on one line 
	offset = (y << 7) + x;                                            //left shift by 7 is equivalent to to multiplying by 128
	while ( *(text_ptr) ) {                                           //while loop continues as long as the value pointed by text_ptr (current character in string) is not null (end of string)
	    *(character_buffer + offset) = *(text_ptr);		                                        
		++text_ptr;                                                   //increments text_ptr to move to the next character in the string (increments BEFORE)
		++offset;                                                     //increments offset value to move to the next position in the buffer 
	}
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)  {           // x1, y1 = top left corner, x2, y2 = bottom right corner + colour of box
	int *pixel_ptr, row, col;                                                

	// assume that the box coordinates are valid 
	for (row = y1; row <= y2; row++)                                         // nested loop to iterate over each row and column within the box, will draw pixels
		for (col = x1; col <= x2; ++col)  {
			pixel_ptr = (char *)vga_pixel_ptr + (row << 10) + (col << 1);    // << 10 is equivalent to multiplying row with 1024 and the << 1 is equivalent to multiplying the col with 2
																			 
			// set pixel color
			*(short *)pixel_ptr = pixel_color;		                                                                 
		}
}

// =============================================
// === Draw a line
// =============================================

//plot a line 
//at x1,y1 to x2,y2 with color 
//Code is from David Rodgers,
//"Procedural Elements of Computer Graphics",1985

void VGA_line(int x1, int y1, int x2, int y2, short c) {  // x1, y1 are the coordinates of the starting point of the line and x2, y2 of the ending point, c is colour 	
	int e;                                                // keeps track of error while drawing the line
	signed int dx,dy,j, temp;                             // dx, dy represent the differences in x and y coordinates  
	signed int s1,s2, xchange;                            // s1, s2 are used to determine the sign of differences, xchange is a flag indicating id x and y coord need to be exchanged
    signed int x,y;                                       // current position while drawing the line 
	int *pixel_ptr ;                                      // pointer used to access the VGA pixel memory
        
	x = x1;
	y = y1;
	
	//take absolute value
	if (x2 < x1) {
		dx = x1 - x2;
		s1 = -1;
	}

	else if (x2 == x1) {
		dx = 0;
		s1 = 0;
	}

	else {
		dx = x2 - x1;
		s1 = 1;
	}

	if (y2 < y1) {
		dy = y1 - y2;
		s2 = -1;
	}

	else if (y2 == y1) {
		dy = 0;
		s2 = 0;
	}

	else {
		dy = y2 - y1;
		s2 = 1;
	}

	xchange = 0;   

	if (dy>dx) {
		temp = dx;
		dx = dy;
		dy = temp;
		xchange = 1;
	} 

	e = ((int)dy<<1) - dx;  
	 
	for (j=0; j<=dx; j++) {
		//video_pt(x,y,c);
		pixel_ptr = (char *)vga_pixel_ptr + (y<<10)+ (x<< 1); 
		// set pixel color
		*(short *)pixel_ptr = c;	
		 
		if (e>=0) {
			if (xchange==1) x = x + s1;
			else y = y + s2;
			e = e - ((int)dx<<1);
		}

		if (xchange==1) y = y + s2;
		else x = x + s1;

		e = e + ((int)dy<<1);
	}
}
