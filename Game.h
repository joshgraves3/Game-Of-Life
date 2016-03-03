#ifndef GAME_H
#define GAME_H
#include <string>
using namespace std;

class Game {
	public:
		Game(int h, int w);
		Game();
		~Game();
		int getHeight() { return height; }
		int getWidth() { return width; }
		void setHeight(int height);
		void setWidth(int width);	
		void makeGrid();
		void printArray();
		void initialPopulation(int height, int width, double popDensity);
		void mapPopulation(string file);
		void copy(char** array1, char** array2);
		void normalMode();
		void donutMode();
		void mirrorMode();
		string askUser();
		void userEnter(string extra);
		bool checkNeighbors(string simulation, int genCount);
		bool compareArrays(char** array1, char** array2);
		bool checkDead(char** array1);
		string file;
	private://private to only initialize once
		char** cellArray;
		char** tempArray;
		int height;
		int width;
};

#endif