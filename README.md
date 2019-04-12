# serial11
Serial utilities for communicating with a pdp-11.

Both build with ANSI C and no dependencies.  They're intended for use on Linux.

These utilities work with octal dump files, which look like

```
; Main Program
001016 005000 ;CLR R0
001020 005002 ;CLR R2
001022 012706 ;MOV #770,R6
001024 000770 
001026 005037 ;CLR PSW
```

The first column is the address (16 bits!) and the second is the contents of that address.  Following pdp-11 convention, addresses are even, and contents are 16 bit words.  Comments begin with semicolons, causing the rest of the line to be ignored.

## serial11.c
This program transfers a human-readable octal file to a running PDP-11.  The program walks you through toggling in a short loader program that keeps the destination address in R2.  It permits different blocks of code to be in different locations.  Each time there is a discontinuity, the program stops and askes you to HALT the processor, adjust R2 and continue.  This is tedious, but keeps the loader code short.

## octdump.c
A very simplistic program to convert a binary file to an octal dump.
