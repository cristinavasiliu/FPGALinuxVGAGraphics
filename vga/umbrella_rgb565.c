/* The following c code was taken and adapted from "DE1-SoC: University Computer
Graphics, audio, IPC Cornell ece5760". See: "One c example" https://people.ece.c
ornell.edu/land/courses/ece5760/DE1_SOC/HPS_peripherials/univ_pgm_computer.index
.html */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm_brl4.h"                        //address map file 

// Declare function prototypes that handle VGA text, drawing filled rectangles, and drawing lines on the VGA monitor

void VGA_text (int, int, char *);
void VGA_box (int, int, int, int, short);
void VGA_line(int, int, int, int, short) ;

// virtual to real address pointers

volatile unsigned int * red_LED_ptr = NULL ;             // virtual - used to access memory-mapped I/O, values may change unexpectidly due to external factors
void *h2p_lw_virtual_base;                               
            
volatile unsigned int * vga_pixel_ptr = NULL ;
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
	
	vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);
    // ========================================================================

	FILE *pic_pointer;
        pic_pointer=fopen("rgb565data.bin", "rb");
        if (!pic_pointer) { 
          printf("Unable to open picture file!");    
	  return 1;
        }
        char pic_buffer[2*32*32];
        //unsigned short pic_buffer;
	int i=0;
        int j=0;
	while (!feof(pic_pointer)) {
          fread(pic_buffer, 2, 2, pic_pointer);
        }
           
        //printf("%s", pic_buffer);d(pic_buffer, 2, 32*32, pic_pointer);
        //}

        printf("%s", *pic_buffer);

        
        /*for(i=0; i<(2*32*32); i++) {
               
		vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base+(i&63)+(i>>6)*1024);
		*vga_pixel_ptr =*(pic_buffer+i);
                printf("%x , %d \n",*pic_buffer, i);

       }*/
       fclose(pic_pointer);
       vga_image(0, 0,pic_buffer,vga_pixel_ptr); 
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

    // just a dot in the upper left corner
	*vga_pixel_ptr = 0xffff ;
    short color;
	while(1) {                                      // enters an infinite loop and repeatedly toggles a LED on and off while drawing coloured boxes on the FPGA
		*(red_LED_ptr) = 0x1;		                // turn on LEDR_0
		VGA_box (100, 100, 200, 200, color++);      // the colour variable in incremented, causing the colour of the boxes to change with each iteration
		*(red_LED_ptr) = 0x0;		                // turn off LEDR_0
		VGA_box (100, 100, 200, 200, color++);		// the colour variable in incremented, causing the colour of the boxes to change with each iteration
	}
	
}

void vga_image(int x, int y, char * pic_buffer, char * vga_pixel_ptr) {
int i;   
for(i=0; i<(2*32*32); i++) {

                vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base+(i&63)+(i
>>6)*1024);
                *vga_pixel_ptr =*strdup(pic_buffer+i);
        //        printf("%x , %d \n",*pic_buffer, i);

       }

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
