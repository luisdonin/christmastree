# christmastree
Christmas tree in C with NCurses, written for a class assignment for my Computer Engineering class at UTFPR - Brazil;
The code is written in C and uses the NCurses library for the graphics.
It draws a christmas tree in the center of the screen that increases in size when the user presses the up key and increases in size when the user presses the down key. 
The tree changes in color when it reaches a set height, giving the blinking effect of the christmas lights.

---- Instructions for compiling ---- 
gcc -o main main.c -ln curses

compile in full screen otherwise it will glitch due to the x and y axis, the positions are calculated using a set value for full screen.
