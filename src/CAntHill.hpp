//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_CANTHILL_HPP
#define ANTWARS_CANTHILL_HPP

#include <vector>
#include <string>
#include "AMapObject.hpp"
#include "AAnt.hpp"
#include "CAntQueen.hpp"
#include "CAnt.hpp"

using namespace std;

/**
 * Class represents an anthill.
 */
class CAntHill : public AMapObject {
    /**
     * Name of ant hill.
     */
    char name;
public:
    /**
     * Ants in ant hill.
     */
    std::vector<AAnt *> ants;
    /**
     * Ants on the road which were sent by ant hill.
     */
    std::vector<AAnt *> antsOnTheRoad;
    /**
     * Path which will all ants on the road follow.
     */
    std::vector<pair<int, int>> path;
    /**
     * Pointers of ants for memory management purposes.
     */
    std::vector<AAnt *> antsToDelete;
public:
    /**
     * Constructor of CAntHill.
     * @param position
     * @param name
     * @param ants
     */
    CAntHill(const std::pair<int, int> &position, char name, const std::vector<AAnt *> &ants);

    /**
     * Implicit destructor of CAntHill.
     */
    ~CAntHill() override;

    /**
     * Checks if can be stepped on CAntHill. Always returns false.
     * @return bool
     */
    bool canStepOn() const override;

    /**
     * Checks if there are some ants in CAntHill.
     * @return bool
     */
    bool isEmpty() const;

    /**
     * Generates new CAnt in CAntHill. Generates CAntQueen if CAntHill is empty.
     */
    void generateAnt();

    /**
     * Moves CAntHill. Always returns false.
     * @return bool
     */
    bool move() override;

    /**
     * Renders CAntHill to the given window.
     * @param win
     */
    void render(WINDOW *win) const override;

    /**
     * Returns name of the CAntHill
     * @return char
     */
    char getName() const;

    /**
     * Sets path which will be followed by AAnts in CAntHill.
     * @param path
     */
    void setPath(const vector<pair<int, int>> &path);


};


#endif //ANTWARS_CANTHILL_HPP
