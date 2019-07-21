//
// Created by matej on 10/05/2019.
//

#include "CEmptyTile.hpp"

CEmptyTile::CEmptyTile(const std::pair<int, int> &position) : AMapObject(position), name(' ') {}

bool CEmptyTile::canStepOn() const {
    return true;
}

bool CEmptyTile::move() {
    return false;
}

void CEmptyTile::render(WINDOW *win) const {
    mvwprintw(win, getX() + 1, getY() + 1, " ");
}

