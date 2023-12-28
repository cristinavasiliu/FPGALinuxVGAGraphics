"""
This python script requires the opencv-python module (cv2) to run.
To install this module on OSX/Linux systems, type the following in a
terminal window: pip install opencv-python

2019/06/01 Written. (Arthur Hlaing)
2019/06/03 Removed need for math module, added command line arguments. (AH)
"""

import cv2
import numpy as np
import sys
import struct
import os

if len(sys.argv) != 5:
    print("Error: required 4 arguments but provided %s arguments." % str(len(sys.argv) - 1))
    print("Usage: python img2v.py OUT_MODE SIZE_X SIZE_Y IMAGE_FILENAME > OUT_FILENAME.v" )
    print("       OUT_MODE=0      For use in a case statement" )
    print("       OUT_MODE=1      For use in an initial block" )
    print("       SIZE_X, SIZE_Y  Image will be scaled to this size in pixels" )
    print("Example: python img2v.py 0 16 16 mario.jpg > rom.v" )
    quit()    
#argument 1=OUT_MODE, 2=SIZE_X, 3=SIZE_Y, 4=IMAGE_FILENAME
IMAGE_FILENAME = sys.argv[4] # input image file name
IMAGE_SHAPE = (int(sys.argv[2]),int(sys.argv[3]))
MODE = int(sys.argv[1])

def dec2bin(x, length):
    # remove "0b" from string
    tmp = bin(x)[2:]
    # append leading 0s if four binary bits are not present
    for i in range(0, length - len(tmp)):
        tmp = "0" + tmp
    return tmp
img = cv2.imread(IMAGE_FILENAME)

#Image shape 0 = height (e.g. 67 pixles), image shape 1 = width (e.g. 100 pixels)
#print("img shape 0 = %d shape 1 = %d", img.shape[0], img.shape[1])

if img is None:
    print("Error: %s cannot be read." % (IMAGE_FILENAME))
    quit()

# crop image to square
# if height is greater than width, crop height same length as width
if img.shape[0] > img.shape[1]:

    midpoint = int(img.shape[0]/2)
    left = midpoint-int(img.shape[1]/2)
    right = midpoint+int(img.shape[1]/2)
    img = img[left:right,:,:]
    
# if width is greater than height, make width same length as height
elif img.shape[1] > img.shape[0]:
    midpoint = int(img.shape[1]/2)
    left = midpoint-int(img.shape[0]/2)
    right = midpoint+int(img.shape[0]/2)
    img = img[:,left:right,:]

# resize image to e.g. 16x16
img = cv2.resize(img, IMAGE_SHAPE)


# create output (binary) file name
infile_base = os.path.splitext(os.path.basename(IMAGE_FILENAME))[0]
bin_file_base = infile_base + "_" + str(IMAGE_SHAPE[0]) + "x" + str(IMAGE_SHAPE[1]) + ".bin"
bin_file_path = "/root/FPGALinuxVGAGraphics/vga/" + bin_file_base

addr = 0
for h in range(0, img.shape[0]):
    for w in range(0, img.shape[1]):

        # convert to RGB 565
        red = int(img[h,w,2]/8) # 5bit, 2^5=32 levels => 256/32=8 (division factor)
        grn = int(img[h,w,1]/4) # 6bit, 2^6=64 levels => 256/64=4 (division factor)
        blu = int(img[h,w,0]/8)

        pix = (red<<(6+5)) | (grn<<5) | (blu)

       #print("img pixel = %x ", pix)

        if (addr==0): 
          f = open(bin_file_path, "wb")    #create binary file
        binary_data = struct.pack('H',pix) #pack integer into 2 byte unsigned short integer
        f.write(binary_data)               #write pixel into binary file

        addr += 1


f.close()
