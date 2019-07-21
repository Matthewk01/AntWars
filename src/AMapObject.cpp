//
// Created by matej on 10/05/2019.
//

#include "AMapObject.hpp"

AMapObject::AMapObject(std::pair<int, int> position) : position(std::move(position)) {}

int AMapObject::getX() const {
    return position.first;
}

int AMapObject::getY() const {
    return position.second;
}

std::pair<int, int> AMapObject::getPosition() const {
    return position;
}

void AMapObject::setX(int x) {
    position.first = x;
}

void AMapObject::setY(int y) {
    position.second = y;
}

void AMapObject::setPosition(std::pair<int, int> pos) {
    this->position = pos;
}

