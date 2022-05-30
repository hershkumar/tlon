#include <iostream>
#include <curses.h>
#include <panel.h>
#include "math.cpp"
#include "gates.cpp"
#include "config.cpp"
#include "log.cpp"
// height and width of the terminal screen
int term_height;
int term_width;
//Instantiate the logger object with a dummy filename
Logger log_obj = Logger();

// setup the curses session
int curses_setup()
{
	initscr();
	// start color system
	start_color();
	use_default_colors();
	// refresh screen
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, true);

	// get the terminal size
	getmaxyx(stdscr, term_width, term_height);
	getch();

	return 0;
}
int setup()
{
	curses_setup();
	read_config();
	
	// open the log file
	Logger log_obj = Logger(get_log_filename());
	return 0;
}
// function that shuts down the program
int shutdown()
{
	// clear the screen
	clear();
	// end the ncurses session
	endwin();
	// Close the config file
	config_close();
	// Close the log file
	log_obj.log_string("Closing Logger object");
	log_obj.close();
	return 0;
}

int main(int argc, char const *argv[])
{
	// Call the setup function
	setup();
   	// main control body goes here
	
	// shut down the program
	log_obj.log_string("Shutting down tlon...");
	shutdown();
	return 0;
}
