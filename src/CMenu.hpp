//
// Created by matej on 10/05/2019.
//

#ifndef ANTWARS_CMENU_HPP
#define ANTWARS_CMENU_HPP

#include <iostream>
#include <fstream>
#include <array>
#include <curses.h>
#include <unistd.h>
#include <sstream>
#include "CGameState.hpp"
#include "Difficulty.hpp"
#include "CMap.hpp"
#include "CObstacle.hpp"
#include "CEmptyTile.hpp"

using namespace std;


/**
 * Class represents the menu of the game.
 */
class CMenu {
    /**
     * Pointer to the game state.
     */
    CGameState *game;
    /*
     * Pointer to the map.
     */
    CMap *map;
    /**
     * Difficulty of the game.
     */
    Difficulty chosenDifficulty;

public:
    /**
     * Constructor of CMenu
     */
    CMenu() : game(nullptr), map(nullptr), chosenDifficulty(Difficulty::EASY) {}

    /**
     * Destructor of CMenu which deletes game and map attributes.
     */
    ~CMenu() {
        delete game;
        delete map;
    }

    /**
     * Displays the menu and initiates the main loop.
     */
    void showMenu();

    /**
     * Menu to choose difficulty of the game
     * @param menuWin
     */
    void chooseDifficulty(WINDOW *menuWin);

    /**
     * Loads map file with given name.
     * @param win
     * @param mapFileName
     */
    void loadMap(WINDOW *win, const string &mapFileName);

    /**
     * Creates new game, if map is null, loads default map file.
     */
    void newGame();

    /**
     * Loads game file and starts the game.
     * @param win
     */
    void loadGame(WINDOW *win);

    /**
     * Shutdowns the game.
     */
    void exitGame();

private:
    /**
     * Prints title of the game to given coordinates.
     * @param y
     * @param x
     */
    void printGameTitle(int y, int x) const;

    /**
     * Renders input window.
     * @param menu
     * @return string
     */
    string inputMenu(WINDOW *menu);

    /**
     * Prints message to the window.
     * @param win
     * @param message
     */
    void statusMessage(WINDOW *win, const string &message) const;
};


#endif //ANTWARS_CMENU_HPP
