# Multithreaded Matrix Multiplication

## Description

This is a simple C program that takes in two arguments, the names of the files containing matrix_1 and matrix_2, and multiplies the two matrices by creating a thread for each row of matrix_1 and distributing the multiplication job among threads, thus increasing CPU utilization.

The matrices need to be contained in files in the following format:

>row_count column_count <br/>
mat[0][0] mat[0][1] mat[0][2] . . . <br/>
mat[1][0] mat[1][1] mat[1][2]  . . .<br/>
mat[2][0] mat[2][1] mat[2][2]  . . . <br>
. <br>
. <br>
. <br>

An example of a 3 by 3 matrix supplied within the program would look like this: 

>3 3 <br>
1 2 3 <br>
4 5 6 <br>
7 8 9 <br>
