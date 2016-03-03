#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void getInput(); //define the method

int main (int argc, char** argv) {

	getInput();

	return 0;
}

//method to get all of the user input 
void getInput() {

	Game* g;
	bool mCondition = true;
	string typeAnswer;
	while (mCondition) { //checks to see if user wants to provide a map
		cout << "Would you like to provide a map?" << endl;
		string answer;
		cin >> answer;

		if (answer != "yes" && answer != "no") 
			cout << "Please enter 'yes' or 'no'." << endl;
		else {
			typeAnswer = answer;
			mCondition = false;
		}

	}

	mCondition = true;
	string modeAnswer;
	while (mCondition) { //checks to see what mode user wants
		cout << "Which mode would you like to simulate?" << endl;
		string mode;
		cin >> mode;

		if (mode != "normal" && mode != "donut" && mode != "mirror")
			cout << "Please enter 'normal', 'donut', or 'mirror'." << endl;
		else {
			modeAnswer = mode;
			mCondition = false;
		}
	}

	if (typeAnswer == "yes") {
		cout << "Enter the text file that you would like to load." << endl;
		string file;
		cin >> file; //take in text file to make into grid

		g = new Game(); //instantiate new class
		g->mapPopulation(file); //make new grid based on the file provided
		//carry out different modes
		if (modeAnswer == "normal")
			g->normalMode();
		else if (modeAnswer == "donut")
			g->donutMode();
		else if (modeAnswer == "mirror") 
			g->mirrorMode();
		delete g; //delete allocated instantiation
	}

	else if (typeAnswer == "no") {
		cout << "Enter the height of the grid:" << endl;
		string answer1;
		cin >> answer1;
		int height = stoi(answer1); //get the height from console input
		
		cout << "Enter the width of the grid:" << endl;
		string answer2;
		cin >> answer2;
		int width = stoi(answer2); //get the width from console input

		cout << "Enter population density (number between 0 and 1):" << endl;
		string answer3;
		cin >> answer3;
		double density = stod(answer3); //get the density from console input
		
		g = new Game(height, width); //instantiate with height and width
		g->makeGrid(); //make the grid based on those parameters
		g->initialPopulation(height, width, density); //initially populate the newly made grid
		//carry out different modes
		if (modeAnswer == "normal")
			g->normalMode();
		else if (modeAnswer == "donut")
			g->donutMode();
		else if (modeAnswer == "mirror") 
			g->mirrorMode();
		delete g; //delete allocated instantiation 
	}
}