//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#include "AAnt.hpp"

AAnt::AAnt(const std::pair<int, int> &position, char name, int currentHp, const int maxHp,
           const int damage)
        : AMapObject(position), name(name), currentHP(currentHp), maxHP(maxHp), damage(damage), path{},
          pathBackToAntHill{} {}

bool AAnt::isAlive() const {
    return currentHP > 0;
}

char AAnt::getName() const {
    return name;
}

int AAnt::getHealthPoints() const {
    return currentHP;
}

void AAnt::setFullHP() {
    currentHP = maxHP;
}

bool AAnt::canStepOn() const {
    return false;
}

void AAnt::setPath(const vector<pair<int, int>> &path) {
    AAnt::path = path;
}

void AAnt::setPathBackToAntHill(const vector<pair<int, int>> &pathBackToAntHill) {
    AAnt::pathBackToAntHill = pathBackToAntHill;
}

pair<int, int> AAnt::nextMovePosition() const {
    if (path.empty())
        return pair<int, int>{};
    return path.back();
}

void AAnt::setCurrentHp(int currentHp) {
    currentHP = currentHp;
}

int AAnt::getCurrentHp() const {
    return currentHP;
}

int AAnt::getMaxHp() const {
    return maxHP;
}




