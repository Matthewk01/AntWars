#include <utility>

//
// Created by matej on 10/05/2019.
//

#ifndef ANTWARS_AMAPOBJECT_HPP
#define ANTWARS_AMAPOBJECT_HPP


#include <curses.h>

using namespace std;

/**
 * Class represents an abstract map object.
 */
class AMapObject {
protected:
    /**
     * Position of map object.
     */
    std::pair<int, int> position;
public:
    /**
     * Constructor of AMapObject
     * @param position
     */
    explicit AMapObject(std::pair<int, int> position);

    /**
     * Implicit destructor of AMapObjects.
     */
    virtual ~AMapObject() = default;

    /**
     * Pure virtual method which checks if can be stepped on an object.
     * @return
     */
    virtual bool canStepOn() const = 0;

    /**
     * Pure virtual method to move the object.
     * @return bool
     */
    virtual bool move() = 0;

    /**
     * Pure virtual method to render an object to the given window.
     * @param win
     */
    virtual void render(WINDOW *win) const = 0;

    /**
     * Return x coordinate of an object.
     * @return int
     */
    int getX() const;

    /**
     * return y coordinate of an object.
     * @return int
     */
    int getY() const;

    /**
     * Returns both coordinates of an object.
     * @return pair<int,int>
     */
    std::pair<int, int> getPosition() const;

    /**
     * Sets x coordinate of an object.
     * @param x
     */
    void setX(int x);

    /**
     * Sets y coordinate of an object.
     * @param y
     */
    void setY(int y);

    /**
     * Sets both coordinates of an object.
     * @param pos
     */
    void setPosition(std::pair<int, int> pos);

};


#endif //ANTWARS_AMAPOBJECT_HPP
