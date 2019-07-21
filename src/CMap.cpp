//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//


#include "CMap.hpp"
#include "CEmptyTile.hpp"

CMap::CMap(const vector<vector<AMapObject *>> &map, int width, int height, const vector<CAntHill *> &playerHills,
           const vector<CAntHill *> &aiHills, const vector<AMapObject *> &objectsToDelete, const string &fileName)
        : map(map), width(width),
          height(height),
          playerHills(playerHills),
          aiHills(aiHills),
          objectsToDelete(
                  objectsToDelete), fileName(fileName) {}

void CMap::printMap(WINDOW *gameWindow) const {
    for (auto &i : map) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->render(gameWindow);
            }
        }
    }
    wrefresh(gameWindow);
}

CMap::~CMap() {
    for (auto &a1 : playerHills) {
        delete a1;
    }

    for (auto &a2 : aiHills) {
        delete a2;
    }

    for (auto object : objectsToDelete) {
        delete object;
    }
}

int CMap::getWidth() const {
    return width;
}

int CMap::getHeight() const {
    return height;
}

const vector<CAntHill *> &CMap::getPlayerHills() const {
    return playerHills;
}

const vector<CAntHill *> &CMap::getAiHills() const {
    return aiHills;
}

vector<pair<int, int>> CMap::generatePathFromTo(const CAntHill &from, const CAntHill &to) {


    if (&from == &to)
        return vector<pair<int, int>>();

    const array<pair<int, int>, 5> validMoves = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)};
    std::map<pair<int, int>, bool> visited;
    std::map<pair<int, int>, pair<int, int>> prevNode;
    std::queue<pair<int, int>> queue;

    // BFS
    queue.push(from.getPosition());
    visited[from.getPosition()] = true;
    while (!queue.empty()) {
        pair<int, int> node = queue.front();
        queue.pop();

        for (const auto &validMove : validMoves) {
            pair<int, int> nextMove = make_pair(node.first + validMove.first, node.second + validMove.second);
            if ((map[nextMove.first][nextMove.second] == nullptr || map[nextMove.first][nextMove.second]->canStepOn() ||
                 nextMove == to.getPosition()) &&
                !visited[nextMove]) {
                queue.push(nextMove);
                visited[nextMove] = true;
                prevNode[nextMove] = node;
            }
        }
    }

    // Reconstruct path from target to beginning
    vector<pair<int, int>> path{};
    pair<int, int> zeroPoint = make_pair(0, 0);
    for (pair<int, int> at = to.getPosition(); at != zeroPoint; at = prevNode[at]) {
        path.push_back(at);
    }
//    std::reverse(path.begin(), path.end());

    if (path[path.size() - 1] == from.getPosition()) {
        path.pop_back();
        return path;
    }

    return vector<pair<int, int>>();
}

char CMap::manageAntHillAttacks(CAntHill &from, CAntHill &to) {
    // Check spawn collision and anthill emptiness
    if (from.path.empty()) {
        return from.getName();
    }

    // Spawn new ant if there is a place to.
    if ((map[from.getX() - 1][from.getY()] != nullptr) ? map[from.getX() - 1][from.getY()]->canStepOn() : true) {
        if (!from.ants.empty()) {
            AAnt *ant = from.ants.back();
            ant->setCurrentHp(ant->getMaxHp());
            from.ants.pop_back();
            ant->path = from.path;
            from.antsOnTheRoad.push_back(ant);
        }
    }

    for (auto &antToMove : from.antsOnTheRoad) {
        if (!antToMove->isAlive())
            continue;
        pair<int, int> nextPosition = antToMove->nextMovePosition();

        // Check collisions
        if (map[nextPosition.first][nextPosition.second] == nullptr) {
            map[antToMove->getX()][antToMove->getY()] = nullptr;
            antToMove->move();
            map[antToMove->getX()][antToMove->getY()] = antToMove;
        } else if (map[nextPosition.first][nextPosition.second]->canStepOn()) {
            map[antToMove->getX()][antToMove->getY()] = nullptr;
            antToMove->move();
            map[antToMove->getX()][antToMove->getY()] = antToMove;
        }
        // Find nearby enemy ants and attack
        const array<pair<int, int>, 4> attackDirections = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0),
                                                           make_pair(0, -1)};
        // For each direction look if enemy ant is nearby and attack him.
        for (auto &direction : attackDirections) {
            for (auto &myHill : playerHills) {
                if (myHill == &from) {
                    for (auto &enemyHill : aiHills) {
                        for (auto &ant : enemyHill->antsOnTheRoad) {
                            if (map[antToMove->getX() + direction.first][antToMove->getY() + direction.second] == ant) {
                                antToMove->attackAnt(*ant);
                                if (!antToMove->isAlive()) {
                                    map[antToMove->getX()][antToMove->getY()] = nullptr;
                                }
                                if (!(ant->isAlive())) {
                                    map[ant->getX()][ant->getY()] = nullptr;
                                }
                            }
                        }
                    }
                }
            }
            for (auto &aiHill : aiHills) {
                if (aiHill == &from) {
                    for (auto &enemyHill : playerHills) {
                        for (auto &ant : enemyHill->antsOnTheRoad) {
                            if (map[antToMove->getX() + direction.first][antToMove->getY() + direction.second] == ant) {
                                antToMove->attackAnt(*ant);
                                if (!antToMove->isAlive()) {
                                    map[antToMove->getX()][antToMove->getY()] = nullptr;
                                }
                                if (!(ant->isAlive())) {
                                    map[ant->getX()][ant->getY()] = nullptr;
                                }
                            }
                        }
                    }
                }
            }
        }

        for (auto &direction : attackDirections) {
            // If target anthill is nearby do:
            if (map[antToMove->getX() + direction.first][antToMove->getY() + direction.second] == &to) {
                bool isPlayer = true, captured = false;

                // Decide whether from and to antHills are playerHill or aiHill and whether they are already captured.
                for (auto &hill : playerHills) {
                    if (&from == hill) {
                        isPlayer = true;
                    }
                }

                for (auto &hill : aiHills) {
                    if (&from == hill) {
                        isPlayer = false;
                    }
                }

                if (isPlayer) {
                    for (auto &hill : playerHills) {
                        if (&to == hill) {
                            captured = true;
                        }
                    }
                } else {
                    for (auto &hill : aiHills) {
                        if (&to == hill) {
                            captured = true;
                        }
                    }
                }
                // If anthill is already mine just put ant in to it.
                if (captured) {
                    map[antToMove->getX()][antToMove->getY()] = nullptr;
                    antToMove->setCurrentHp(0);
                    to.ants.push_back(antToMove);
                    break;
                    // If it is not mine and it is empty capture it
                } else if (to.ants.empty()) {
                    if (isPlayer) {
                        for (auto it = aiHills.begin(); it != aiHills.end(); ++it) {
                            if (*it == &to) {
                                playerHills.push_back(*it);
                                aiHills.erase(it);
                                break;
                            }
                        }
                    } else {
                        for (auto it = playerHills.begin(); it != playerHills.end(); ++it) {
                            if (*it == &to) {
                                aiHills.push_back(*it);
                                playerHills.erase(it);
                                break;
                            }
                        }
                    }

                    map[antToMove->getX()][antToMove->getY()] = nullptr;
                    antToMove->setCurrentHp(0);
                    to.ants.push_back(antToMove);

                    return to.getName();
                    // If it is not mine and it is empty kill all ants in it.
                } else {
                    for(auto ant : to.antsOnTheRoad) {
                        ant->setCurrentHp(0);
                    }
                    to.ants.clear();
                }
                break;
            }
        }
    }
    return '0';
}

const string &CMap::getFileName() const {
    return fileName;
}

void CMap::setPlayerHills(const vector<CAntHill *> &playHills) {
    CMap::playerHills = playHills;
}

void CMap::setAiHills(const vector<CAntHill *> &artificalIHills) {
    CMap::aiHills = artificalIHills;
}



