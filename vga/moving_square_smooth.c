/* The following c code was taken and adapted from "DE1-SoC: University Computer
Graphics, audio, IPC Cornell ece5760". See: "One c example" https://people.ece.c
ornell.edu/land/courses/ece5760/DE1_SOC/HPS_peripherials/univ_pgm_computer.index
.html */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm_brl4.h"  

#define NR_COLUMNS 320
#define NR_LINES 240                  

void VGA_box (int, int, int, int, short);
void VGA_bounce();

volatile unsigned int * vga_pixel_ptr = NULL ;
void *vga_pixel_virtual_base;

int fd;                                                 

int main(void)  {

	// ======================= get FPGA addresses ==========================
	
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )  {                                                        
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
                                                                                                                     
	// ========================= get VGA pixel addr ===========================
	
	vga_pixel_virtual_base = mmap( NULL, FPGA_ONCHIP_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_ONCHIP_BASE);  
	if( vga_pixel_virtual_base == MAP_FAILED )  {
		printf( "ERROR: mmap3() failed...\n" );
		close(fd);
		return(1);
	}
                                                   
	vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);    

	// ========================================================================

	VGA_bounce();	
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)  {           
	int *pixel_ptr, row, col;                                                

	// assume that the box coordinates are valid 
	for (row = y1; row <= y2; row++)                                      
		for (col = x1; col <= x2; col++)  {
			pixel_ptr = (char *)vga_pixel_ptr + (row << 10) + (col << 1);   
																			 
			// set pixel color
			*(short *)pixel_ptr = pixel_color;		                                                                 
		}
}

/****************************************************************************************
 * Clear the screen
****************************************************************************************/

void screen_clear() {
    VGA_box (0, 0, 319, 239, 0);  
}

/****************************************************************************************
 * Function that moves rectangle on VGA screen 
****************************************************************************************/

void VGA_bounce() {
    int x1 ;
    int y1 ;
    int colour  = 0xfde0;    // orange
	int colour1 = 0xf800;    // full red
    int i, j;
  
	screen_clear();
	while(1) {
	    y1=0;
	    for(i=0; i<=6; i++) {
	      if(i%2 == 0) {                                           // from left to right
		      x1=0;
			  //y1=0;
			  for(j=0; j<=250; j++) {
			     if (j==0) {
				   VGA_box(x1, y1, x1+40, y1+40, 0x001f);
				 }
				 else {
				   
				   VGA_box(x1+40, y1, x1+40, y1+40, 0x001f);       // right part of square
				   VGA_box(x1, y1, x1, y1+40, 0);                  // left part of square
				   x1=x1+1;
				 }
				 usleep(40000); //or 1000000/25
			  }
		  }
		  else {                                                  // from right to left 
		      x1=260;
			  //y unchanged
			  for(j=0; j<=25; j++) {
			  	 VGA_box(x1, y1, x1+40, y1+40, colour1=colour1+2);
			     x1=x1-10;
			     y1=y1+1;
			     usleep(250000);
			     screen_clear();
			  }
		  }
	    }
	}


}
