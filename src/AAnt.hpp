//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#ifndef ANTWARS_AANT_HPP
#define ANTWARS_AANT_HPP

#include <string>
#include <cctype>
#include <vector>
#include "AMapObject.hpp"

using namespace std;

#define MAX_HP 100
#define DAMAGE 35

/**
 * Class which represent an abstract ant.
 */
class AAnt : public AMapObject {
protected:
    /**
     * Name of an ant.
     */
    char name;
    /**
     * Current health points of an ant.
     */
    int currentHP;
    /**
     * Maximum possible health points of an ant.
     */
    const int maxHP;
    /**
     * Damage of an ant.
     */
    const int damage;
public:
    /**
     * Path which an ant will follow.
     */
    vector<pair<int, int>> path;
    /**
     * Path to the ant hill which will an ant follow.
     */
    vector<pair<int, int>> pathBackToAntHill;

    /**
     * Constructor of AAnt
     * @param position
     * @param name
     * @param currentHp
     * @param maxHp
     * @param damage
     */
    AAnt(const std::pair<int, int> &position, char name, int currentHp = MAX_HP, int maxHp = MAX_HP,
         int damage = DAMAGE);

    /**
     * Implicit destructor of AAnt
     */
    ~AAnt() override = default;

    /**
     * Virtual method to attack an ant.
     * @param target
     */
    virtual void attackAnt(AAnt &target) = 0;

    /**
     * The next position of an ant which moves.
     * @return pair<int,int>
     */
    virtual pair<int, int> nextMovePosition() const;

    /**
     * Checks whether an ant is alive.
     * @return bool
     */
    bool isAlive() const;

    /**
     * Returns name of an ant.
     * @return char
     */
    char getName() const;

    /**
     * Returns current health points of an ant.
     * @return int
     */
    int getHealthPoints() const;

    /**
     * Sets current hp to the maximum.
     */
    void setFullHP();

    /**
     * Checks if can be stepped on an ant.
     * @return bool
     */
    bool canStepOn() const override;

    /**
     * Sets path which will an ant follow.
     * @param path
     */
    void setPath(const vector<pair<int, int>> &path);

    /**
     * Sets path which will an ant follow when going back to the ant hill.
     * @param pathBackToAntHill
     */
    void setPathBackToAntHill(const vector<pair<int, int>> &pathBackToAntHill);

    /**
     * Sets current ant health points.
     * @param currentHp
     */
    void setCurrentHp(int currentHp);

    /**
     * Returns current health points of an ant.
     * @return
     */
    int getCurrentHp() const;

    /**
     * Returns maximum health points which an ant can have.
     * @return
     */
    int getMaxHp() const;

};


#endif //ANTWARS_AANT_HPP
