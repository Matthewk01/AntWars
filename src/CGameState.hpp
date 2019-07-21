//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_CGAMESTATE_HPP
#define ANTWARS_CGAMESTATE_HPP

#include <vector>
#include <curses.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include "CMap.hpp"
#include "Difficulty.hpp"
#include "CAntHill.hpp"

/**
 * Class represents the running game.
 */
class CGameState {
    CMap *map;
    Difficulty gameDifficulty;
    bool gameEnd;
    WINDOW *gameWindow;
    WINDOW *commandWindow;
    WINDOW *statusWindow;
    std::map<char, char> attackCommands;
    string statusMessage;
public:
    /**
     * Constructor of CGameState
     * @param map
     * @param gameDifficulty
     */
    CGameState(CMap *map, Difficulty gameDifficulty = Difficulty::EASY);

    /**
     * Implicit destructor of CGameState
     */
    ~CGameState() = default;

    /**
     * Initiates the game and starts the game loop.
     */
    void startGame();

    /**
   * Initializes all CAntHills with that many ants.
   * @param count
   */
    void initAntHills(int count);

private:

    /**
     * Game loop.
     */
    void gameLoop();

    /**
     * Creates game window.
     */
    void createGameWindow();

    /**
     * Creates command window.
     */
    void createCommandWindow();

    /**
     * Creates status window.
     */
    void createStatusWindow();

    /**
     * Saves the game to the file.
     */
    void saveGame(const string &fileName);

    /**
     * Updates the status window.
     */
    void updateStatusWindow();

    /**
     * Updates the game window.
     */
    void updateGameWindow();

    /**
     * Updates all parts of the game.
     */
    void update();

    /**
     * Generate that many ants in all CAntHills.
     * @param count
     */
    void generateAnts(int count);

    /**
     * Gets input from user.
     */
    void input();

    /**
     * Handles input from user.
     * @param command
     */
    void handleInput(const string &command);

    /**
     * Updates the command window.
     */
    void updateCommandWindow();

    /**
     * Handles all commands.
     */
    void handleCommands();

    /**
     * Finds needed CAntHill and calls generatePathFromTo() from CMap to generate path for ants.
     * @param from
     * @param to
     */
    void generatePath(char from, char to);

    /**
     * Checks if someone won the game.
     */
    void gameStateCheck();

    /**
     * Initializes the AI.
     */
    void generateAiAttackCommand();

};


#endif //ANTWARS_CGAMESTATE_HPP
