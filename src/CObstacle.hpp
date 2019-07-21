//
// Created by matej on 10/05/2019.
//

#ifndef ANTWARS_COBSTACLE_HPP
#define ANTWARS_COBSTACLE_HPP


#include "AMapObject.hpp"

/**
 * Class represents a tile of the map which cannot be stepped on.
 */
class CObstacle : public AMapObject {
private:
    char name;
public:
    /**
     * Constructor of CObstacle
     * @param position
     */
    explicit CObstacle(const std::pair<int, int> &position);

    /**
     * Implicit destructor of CObstacle
     */
    ~CObstacle() override = default;

    /**
     *  Checks if can be steppedOn CObstacle
     * @return bool
     */
    bool canStepOn() const override;

    /**
     * CObstacle cannot be moved with. Always returns false.
     * @return bool
     */
    bool move() override;

    /**
     * Draws CObstacle to the given window
     * @param win
     */
    void render(WINDOW *win) const override;
};


#endif //ANTWARS_COBSTACLE_HPP
