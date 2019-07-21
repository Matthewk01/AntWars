//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_CANT_HPP
#define ANTWARS_CANT_HPP


#include "AAnt.hpp"

/**
 * Class represents an ant.
 */
class CAnt : public AAnt {
public:

    /**
     * Constructor of CAnt
     * @param position
     * @param name
     */
    CAnt(const std::pair<int, int> &position, char name);

    /**
     * Implicit destructor of CAnt.
     */
    ~CAnt() override;

    /**
     * Attacks the given AAnt.
     * @param target
     */
    void attackAnt(AAnt &target) override;

    /**
     * Moves the CAnt.
     * @return bool
     */
    bool move() override;

    /**
     * Renders the CAnt to the given window.
     * @param win
     */
    void render(WINDOW *win) const override;

};


#endif //ANTWARS_CANT_HPP
