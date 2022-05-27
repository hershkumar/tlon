#include <iostream>
#include <curses.h>
int term_height;
int term_width;

// setup the curses session
int curses_setup(){
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

    // make a window the size of the screen
    WINDOW * win = newwin(term_height, term_width, 0, 0);

    return 0;
}

// function that shuts down the program
int shutdown(){
    // end the ncurses session
    endwin();
    return 0;
}

int main(int argc, char const *argv[]){
    // set the curses system up
    curses_setup();
    
    // shut down the program
    shutdown();
    return 0;
}
