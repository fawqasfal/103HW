#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
/*
Hello, 

I finished this assignment at 10:30. Unfortunately, 
I was unable to submit it to Blackboard because I could not remember my password and had to reset it. 
I'm sorry my past 2 assignments have been late. ﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿I have been going through a family-related issue and have been visiting the doctor. 
I can retrieve the doctor's note if you want. 
My GPA in this class is very important to me. Please, do not dock points from this assignment for being late. 
Furthermore, due to issues with random_shuffle that causes it to return the same shuffle despite srand() being initialized, 
I have had to use an alternative that requires at least c++11. Use -std=c++11 when compiling in order for the program to work.*/
using namespace std;

class Card {
public:
	string suit;
	string num;
	string name; //now we don't have to do any << operator overloading on printing
	Card(string s, string n) : suit(s), num(n) {name = s + n;}
};

class Player { 
public: 
	int num;
	vector<Card> hand;
	Player(int n) {num = n;}
	void push_back(const Card& c) {hand.push_back(c);}
	int to_swap() { 
		//finds the least common suit in hand and returns the index of the card to swap. 
		map<string, vector<int> > indices; //sorts indices into vectors accessed by suit type. 
		for(int i = 0; i < hand.size(); i++) indices[hand[i].suit].push_back(i);
		string suits[4] = {"D", "S", "H", "C"}; int smallest  = 7; int ind_smallest = 0; 
		for(string suit : suits) if(indices[suit].size() < smallest && indices[suit].size() != 0) 
			//test for !=0 because there will be no index to return otherwise
			{smallest = indices[suit].size(); ind_smallest = indices[suit][0];} 
		return ind_smallest;
	}
};

string to_string(const Player& p) {
	//converts player's hand into a printable string. Now we don't have to do any operator overloading on printing. 
	string ans; 
	for(char i  = 'a'; i < 'a' + 7; i++) 
		ans += string("(") + i +  string(")") + " " + p.hand[i - 'a'].name + "   "; //cycles through (a) Card, (b) Card, etc. 
	return ans;
}

bool isWon(const vector<Player>& players, vector<Player>& winners) {
	for(Player player : players) {
		bool winner= true; 
		for(int i = 1; i < player.hand.size() && winner; i++) if(player.hand[i].suit != player.hand[i - 1].suit) winner = false;
		//loops through hand : if we ever find a switch in the suits between two adjacent cards, player is not a winner & break loop. 
		if(winner) winners.push_back(player);
	}
	return winners.size() > 0;
}
vector<Card> deck_gen() {
	vector<Card> deck;
	string suits[4] =  {"D", "S", "H", "C"};
	string nums[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "K", "Q"};
	for(int n = 0 ; n < 13; n++) for(int s = 0; s < 4; s++) deck.push_back(Card(suits[s], nums[n])); //fills deck 
	return deck;
}

int main() {
	srand((unsigned) time(NULL));	
	vector<Player> players;
	vector<Player> winners; 
	for(int i=0; i < 5; i++) players.push_back(Player(i));
	vector<Card> deck = deck_gen();
	random_shuffle(begin(deck), end(deck), [](int n) { return rand() % n; }); 	
	for(int i = 0; i < players.size(); i++) for (int j = 0; j < 7; j++) players[i].hand.push_back(deck[7*i + j]);
	while(!isWon(players, winners)) {
		int swap_choice;
		string inp;
		do {
			cout << to_string(players[0]) << endl;
			cout << "Which one to replace? ";
			getline(cin, inp);
			if (inp == "?") for (Player player : players) cout << "Player " << player.num << "   " << to_string(player) << endl;
			swap_choice = inp[0] - 'a';
 			cout << endl;
		} while(inp.size() > 1 || swap_choice < 0 || swap_choice > 7); //do-while loop 
		vector<int> swapped_index; 
		swapped_index.push_back(swap_choice);
		cout << "Hint: player 0 passed " << players[0].hand[swap_choice].name << " to player 1\n";
		for(int i = 1; i < players.size(); i ++) { //figure out which index needs to be swapped and store in swapped_index.
			int s = players[i].to_swap();
			swapped_index.push_back(s);
			cout << "Hint: player " << players[i].num << " passed ";
			cout << players[i].hand[s].name << " to player " << (players[i].num+1) % 5 << "\n";
		}
		for (int i = 0; i < players.size(); i++) {
			/*we need a new loop for the actual swapping becuase we don't want to want Player X's swap to affect Player X+1's choice.
			all swaps need to be done simultaneously, i.e. if Player 0 swaps HK to Player 1, Player 1 can't swap HK that turn.*/
			players[(i+1)%5].hand.push_back(players[i].hand[swapped_index[i]]); //adds swapped card to the next player's hand...
			players[i].hand.erase(players[i].hand.begin() + swapped_index[i]); //...and removes the card from this player's. 
		}

	}
	cout << endl;
	for (Player winner : winners) cout << "My Ship Sails! --- Player " << winner.num << " Won...\n" << endl;
	cout << "[ Final card distributions ]" << endl;
	for (Player player : players) cout << "Player " << player.num << "   " << to_string(player) << endl;
	return 0;
}