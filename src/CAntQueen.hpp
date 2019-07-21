//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_CANTQUEEN_HPP
#define ANTWARS_CANTQUEEN_HPP


#include "AAnt.hpp"

/**
 * Class represents an antqueen.
 */
class CAntQueen : public AAnt {
public:
    /**
     * Constructor of CAntQueen.
     * @param position
     * @param name
     */
    CAntQueen(const std::pair<int, int> &position, char name);

    /**
     * Implicit destructor of CAntQueen.
     */
    ~CAntQueen() override;

    /**
     * Attacks the given AAnt.
     * @param target
     */
    void attackAnt(AAnt &target) override;

    /**
     * Moves the CAntQueen.
     * @return bool
     */
    bool move() override;

    /**
     * Renders the CAntQueen to the given window.
     * @param win
     */
    void render(WINDOW *win) const override;

};


#endif //ANTWARS_CANTQUEEN_HPP
