//
// Created by matej on 10/05/2019.
//




#include "CMenu.hpp"


using namespace std;

void CMenu::showMenu() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    enum Choices {
        NEW_GAME, CHOOSE_DIFFICULTY, LOAD_MAP, LOAD_GAME, EXIT_GAME
    };
    const array<string, 5> menuChoices{"Play New Game", "Choose Difficulty", "Load Map", "Load Game", "Exit The Game"};
    WINDOW *menuWin = newwin(menuChoices.size() + 2, xMax - 20, yMax / 2 - 5, 10);
    box(menuWin, 0, 0);
    refresh();
    wrefresh(menuWin);
    keypad(menuWin, true);

    int choice = 0, highlight = 0;
    bool leaveRequested = false;
    size_t menuSize = menuChoices.size();
//    wprintw(menuWin, "%d", menuSize);

    while (!leaveRequested) {
        clear();
        refresh();
        printGameTitle(5, 0);
        wclear(menuWin);
        box(menuWin, 0, 0);
        wrefresh(menuWin);
        for (size_t i = 0; i < menuSize; ++i) {
            if (i == static_cast<size_t>(highlight)) {
                wattron(menuWin, A_REVERSE);
            }
            if (i == 1) {
                mvwprintw(menuWin, i + 1, getmaxx(menuWin) / 2 - 10, menuChoices[i].c_str());
                mvwprintw(menuWin, i + 1, getmaxx(menuWin) / 2 + 7,
                          (chosenDifficulty == Difficulty::EASY ? "(Currently: EASY)" : "(Currently: HARD)"));
            }
            mvwprintw(menuWin, i + 1, getmaxx(menuWin) / 2 - 10, menuChoices[i].c_str());
            wattroff(menuWin, A_REVERSE);
        }
        choice = wgetch(menuWin);

        switch (choice) {
            case KEY_DOWN:
                ++highlight;
                if (static_cast<size_t>(highlight) >= menuSize)
                    highlight = menuSize - 1;
                break;
            case KEY_UP:
                --highlight;
                if (highlight < 0)
                    highlight = 0;
                break;
            case 10:
                switch (highlight) {
                    case NEW_GAME:
                        newGame();
                        delete game;
                        delete map;
                        game = nullptr;
                        map = nullptr;
                        break;
                    case CHOOSE_DIFFICULTY:
                        chooseDifficulty(menuWin);
                        break;
                    case LOAD_MAP:
                        loadMap(menuWin, "");
                        break;
                    case LOAD_GAME:
                        loadGame(menuWin);
                        delete game;
                        delete map;
                        game = nullptr;
                        map = nullptr;
                        break;
                    case EXIT_GAME:
                        exitGame();
                        leaveRequested = true;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void CMenu::printGameTitle(int y, int x) const {
    mvprintw(y, x, "                 _     __          __              \n"
                   "     /\\         | |    \\ \\        / /\\             \n"
                   "    /  \\   _ __ | |_    \\ \\  /\\  / /  \\   _ __ ___ \n"
                   "   / /\\ \\ | '_ \\| __|    \\ \\/  \\/ / /\\ \\ | '__/ __|\n"
                   "  / ____ \\| | | | |_      \\  /\\  / ____ \\| |  \\__ \\\n"
                   " /_/    \\_\\_| |_|\\__|      \\/  \\/_/    \\_\\_|  |___/\n"
                   "                                                   \n"
                   "                                                   ");
    refresh();
}

void CMenu::chooseDifficulty(WINDOW *menuWin) {
    enum Choices {
        EASY, HARD, RETURN
    };
    const array<string, 3> menuChoices{"EASY_DIFFICULTY", "HARD_DIFFICULTY", "Return to the menu"};
    int choice = 0, highlight = 0;
    bool leaveRequested = false;
    size_t menuSize = menuChoices.size();
//    wprintw(menuWin, "%d", menuSize);

    while (!leaveRequested) {
        wclear(menuWin);
        box(menuWin, 0, 0);
        wrefresh(menuWin);
        for (size_t i = 0; i < menuSize; ++i) {
            if (i == static_cast<size_t>(highlight)) {
                wattron(menuWin, A_REVERSE);
            }
            mvwprintw(menuWin, i + 1, getmaxx(menuWin) / 2 - 10, menuChoices[i].c_str());
            wattroff(menuWin, A_REVERSE);
        }
        choice = wgetch(menuWin);

        switch (choice) {
            case KEY_DOWN:
                ++highlight;
                if (static_cast<size_t>(highlight) >= menuSize)
                    highlight = menuSize - 1;
                break;
            case KEY_UP:
                --highlight;
                if (highlight < 0)
                    highlight = 0;
                break;
            case 10:
                switch (highlight) {
                    case EASY:
                        chosenDifficulty = Difficulty::EASY;
                        break;
                    case HARD:
                        chosenDifficulty = Difficulty::HARD;
                        break;
                    case RETURN:
                        break;
                    default:
                        break;
                }
                leaveRequested = true;
                break;
            default:
                break;
        }
    }
}

void CMenu::loadMap(WINDOW *win, const string &mapFileName) {
    string fileName;
    if (win != nullptr) {
        while (fileName.empty()) {
            fileName = inputMenu(win);
            if (fileName == "exit")
                return;
        }
    } else if (!mapFileName.empty()) {
        fileName = mapFileName;
    } else {
        fileName = "default";
    }
    // open file and get data from it
    ifstream mapFile("./examples/" + fileName);
    if (!mapFile.is_open()) {
        statusMessage(win, "File could not be opened!");
        return;
    } else {
        statusMessage(win, "File opened successfully!");
    }

    int width = 0, height = 0;
    char delimiter = 0;
    if (!(mapFile >> width >> delimiter >> height >> ws)) {
        statusMessage(win, "Error during reading file.");
        return;
    }
    mapFile.clear();
//    printw("%d %c %d", width, delimiter, height);
//    refresh();
//    wgetch(win);

    vector<vector<AMapObject *>> mapOfObjects(height);
    vector<CAntHill *> playersHills;
    vector<CAntHill *> aiHills;
    vector<AMapObject *> objectsToDelete;
    bool visited[100][100];
    for (auto &i : visited) {
        for (auto &j : i) {
            j = false;
        }
    }
    for (auto &i : mapOfObjects) {
        i = vector<AMapObject *>(width);
    }
    string line;
    char antHillName = static_cast<char>(65);
    for (int row = 0; row < height; ++row) {
        if (!(getline(mapFile, line))) {
            statusMessage(win, "Error reading file!");
            return;
        }
        for (size_t i = 0; i < line.size(); ++i) {
            switch (line[i]) {
                case '#':
                    mapOfObjects[row][i] = new CObstacle(make_pair(row, i));
                    objectsToDelete.push_back(mapOfObjects[row][i]);
                    break;
                case ' ':
                    mapOfObjects[row][i] = new CEmptyTile(make_pair(row, i));
                    objectsToDelete.push_back(mapOfObjects[row][i]);
                    break;
                case 'P':
                    if (!visited[row][i]) {
                        CAntHill *antHill = new CAntHill(make_pair(row, i), antHillName++, vector<AAnt *>());
                        for (int m = 0; m < 3; ++m) {
                            for (int n = 0; n < 5; ++n) {
                                visited[row + m][i + n] = true;
                                mapOfObjects[row + m][i + n] = antHill;
                            }
                        }
                        playersHills.push_back(antHill);
                    }
                    break;
                case 'A':
                    if (!visited[row][i]) {
                        CAntHill *antHill = new CAntHill(make_pair(row, i), antHillName++, vector<AAnt *>());
                        for (int m = 0; m < 3; ++m) {
                            for (int n = 0; n < 5; ++n) {
                                visited[row + m][i + n] = true;
                                mapOfObjects[row + m][i + n] = antHill;
                            }
                        }
                        aiHills.push_back(antHill);
                    }
                    break;
                default:
                    mapOfObjects[row][i] = new CEmptyTile(make_pair(row, i));
                    break;
            }
        }
    }

    map = new CMap(mapOfObjects, width, height, playersHills, aiHills, objectsToDelete, fileName);
    mapFile.close();
}

void CMenu::newGame() {
    if (map == nullptr) {
        loadMap(nullptr, "");
    }
    if (!map)
        return;
    game = new CGameState(map, chosenDifficulty);
    game->initAntHills(15);
    game->startGame();
}

void CMenu::loadGame(WINDOW *win) {
    string fileName;
    while (fileName.empty()) {
        fileName = inputMenu(win);
        if (fileName == "exit")
            return;
    }
    ifstream gameFile("./examples/" + fileName);
    if (!gameFile.is_open()) {
        statusMessage(win, "File could not be opened!");
        return;
    } else {
        statusMessage(win, "File opened successfully!");
        delete map;
    }

    string mapFileName;
    string chosenDiff;
    char antHillName;
    size_t antHillAntCount;

    if (!(gameFile >> mapFileName >> chosenDiff >> ws)) {
        gameFile.close();
        statusMessage(win, "Error during reading gameFile!");
        return;
    }
    gameFile.clear();
    chosenDifficulty = (chosenDiff == "EASY" ? Difficulty::EASY : Difficulty::HARD);

    loadMap(nullptr, mapFileName);

    vector<CAntHill *> allCAntHills{};
    vector<CAntHill *> playerAntHills{};
    vector<CAntHill *> aiAntHills{};
    for (auto &antHill : map->getPlayerHills()) {
        allCAntHills.push_back(antHill);
    }
    for (auto &antHill : map->getAiHills()) {
        allCAntHills.push_back(antHill);
    }

    string line;
    getline(gameFile, line);
    stringstream strStream(line);
    while (strStream >> antHillName >> antHillAntCount) {
        for (auto &antHill : allCAntHills) {
            if (antHill->getName() == antHillName) {
                playerAntHills.push_back(antHill);
                while (antHill->ants.size() != antHillAntCount) {
                    antHill->generateAnt();
                }
                break;
            }
        }
    }
    gameFile.clear();
    getline(gameFile, line);
    strStream.clear();
    strStream.str(line);
    while (strStream >> antHillName >> antHillAntCount) {
        for (auto &antHill : allCAntHills) {
            if (antHill->getName() == antHillName) {
                aiAntHills.push_back(antHill);
                while (antHill->ants.size() != antHillAntCount) {
                    antHill->generateAnt();
                }
                break;
            }
        }
    }

    map->setPlayerHills(playerAntHills);
    map->setAiHills(aiAntHills);

    gameFile.close();
    game = new CGameState(map, chosenDifficulty);
    game->startGame();
}

void CMenu::exitGame() {
    clear();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    mvprintw(yMax / 2, xMax / 2 - 10, "Thanks for playing!");
    refresh();
    sleep(1);
}

string CMenu::inputMenu(WINDOW *menuWin) {
    wclear(menuWin);
    box(menuWin, 0, 0);
    echo();
    nocbreak();
    statusMessage(menuWin, "Enter the file name (Type 'exit' to return to the menu): ");
    char c;
    string fileName{};

    while ((c = wgetch(menuWin)) != '\n') {
        if (!isalnum(c) && c != '.') {
            fileName.clear();
            break;
        }
        fileName.push_back(c);
    }

    if (fileName == "exit") {
        noecho();
        cbreak();
        return fileName;
    }

    wclear(menuWin);
    box(menuWin, 0, 0);
    if (fileName.empty()) {
        statusMessage(menuWin, "Invalid name format, try again!");
    } else {
        statusMessage(menuWin, "File name input successful!");
    }
    noecho();
    cbreak();
    return fileName;

}

void CMenu::statusMessage(WINDOW *win, const string &message) const {
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 3, 1, message.c_str());
    wrefresh(win);
    sleep(1);
}
