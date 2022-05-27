#ifndef puzzle_h_
#define puzzle_h_

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

// vector of 2d vectors representing puzzles (vector of 2D grids)
typedef std::vector<std::vector<char>> puzzle_type;
typedef std::vector<std::vector<std::vector<char>>> puzzles_type;

class puzzle {
public:
	// constructor
	puzzle (std::vector<std::string> dict_info, puzzle_type puzzle_info, std::vector<int> constraints_info);

	//	public member function
	void find_solutions(int x1, int y1, int x2, int y2, std::vector<int> cons, std::vector<std::vector<int>> word_locs);
	puzzles_type give_solutions() {return solved_puzzles;}
	int give_num_solutions() {return solutions;}
	void print(std::string solution_mode, std::string output_mode);
	std::vector<int> give_constraints() {return constraints;}
	bool valid_puzzle(puzzle_type possible_solution, std::string word, std::vector<int> start_pos, std::vector<std::string> words);


private:
	// private member functions
	bool valid_puzzle(puzzle_type possible_solution);

	// variables
	puzzle_type unsolved_puzzle;
	std::vector<std::string> dict;
	std::vector<int> constraints;
	std::vector<std::vector<std::vector<int>>> words_locs;
	puzzles_type solved_puzzles;
	int solutions;
};

// initializing values
puzzle::puzzle (std::vector<std::string> dict_info, puzzle_type puzzle_info, std::vector<int> constraints_info) {
	dict = dict_info;
	unsolved_puzzle = puzzle_info;
	constraints = constraints_info;
	std::sort (constraints.begin(), constraints.end());
	solutions = 0;
}

void puzzle::find_solutions(int x1, int y1, int x2, int y2, std::vector<int> cons, std::vector<std::vector<int>> word_locs) {
	// if bottom right corner of grid reached
	if ((unsolved_puzzle.size() - 1 == y1) && (unsolved_puzzle.size() - 1 == y2) && (unsolved_puzzle[0].size() - 1 == x1) && (unsolved_puzzle[0].size() - 1 == x2)) {
		return;
	}
	// if all words found
	if (cons.size() == 0) {
		// check is solution is already found
		sort(word_locs.begin(),word_locs.end());
		if (std::find(words_locs.begin(), words_locs.end(), word_locs) == words_locs.end()) {
			words_locs.push_back(word_locs);
			puzzle_type solution = unsolved_puzzle;
			for (int x = 0; x < word_locs.size(); x++) {
				// horizontal word
				if (word_locs[x][3] == 0) {
					for (int y = 0; y <= (word_locs[x][2] - word_locs[x][1]); y++) {
						solution[word_locs[x][0]][word_locs[x][1] + y] = '$';
					}
				}
				// vertical word
				else {
					for (int y = 0; y <= (word_locs[x][2] - word_locs[x][1]); y++) {
						solution[word_locs[x][1] + y][word_locs[x][0]] = '$';
					}
				}
			}
			// inverse of grid to show solution
			for (int i = 0; i < solution.size(); i++) {
				for (int j = 0; j < solution[0].size(); j++) {
					if (solution[i][j] == '$') {
						solution[i][j] = unsolved_puzzle[i][j];
					}
					else {
						solution[i][j] = '#';
					}
				}
			}
			std::string word = "";
			std::vector<std::string> words;
			if (valid_puzzle(solution)) {
				solutions++;
				solved_puzzles.push_back(solution);
			}
		}
		return;
	}
	// if word fits length constraints (horizontal)
	if ((y2 == y1) && (std::find(cons.begin(), cons.end(), (x2 - x1 + 1)) != cons.end())) {
		std::string word = "";
		for (int i = 0; i <= (x2 - x1); i++) {
			word += unsolved_puzzle[y1][x1 + i];
		}
		// if word in dictionary
		if (std::find(dict.begin(), dict.end(), word) != dict.end()) {
			// 0 means horizontal word
			std::vector<int> loc{y1, x1, x2, 0};
			if (std::find(word_locs.begin(), word_locs.end(), loc) == word_locs.end()) {
				word_locs.push_back(loc);
				cons.erase(std::find(cons.begin(), cons.end(), (x2 - x1 + 1)));
				find_solutions(0, 0, 0, 0, cons, word_locs);
			}
		}
	}
	// if word fits length constraints (vertical)
	else if ((x2 == x1) && (std::find(cons.begin(), cons.end(), (y2 - y1 + 1)) != cons.end())) {
		std::string word = "";
		for (int i = 0; i <= (y2 - y1); i++) {
			word += unsolved_puzzle[y1 + i][x1];
		}
		// if word in dictionary
		if (std::find(dict.begin(), dict.end(), word) != dict.end()) {
			// 1 means horizontal word
			std::vector<int> loc{x1, y1, y2, 1};
			if (std::find(word_locs.begin(), word_locs.end(), loc) == word_locs.end()) {
				word_locs.push_back(loc);
				cons.erase(std::find(cons.begin(), cons.end(), (y2 - y1 + 1)));
				find_solutions(0, 0, 0, 0, cons, word_locs);
			}
		}
	}

	// recursion
	if ((x2 < unsolved_puzzle[0].size() - 1) && (y1 == y2)) {
		find_solutions(x1, y1, x2 + 1, y2, cons, word_locs);
	}
	if ((y2 < unsolved_puzzle.size() - 1) && (x1 == x2)) {
		find_solutions(x1, y1, x2, y2 + 1, cons, word_locs);
	}
	if ((x2 > x1) && (unsolved_puzzle[0].size() - 1 == x2)) {
		find_solutions(x1 + 1, y1, x1 + 1, y1, cons, word_locs);
	}
	if ((y2 > y1) && (unsolved_puzzle.size() - 1 == y2)) {
		find_solutions(x1, y1 + 1, x1, y1 + 1, cons, word_locs);
	}
}

bool puzzle::valid_puzzle (puzzle_type possible_solution) {
	std::vector<std::string> words;
	// go through each letter on grid
	for (int y = 0; y < possible_solution.size(); y++) {
		for (int x = 0; x < possible_solution[0].size(); x++) {
			bool word_down = false;
			bool word_right = false;
			// check is word starts in pos
			if (possible_solution[y][x] != '#') {
				// if word is starting on top wall and goes down
				if ((y == 0) && (possible_solution[y + 1][x] != '#')) {
					word_down = true;
				}
				// if word is starting on a '#' and goes down
				else if ((possible_solution.size() - 1 != y) && (possible_solution[y + 1][x] != '#') && (possible_solution[y - 1][x] == '#')) {
					word_down = true;
				}
				// if word is starting on left wall and goes right
				if ((x == 0) && (possible_solution[y][x + 1] != '#')) {
					word_right = true;
				}
				// if word is starting on a '#' and goes right
				else if ((possible_solution[0].size() - 1 != x) && (possible_solution[y][x + 1] != '#') && (possible_solution[y][x - 1] == '#')) {
					word_right = true;
				}
			}
			// if valid word exists going down
			if (word_down) {
				std::string word = "";
				bool next_valid = true;
				int offset = 0;
				while (next_valid) {
					word += possible_solution[y + offset][x];
					offset++;
					if ((possible_solution.size() == y + offset) || (possible_solution[y + offset][x] == '#')) {
						next_valid = false;
					}
				}
				words.push_back(word);
			}
			// if valid word exists going right
			if (word_right) {
				std::string word = "";
				bool next_valid = true;
				int offset = 0;
				while (next_valid) {
					word += possible_solution[y][x + offset];
					offset++;
					if ((possible_solution[0].size() == x + offset) || (possible_solution[y][x + offset] == '#')) {
						next_valid = false;
					}
				}
				words.push_back(word);
			}
		}
	}
	if (words.size() == constraints.size()) {
		return true;
	}
	else {
		return false;
	}
}

void puzzle::print(std::string solution_mode, std::string output_mode) {
	if (output_mode == "count_only") {
		std::cout << "Number of solutions: " << solutions << std::endl;
	}
	else if (output_mode == "print_boards") {
		if (solution_mode == "one_solution") {
			if (solutions > 0) {
				std::cout << "Number of solutions: " << 1 << std::endl;
				std::cout << "Board:" << std::endl;
			}
			else {
				std::cout << "Number of solutions: " << 0 << std::endl;
			}
			for (int x = 0; x < solved_puzzles[0].size(); x++) {
				for (int y = 0; y < solved_puzzles[0][x].size(); y++) {
					std::cout << solved_puzzles[0][x][y];
				}
				std::cout << std::endl;
			}
		}
		else if (solution_mode == "all_solutions") {
			std::cout << "Number of solutions: " << solutions << std::endl;
			for (int i = 0; i < solved_puzzles.size(); i++) {
				std::cout << "Board:" << std::endl;
				for (int x = 0; x < solved_puzzles[i].size(); x++) {
					for (int y = 0; y < solved_puzzles[i][x].size(); y++) {
						std::cout << solved_puzzles[i][x][y];
					}
					std::cout << std::endl;
				}
			}
		}
		else {
			std::cerr << "invalid solution_mode" << std::endl;
		}
	}
	else {
		std::cerr << "invalid output_mode" << std::endl;
	}
}

#endif