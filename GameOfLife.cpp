#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <thread>

bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

bool isInteger(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

class Cell {
public:
    bool isAlive = false;
};

class Game {
public:
    std::string gameName;
    std::vector<std::vector<Cell>> field;
    std::vector<int> birthRules;
    std::vector<int> survivalRules;
    int numRows = 0;
    int numCols = 0;
    int curIteration = 1;

    Game(std::string name, int rows, int cols) {
        birthRules = {3};
        survivalRules = {2, 3};
        gameName = name;
        numRows = rows;
        numCols = cols;
        field.resize(numRows, std::vector<Cell>(numCols));
    }

    void readFromFile(std::string filename) {
        std::ifstream inputFile(filename);
        std::string line;
        std::cout << filename << std::endl;
        if (inputFile.is_open()) {
            std::getline(inputFile, line);
            std::getline(inputFile, line);
            gameName = line.substr(3);
            std::getline(inputFile, line);
            parseRules(line.substr(3));
            std::getline(inputFile, line);
            parseSize(line.substr(3));
            std::cout << numRows << std::endl;
            std::cout << numCols << std::endl;
            while (std::getline(inputFile, line)) {
                parseCell(line);
            }
            inputFile.close();
        } else {
            std::cerr << "Unable to open input file!" << std::endl;
        }
    }

    void generatePulsar() {
        std::vector<std::string> Pulsar = {
                "...OOO...OOO...",
                "...............",
                ".O....O.O....O.",
                ".O....O.O....O.",
                ".O....O.O....O.",
                "...OOO...OOO...",
                "...............",
                "...OOO...OOO...",
                ".O....O.O....O.",
                ".O....O.O....O.",
                ".O....O.O....O.",
                "...............",
                "...OOO...OOO..."
        };
        for (int i = 0; i < Pulsar.size(); i++)
            for (int j = 0; j < Pulsar[0].size(); j++)
                if (Pulsar[i][j] == 'O')
                    field[2 + i][2 + j].isAlive = true;
    }

    void generateGliderGun() {
        std::vector<std::string> gosperGliderGun = {
                "........................O...........",
                "......................O.O...........",
                "............OO......OO............OO",
                "...........O...O....OO............OO",
                "OO........O.....O...OO..............",
                "OO........O...O.OO....O.O...........",
                "..........O.....O.......O...........",
                "...........O...O....................",
                "............OO......................"
        };
        for (int i = 0; i < gosperGliderGun.size(); i++)
            for (int j = 0; j < gosperGliderGun[0].size(); j++)
                if (gosperGliderGun[i][j] == 'O')
                    field[2 + i][2 + j].isAlive = true;
    }

    void generateGlider() {
        field[0][1].isAlive = true;
        field[1][2].isAlive = true;
        field[2][0].isAlive = true;
        field[2][1].isAlive = true;
        field[2][2].isAlive = true;
    }

    void generateBlinker() {
        field[2][2].isAlive = true;
        field[2][3].isAlive = true;
        field[2][4].isAlive = true;
    }

    void generatePentaDecathlon() {
        std::vector<std::string> PentaDecathlon = {
                ".O.",
                ".O.",
                "O.O",
                ".O.",
                ".O.",
                ".O.",
                ".O.",
                "O.O",
                ".O.",
                ".O.",
        };
        for (int i = 0; i < PentaDecathlon.size(); i++)
            for (int j = 0; j < PentaDecathlon[0].size(); j++)
                if (PentaDecathlon[i][j] == 'O')
                    field[7 + i][21 + j].isAlive = true;
    }

    void generateRandomState() {
        birthRules = {3};
        survivalRules = {2, 3};
        std::random_device rd; //случайное число
        std::mt19937 gen(rd()); //алгоритм Mersenne Twister
        std::uniform_int_distribution<> dis(0, 4); //объект для генерации рандомных чисел от 0 до 2
        int variant = dis(gen);
        switch (variant) {
            case 0: {
                generateGlider();
                break;
            }
            case 1: {
                generateGliderGun();
                break;
            }
            case 2: {
                generatePulsar();
                break;
            }
            case 3: {
                generateBlinker();
                break;
            }
            case 4: {
                generatePentaDecathlon();
                break;
            }
        }
    }

    void printState() {
        system("clear");
        std::cout << "#Life 1.06" << std::endl;
        std::cout << "#N " << gameName << std::endl;
        std::cout << "#R B";
        for (int i : birthRules) {
            std::cout << i;
        }
        std::cout << "/S";
        for (int i : survivalRules) {
            std::cout << i;
        }
        std::cout << std::endl;
        std::cout << curIteration << std::endl;
        std::cout << "┌" << std::string(field[0].size(), '-') << "┐" << std::endl;
        for (const auto& row : field) {
            std::cout << "│";
            for (const auto& cell : row) {
                std::cout << (cell.isAlive ? 'X' : ' ');
            }
            std::cout << "│" << std::endl;
        }
        std::cout << "└" << std::string(field[0].size(), '-') << "┘" << std::endl;
    }

    void saveToFile(std::string filename) {
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            outputFile << "#Life 1.06" << std::endl;
            outputFile << "#N " << gameName << std::endl;
            outputFile << "#R B";
            for (int i : birthRules) {
                outputFile << i;
            }
            outputFile << "/S";
            for (int i : survivalRules) {
                outputFile << i;
            }
            outputFile << std::endl;
            outputFile << "#S ";
            outputFile << numRows << " " << numCols << std::endl;
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (field[i][j].isAlive) {
                        outputFile << j << " " << i << std::endl;
                    }
                }
            }
            outputFile.close();
            std::cout << "Game saved to " << filename << std::endl;
        } else {
            std::cerr << "Unable to open output file!" << std::endl;
        }
    }

    void calculateNextState() {
//        printState();
//        std::chrono::seconds delay(1);
//        std::this_thread::sleep_for(delay);
        std::vector<std::vector<Cell>> nextState(numRows, std::vector<Cell>(numCols));
        curIteration++;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                int numNeighbors = countNeighbors(i, j);
                bool isAlive = field[i][j].isAlive;
                if (isAlive && std::find(survivalRules.begin(), survivalRules.end(), numNeighbors) == survivalRules.end()) {
                    isAlive = false;
                } else if (!isAlive && std::find(birthRules.begin(), birthRules.end(), numNeighbors) != birthRules.end()) {
                    isAlive = true;
                }
                nextState[i][j].isAlive = isAlive;
            }
        }
        field = nextState;
    }

    int countNeighbors(int row, int col) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (!(i == 0 && j == 0)) {
                    int newRow = (row + i + numRows) % numRows;
                    int newCol = (col + j + numCols) % numCols;
                    if (field[newRow][newCol].isAlive) {
                        count++;
                    }
                }
            }
        }

        return count;
    }

    void parseRules(std::string ruleString) {
        int slashIndex = ruleString.find('/');
        std::string birthString = ruleString.substr(1, slashIndex - 1);
        std::string survivalString = ruleString.substr(slashIndex + 2);
        birthRules.clear();
        survivalRules.clear();
        for (char c : birthString)
            birthRules.push_back(c - '0');
        for (char c : survivalString)
            survivalRules.push_back(c - '0');
    }

    void parseSize(std::string sizeString) {
        int spaceIndex = sizeString.find(' ');
        if (spaceIndex == -1) {
            std::cerr << "Wrong field size!" << std::endl;
            return;
        }
        std::string str1 = sizeString.substr(0, spaceIndex);
        std::string str2 = sizeString.substr(spaceIndex + 1);
        if (!(isInteger(str1) && isInteger(str2))) {
            std::cerr << "Wrong field size!" << std::endl;
            return;
        }
        numRows = std::stoi(str1);
        numCols = std::stoi(str2);
        field.resize(numRows, std::vector<Cell>(numCols));
    }

    void parseCell(std::string cellString) {
        int spaceIndex = cellString.find(' ');
        if (spaceIndex == -1) {
            std::cerr << "Wrong cell coordinate!" << std::endl;
            return;
        }
        std::string str1 = cellString.substr(0, spaceIndex);
        std::string str2 = cellString.substr(spaceIndex + 1);
        if (!(isInteger(str1) && isInteger(str2))) {
            std::cerr << "Wrong cell coordinate!" << std::endl;
            return;
        }
        int col = std::stoi(str1);
        int row = std::stoi(str2);
        col = (col + numCols) % numCols;
        row = (row + numRows) % numRows;
        field[row][col].isAlive = true;
    }
};

int main(int argc, char* argv[]) {
    std::string inputFilename;
    std::string outputFilename;
    int numIterations = 0;
    int mode = 0; // 0 - no arguments, 1 - input file only, 2 - random, 3 - input file, iterations and output file
    if (argc == 1) {
        mode = 2;
    }
    else if (argc == 2) {
        mode = 1;
        inputFilename = argv[1];
    }
    else {
        inputFilename = argv[1];
        for (int i = 2; i < argc; i++) {
            if (std::strcmp(argv[i], "-i") == 0) {
                if (i + 1 < argc) {
                    numIterations = std::atoi(argv[i + 1]);
                    i++;
                } else {
                    std::cerr << "Invalid arguments!" << std::endl;
                    return 0;
                }
            } else if (std::strncmp(argv[i], "--iterations=", 13) == 0) {
                numIterations = std::atoi(argv[i] + 13);
            } else if (std::strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    outputFilename = argv[i + 1];
                    i++;
                } else {
                    std::cerr << "Invalid arguments!" << std::endl;
                    return 0;
                }
            } else if (std::strncmp(argv[i], "--output=", 9) == 0) {
                outputFilename = argv[i] + 9;
            } else {
                outputFilename = argv[i];
            }
        }
        if (numIterations > -1 && (endsWith(outputFilename, ".lif") || endsWith(outputFilename, ".life")) && (endsWith(inputFilename, ".lif") || endsWith(inputFilename, ".life")))
            mode = 3;
    }
//    std::cout << mode << std::endl;
    if (mode == 1 || mode == 3)
        if (!(endsWith(inputFilename, ".lif")) || endsWith(inputFilename, ".life"))
            std::cerr << "Invalid input filename!" << std::endl;
    Game game("My Game of Life", 25, 50); // Change the size of the game as needed
    switch (mode) {
        case 1:
            game.readFromFile(inputFilename);
            break;
        case 2:
            game.generateRandomState();
            break;
        case 3:
            game.readFromFile(inputFilename);
            break;
        default:
            std::cerr << "Invalid mode!" << std::endl;
            return 0;
    }
    if (mode == 1 || mode == 3) {
        for (int i = 0; i < numIterations; i++)
            game.calculateNextState();
        if (mode == 3)
            game.saveToFile(outputFilename);
    }
//    std::cout << "arguments have done." << std::endl;
    std::string command = "";
    std::string output_file = "";
    int num_iter = 0;
    while (true) {
        std::cin >> command;
        if (command == "dump") {
            std::cin >> command;
            game.saveToFile(command);
        }
        else if (command == "exit") {
            std::cout << "Bye bye!" << std::endl;
            return 0;
        }
        else if (command == "help") {
            std::cout << "dump <filename> - save game to the file 'filename'" << std::endl;
            std::cout << "exit - exit" << std::endl;
            std::cout << "help - help" << std::endl;
            std::cout << "t <n=1> (or tick <n=1>) - calculate n (by default n = 1) iterations and print game field" << std::endl;
        }
        else if (command == "t" || command == "tick") {
            std::cin >> command;
            if (command == "dump") {
                game.calculateNextState();
                std::cin >> command;
                game.saveToFile(command);
            }
            else if (command == "exit") {
                game.calculateNextState();
                return 0;
            }
            else if (command == "help") {
                game.calculateNextState();
                std::cout << "dump <filename> - save game to the file 'filename'" << std::endl;
                std::cout << "exit - exit" << std::endl;
                std::cout << "help - help" << std::endl;
                std::cout << "t <n=1> (or tick <n=1>) - calculate n (by default n = 1) iterations and print game field" << std::endl;
            }
            else if (isInteger(command)) {
                num_iter = std::stoi(command);
                for (int i = 0; i < num_iter; ++i)
                    game.calculateNextState();
                game.printState();
            }
            else
                std::cerr << "Wrong command!" << std::endl;
        }
        else
            std::cerr << "Wrong command!" << std::endl;
    }
    return 0;
}