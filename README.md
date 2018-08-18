# Maze-Solving
CS211
This is a C program that will find its way through a maze using the depth-first search algorithm. This program takes input from a file where the filename is specified in the command line arguments. The input file will only contain two integer values per line of input:
* The first valid line gives the size of the 2-D maze (the number of rows given by the first
number, then the number of columns given by the second number), valid values are >= 1
* The second valid line gives the coordinates of the starting position in the maze
* The third valid line gives the coordinates of the ending position in the maze
* The remaining valid lines in the file give the coordinates of blocked positions in the maze 

The following shows an example of such an input file. The coordinates are given with the row
listed first and the column listed second. A maze of NxM has rows numbered from 1 to N and
columns number from 1 to M.

10 20

1 1

10 20

5 1

4 2

3 3

1 10

2 9
3 8

4 7

5 6

6 5

7 4

8 3


