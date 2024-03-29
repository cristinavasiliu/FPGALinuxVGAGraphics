# FPGALinuxVGAGraphics
## Linux on FPGA (DE1-SoC) + VGA graphics 

![alt text](https://github.com/cristinavasiliu/FPGALinuxVGAGraphics/blob/main/bin_images/everest.jpg)

<h3>Developing Linux programs for the DE1-SoC board:</h3> 

There are two ways you can develop a Linux program for the DE1-SoC board. The first approach is 'native compilation' where you write and compile code using the command-line or GUI interface from Linux. The second approach is called 'cross compilation'. Here you write and compile the program on a host computer (ex: Windows), then transfer the executable file onto the microSD card that contains the Linux file system. See pg 14-15: https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/DE1-SoC-UP-Linux/Linux.pdf<br />

How to run using native compilation: <br />
Run the following command in the Linux terminal: tclsh run_umbrella.tcl <br />
(Make sure that TCL script, shell script, header file (.h) and the c file are in the same directory) <br />

Or: <br />
Compile c file directly:
gcc <file_name>.c -o <file_name> <br />
Then run: 
./<file_name> <br />

 <h3>Linux images:</h3>

 I used two Linux images: <br />
'Linux LXDE Desktop’ image from Terasic: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=836&PartNo=4 <br />
And a Linux console for the DE1-SoC board from Intel: https://fpgacademy.org/courses.html <br />

