//MUST BE COMPILED WITH COMPILER FLAG -std=c++11!! Otherwise, lambda functions will not work!
#include "Graph.h"
#include "std_lib_facilities.h"
#include "GUI.h"
#include <iostream>
#include <string>
#include <ctime>
#include <math.h>

using namespace Graph_lib;
using namespace std;

struct Our_window : Graph_lib::Window {
	Our_window(Point xy, int w, int h, const string& title);
	private:
		Button drop_button;
		Button quit_button;
		Button menu_button;
		In_box drop_count;
		Menu motion_menu;
		Rectangle text_box; //green box
		Text approx_pi; //white text on green box that signifies how many intersections we have and what pi is approximated to
		bool listed = false; //whether we list drop count or not
		vector<Line*> lines; //needles
		vector<Text*> drop_order; //drop count of needles
		const int LEN = 170;

		void menu_pressed() {menu_button.hide(); motion_menu.show();};
		void hide_menu() {motion_menu.hide(); detach(text_box); detach(approx_pi); menu_button.show();};
		//it also makes sense to get rid of the text showing what pi is approximated to when we press a new button, unless we pressed count
		void show_text() {for (auto order : drop_order) attach(*order);};
		void hide_text() {for (auto order : drop_order) detach(*order);};
		int intersects(); //how many needle intersections we have
		Point center_pt(Line* line);
		void drop();
		void count();
		void rotate();
		void unlist();
		void quit();

};

Our_window::Our_window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	 drop_button(Point(x_max() - 100, 0), 50, 20, "Drop", [](Address, Address pw) { reference_to<Our_window>(pw).drop();}),
	 quit_button(Point(x_max() - 50, 0), 50, 20, "Quit", [](Address, Address pw) { reference_to<Our_window>(pw).quit();}),
	 menu_button(Point(x_max() - 100, 20), 100, 20, "Motion Menu", [](Address, Address pw) {reference_to<Our_window>(pw).menu_pressed();}),
	 drop_count(Point(x_max() - 200, 0), 100, 20, "Enter drop count"),
	 motion_menu(Point(x_max() - 70, 20), 70, 20, Menu::vertical, "Motion Menu"),
	 text_box(Point(x_max() / 2 - 200, y_max()/2 - 50), 460, 20),
	 approx_pi(Point(x_max() / 2- 200, y_max()/2 - 35), "Found 0 crossed needles, and estimated pi is nan!")

{
	motion_menu.attach(new Button(Point(0,0),0,0, "Count", [](Address, Address pw) { reference_to<Our_window>(pw).count();}));
	motion_menu.attach(new Button(Point(0,0),0,0, "Rotate", [](Address, Address pw) { reference_to<Our_window>(pw).rotate();}));
	motion_menu.attach(new Button(Point(0,0),0,0, "(Un)list", [](Address, Address pw) { reference_to<Our_window>(pw).unlist();}));
	text_box.set_fill_color(Color::dark_green);
	approx_pi.set_color(Color::white);
	approx_pi.set_font_size(17);
	attach(drop_button);
	attach(quit_button);
	attach(drop_count);
	attach(motion_menu);
	motion_menu.hide();
	attach(menu_button);
}
//2 supplementary functions :
int Our_window::intersects() {
	int crossed = 0;
		for (auto line : lines) {
			int x1 = line->point(0).x; int x2 = line->point(1).x;
			for (int i = 0; i < 6; i++) //one line for-loop, don't need brace
				if ((LEN*(i+1) < x2 && LEN*(i+1) > x1) || (LEN*(i+1) > x2 && LEN*(i+1) < x1)) { //LEN*(i+1) is the x_val of wood
					//for the needle to intersect, either x1 is on the left side of "wood" and x2 on the right or vica versa
					line->set_color(Color::red);
					crossed++;
				}
		}
	return crossed;
}
Point Our_window::center_pt(Line* line) {
	int c_x = (int)round((line->point(0).x + line->point(1).x) / 2.0);
	int c_y = (int)round((line->point(0).y + line->point(1).y) / 2.0);
	return Point(c_x, c_y);
}
void Our_window::drop() {
	int stick_amt = drop_count.get_int();
	if (stick_amt < 0) return; //user did not input an integer
	int min_x = LEN; int max_x = x_max() - LEN; int x_range = max_x - min_x + 1; //x cannot go out of bounds
	int min_y = LEN; int max_y = y_max() - LEN; int y_range = max_y - min_y + 1; //may get better results if we limit y, as well
	for (auto line : lines) {line->set_color(Color::black);} //makes sense to recolor red lines on new drop
	for (int i = 0; i < stick_amt; i++) {
		int rand_angle = rand() % 360;
		int x1 = min_x + rand()  % x_range; int y1 = min_y + rand() % y_range;
		int x2 = x1 + (int)round(cos(rand_angle)*LEN); int y2 = y1 + (int)round(sin(rand_angle)*LEN);
		Line* line = new Line(Point(x1,y1), Point(x2,y2)); lines.push_back(line);
		Text* text = new Text(center_pt(line), to_string(lines.size() + i)); drop_order.push_back(text);
		attach(*line); if (listed) attach(*text); //if listed is already on, it makes sense to list the new needles
	}
	hide_menu();
	redraw();
}
void Our_window::count() {
	int crossed = intersects();
	double estimated_pi = 2.0 * lines.size() / crossed; estimated_pi = round(estimated_pi*100000)/100000; //rounds to 5 decimal pts
	string fin = "Found  " + to_string(crossed) + " crossed needles, and estimated pi is " + to_string(estimated_pi) + "!";
	approx_pi.set_label(fin);
	hide_menu();
	attach(text_box); //hide_menu() hides the pi approximation text because this is what we want in every other case, so we reattach here
	attach(approx_pi);
}
void Our_window::rotate() {
	for (int i = 0; i < lines.size(); i++) {
		int theta = rand() % 360; Point ctr = center_pt(lines[i]);
		int x1 =  ctr.x + (int)round(cos(theta)*(LEN/2)); int x2 = ctr.x - (int)round(cos(theta)*(LEN/2));
		int y1 = ctr.y + (int)round(sin(theta)*(LEN/2)); int y2 = ctr.y - (int)round(sin(theta)*(LEN/2));
		Line* new_Line = new Line(Point(x1,y1), Point(x2,y2)); //just completely set to a new line with the same center and random angle
		detach(*lines[i]); delete lines[i]; lines[i] = new_Line; attach(*lines[i]);
		//because C++ cannot change the object that a ptr refers to, we delete obj AND ptr, and replace array contents with new one
	}
	hide_menu();
}
void Our_window::unlist() {
	for (auto line : lines) {line->set_color(Color::black);} //makes sense to recolor red lines on unlist
	listed ?  hide_text() : show_text();
	listed = !listed;
	hide_menu();
}
void Our_window::quit() {
	for (auto line : lines) {detach(*line); delete line;} detach(drop_button); detach(menu_button);
	detach(quit_button); detach(drop_count); detach(motion_menu); detach(text_box); detach(approx_pi);
	for (auto text : drop_order) {detach(*text); delete text;}
	for (int i = 0; i < motion_menu.selection.size(); i++) {detach(motion_menu.selection[i]);}
	hide();
}
int main () {
    std::srand(std::time(0)); // use current time as seed for random generator
    Our_window win(Point(0,0), 1190, 740, "Programming Assignment #7");
    vector<Line*> init_lines; //green lines
    /*Even though these lines don't reach the top/bottom of the window, your program shows needles at top/bottom intersecting with them
      regardless. I know it's to avoid the green lines from overlapping with the text box, but it still looks unintuitive to me.*/
    for (int i = 0; i < 6; i++) {
    	Line* this_line = new Line(Point(170*(i+1),30), Point(170*(i+1),y_max() - 50));
    	this_line->set_color(Color::dark_green);
    	init_lines.push_back(this_line);
    }
    for (auto line_ref : init_lines) win.attach(*line_ref);
    return gui_main();
    for (auto line_ref : init_lines) delete line_ref;
}
