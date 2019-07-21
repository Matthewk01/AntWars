//
// Created by matej on 10/05/2019.
//

#ifndef ANTWARS_CEMPTYTILE_HPP
#define ANTWARS_CEMPTYTILE_HPP


#include "AMapObject.hpp"

/**
 * Class represents an empty tile of the game.
 */
class CEmptyTile : public AMapObject {
private:
    char name;
public:
    /**
     * Constructor of CEmptyTile.
     * @param position
     */
    explicit CEmptyTile(const std::pair<int, int> &position);

    /**
     * Implicit destructor of CEmptyTile.
     */
    ~CEmptyTile() override = default;

    /**
     * Checks if can be stepped on CEmptyTile. Always returns true.
     * @return bool
     */
    bool canStepOn() const override;

    /**
     * Can if an CEmptyTile can be moved. Always returns false.
     * @return bool
     */
    bool move() override;

    /**
     * Renders CEmptyTile to the given window.
     * @param win
     */
    void render(WINDOW *win) const override;
};


#endif //ANTWARS_CEMPTYTILE_HPP
