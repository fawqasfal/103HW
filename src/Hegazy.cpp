#include <iostream>
#include <ctime>
using namespace std;
/* This is a "bar betting game" arranged from a traditional "Going to Boston" version. You will play with a computer. 
At the beginning, you and computer have the initial allowance of $1,000 each, and the game will continue until one becomes broke...

On one turn, you and computer roll three dice and show the two numbers of them but one die number being hidden. 
Based on the odds you think of, you will bet $1 to $10. 
The outcome is called "Big Fish!" if the three numbers are identical such as 1-1-1 and 2-2-2.

If not Big Fish, set aside the highest die and re-roll the other two. Keep the highest die of the two and re-roll the last. 
The total of all three dice is scored, 
and the bet amount multiplied by the difference of yours and computer's will be added/subtracted to each allowance. 
If Big Fish, the bet amount will be multiplied by 50 and added/subtracted. See the below example of a game session:
*/
void roll(int d, int* rolls) { //rolls d 6-sided dice, puts answer into rolls
	for (int i = 0; i < d; i++) {
		rolls[i] = (rand() % 6) + 1;
	}
}
int max_3(int a, int b, int c) { //returns biggest of 3 numbers
	return max(max(a, b),c);
}
int machine_roll(int* cpu_roll) { //rolls for the machine. abstracted in a function because it occurs twice in code
	int cpu_biggest[2];
	cout << "Machine had " << "(" << cpu_roll[0] << ", " << cpu_roll[1] << ", " << cpu_roll[2] << ")"; //3-roll
	if (cpu_roll[0] == cpu_roll[1] && cpu_roll[1] == cpu_roll[2]) {
		cout << "... Big Fish!!!\n";
		return 0;
	}
	cpu_biggest[0] = max_3(cpu_roll[0], cpu_roll[1], cpu_roll[2]);
	roll(2, cpu_roll);
	cout << "-> Machine rolled (" << cpu_roll[0] << ", " << cpu_roll[1] << ") and then rolled ("; //2-roll
	cpu_biggest[1] = max(cpu_roll[0], cpu_roll[1]);
	int last_roll = (rand() % 6) + 1;
	cout << last_roll << ") -> Machine scored "; //final roll and score
	int cpu_score = cpu_biggest[0] + cpu_biggest[1] + last_roll;
	cout << cpu_score << ".\n";
	return cpu_score;
}
int main(int argc, char* argv[]) {
	cout << "Type anything in the command line parameter to run in auto-simulate mode.\n";
	srand( (unsigned)time( NULL ) );
	int p1_cash, cpu_cash, curr_round;
	p1_cash = cpu_cash = 1000;
	curr_round = 0;
	while (p1_cash > 0 && cpu_cash > 0) {
		int p1_roll[3], cpu_roll[3];
		roll(3, p1_roll);
		roll(3, cpu_roll);
		int p1_biggest[2]; //biggest of the 2 initial rolls
		int bet;
		if (argc > 1) {
			if (p1_roll[0] == p1_roll[1]) bet = 10; 
			else bet = (rand() % 5) + 1;
		}
		cout << "[Round " << ++curr_round << "] " << "You rolled (" << p1_roll[0] << ", " << p1_roll[1] << ", ?), "; 
		cout << "machine rolled (" << cpu_roll[0] << ", " << cpu_roll[1] << ", ?)..." << endl;
		cout << "How much are you going to bet: ";
		if (argc == 1) cin >> bet;
		else cout << bet << endl;
		while (bet > 10 || bet < 1) { //if the bet is too small or too big 
			cout << " - your bet must between $1 and $10, type again: ";
			cin >> bet;
		}
		cout << "You had " << "(" << p1_roll[0] << ", " << p1_roll[1] << ", " << p1_roll[2] << ")-> "; 
		if (p1_roll[0] == p1_roll[1] && p1_roll[1] == p1_roll[2]) { //the big fish
			cout << " ... Big Fish!!!\n";
			int cpu_score = machine_roll(cpu_roll);
			if (cpu_score == 0) { // double big-fish
				cout << "Wow!!! A double Big Fish!! No one wins!";
				cout << " You have $" << p1_cash << ", machine has $" << cpu_cash << "...\n";
			}
			else { 
				p1_cash += bet * 50;
				cpu_cash -= bet * 50;
				cout << "You won :) You have $" << p1_cash << ", machine has $" << cpu_cash << "...\n";	
			}
		}
		else {
			p1_biggest[0] = max_3(p1_roll[0], p1_roll[1], p1_roll[2]);
			roll(2, p1_roll);
			cout << "You rolled (" << p1_roll[0] << ", " << p1_roll[1] << ") and then rolled (";
			p1_biggest[1] = max(p1_roll[0], p1_roll[1]); 
			int last_roll = (rand() % 6) + 1;
			cout << last_roll << ") -> You scored ";
			int p1_score = p1_biggest[0] + p1_biggest[1] + last_roll;
			cout << p1_score << ".\n";
			int cpu_score = machine_roll(cpu_roll);
			if (cpu_score == 0) {
				p1_cash -= bet * 50;
				cpu_cash += bet * 50;
				cout << "You lost :( You have $" << p1_cash << ", machine has $" << cpu_cash << "...\n"; 
			} else {
				p1_cash += bet * (p1_score - cpu_score);
				cpu_cash -= bet * (p1_score - cpu_score);
				if (p1_score >= cpu_score) cout << "You won :) "; else cout << "You lost :( ";
				cout << "You have $" << p1_cash << ", machine has $" << cpu_cash << "...\n"; 
			}
		}
		cout << "\n";	
	}
	//debt : when one of the players has less than 0 dollars due to a large last bet
	if (p1_cash < 0 ) cout << "You owe the machine $" << 0-p1_cash << ".\n";
	if (cpu_cash < 0) cout << "The machine owes you $" << 0-cpu_cash << ".\n";
}
