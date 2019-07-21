//
// Created by matej on 10/05/2019.
//

#include "CObstacle.hpp"

CObstacle::CObstacle(const std::pair<int, int> &position) : AMapObject(position), name('#') {}

bool CObstacle::canStepOn() const {
    return false;
}

bool CObstacle::move() {
    return false;
}

void CObstacle::render(WINDOW *win) const {
    mvwprintw(win, getX() + 1, getY() + 1, "#");
}
