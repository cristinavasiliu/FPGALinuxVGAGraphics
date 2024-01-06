/* This header file was taken and adapted from Cornel. See: https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_peripherials/univ_pgm_computer.index.html */
/* This file provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */

#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_SPAN      0x00040000 //256 KB
//#define FPGA_ONCHIP_END     0xC803FFFF

#define FPGA_CHAR_BASE        0xC9000000 
#define FPGA_CHAR_SPAN        0x00002000  //8 KB 
//#define FPGA_CHAR_END       0xC9001FFF


/* Cyclone V FPGA devices */
#define HW_REGS_BASE          0xff200000
#define HW_REGS_SPAN          0x00005000 //20 KB

// === now offsets from the BASE ===
#define LEDR_BASE             0x00000000
 
