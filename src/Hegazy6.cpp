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

const int R = 200;
void display_circles(Simple_window& win, int rings) {
    vector<Circle*> circles;
    Text t(Point(180,500), "After " + to_string(rings) + " rings generation");
    t.set_font(Font::helvetica);
    t.set_color(Color::black);
    t.set_font_size(15);
    win.attach(t);
    win.set_label("Programming Assignment #6");
    Circle base_circle(Point(250,250), R);
    base_circle.set_color(rand() % 256);
    win.attach(base_circle);

    for(int i = 0; i < rings; i++) {
        int n = rand() % 47 + 2; //max of 48 chosen arbitrarily
        for (int k = 0; k < n; k++) {
            double r = R*sin(M_PI/n)/(1 + sin(M_PI/n)); //little radii
            double x = 250 + (200-r)*cos(k*2*M_PI/n); //x and y positions
            double y = 250 + (200-r)*sin(k*2*M_PI/n);
            Circle* ck = new Circle(Point(x,y),r);
            ck->set_color(rand() % 256);
            win.attach(*ck);
            circles.push_back(ck);
        }
    }
    win.wait_for_button();
    for (int i = 0; i < circles.size(); i++)  win.detach(*(circles[i]));
    win.detach(base_circle);
    for (int i = 0; i < circles.size(); i++) delete circles[i];
}
int main () {
    std::srand(std::time(0)); // use current time as seed for random generator
    int rings;
    cout << "How many rings should there be?" << endl;
    cin >> rings;
    if (rings <= 0) {exit(1);}
    Point tl(0,0);    // top-left corner of our window
    Simple_window win(tl,500,500,"Programming Assignment #6");
    while (rings >= 0) {
        display_circles(win, rings);
        cout << "How many rings should there be?" << endl;
        cin >> rings;
    }
    win.hide();
    return 0;
}
//----
