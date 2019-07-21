//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#include "CAntHill.hpp"

CAntHill::CAntHill(const std::pair<int, int> &position, char name, const std::vector<AAnt *> &ants) : AMapObject(
        position), name(name), ants(ants), antsOnTheRoad{}, antsToDelete{} {}


CAntHill::~CAntHill() {
    for (auto &ant: antsToDelete) {
        delete ant;
    }
}


bool CAntHill::canStepOn() const {
    return false;
}

bool CAntHill::move() {
    return false;
}

bool CAntHill::isEmpty() const {
    return ants.empty();
}

void CAntHill::render(WINDOW *win) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i == 0 && j == 0) {
                mvwprintw(win, getX() + 1 + i, getY() + 1 + j, "O");
            } else if (i == 1) {
                string antHillRow{};
                antHillRow.push_back('^');
                antHillRow.push_back(name);
                if (ants.size() < 10) {
                    antHillRow.push_back('0');
                }
                antHillRow += std::to_string(ants.size());
                antHillRow.push_back('^');
                mvwprintw(win, getX() + 1 + i, getY() + 1 + j, antHillRow.c_str());
                break;
            } else {
                mvwprintw(win, getX() + 1 + i, getY() + 1 + j, "^");
            }
        }
    }
}

char CAntHill::getName() const {
    return name;
}

void CAntHill::generateAnt() {
    AAnt *ant;
    if (ants.empty()) {
        ant = new CAntQueen(make_pair(getX() - 1, getY()), getName());
    } else {
        ant = new CAnt(make_pair(getX() - 1, getY()), tolower(getName()));
    }
    ants.push_back(ant);
    antsToDelete.push_back(ant);
}

void CAntHill::setPath(const vector<pair<int, int>> &path) {
    CAntHill::path = path;
}


