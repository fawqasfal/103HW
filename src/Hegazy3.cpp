#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std; 
void split(const string &inp, vector<string> &outp, char delim) { 
	//splits strinp on char delim, and appends tokenized vector to outp
	stringstream inpstream;
	string elem;
	inpstream.str(inp); //prepare stream w/ input string
	while (getline(inpstream, elem, delim)) outp.push_back(elem); //uses getline on stream to sift through string for delims
}

string scramble(vector<string> &words, int round) {
	//takes the words chosen to scramble together, and the current round number (indexed at 0), and returns the final scrambled word
	string word;
	for (string elem : words) word += elem;
	random_shuffle(word.begin(), word.end()); //our final scrambled word to insert underscores in
	while (count(word.begin(), word.end(), '_') < round && count(word.begin(), word.end(), '_') != word.size())
		word[rand() % word.size()] = '_'; //reassigns random index if its already an underscore
	return word;
}

void random_select(vector<string> &words, vector<string> &outp, int amt) {
	//selects amt words from words randomly, appends to outp
	for (int i = 0; i < amt; i++) {
		string new_word = words[rand() % words.size()];
		while(find(outp.begin(), outp.end(), new_word) != outp.end() && !(amt > words.size())) 
			new_word = words[rand() % words.size()]; //reassigns random index if its already in the outp
		outp.push_back(new_word);
	}
}

void change(bool correct, char* last, int* round){
	//changes the status of the last round (upgrade/downgrade, win, or loss) and round amt given the last round & correctness
	if (!correct) {
		cout << "Nope!" << endl; 
		if (*last == 'L' && *round > 0) { //last was a loss and WASNT a downgrade. 0 check is to ensure rounds never get negative
			cout << "Missed two consecutive times, challenge goes down!" << endl;
			*round -= 1; 
			*last = 'C'; //signifies a downgrade 
		} else *last = 'L';
	} else {
		cout << "Yes!" << endl; 
		if(*last == 'W') {//last was won and WASNT a upgrade
			cout << "Succeeded two consecutive times, challenge goes up!" << endl;
			*round += 1;
			*last = 'C';
		} else *last = 'W';
	}
}

int main() {
	vector<string> animals;
	string inp;
	int round = 0;
	char last; //if 'W', that means last round was won. if 'L', lost. if 'C', that means a change, so no up/downgrading allowed
	srand((unsigned) time(NULL));	
	while (animals.size() < 5) {
		cout << "Enter at least " << 5 - animals.size() << " unique animal names, e.g., cat, dog, etc..." << endl;
		while (getline(cin,inp) && inp != "") split(inp, animals, ' ');
		sort(animals.begin(), animals.end());
		animals.erase(unique(animals.begin(), animals.end()), animals.end()); //erases duplicates 
	}
	
	for (int i = 0; i < animals.size(); i++) cout << (i + 1) << ": " << animals[i] << endl;
	while (inp != "quit") {
		//main game to loop through
		int word_num =  1 + rand() % 3;
		vector<string> words;
		vector<string> attempt;
		random_select(animals, words, word_num);
		string scrambled = scramble(words, round);
		string plural; if (word_num == 1) plural = " animal "; else plural = " animals ";
		cout << "What are " << word_num << plural << "in " << scrambled << "? ";
		getline(cin,inp);
		if (inp == "quit") {
			cout << "Bye..." << endl;
			return 0;
		}
		while (inp == "?") {
			for (int i = 0; i < animals.size(); i++) cout << (i + 1) << ": " << animals[i] << endl;
			string plural; if (word_num == 1) plural = " animal "; else plural = " animals ";
			cout << "What are " << word_num << plural << "in " << scrambled << "? ";
			getline(cin,inp);
		}
		split(inp, attempt, ' ');
		while (attempt.size() != words.size()) {
			cout << "Your number of input is incorrect. Enter again ";
			getline(cin, inp);
			attempt.clear();
			split(inp, attempt, ' ');
		}
		sort(attempt.begin(), attempt.end());
		sort(words.begin(), words.end());
		change(attempt == words, &last, &round);
	}
	return 0;
}