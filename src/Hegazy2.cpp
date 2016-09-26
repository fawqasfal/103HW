#include <iostream>
#include <ctime>
#include <chrono> 
#include <ctype.h>
using namespace std;
string gen() {  //generates string
	string ans;
	for (int i = 0; i < 7; i++) {
		if (rand() % 5 == 0) { //wild character
			if (rand() % 2 == 0) ans += "[0-9]"; 
			else ans += "[%-&]"; 
		}
		else { 
			if (rand() % 2 == 0) ans += ('A' + (rand() % 26)); //uppercase
			else ans += ('a' + (rand() % 26)); //lowercase
		}
	}
	return ans;
}
string replace(string full, string out, string in) { 
	size_t f;
	while ((f = full.find(out)) != string::npos) full.replace(f, out.length(), in);
	return full;
}
int diff(string attempt, string right) { //calculates error between the attempt and the correct string
	right = replace(right, "[0-9]","0"); //replaces[0-9] with 0 
	right = replace(right, "[%-&]", "*"); // and [%-&] with *, for offset calculation
	while (attempt.length() > right.length()) right += " ";
	while (right.length() > attempt.length()) attempt +=  " "; //padding
	int offset = 0;
	for (int i = 0; i < attempt.length(); i++) { 
		if ((right[i] == '0' && isdigit(attempt[i])) || (right[i] == '*' && attempt[i] >= '!' && attempt[i] <= '/')) continue; 
		//if its number or symbol, and the answer is correct just move on
		if (attempt[i] != right[i]) offset += abs(attempt[i] - right[i]);
	}
	return offset;
}
int main() {
	int points = 1000;
	srand( (unsigned)time( NULL ) );
	while (points  > 0) {
		string new_str = gen();
		string attempt;
		cout << "Your current points " << points <<  ", just type -> " << new_str << ": ";
		chrono::system_clock::time_point t1 = chrono::system_clock::now();
		getline(cin, attempt);
		chrono::system_clock::time_point t2 = chrono::system_clock::now();
		int elapsed = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		int overtime = elapsed - 7000; 
		int error = diff(attempt, new_str);
		if (overtime <= 0) cout << elapsed << " milliseconds, you made it within the interval of 7000..." << endl;
		else cout << elapsed << " milliseconds, you *failed* it within the interval of 7000..." << endl; 
		if (error == 0) {
			if (overtime <= 0) points += 500;
			else points -= overtime;
		}
		else {
			int loss = error;
			if (overtime > 0) loss += (error + overtime);
			cout << "String offset is " << error << ", your total penalty is " << loss << "..." << endl;
			points -= loss;
		}
	}
	cout << "Bye...\n";
	return 0;
}