
//
// This is example code from Chapter 12.7.6 "Rectangles" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"    // if we want that "Next" button
#include "Graph.h"
#include "std_lib_facilities.h"
#include <iostream>
#include <string>
#include <ctime>
#include <math.h>

using namespace Graph_lib;
using namespace std;

//------------------------------------------------------------------------------

int main () {
    std::srand(std::time(0)); // use current time as seed for random generator
	int rings;
	cout << "How many rings should there be?" << endl;
	cin >> rings;
	if (rings < 0) {exit(1);}
try
{
    Point tl(0,0);    // top-left corner of our window
    Simple_window win(tl,500,500,"Programming Assignment #6");
    // screen coordinate tl for top-left corner
    // window size(600*400)
    // title: Canvas

    Text t(Point(180,500), "After " + to_string(rings) + " rings generation");
    win.attach(t);
    t.set_font(Font::helvetica);
    t.set_color(Color::black);
    t.set_font_size(15);
    int R = 200; //big radius
    Circle* circle = new Circle(Point(250,250),R);
    circle->set_color(rand() % 256);
    win.attach(*circle);
   for(int i = 0; i < rings; i++) {
		int n = rand() % 47 + 2; //max of 48 was chosen arbitrarily
		for(int k = 0; k < n; k++) {
			double r = R*sin(M_PI/n)/(1 + sin(M_PI/n)); //little radii
			double x = 250 + (200-r)*cos(k*2*M_PI/n); //x and y positions
			double y = 250 + (200-r)*sin(k*2*M_PI/n);
			Circle* ck = new Circle(Point(x,y),r);
			ck->set_color(rand() % 256);
			win.attach(*ck);
		}
	}

    while(win.wait_for_button()) {
    	int n = rand() % 47 + 2; //max of 48 was chosen arbitrarily
    	for(int k = 0; k < n; k++) {
    		double r = R*sin(M_PI/n)/(1 + sin(M_PI/n)); //little radii
    		double x = 250 + (200-r)*cos(k*2*M_PI/n); //x and y positions
    		double y = 250 + (200-r)*sin(k*2*M_PI/n);
    		Circle* ck = new Circle(Point(x,y),r);
    		ck->set_color(rand() % 256);
    		win.attach(*ck);
    	}
    	rings++;
    	t.set_label("After " + to_string(rings) + " rings generation");
    }
}
catch(exception& e) {
    // some error reporting
    return 1;
}
catch(...) {
    // some more error reporting
    return 2;
}
}
//------------------------------------------------------------------------------
