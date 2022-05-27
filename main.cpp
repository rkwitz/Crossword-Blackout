#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "puzzle.h"


int main(int argc, char* argv[]) {
	// dictionary file and puzzle file (grid)
	std::ifstream dict_file (argv[1]);
	std::ifstream grid_file (argv[2]);

	// dict stores words in the dictonary, and grid stores the puzzle, constraints stores word constraints
	std::vector<std::string> dict;
	std::vector<std::vector<char>> grid;
	std::vector<int> constraints;
	std::vector<char> row;
	std::string line;

	// write file to dictionary
	if (dict_file.is_open()) {
		while (getline (dict_file, line)) {
			dict.push_back(line);
		}
		dict_file.close();
	}

	// write file to grid and constraints
	if (grid_file.is_open()) {
		while (getline (grid_file, line)) {
			if (line[0] == '+') {
				constraints.push_back(std::stoi(line.substr(1)));
			}
			else if (line[0] != '!') {
				for (int i = 0; i < line.length(); i++) {
					row.push_back(line[i]);
				}
				grid.push_back(row);
				row.clear();
			}
		}
	}

	puzzle p (dict, grid, constraints);
	std::vector<std::vector<int>> empty;
	p.find_solutions(0, 0, 0, 0, p.give_constraints(), empty);
	p.print(argv[3], argv[4]);



	return 0;

}