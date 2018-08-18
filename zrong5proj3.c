#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
int DebugMode;

typedef struct maze {
	char **arr;
	int xsize, ysize;
	int xstart, ystart;
	int xend, yend;
} maze;

typedef struct node {
	struct node *next;
	int x, y;
}node;

//initializes the stack of linked lists
void init_stack(struct node **a, int xstart, int ystart) {
	*a = (struct node*)malloc(sizeof(struct node));
	(*a)->x = xstart;
	(*a)->y = ystart;
	(*a)->next = NULL;
}

//pushes nodes onto the stack
void push(struct node **a, int x, int y) {
	//create a new node and set data members
	struct node *new_node = (struct node*)malloc(sizeof(struct node));
	new_node->x = x;
	new_node->y = y;
	new_node->next = NULL;

	//base case
	if (*a == NULL) {
		*a = new_node;
		return;
	}
	new_node->next = *a;
	*a = new_node;
	//debug mode message
	if(DebugMode == TRUE){
		printf("Coordinates being pushed: [%d,%d]\n", x, y);
	}
}

//pops the head of the list
void pop(struct node **a) {
	struct node *last = *a;
	//debug mode message
	if(DebugMode == TRUE){
		printf("Coordinates being popped: [%d,%d]\n", (*a)->x, (*a)->y);
	}
	//base case
	if (*a == NULL) {
		printf("Error popping stack, stack is empty\n");
		return;
	}
	else if((*a)->next == NULL){
		free(last);
		*a = NULL;
	}
	else{
		*a = (*a)->next;
		free(last);
	}
}

//prints the linked list in reverse
void print_rev(struct node *a){
    if (a == NULL) return;
    // print the list after head node
    print_rev(a->next);
    // After everything else is printed, print head
    printf("[%d, %d]\n", a->x, a->y);
}

//this function returns true or false depending if the stack is empty
int is_empty(struct node *a) {
	if (a == NULL) return TRUE;
	else return FALSE;
}

//this function access the top of the array
void get_top(struct node *a, int *x, int *y) {
	*x = a->x;
	*y = a->y;
}

//clears the stack for use again
void clear_stack(struct node **a){
	struct node *curr = *a;
	struct node *next;

	//iterate through the stack and free memory
	while(curr != NULL){
		next = curr -> next;
		free(curr);
		curr = next;
	}
	*a = NULL;
}

//clears the array for use again
void clear_arr(char ***arr, int x){
	int i = 0;
	//iterate through the array and free memory
	for(i; i < x; i++){
		free((*arr)[i]);
	}
	free((*arr));
}

int main(int argc, char **argv) {
	struct maze m1;
	struct node *stack = NULL;
	int xpos, ypos;
	int i, j;
	int curr_x, curr_y;
	DebugMode = FALSE;
	FILE *src;

	//error checking for command line arguments
	//if only 1 argument, return error and exit
	if(argc < 2){
		printf("Usage: %s <input file name>\n", argv[0]);
    exit(-1);
  }
	//if more than 3 arguments, return error and exit
	else if(argc > 3){
		printf("Invalid inputs: > 3 exiting..");
		exit(-1);
	}
	//determine if debug mode is turned on
	for(i = 0 ; i < argc ; i++ ){
    if(strcmp(argv[i], "-d") == 0){
			DebugMode = TRUE;
			break;
		}
  }

	//open maze.txt depending on position of d flag
	if(i == 1) src = fopen(argv[2], "r");
	else src = fopen(argv[1], "r");

	// read in the size of the maze and if x or y coordinates
	//are < 0, ignore and read next line
	while (fscanf(src, "%d %d", &m1.xsize, &m1.ysize) != EOF) {
		if (m1.xsize > 0 && m1.ysize > 0) {
			break;
		}
		else {
			fprintf(stderr, "Invalid: Maze sizes must be greater than 0\n");
			continue;
		}
		if (fscanf(src, "%d %d", &m1.xsize, &m1.ysize) == EOF) {
			fprintf(stderr, "No size x or y positions are valid in the file\n");
			fprintf(stderr, "Exiting...\n");
			exit(0);
		}
	}

	//read in the starting position in the maze and if x or y
	//coordinates are < 0, ignore and read next line
	while (fscanf(src, "%d %d", &m1.xstart, &m1.ystart) != EOF) {
		if (m1.xstart > 0 && m1.ystart > 0) {
			if (m1.xstart > m1.xsize) {
				fprintf(stderr, "Invalid: row %d is outside range from 1 to %d\n", m1.xstart, m1.xsize);
				continue;
			}
			else if (m1.ystart > m1.ysize) {
				fprintf(stderr, "Invalid: column %d is outside range from 1 to %d\n", m1.ystart, m1.ysize);
				continue;
			}
			else if ((m1.xstart * m1.ystart) <= m1.xsize * m1.ysize) {
				break;
			}
		}
		if (fscanf(src, "%d %d", &m1.xstart, &m1.ystart) == EOF) {
			fprintf(stderr, "No starting x or y positions are valid in the file\n");
			fprintf(stderr, "Exiting...\n");
			exit(0);
		}
	}

	//read in the starting position in the maze and if x or y
	//coordinates are < 0, ignore and read next line//
	while (fscanf(src, "%d %d", &m1.xend, &m1.yend) != EOF) {
		if (m1.xend > 0 && m1.yend > 0) {
			if (m1.xend > m1.xsize) {
				fprintf(stderr, "Invalid: row %d is outside range from 1 to %d\n", m1.xend, m1.xsize);
				continue;
			}
			else if (m1.yend > m1.ysize) {
				fprintf(stderr, "Invalid: column %d is outside range from 1 to %d\n", m1.yend, m1.ysize);
				continue;
			}
			else if ((m1.xend * m1.yend) <= m1.xsize * m1.ysize) {
				break;
			}
		}
		if (fscanf(src, "%d %d", &m1.xend, &m1.yend) == EOF) {
			fprintf(stderr, "No ending x or y positions are valid in the file\n");
			fprintf(stderr, "Exiting...\n");
			exit(0);
		}
	}

	//initialize the array with size defined by the file
	m1.arr = malloc((m1.xsize + 2) * sizeof(char*));
	for (i = 0; i < m1.xsize + 2; i++) {
		m1.arr[i] = malloc((m1.ysize + 2) * sizeof(char*));
	}

	// initialize the maze to empty
	for (i = 0; i < m1.xsize + 2; i++)
		for (j = 0; j < m1.ysize + 2; j++)
			m1.arr[i][j] = '.';

	// mark the borders of the maze with *'s
	for (i = 0; i < m1.xsize + 2; i++) {
		m1.arr[i][0] = '*';
		m1.arr[i][m1.ysize + 1] = '*';
	}
	for (i = 0; i < m1.ysize + 2; i++) {
		m1.arr[0][i] = '*';
		m1.arr[m1.xsize + 1][i] = '*';
	}

	// mark the starting and ending positions in the maze
	m1.arr[m1.xstart][m1.ystart] = 's';
	m1.arr[m1.xend][m1.yend] = 'e';

	// mark the blocked positions in the maze with *'s
	while (fscanf(src, "%d %d", &xpos, &ypos) != EOF) {
		//check if block position is the same as staring
		if (xpos == m1.xstart && ypos == m1.ystart) {
			fprintf(stderr, "Invalid: attempting to block starting position\n");
			continue;
		}
		//check if block position is the same as ending
		else if (xpos == m1.xend && ypos == m1.yend) {
			fprintf(stderr, "Invalid: attempting to block ending position\n");
			continue;
		}
		//check if block position is within the maze size
		else if ((xpos *ypos) <= m1.xsize * m1.ysize)
			m1.arr[xpos][ypos] = '*';
	}

	//print them out to verify the input
	printf("size: %d, %d\n", m1.xsize, m1.ysize);
	printf("start: %d, %d\n", m1.xstart, m1.ystart);
	printf("end: %d, %d\n\n", m1.xend, m1.yend);

	printf("Initial maze: \n");
	// print out the initial maze
	for (i = 0; i < m1.xsize + 2; i++) {
		for (j = 0; j < m1.ysize + 2; j++)
			printf("%c", m1.arr[i][j]);
		printf("\n");
	}
	printf("\n");

	//initialize stack
	init_stack(&stack,m1.xstart, m1.ystart);
	//set current position as starting position
	curr_x = m1.xstart;
	curr_y = m1.ystart;
	m1.arr[m1.xstart][m1.ystart] = 'V';


	while (!is_empty(stack)) {
		//checks if right position is unvisited and not the wall
		if (m1.arr[curr_x][curr_y + 1] != 'V' && m1.arr[curr_x][curr_y + 1] != '*') {
			//shift current position up by 1
			curr_y += 1;
			//pushes coordinate onto the stack
			push(&stack, curr_x, curr_y);
			//marks as visited
			m1.arr[curr_x][curr_y] = 'V';
			continue;
		}
		//checks if down position is unvisited and not the wall
		else if (m1.arr[curr_x + 1][curr_y] != 'V' && m1.arr[curr_x + 1][curr_y] != '*') {
			//shift current position right by 1
			curr_x += 1;
			//pushes coordinate onto the stack
			push(&stack, curr_x, curr_y);
			//marks as visited
			m1.arr[curr_x][curr_y] = 'V';
			continue;
		}
		//checks if left position is unvisited and not the wall
		else if (m1.arr[curr_x][curr_y - 1] != 'V' && m1.arr[curr_x][curr_y - 1] != '*') {
			//shift current position down by 1
			curr_y -= 1;
			//pushes coordinate onto the stack
			push(&stack, curr_x, curr_y);
			//marks as visited
			m1.arr[curr_x][curr_y] = 'V';
			continue;
		}
		//checks if up position is unvisited and not the wall
		else if (m1.arr[curr_x - 1][curr_y] != 'V' && m1.arr[curr_x - 1][curr_y] != '*') {
			//shift current position left by 1
			curr_x -= 1;
			//pushes coordinate onto the stack
			push(&stack, curr_x, curr_y);
			//marks as visited
			m1.arr[curr_x][curr_y] = 'V';
			continue;
		}
		//if no position around is valid and stack isn't empty, pop the stack
		if (!is_empty(stack)) {
			pop(&stack);
			//if stack is empty after popping, print message
			if (is_empty(stack)) {
				printf("Maze after trying to solve: \n");
				//prints out the final version of the maze
				for (i = 0; i < m1.xsize + 2; i++) {
					for (j = 0; j < m1.ysize + 2; j++)
						printf("%c", m1.arr[i][j]);
					printf("\n");
				}
				printf("**There is no solution to this maze**\n");
				break;
			}
			//set current x and y position to their previous positions if
			//stack is not empty after pop
			else{
				curr_x = stack->x;
				curr_y = stack->y;
			}
		}
		//if the head of the stack stores the end of the maze, path is found
		if (curr_x == m1.xend && curr_y == m1.yend) {
			printf("Maze after solving: \n");
			//prints out the final version of the maze
			for (i = 0; i < m1.xsize + 2; i++) {
				for (j = 0; j < m1.ysize + 2; j++)
					printf("%c", m1.arr[i][j]);
				printf("\n");
			}
			printf("The answer for this maze is: \n");
			//prints the stack in reverse
			print_rev(stack);
			printf("\n");
			break;
		}
	}
	//clears the stack and the array
	clear_arr(&m1.arr, m1.xsize+2);
	clear_stack(&stack);
	return 0;
}
