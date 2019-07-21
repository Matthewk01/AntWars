//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//


#include "CAntQueen.hpp"
#include "CAntHill.hpp"

CAntQueen::CAntQueen(const std::pair<int, int> &position, char name) : AAnt(
        position, name, 350, 350, 10) {}

CAntQueen::~CAntQueen() = default;

void CAntQueen::attackAnt(AAnt &target) {
    int luck = rand() % 5;
    if (luck == 5) {
        target.setCurrentHp(target.getCurrentHp() - (damage + (rand() % 30)));
        currentHP += rand() % 20;
        if (currentHP > maxHP)
            setCurrentHp(maxHP);
    }
    target.setCurrentHp(target.getCurrentHp() - (damage + (rand() % 10)));
    currentHP -= ((damage - 30) + (rand() % 5));
}

bool CAntQueen::move() {
    if (path.empty())
        return false;
    pair<int, int> moveTo = path.back();
    path.pop_back();
    setPosition(moveTo);
    return true;
}

void CAntQueen::render(WINDOW *win) const {
    if(this->isAlive()) {
        string nameStr{};
        nameStr.push_back(name);
        mvwprintw(win, getX() + 1, getY() + 1, nameStr.c_str());
    } else {
        mvwprintw(win, getX() + 1, getY() + 1, " ");
    }
}

