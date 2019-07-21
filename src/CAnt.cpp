//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#include "CAnt.hpp"

CAnt::CAnt(const std::pair<int, int> &position, char name) : AAnt(position, name) {}

CAnt::~CAnt() = default;

void CAnt::attackAnt(AAnt &target) {
    target.setCurrentHp(target.getCurrentHp() - (damage + (rand() % 20)));
    currentHP -= damage + (rand() % 20);
}

bool CAnt::move() {
    if (path.empty())
        return false;
    pair<int, int> moveTo = path.back();
    path.pop_back();
    setPosition(moveTo);
    return true;
}

void CAnt::render(WINDOW *win) const {
    if(this->isAlive()) {
        string nameStr{};
        nameStr.push_back(name);
        mvwprintw(win, getX() + 1, getY() + 1, nameStr.c_str());
    } else {
        mvwprintw(win, getX() + 1, getY() + 1, " ");
    }
}

