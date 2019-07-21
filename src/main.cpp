//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <array>
#include "CMenu.hpp"

using namespace std;

void initNcurses();

int main() {

    // initscr(), endwin(), move(), mvprintw(), refresh(), clear(), getch(), wborder(), box(), newwin(), wrefresh(), attron(), attroff()
    // Attrs: A_REVERSE, A_BOLD, init_pair()
    // keypad() so we can use arrow keys
    srand(time(NULL));
    initNcurses();
    CMenu gameMenu{};
    gameMenu.showMenu();
    endwin();
    return 0;
}

void initNcurses() {
    initscr();
    cbreak();
    noecho();
}
