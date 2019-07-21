//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_CMAP_HPP
#define ANTWARS_CMAP_HPP


#include <vector>
#include <curses.h>
#include <map>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <algorithm>
#include "AMapObject.hpp"
#include "CAntHill.hpp"

using namespace std;

/**
 * Class represents a map of the game.
 */
class CMap {
    std::vector<std::vector<AMapObject *>> map;
    int width;
    int height;
    std::vector<CAntHill *> playerHills;
    std::vector<CAntHill *> aiHills;
    std::vector<AMapObject *> objectsToDelete;
    string fileName;
public:
    /**
     * Constructor of CMap
     * @param map
     * @param width
     * @param height
     * @param playerHills
     * @param aiHills
     * @param objectsToDelete
     * @param fileName
     */
    CMap(const vector<vector<AMapObject *>> &map, int width, int height, const vector<CAntHill *> &playerHills,
         const vector<CAntHill *> &aiHills, const vector<AMapObject *> &objectsToDelete, const string &fileName);

    /**
     * Destructor of CMap which deletes all map objects.
     */
    ~CMap();

    /**
     * Renders the map to the window.
     * @param gameWindow
     */
    void printMap(WINDOW *gameWindow) const;

    /**
     * Returns width of the map.
     * @return int
     */
    int getWidth() const;

    /**
     * Returns height of the map
     * @return int
     */
    int getHeight() const;

    /**
     * Returns vector of playerHills.
     * @return vector<CAntHill*>
     */
    const vector<CAntHill *> &getPlayerHills() const;

    /**
     * Returns vector of AIHills
     * @return vector<CAntHill*>
     */
    const vector<CAntHill *> &getAiHills() const;

    /**
     * Generates and returns path from CAntHill to another using BFS.
     * @param from
     * @param to
     * @return vector<pair<int,int>>
     */
    vector<pair<int, int>> generatePathFromTo(const CAntHill &from, const CAntHill &to);

    /**
     * Handles attack command from one CAntHill to another.
     * @param from
     * @param to
     */
    char manageAntHillAttacks(CAntHill &from, CAntHill &to);

    /**
     * Returns name of the file with map.
     * @return const string
     */
    const string &getFileName() const;

    /**
     * Sets playerHills attribute of CMap.
     * @param playHills
     */
    void setPlayerHills(const vector<CAntHill *> &playHills);

    /**
     * Sets aiHills attribute of CMap.
     * @param artificalIHills
     */
    void setAiHills(const vector<CAntHill *> &artificalIHills);

};


#endif //ANTWARS_CMAP_HPP
