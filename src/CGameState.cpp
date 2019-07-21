//
// Created by karpimat, Email: karpimat@fit.cvut.cz
//

#include "CGameState.hpp"


CGameState::CGameState(CMap *map, Difficulty gameDifficulty) : map(map), gameDifficulty(gameDifficulty),
                                                               gameEnd(false), gameWindow{nullptr},
                                                               commandWindow{nullptr}, statusWindow{nullptr},
                                                               attackCommands{},
                                                               statusMessage{} {}

void CGameState::startGame() {
    createGameWindow();
    createCommandWindow();
    createStatusWindow();
    update();
    generateAiAttackCommand();
    gameLoop();
}

void CGameState::gameLoop() {
    int ticks = 1;
    while (!gameEnd) {
        input();
        update();

        if (ticks % 10 == 0) {
            generateAiAttackCommand();
            generateAnts(1);
        }

        gameStateCheck();
        sleep(1);
        ticks++;
    }
}

void CGameState::input() {
    echo();
    nocbreak();
    nodelay(commandWindow, true);
    string commandStr{};
    char c = 0;
    int i = 10;
    if ((c = mvwgetch(commandWindow, 1, i++)) != ERR) {
        nodelay(commandWindow, false);
        commandStr.push_back(c);

        while ((c = mvwgetch(commandWindow, 1, i++)) != '\n') {
            if (isalnum(c) || c == '-' || c == '>' || c == '.') {
                commandStr.push_back(c);
            }
        }
        handleInput(commandStr);
    }

    nodelay(commandWindow, true);
    noecho();
    cbreak();
}

void CGameState::handleInput(const string &command) {
    if (command == "exit") {
        gameEnd = true;
        statusMessage = "Exit requested!";
        return;
    } else if (command[1] == '-' && command[2] == '>') {
        if (isalpha(command[0]) && isalpha(command[3]) && command.size() == 4) {
            for (auto &hill : map->getPlayerHills()) {
                if (toupper(command[0]) == hill->getName()) {
                    for(auto & comm : attackCommands) {
                        if(toupper(command[0]) == comm.first){
                            statusMessage = "Command with this antHill already exists!";
                            return;
                        }
                    }
                    attackCommands.emplace(toupper(command[0]), toupper(command[3]));
                    generatePath(toupper(command[0]), toupper(command[3]));
                    string tmpMsg{"Attack initiated from "};
                    tmpMsg.push_back(toupper(command[0]));
                    tmpMsg += " to ";
                    tmpMsg.push_back(toupper(command[3]));
                    statusMessage = tmpMsg;
                }
            }
        }
    } else if (command.substr(0, 4) == "save") {
        if (command.size() < 5) {
            statusMessage = "Wrong filename!";
        } else {
            statusMessage = "Saving game to the file '" + command.substr(4) + "' started!";
            saveGame(command.substr(4));
        }
    } else {
        statusMessage = "Unknown command!";
    }
}

void CGameState::update() {
    updateStatusWindow();
    updateGameWindow();
    updateCommandWindow();
    handleCommands();
}

void CGameState::createGameWindow() {
    clear();
    gameWindow = newwin(map->getHeight() + 2, map->getWidth() + 2, 0, getmaxx(stdscr) / 2 - (map->getWidth() / 2));
    box(gameWindow, 0, 0);
    refresh();
    wrefresh(gameWindow);
}

void CGameState::createCommandWindow() {
    commandWindow = newwin(3, map->getWidth() + 2, map->getHeight() + 2, getmaxx(stdscr) / 2 - (map->getWidth() / 2));
    box(commandWindow, 0, 0);
    mvwprintw(commandWindow, 1, 1, "Command: ");
    refresh();
    wrefresh(commandWindow);
}

void CGameState::createStatusWindow() {
    statusWindow = newwin(8, map->getWidth() + 2, map->getHeight() + 5, getmaxx(stdscr) / 2 - (map->getWidth() / 2));
    box(statusWindow, 0, 0);
    refresh();
    wrefresh(statusWindow);
}

void CGameState::updateStatusWindow() {
    wclear(statusWindow);
    box(statusWindow, 0, 0);
    mvwprintw(statusWindow, 1, 1, "Info about the game: ");
    mvwprintw(statusWindow, 2, 1, "Commands: 'A->B', 'save <filename>', 'exit'");
    mvwprintw(statusWindow, 3, 1,
              (string("Difficulty: ") + string((gameDifficulty == Difficulty::EASY ? "EASY" : "HARD"))).c_str());
    mvwprintw(statusWindow, 4, 1, "Player AntHills: ");
    string HillsNames{};
    for (const auto &a : map->getPlayerHills()) {
        HillsNames.push_back(a->getName());
        HillsNames.push_back(' ');
    }
    wprintw(statusWindow, HillsNames.c_str());
    mvwprintw(statusWindow, 5, 1, "AI AntHills: ");
    HillsNames.clear();
    for (const auto &a : map->getAiHills()) {
        HillsNames.push_back(a->getName());
        HillsNames.push_back(' ');
    }
    wprintw(statusWindow, HillsNames.c_str());
    mvwprintw(statusWindow, 6, 1, "Command answer: ");
    wprintw(statusWindow, statusMessage.c_str());
    wrefresh(statusWindow);
}

void CGameState::updateGameWindow() {
    wclear(gameWindow);
    box(gameWindow, 0, 0);
    map->printMap(gameWindow);
    wrefresh(gameWindow);
}

void CGameState::updateCommandWindow() {
    wclear(commandWindow);
    box(commandWindow, 0, 0);
    mvwprintw(commandWindow, 1, 1, "Command: ");
    wrefresh(commandWindow);
}

void CGameState::generateAnts(int count) {
    for (int i = 0; i < count; ++i) {
        for (auto &antHill : map->getAiHills()) {
            if (!antHill->isEmpty()) {
                antHill->generateAnt();
            }
        }
        for (auto &antHill : map->getPlayerHills()) {
            if (!antHill->isEmpty()) {
                antHill->generateAnt();
            }
        }
    }
}

void CGameState::handleCommands() {
    for (auto it = attackCommands.begin(); it != attackCommands.end(); ++it) {
        CAntHill *from = nullptr, *to = nullptr;
        for (const auto &hill : map->getPlayerHills()) {
            if (hill->getName() == it->first) {
                from = hill;
            }
        }
        for (const auto &hill : map->getAiHills()) {
            if (hill->getName() == it->first) {
                from = hill;
            }
        }
        for (const auto &hill : map->getPlayerHills()) {
            if (hill->getName() == it->second) {
                to = hill;
            }
        }
        for (const auto &hill : map->getAiHills()) {
            if (hill->getName() == it->second) {
                to = hill;
            }
        }
        if (from != nullptr && to != nullptr) {

            bool isAlive = true;
            if (from->ants.empty()) {
                isAlive = false;
            }
            for (auto &ant : from->antsOnTheRoad) {
                if (ant->isAlive()) {
                    isAlive = true;
                }
            }

            if (!isAlive) {
                attackCommands.erase(it);
                return;
            }

            char toDelete = map->manageAntHillAttacks(*from, *to);
            if (toDelete != '0') {
                for (auto &command : attackCommands) {
                    if (command.first == toDelete) {
                        attackCommands[toDelete] = '9';
                        break;
                    }
                }
            }
            updateGameWindow();
        }
    }
}

void CGameState::generatePath(char from, char to) {
    CAntHill *fromHill = nullptr, *toHill = nullptr;
    for (const auto &hill : map->getPlayerHills()) {
        if (hill->getName() == from) {
            fromHill = hill;
        }
    }
    for (const auto &hill : map->getAiHills()) {
        if (hill->getName() == from) {
            fromHill = hill;
        }
    }
    for (const auto &hill : map->getPlayerHills()) {
        if (hill->getName() == to) {
            toHill = hill;
        }
    }
    for (const auto &hill : map->getAiHills()) {
        if (hill->getName() == to) {
            toHill = hill;
        }
    }
    if (fromHill != nullptr && toHill != nullptr) {
        vector<pair<int, int>> path = map->generatePathFromTo(*fromHill, *toHill);
        if (path.empty()) {
            statusMessage = "Path could not been found!";
            updateStatusWindow();
        } else {
            fromHill->setPath(path);
        }
    }
}

void CGameState::gameStateCheck() {
    if (map->getPlayerHills().empty()) {
        statusMessage = "AI won the game!";
        updateStatusWindow();
        sleep(3);
        gameEnd = true;
    } else if (map->getAiHills().empty()) {
        statusMessage = "You won the game!";
        updateStatusWindow();
        sleep(3);
        gameEnd = true;
    }
}

void CGameState::generateAiAttackCommand() {
    CAntHill *from, *to;
    unsigned int sizeCheck;
    if (map->getAiHills().empty() || map->getPlayerHills().empty())
        return;
    switch (gameDifficulty) {
        case Difficulty::EASY:
            from = map->getAiHills()[rand() % map->getAiHills().size()];
            to = map->getPlayerHills()[rand() % map->getPlayerHills().size()];
            if(attackCommands.count(from->getName()) == 1) {
                    return;
            }
            statusMessage = "AI initiated attack from ";
            statusMessage.push_back(from->getName());
            statusMessage += " to ";
            statusMessage.push_back(to->getName());
            generatePath(from->getName(), to->getName());
            attackCommands.emplace(from->getName(), to->getName());
            break;
        case Difficulty::HARD:
            sizeCheck = INT16_MAX;
            for(auto & antHill : map->getPlayerHills()) {
                if(antHill->ants.size() < sizeCheck){
                    sizeCheck = antHill->ants.size();
                    to = antHill;
                }
            }
            from = map->getAiHills()[rand() % map->getAiHills().size()];
            if(attackCommands.count(from->getName()) == 1) {
                    return;
            }
            statusMessage = "AI initiated attack from ";
            statusMessage.push_back(from->getName());
            statusMessage += " to ";
            statusMessage.push_back(to->getName());
            generatePath(from->getName(), to->getName());
            attackCommands.emplace(from->getName(), to->getName());
            break;
        default:
            break;
    }
}

void CGameState::initAntHills(int count) {
    for (int i = 0; i < count; ++i) {
        for (auto &antHill : map->getAiHills()) {
            antHill->generateAnt();
        }
        for (auto &antHill : map->getPlayerHills()) {
            antHill->generateAnt();
        }
    }
}

void CGameState::saveGame(const string &fileName) {
    ofstream file("./examples/" + fileName);
    if (!file.is_open()) {
        statusMessage = "Error during opening the file.";
        file.close();
        return;
    }
    if (!(file << map->getFileName() << endl)) {
        statusMessage = "Error during map fileName to the file.";
        file.close();
        return;
    }
    if (!(file << (gameDifficulty == Difficulty::EASY ? "EASY" : "HARD") << endl)) {
        statusMessage = "Error during saving game difficulty to the file.";
        file.close();
        return;
    }
    // Player ant hils
    for (size_t i = 0; i < map->getPlayerHills().size(); ++i) {
        if (i != 0)
            file << ' ';
        if (!(file << map->getPlayerHills()[i]->getName() << ' ' << map->getPlayerHills()[i]->ants.size())) {
            statusMessage = "Error during saving playerAntHills to the file.";
            file.close();
            return;
        }
    }
    file << endl;
    // AI ant hills
    for (size_t i = 0; i < map->getAiHills().size(); ++i) {
        if (i != 0)
            file << ' ';
        if (!(file << map->getAiHills()[i]->getName() << ' ' << map->getAiHills()[i]->ants.size())) {
            statusMessage = "Error during saving aiAntHills to the file.";
            file.close();
            return;
        }
    }
    file << endl;

    statusMessage = "Save to the file '" + fileName + "' completed!";
    file.close();
}

