
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
vector<Circle*> circles;
const int R = 200;
bool display_circles(Simple_window& win, int rings) {
    Text t(Point(180,500), "After " + to_string(rings) + " rings generation");
    win.attach(t);
    t.set_font(Font::helvetica);
    t.set_color(Color::black);
    t.set_font_size(15);
    win.attach(t);
    for(int i = 0; i < rings; i++) {
        int n = rand() % 47 + 2; //max of 48 was chosen arbitrarily
        for(int k = 0; k < n; k++) {
            double r = R*sin(M_PI/n)/(1 + sin(M_PI/n)); //little radii
            double x = 250 + (200-r)*cos(k*2*M_PI/n); //x and y positions
            double y = 250 + (200-r)*sin(k*2*M_PI/n);
            Circle* ck = new Circle(Point(x,y),r);
            ck->set_color(rand() % 256);
            circles.push_back(ck);
        }
    }
    for (int i = 0; i < circles.size(); i++) {
        win.attach(*circles[i]);
    }
    win.wait_for_button();

    for (int i = 0; i < circles.size(); i++) {
        win.detach(*circles[i]);
    }
    win.detach(t);
    return true;
}
int main () {
    std::srand(std::time(0)); // use current time as seed for random generator
    int rings;
    cout << "How many rings should there be?" << endl;
    cin >> rings;
    if (rings < 0) {exit(1);}

    Point tl(0,0);    // top-left corner of our window
    Simple_window win(tl,500,500,"Programming Assignment #6");
    Circle base_circle(Point(250,250), R);
    base_circle.set_color(rand() % 256);
    win.attach(base_circle);
    while(display_circles(win, rings)) rings++;
    win.detach(base_circle);
    for (int i = 0; i < circles.size(); i++) delete circles[i];
}
//------------------------------------------------------------------------------
