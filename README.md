# Linux on FPGA (DE1-SoC) + VGA graphics 

The project is divided into two main components: first, setting up Linux on the FPGA (specifically the DE1-SoC board from Intel), and second, implementing bare-metal VGA graphics using the FPGA.

 <h3>Linux images:</h3>

I used two Linux images for this project: <br />
'Linux LXDE Desktop’ from Terasic: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=836&PartNo=4 <br />
And a Linux console for the DE1-SoC from Intel: https://fpgacademy.org/courses.html <br />

<h3>Developing Linux programs for the DE1-SoC board:</h3> 

There are two methods for developing a Linux program for the DE1-SoC board: native compilation and cross-compilation. Native compilation involves writing and compiling code directly on the DE1-SoC board using the Linux command line or GUI interface. In contrast, cross-compilation allows you to write and compile the program on a host computer (ex: Windows), and then transfer the executable file to the microSD card containing the Linux file system. For more information, refer to pages 14-15:
https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/DE1-SoC-UP-Linux/Linux.pdf<br />

How to run using native compilation: <br />
Run the following command in the Linux terminal: tclsh run_umbrella.tcl <br />
(Make sure that TCL script, shell script, header file (.h) and the c file are in the same directory) <br />

Or: <br />
Compile c file directly:
gcc <file_name>.c -o <file_name> <br />
Then run: 
./<file_name> <br />

 <h3>VGA Graphics:</h3>

The VGA graphics were created by accessing the boards I/O peripherlas via its address range. Various HPS memory componets and Cycolne V FPGA compinents were used to render text, shapes and images on the VGA screen (see project documwntation for full explanation). Below are the demos of each C file. 

1. Item Zed
   1. Nested Numeric # indented 3 spaces
2. Item One

1. coloured_dot.c <br />
Contains the following: a multi-coloured square, rectangle, line, text, a white pixel on the top left corner, a red comet on the top left corner, and a RGB pixel.  <br />

https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/319bb43e-fc0c-4fb2-a903-1d9e1df6c843  

moving_square_screensaver.c  <br />
Multi-coloured moving square screensaver. To improve make it more smooth.  <br />

https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/d483fa64-82bd-411b-b9a2-bed743c0ad0f  <br />

moving_square_test1.c  <br />
No refresh between moving square results in a blue line effect that colours the screen from left to right.  <br />

https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/303aa5d6-fd8d-4288-b5fb-1729e3a1562b  <br />

moving_square_test2.c  <br />
The refresh rate is too high, therefore you can see the square moving. Still doesn't move smoothly.  <br />

https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/c8b48940-a349-4b7a-be9f-981717955849  <br />

moving_square_smooth.c  <br />
Blue square moves smoothly  <br />

https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/40c8a9cb-e271-437e-a049-d4b8017577b5  <br />

umbrella.c (left) and everest_image.c (right) <br />

![demo](https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/assets/102209384/f8f4df8b-80f6-414e-9ed5-08cf150f1d01)  <br />
