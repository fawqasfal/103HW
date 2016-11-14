/*
 * Hegazy5.cpp
 *
 *  Created on: Nov 10, 2016
 *      Author: hesha
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <time.h>
using namespace std;
string swap(vector<vector<short> >& board);
void gen_board(vector<vector<short> >& board) {
	//generates a fixed solved board...
	short fixed[][9] = {
			{2,3,4,8,9,1,5,6,7},
			{5,6,7,2,3,4,8,9,1},
			{8,9,1,5,6,7,2,3,4},
			{3,4,5,9,1,2,6,7,8},
			{6,7,8,3,4,5,9,1,2},
			{9,1,2,6,7,8,3,4,5},
			{1,2,3,7,8,9,4,5,6},
			{4,5,6,1,2,3,7,8,9},
			{7,8,9,4,5,6,1,2,3}
	};
	for(auto nums : fixed) {vector<short> row(nums, nums+9); board.push_back(row);}
	//... and randomizes it, so no initial board is the same.
	int rand_amt = rand()%6 + 5; //the random amount of times to perform random swaps on the initialized board.
	for (int i = 0; i < rand_amt; i++) {
		swap(board);
	}
}

string to_string(vector<vector<short> >& board) {
	string ans;
	ans += "   A B C D E F G H I\n";
	char row_name = 'P';
	for (auto row : board) {
		ans += row_name; ans += ' ';
		for (short chr : row) {
			ans+= ' ';
			if (chr != '-') ans+= (char) '0' + chr; else ans += chr;
		}
		ans += "\n";
		row_name++;
	}
	return ans;

}
void get_cols(vector<vector<short> >& board, vector<short>& ans, int col) {
	ans.clear();
	for (auto row : board) ans.push_back(row[col]);
}
void set_cols(vector<vector<short> >& board, vector<short>& set_to, int col) {
	for (int i = 0; i < 9; i++)
		board[i][col] = set_to[i];
}
string swap(vector<vector<short> >& board) {
	string ans; int first = rand() % 9; int second; do{second = rand() % 9;}while(second ==first); int kind = rand() % 2;
	//if kind = 0, we swap rows. if kind = 1, we swap cols.
	if (kind) { //Swap columns.
		ans += "Trying to swap columns "; ans += 'A' + first; ans+= " and ";  ans += 'A' + second; ans += "...\n";
		if (abs(second - first) > 2) {
			//You have to swap all three.
			first = first - (first % 3); second = second - (second % 3); //resets first to the first elements of each three-square.
			for(int i = 0; i < 3; i++) {
				ans += "- Columns "; ans += 'A' + first + i; ans += " and "; ans += 'A' + second + i; ans += " were swapped...\n";
				vector<short> temp_2; get_cols(board, temp_2, second + i); vector<short> temp_1; get_cols(board, temp_1, first+i);
				set_cols(board, temp_2, first + i); set_cols(board, temp_1, second + i);
			}
		} else {//You can swap just one without breaking the square.
			ans += "- Columns "; ans += 'A' + first; ans += " and "; ans += 'A' + second; ans += " were swapped...\n";
			vector<short> temp_2; get_cols(board, temp_2, second ); vector<short> temp_1; get_cols(board, temp_1, first);
			set_cols(board, temp_2, first ); set_cols(board, temp_1, second );
		}
	}
	else { //Swap rows.
		ans += "Trying to swap rows "; ans += 'P' + first; ans+= " and "; ans += 'P' + second; ans += "...\n";
		if (abs(second - first) > 2) {
			//You have to swap all three.
			first = first - (first % 3); second = second - (second % 3); //resets first to the first elements of each three-square.
			for(int i = 0; i < 3; i++) {
				ans += "- Rows "; ans += 'P' + first + i; ans += " and "; ans += 'P' + second + i; ans += " were swapped...\n";
				board[first + i].swap(board[second + i]);
			}
		}
		else {//You can swap just one without breaking the square.
			ans += "- Rows "; ans += 'P' + first; ans += " and "; ans += 'P' + second; ans += " were swapped...\n";
			board[first].swap(board[second]);
		}
	}
	return ans;
}
string erase(vector<vector<short> >& board) {
	string ans; short prev;
	do {
		int row = rand() % 9; int col = rand() % 9; prev = board[row][col];
		board[row][col] = '-';
		ans = "Erasing row "; ans += 'P' + row; ans += " column "; ans += 'A' + col;
	} while(prev == '-');
	return ans + "\n";
}
string square_err(vector<vector<short> >& board) {
	//If the string is blank, all squares are fine.
	string err;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			vector<short> this_board;
			for (int k = 0; k < 3; k++) {
				this_board.push_back(board[3*i+k][3*j]);
				this_board.push_back(board[3*i+k][3*j+1]);
				this_board.push_back(board[3*i+k][3*j+2]);
			}
			for (short l = 1; l < 10; l++)
				if (find(this_board.begin(), this_board.end(), l) == this_board.end()) {
					err += "- Found inconsistency in component starting at row ";
					err += ('P' + (3*i)); err += " and column "; err += ('A' + 3*j); err += "...\n";
					break;
				}
			}
	}
	return err;
}

string vert_err(vector<vector<short> >& board) {
	//verifies rows AND cols at the same time. if all is good, err should be empty
	string err;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			if(board[i][j] == '-') {
				err += "- Found inconsistency in row ";
				err += 'P' + i;
				err += "...\n- Found inconsistency in column ";
				err += 'A' + j;
				err += "...\n";
			}
	return err;
}


int main() {
	srand((unsigned)time(NULL));
	vector<vector<short> > board;
	gen_board(board);
	cout << "Welcome Sudoku Initializer!" << endl;
	string inp;
	do {
		getline(cin, inp);
		if(inp == "show") {
			cout << to_string(board);
		}
		else if (inp == "verify") {
			string sq_err = square_err(board);
			string v_err = vert_err(board);
			if(sq_err + v_err == "") cout <<"Board is correct.\n";
			else cout << sq_err << v_err;

		}
		else if (inp == "erase") {
			cout << erase(board);
		}
		else if (inp == "swap") {
			cout << swap(board);

		} else if (inp != "quit") {
			cout << "Invalid input. Try 'show', 'verify', 'erase' or 'swap'." << endl;
		}
	} while(inp != "quit"); //do-while loop
	cout << "Bye..." << endl;
	return 0;
}
