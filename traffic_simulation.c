//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define SPACE 0
#define WALL -1
#define EXIT -2

/***********************************************************/
/***********************************************************/
/******* ADD THE REQUIRED FUNCTIONS BELOW THIS POINT *******/
/***********************************************************/
/***********************************************************/

// Your function definitions should go here...
void InitialiseRoad(int road[NUM_ROWS][NUM_COLS], char side, int pos)
{
	int i, j;
	
	//use for loop to add 'walls' and spaces' to the array in the proper areas
	//i.e walls around the outside + space in the middle
	for (i=0; i < NUM_ROWS; i++){
		for (j=0; j < NUM_COLS; j++){
			if ((i == 0) || (i == NUM_ROWS -1) ||( j==0) || (j == NUM_COLS -1)){
				road[i][j] = WALL;
			}
			if (((i > 0) && (i< NUM_ROWS -1)) && ((j > 0) && (j < NUM_COLS -1))){
				road[i][j] = SPACE;
			}
		}
	}

	//use if statements to check what side is and add 'exit' into the array
	//in the correct position and wall side
	if (side == 'N'){
		road[0][pos] = EXIT;
	} else if (side == 'E'){
		road[pos][NUM_COLS-1] = EXIT;
	} else if (side == 'W'){
		road[pos][0] = EXIT;
	} else{
		road[NUM_ROWS-1][pos] = EXIT;
	}

}

void PrintRoad(int road[NUM_ROWS][NUM_COLS])
{
	int i, j;

	//use for loop and if statements to print out the road from array
	for (i=0; i < NUM_ROWS; i++){
		for (j=0; j < NUM_COLS; j++){
			if (road[i][j] == SPACE){ //if 0 print ' '
				printf(" ");
			} else if (road[i][j] == WALL){ //if -1 print '#'
				printf("#");
			} else if (road[i][j] == EXIT){ //if -2 print 'O'
				printf("O");
			} else {
				printf("%c", road[i][j] + 64); //print car using alphabet
			}
		}
		//if (i == NUM_ROWS-1){

		//} else {
			printf("\n");
		//}
		
	}
}

double PercentUsed(int road[NUM_ROWS][NUM_COLS])
{
	int i, j;
	double full = 0;
	double empty = 0;
	double percent;

	//use for loops and if statements to see how many spaces vs cars there are
	for (i=1; i < NUM_ROWS-1; i++){
		for (j=1; j < NUM_COLS-1; j++){
			if (road[i][j] == SPACE){
				empty++;
			} else {
				full++;
			}
		}
	}

	//calculate the percentage of the array that is empty/full
	percent = full/(empty+full) * 100;
	return percent;

}

void AddCar(int road[NUM_ROWS][NUM_COLS], int row, int col, int size)
{
	int i, j, alphabet, k;
	int count = 1;
	int check = 0;
	int collide = 0;
	int temp[27] = {0};

	//use for loops and if statments to check if any cars had been added already
	for (i=1; i < NUM_ROWS-1; i++){
		for (j=1; j < NUM_COLS-1; j++){
			for (alphabet = 1; alphabet < 27; alphabet++){
				if (road[i][j] == alphabet){ //check if road is equal to 1-26 i.e a car
					for (k = 0; k < 27; k++){
						// checks an array to see if the car value has been stored already or not
						if (road[i][j] != temp[k]){ 
						check++;
						}
					}
					//if the car value hasn't been found yet it will be added to the array
					//to check the next time through the loop
					if (check == 27){
						temp[count] = road[i][j];
						count++;
					}
					check = 0;
				}
			}
		}
	}

	//check if size is negative or positive i.e car is horizontal or vertical
	if (size>0){
		//this for loop will check to see whether the road is clear for the car to be placed
		for (k = 0; k < size; k++){
			if (road[row][col+k] != SPACE){
				collide = 1;
			}
		}
		//only if the car can be placed will this for loop be run
		//and the car added
		if (collide == 0){
			for (k=0; k < size; k++){
				road[row][col + k] = count;
			}
		}
	} else { //this code runs the same as previous but for the vertical placement
			 //the row is changed not the column
		for (k = 0; k < -size; k++){
			if (road[row + k][col] != SPACE){
				collide = 1;
			} 
		}

		if (collide == 0){
			for (k=0; k <-size; k++){
				road[row + k][col] = count;
			}
		}
	}
}

void FindCar(int road[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int *rowEnd, int *colEnd)
{
	int i, j, k;
	int count = 0;
	int row[100] = {0};
	int col[100] = {0};

	//this checks for the car that is being found and finds how long it is 
	//also stores the position of all the car segments
	for (i=0; i<NUM_ROWS; i++){
		for (j=0; j<NUM_COLS; j++){
			if (road[i][j] == move - 64){
				count++;
				row[count] = i;
				col[count] = j;
			}
		}
	}

	//finds the start and end position of the car
	for (k=1; k < (count + 1); k++){
		if (k == 1){ //start position
			*rowStart = row[k];
			*colStart = col[k];
		}
		if (k == count){ //end position
			*rowEnd = row[k];
			*colEnd = col[k];
		}
	}
}

int MoveCar(int road[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1)
{
	int k, car;
	car = road[r0][c0];
	int check = 0;
	int r = -1, c = -1;

	//if the car is horizontal
	if (r0 == r1){
		if (road[r0][c0-1] == SPACE){ //check the left of the car first
			for (k = c0-1; k > 0; k--){ 
				if (road[r0][k] != SPACE){ 
					//if there is a non available space i.e wall or car store the column position
					c = k;
					// exit after the first non space value
					break;
				} else if (road[r0][k] == SPACE){ //see how may available spaces there are
					check++;
				}
				//printf("%d ", check);
			} 
			//remove the car from its starting position in the array
			for (k = c0; k <= c1; k++){
				road[r0][k] = SPACE;
			}
			//move the car to its new position
			for (k = c0; k <= c1; k++){
				//how the car is moved depends on whether it has a clear path to the wall or a car
				//is in its way
				if (c != -1) { //if car is in the way of the moving car
					road[r0][k-(c0-c-1)] = car;
				} else if (c == -1){ //clear path to the wall
					road[r0][k-check] = car;
				}
				//check to see if the car being moved is next to the exit
				if (k == c1){
					if (road[r0][k-check-(c1-c0+1)] == EXIT){
						return 1;
					}
				}
			}
		} else if (road[r0][c1+1] == SPACE){ //check right side of car - same code as before
											 //but checking the other side
			for (k = c1+1; k < NUM_COLS-1; k++){ 
				if (road[r0][k] != SPACE){
					c = k;
					break;
				} else if (road[r0][k] == SPACE){
					check++;
				}
			} 
			for (k = c0; k <= c1; k++){
				road[r0][k] = SPACE;
			}
			for (k = c0; k <= c1; k++){
				if (c != -1) {
					road[r0][k+(c-c1-1)] = car;
				} else if (c == -1){
					road[r0][k+check] = car;
				}
				if (road[r0][k+check+1] == EXIT){
					return 1;
				}
			}
		}
	}
	//if car is vertical
	//same code as when the car is horizontal but the row value is the one being
	//checked and changed
	if (c0 == c1){
		if (road[r0-1][c0] == SPACE){ //check above the car first
			for (k = r0-1; k>0; k--){ 
				if (road[k][c0] == SPACE){
					check++;
				} else if (road[k][c0] != SPACE) {
					r = k; //stores row position if a car is in the way of the moving car
					break;
				}
			}
			for (k = r0; k <= r1; k++){
				road[k][c0] = SPACE;
			}
			for (k = r0; k <= r1; k++){
				if (r != -1) {
					road[k-(r0-r-1)][c0] = car;
				} else if (c == -1){
					road[k-check][c0] = car;
				}
				if (k == r1){
					if (road[k-check-(r1-r0+1)][c0] == EXIT){
						return 1;
					}
				}
			}
		} else if (road[r1+1][c0] == SPACE){ //check below the car after above
			for (k = r1+1; k < NUM_ROWS-1; k++){ 
				if (road[k][c0] == SPACE){
					check++;
				} else if (road[k][c0] != SPACE) {
					r = k;
					break;
				}
			}
			for (k = r0; k <= r1; k++){
				road[k][c0] = SPACE;
			}
			for (k = r0; k <= r1; k++){
				if (r != -1) {
					road[k+(r-r1-1)][c0] = car;
				} else if (c == -1){
					road[k+check][c0] = car;
				}
				if (road[k+check+1][c0] == EXIT){
					return 1;
				}
			}
		}
	}
	//car being moved is not next to the exit so 0 is returned
	return 0;
}


/***********************************************************/
/***********************************************************/
/********* DO NOT MODIFY ANY CODE BELOW THIS POINT *********/
/***********************************************************/
/***********************************************************/

/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove car: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

/* The main Traffic Jam simulation */
int main(void)
{
	int gameOver = 0;
	int road[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf(" _____           __  __ _            _                                        \n");
	printf("|_   _| __ __ _ / _|/ _(_) ___      | | __ _ _ __ ___           ____          \n");
	printf("  | || '__/ _` | |_| |_| |/ __|  _  | |/ _` | '_ ` _ \\  --   __/  |_\\_      \n");
	printf("  | || | | (_| |  _|  _| | (__  | |_| | (_| | | | | | | --- |  _     _``-.    \n");
	printf("  |_||_|  \\__,_|_| |_| |_|\\___|  \\___/ \\__,_|_| |_| |_| --  '-(_)---(_)--'\n\n");

	/* Initialise the road and add cars */
	InitialiseRoad(road, 'W', 2);
	AddCar(road, 3, 3, 2);
	AddCar(road, 1, 1, 2);
	AddCar(road, 2, 1, 3);
	AddCar(road, 3, 2, -2);
	AddCar(road, 5, 2, -2);
	AddCar(road, 4, 4, -2);
	AddCar(road, 6, 3, 3);
	AddCar(road, 1, 5, -3);
	AddCar(road, 2, 6, -2);

	/* Print status */
	printf("ENGGEN131 2020 - C Project\nTraffic Jam!  There is a lot of traffic on the road!\n");
	printf("In fact, %.2f%% of the road is cars!\n\n", PercentUsed(road));

	/* Main simulation loop */
	while (!gameOver) {
		PrintRoad(road);
		input = GetMove();
		FindCar(road, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveCar(road, rowStart, colStart, rowEnd, colEnd);
	}

	/* A car has exited - the simulation is over */
	PrintRoad(road);
	printf("\nCongratulations, you're on your way again!");

	

	return 0;
}
