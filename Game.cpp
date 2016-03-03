#include "Game.h"
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include <unistd.h>

using namespace std;

Game::Game(int height, int width) {
	setHeight(height);
	setWidth(width);
	file = "";
}

Game::Game() {}

Game::~Game() {
	//delete cell array
	for (int k = 0; k < height+2; ++k) {
		delete [] cellArray[k];
	}
	delete [] cellArray;

	//delete temporary array
	for (int k = 0; k < height+2; ++k) {
		delete [] tempArray[k];
	}
	delete [] tempArray;

}

void Game::setHeight(int h) {
	height = h;
}

void Game::setWidth(int w) {
	width = w;
}

//method to make the intial grid with all dead characters
void Game::makeGrid() {
	//allocating cell array
	cellArray = new char*[height+2];

	for (int k = 0; k < height+2; ++k) {
		cellArray[k] = new char[width+2];
	}
	
	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			cellArray[i][j] = '-';
		}
	}
	//allocating temporary array
	tempArray = new char*[height+2];

	for (int k = 0; k < height+2; ++k) {
		tempArray[k] = new char[width+2];
	}

	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			tempArray[i][j] = '-';
		}
	}
}

//method to print out cellArray
void Game::printArray() {
	/*cout << "With buffer zone:" << endl;
	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			cout << cellArray[i][j];
		}
		cout << endl;
	}*/
	//cout << "Without buffer zone:" << endl;
	for (int i = 1; i < height+1; ++i) {
		for (int j = 1; j < width+1; ++j) {
			cout << cellArray[i][j];
		}
		cout << endl;
	}
}

//method to initially populate based on user input
void Game::initialPopulation(int height, int width, double popDensity) {
	//variables to hold how many live cells there should be
	int area = round(popDensity*height*width);
	int check = area;
	//iterate through the non-buffer zone
	for (int i = 1; i < height+1; ++i) {
		for (int j = 1; j < width+1; ++j) {
			int random1 = rand() % height+1;//set two random numbers to randomly populate grid
			int random2 = rand() % width+1;
			if (check != 0) {
				if (cellArray[random1][random2] == 'X') {
					continue;
				}
				else if (cellArray[random1][random2] == '-') {
					cellArray[random1][random2] = 'X';
					check--;//subtract from X's you are supposed to have 
				}
			}
			else if (check == 0)
				break;
		}
	} 
	//cout << "Generation: 0" << endl;
	//printArray();
}

//method to make a grid based on a text file
void Game::mapPopulation(string file) {

	ifstream reader(file);//open the file 
	int lineCounter = 0;
	string line;

	if (reader.is_open()) {
		while (getline (reader, line)) {
			if (lineCounter == 0) //take the first line to set height
				height = stoi(line);
			else if (lineCounter == 1) { //take the second line to set width
				width = stoi(line);
				makeGrid();//make the grid once you have both
			}
			else {
				for (int i = 0; i < line.size(); ++i) { 
					cellArray[lineCounter-1][i+1] = line[i]; //make each line of the grid one by one
				}
			}
			lineCounter++;//go to the next line
		}
	}
	reader.close(); //close the file
}

//method to copy one array to another
void Game::copy(char** array1, char** array2) {

	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			array2[i][j] = array1[i][j];
		}
	}
}

//method to compare arrays to check for stabilization
bool Game::compareArrays(char** array1, char** array2) {

	int count = 0;
	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			if (array1[i][j] == array2[i][j])
				count++;	//count number of same indexes
		}
	}

	if (count == (height+2)*(width+2)) //if the number of same indexes is the size of the grid it stabilized
		return true;
	else {
		return false;
	}
}

//method to check if the population died off
bool Game::checkDead(char** array1) {
	int dead = 0;
	for (int i = 0; i < height+2; ++i) {
		for (int j = 0; j < width+2; ++j) {
			if (array1[i][j] == '-') 
				dead++;	//take the number of dead cells
		}
	}

	if (dead == (height+2)*(width+2)) //if #of dead cells is the same as the grid the population died
		return true;
	else {
		return false;
	}
}

//method to get user input on how they want the program to run
string Game::askUser() {

	bool condition = true;
	string doAnswer;
	while (condition) {
		cout << "Would you like to have a brief pause, press enter, or output generations to a text file?" << endl;
		string answer;
		cin >> answer;
		if (answer != "pause" && answer != "enter" && answer != "file") { //test to make sure answer is one of the three
			cout << "Please enter 'pause', 'enter', or 'file'." << endl;
		}
		else {
			condition = false; //if it is, initialize the variable to return 
			doAnswer = answer;
		}
	}

	return doAnswer; //return the user input to specify the mode
}

//method to test if the user hits enter
void Game::userEnter(string extra) {

	bool condition = true;
	while (condition) {
		cout << "Press 'Enter' to " << extra << endl;
		if (cin.get() == '\n') { //check to see if enter key was pressed
			condition = false;
		}
	}
}

//method that runs through the game and checks the neighbors
bool Game::checkNeighbors(string simulation, int genCount) {

	bool con = true;
	copy(cellArray, tempArray); //copy cellArray to tempArray to modify it 
	//check 8 surrounding boxes for life
	for (int i = 1; i < height+1; ++i) {
		for (int j = 1; j < width+1; ++j) {
			int neighborCount = 0;
			if (cellArray[i-1][j-1] == 'X')
				neighborCount++;
			if (cellArray[i-1][j] == 'X')
				neighborCount++;
			if (cellArray[i-1][j+1] == 'X')
				neighborCount++;
			if (cellArray[i][j-1] == 'X')
				neighborCount++;
			if (cellArray[i][j+1] == 'X')
				neighborCount++;
			if (cellArray[i+1][j-1] == 'X')
				neighborCount++;
			if (cellArray[i+1][j] == 'X')
				neighborCount++;
			if (cellArray[i+1][j+1] == 'X')
				neighborCount++;
			/*one or fewer: empty 
			two: stable, if there was one it stays, if not nothing grows
			three: empty-one grows, occupied-stays
			four or more: dies*/
			if (neighborCount <= 1)
				tempArray[i][j] = '-';
			else if (neighborCount == 2)
				tempArray[i][j] = cellArray[i][j];
			else if (neighborCount == 3) 
				tempArray[i][j] = 'X';
			else if (neighborCount >= 4) 
				tempArray[i][j] = '-';
		}
	}
	//print the first generation to the console
	if (genCount == 1 && simulation != "file") {
		cout << "Generation: 0" << endl;
		printArray();
	}

	if (simulation == "pause") {
		usleep(1000000); //wait a second to print the next generation
		bool comparison = compareArrays(tempArray, cellArray); //check stabilization
		
		if (comparison == true) {
			copy(tempArray,cellArray); //if it is true, copy tempArray back to cellArray to print
			cout << "Generation: " << genCount << endl;
			printArray();
			cout << "Population stabilized." << endl;
			userEnter("to exit the program."); //prompt for user to exit the program
			userEnter("to exit the program.");
			con = false; //used to break out of the loop 
		}
		
		else {
			copy(tempArray,cellArray); //keep going in generations if it did not stabilize
			cout << "Generation: " << genCount << endl;
			printArray();
		}
	
		bool allDead = checkDead(cellArray);
		if (allDead == true) {
			cout << "Population died off." << endl;
			userEnter("to exit the program."); //if the population died off, prompt for user to exit
			userEnter("to exit the program.");
			con = false;
		}
	}

	else if (simulation == "enter") { //same as 'pause', but pressing enter between generations
			
		userEnter("to show next generation.");
		bool comparison = compareArrays(tempArray, cellArray);
		
		if (comparison == true) {
			copy(tempArray,cellArray);
			cout << "Generation: " << genCount << endl;
			printArray();
			cout << "Population stabilized." << endl;
			userEnter("to exit the program.");
			con = false;
		}
		
		else {
			copy(tempArray,cellArray);
			cout << "Generation: " << genCount << endl;
			printArray();
		}
		
		bool allDead = checkDead(cellArray);
		if (allDead == true) {
			cout << "Population died off." << endl;
			userEnter("to exit the program.");
			con = false;
		}
	}

	else if (simulation == "file") { //same as before, but outputting generations to a text file
		
		if (genCount == 1) {
			cout << "Enter a file you would like to output to:" << endl;
			cin >> file; //get the text file the first time through
		}

		ofstream writer(file, ios::app); //append otherwise it will erase every generation
		if (writer.is_open()) {
			if (genCount == 1) { //send first generation to text file
				writer << "Generation: 0" << endl;
				for (int i = 1; i < height+1; ++i) {
					for (int j = 1; j < width+1; ++j) {
						writer << cellArray[i][j];
					}
					writer << endl;
				}
			}
			bool comparison = compareArrays(tempArray, cellArray); //same as previous two cases
		
			if (comparison == true) {
				copy(tempArray,cellArray);
				writer << "Generation: " << genCount << endl;
				for (int i = 1; i < height+1; ++i) {
					for (int j = 1; j < width+1; ++j) {
						writer << cellArray[i][j];
					}
					writer << endl;
				}
				writer << "Population stabilized." << endl;
				con = false;
			}
			
			else {
				copy(tempArray,cellArray);
				writer << "Generation: " << genCount << endl;
				for (int i = 1; i < height+1; ++i) {
					for (int j = 1; j < width+1; ++j) {
						writer << cellArray[i][j];
					}
					writer << endl;
				}
			}
		
			bool allDead = checkDead(cellArray);
			if (allDead == true) {
				writer << "Population died off." << endl;
				con = false;
			}
		}
		writer.close();
	}
	genCount++;
	return con;
}

//method to carry out normal mode
void Game::normalMode() {

	string sim = askUser(); //get which output mode
	bool condition = true;
	int generationCount = 1; //start generation count at 1 since 0 was already printed

	while (condition) {
		condition = checkNeighbors(sim, generationCount); //play the game
		generationCount++;//increase generation
	}
}

//method to carry out donut mode
void Game::donutMode() {

	string sim = askUser();
	bool condition = true;
	int generationCount = 1;
	while(condition) {
		//set buffer to dead
		for (int j = 0; j < width+2; ++j) {
			cellArray[0][j] = '-';
			cellArray[height+1][j] = '-';
		}
		for (int i = 1; i < height+1; ++i) {
			cellArray[i][0] = '-';
			cellArray[i][width+1] = '-';
		}
		//set top and bottom buffers
		for (int j = 1; j < width+1; ++j) {
			cellArray[0][j] = cellArray[height][j];
			cellArray[height+1][j] = cellArray[1][j];
		}
		//set right and left buffers
		for (int i = 1; i< height+1; ++i) {
			cellArray[i][0] = cellArray[i][width];
			cellArray[i][width+1] = cellArray[i][1];
		}
		//set corner buffers
		cellArray[0][0] = cellArray[height][width];
		cellArray[0][width+1] = cellArray[height][1];
		cellArray[height+1][0] = cellArray[1][width];
		cellArray[height+1][width+1] = cellArray[1][1];

		condition = checkNeighbors(sim, generationCount); //play the game
		generationCount++; //increase generation
	}
}

//method to carry out mirror mode
void Game::mirrorMode() {

	string sim = askUser();
	bool condition = true;
	int generationCount = 1;
	while (condition) {
		//set boundaries to dead each time
		for (int j = 0; j < width+2; ++j) {
			cellArray[0][j] = '-';
			cellArray[height+1][j] = '-';
		}
		for (int i = 1; i < height+1; ++i) {
			cellArray[i][0] = '-';
			cellArray[i][width+1] = '-';
		}
		//set the corner reflections
		if (cellArray[1][1] == 'X') {
			cellArray[1][0] = 'X';
			cellArray[0][1] = 'X';
			cellArray[0][0] = 'X';
		}
		if (cellArray[1][width] == 'X') {
			cellArray[0][width] = 'X';
			cellArray[0][width+1] = 'X';
			cellArray[1][width+1] = 'X';
		}
		if (cellArray[height][1] == 'X') {
			cellArray[height][0] = 'X';
			cellArray[height+1][0] = 'X';
			cellArray[height+1][1] = 'X';
		}
		if (cellArray[height][width] == 'X') {
			cellArray[height][width+1] = 'X';
			cellArray[height+1][width] = 'X';
			cellArray[height+1][width+1] = 'X';
		}
		//set the right, left, top, and bottom reflections
		for (int j = 2; j < width; ++j) {
			if (cellArray[1][j] == 'X') {
				cellArray[0][j] = 'X';
			}
			if (cellArray[height][j] == 'X') {
				cellArray[height+1][j] = 'X';
			}
		}

		for (int i = 2; i < height; ++i) {
			if (cellArray[i][1] == 'X') {
				cellArray[i][0] = 'X';
			}
			if (cellArray[i][width] == 'X') {
				cellArray[i][width+1] = 'X';
			}
		}

		condition = checkNeighbors(sim, generationCount); //play the game
		generationCount++; //increase generation 
	}
}














