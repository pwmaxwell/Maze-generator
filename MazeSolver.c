#include <stdio.h>
#include <stdlib.h>
//#include "TutorialConfig.h.in"
#include <getopt.h>


int** maze;
int** splitpath;
int rows = 2; 
int columns = 0;
int count = 1;
int startRow;
int startCol;
int numberOfPaths;
int* currentLocation;	
FILE* journal;

void aloc_maze(){ // taken from class discussion
	maze = malloc(rows * sizeof(int*));
	for(int i = 0; i < rows; i++)
		maze[i] = malloc(columns * sizeof(int*));
	splitpath = malloc(rows * sizeof(int*));
	for(int i = 0; i < rows; i++)
		splitpath[i] = malloc(columns * sizeof(int*));
	
}
void printMaze(){
	for (int i = 0; i < rows; ++i) { //this will print out the maze
		for (int j = 0; j < columns; ++j) {
			if(maze[i][j] !=99) 
				printf("%3i", maze[i][j]); //information on formating taken from https://www.eecs.wsu.edu/~cs150/reading/printf.htm
		}
		printf("\n");
	}	
	printf("\n");
	printf("%i", maze[0][0]);
}


int solveMaze(int row, int col){
	currentLocation = &maze[row][col];  	// sets the current location to where you are currently looking 
	numberOfPaths = 0;
	maze[row][col] = count;
	
	if(row != 0){							// if you are at the top row it will skip this step (to avoid segmentation faults)
		if(maze[row-1][col] == 0){			// 4 if statements which see if there are multiple paths you are able to travel

			numberOfPaths++;
		}
		if(maze[row+1][col] == 0){
			
			numberOfPaths++;
		}
		if(maze[row][col-1] == 0){
			
			numberOfPaths++;
		}
		if(maze[row][col+1] == 0){
			
			numberOfPaths++;
		}
		if(numberOfPaths >= 2){				// if there are multiple paths which you can go it will put that number into the splitpath array. this will be used to track where
			numberOfPaths--;				// you need to return to after you solve the first path
			splitpath[row][col] = numberOfPaths;
		}
		if(maze[row][col -1] == 0){			// four more if statements which look in each direction, see where it can move, and then move there
			
			
			maze[row][col] = count;
			fprintf(journal,"%d %d %d \n",row, col, count);
			count++;
			solveMaze(row,col-1);
		}
		else if(maze[row+1][col] == 0){
			//lastmove = count;
			
			maze[row][col] = count;
			fprintf(journal,"%d %d %d \n",row, col, count);
			count++;
			
			
			solveMaze(row+1, col);
		}	
		else if(maze[row][col +1] == 0){
			//lastmove = count;
			
			maze[row][col] = count;
			fprintf(journal,"%d %d %d \n",row, col, count);
			count++;
			solveMaze(row, col + 1);	
		}
		else if(maze[row-1][col] == 0){
			//lastmove = count;
			
			maze[row][col] = count;
			fprintf(journal,"%d %d %d \n",row, col, count);
			count++;

			solveMaze(row-1, col);
		}
		else{							// if it could not move anywhere it will end since it must be at a deadend 
			maze[row][col] = count;
			return 1;
		}
	}
}

int solveFromJournal(){
	char c;
	FILE* file = fopen("journal.txt", "r");
	
	for(int i = 0; i <rows; i++){ 		// sets all values to -1 as a baseline
		for(int j = 0; j <columns; j++){
			maze[i][j] = -1;
		}
	}
	
	

	
}


void getMaze(char* file_name){
	char c;
	
	FILE* file = fopen(file_name, "r");	
	
	while ((c = getc(file)) != EOF){ // this will determine how many columns and rows are needed to alocate them into the 2d array
		if(c == '\n'){
			columns++;
			break;
		}
		columns++;
	}
	while((c = getc(file)) != EOF){
		if(c == '\n')
			rows++;
	}

	//aloc_maze();
	fclose(file);
}	
void placeNumbers(char* file_name){
	FILE* file = fopen(file_name, "r");
	char c2;
	for(int i = 0; i < rows; i++){  // this will put the numbers into the maze array
	int j;
		for (j = 0; j < columns; j++){
			c2= getc(file);
			if(c2 == 'b')			// four if statements which look at the file and puts the number into the maze which coresponds with the letter in the file
				maze[i][j] = -1;
			else if(c2 == ' ')
				maze[i][j] = 0;
			else if(c2 == 's')
				maze[i][j] = 1;
			else {
				maze[i][j] = 99;
			}	
			if(c2 == 's'){ 			//this will figure out where to start
				startRow = i;
				startCol = j;			
			}
		}
		if (j < columns) {
			while (c2 != '\n')
				c2 = getc(file);
		}
	}
		fclose(file);	
}


	
void searchSplitpath(){
	for(int i =0; i< rows; i++){	// this will go back into splitpath, see where there is a number which is more than 0, and then move to that location in maze
		for(int j=0; j<columns -2 ; j++){		
			while(splitpath[i][j] != 0){
				fprintf(journal,"%d %d %d \n",i, j, count);
	
				count = maze[i][j];
				splitpath[i][j]--;
				solveMaze(i,j);	
			}
		}
	}
}

int main(int argc, char *argv[]){
	char *journalWrite = NULL;
	char *journalRead = NULL;
	
	char *INPUTFILE = argv[1];
	
	int option_index = 1;
	//printf(getopt(argc, argv, "o:j:"));
	while((option_index = getopt(argc, argv, "o:j:")) != -1){
		//printf("TEST");
		switch(option_index){
			case 'o':
				journalWrite = optarg;
				journal = fopen(journalWrite, "w");
				getMaze(INPUTFILE);
				aloc_maze();
				placeNumbers(INPUTFILE);
				solveMaze(startRow, startCol);
				searchSplitpath();
				printMaze();
				fclose(journal);
				
				break;
			case 'j':
				journalRead = optarg;
				journal = fopen(journalRead, "r");
				getMaze(INPUTFILE);
				aloc_maze();
				solveFromJournal();
				printMaze();
				break;
			default:
				//printf("I GOT THERE");
				return 1;
		}
	}
	return 0;
}
