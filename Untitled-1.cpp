#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <random>
#include <chrono>
#include <thread>

const int ROWS = 50; // n
const int COLS = 50; // m

int numarDeRepeteriMapGemerate = 0;
int points = 0;
int lavaLayer = 0;
int dificulty = 0; 

bool exitGame = 0;

const int di[]={-1, 0, 1, 0},
          dj[]={ 0, 1, 0,-1};
int mat[ROWS][COLS] = {0};
int mapOrs[ROWS][COLS] = {0};

COORD playerPosition ;
COORD exitDoor ;

std::vector<int> blueOresChancesBasedOnDifficulty = {40, 30, 15};
std::vector<int> yellowOresChancesBasedOnDifficulty = {5, 3, 1};
std::vector<int> timesToGenerateMapBasedOnDifficulty = {40, 30, 20};
std::vector<int> timeToCreateLavaLayerBasedOnDifficulty = {4, 2, 1};
std::vector<int> propabilitytocretaesoftwallsbasedondifficulty = {25, 60, 80};
std::vector<int> probalilitytocreatehardwallsbasedondifficulty = {25, 30, 35};
std::vector<int> numberOfPointsNeededBasedOnDifficulty = {20, 30, 40};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

COORD randomPozitionOnMap(int x, int y,int z) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribX(0, x);
    std::uniform_int_distribution<> distribY(0, y);

    int rollX = distribX(gen);
    int rollY = distribY(gen);
    if(z == 1) {
        while (rollY < 25) {
            rollY = distribY(gen);
        }
        COORD coord = { static_cast<SHORT>(rollX), static_cast<SHORT>(rollY) };
        return coord;
    } else {
        COORD coord = { static_cast<SHORT>(rollX), static_cast<SHORT>(rollY) };
        return coord;
        }
}


bool FlipAcoinWith(int x){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);
    int roll = distrib(gen);
    if (roll <= x) {
        return 1; 
    } else {
        return 0; 
    }
}

void drawTile(int colorCode) {
    std::cout << "\033[48;5;" << colorCode << "m \033[0m";
}

void gotoXY(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, coord);
}

void map_generate(int x, int y) {
    int dir = std::rand() % 4; 
    int ok = 0; 

    gotoXY(x, y);
    mat[x][y] = 1;
    drawTile(94);

    std::queue<std::pair<int, int>> Q;
    Q.push(std::make_pair(x, y));

    while (!Q.empty()) { 
        int i = Q.front().first;
        int j = Q.front().second;
        for (int k = 0; k < 4; k++) {
            int iv = i + di[k];
            int jv = j + dj[k];
            if (iv == 0 || iv == ROWS  || jv == 0 || jv == COLS) {
                ok++;
            }else if(ok > 100) {
                numarDeRepeteriMapGemerate++;
                return;
            }

            if (iv > 0 && iv < ROWS  && jv > 0 && jv < COLS  && FlipAcoinWith(propabilitytocretaesoftwallsbasedondifficulty[dificulty]) && k == dir && mat[iv][jv] == 0) {
                gotoXY(iv, jv);
                if(FlipAcoinWith(yellowOresChancesBasedOnDifficulty[dificulty]) && mat[iv][jv] == 0) {
                    drawTile(229);
                    mat[iv][jv] = 10;
                } else if(FlipAcoinWith(blueOresChancesBasedOnDifficulty[dificulty]) && mat[iv][jv] == 0) {
                    drawTile(117);
                    mat[iv][jv] = 2;
                } else {
                    drawTile(94);
                    mat[iv][jv] = 1;
                }
                Q.push(std::make_pair(iv, jv)); 
            }
            else if (iv > 0 && iv < ROWS  && jv > 0 && jv < COLS  && FlipAcoinWith(probalilitytocreatehardwallsbasedondifficulty[dificulty]) && mat[iv][jv] == 0) {
                gotoXY(iv, jv);
                if(FlipAcoinWith(yellowOresChancesBasedOnDifficulty[dificulty]) && mat[iv][jv] == 0) {
                    drawTile(229);
                    mat[iv][jv] = 10;
                } else if(FlipAcoinWith(blueOresChancesBasedOnDifficulty[dificulty]) && mat[iv][jv] == 0) {
                    drawTile(117);
                    mat[iv][jv] = 2;
                } else {
                    drawTile(94);
                    mat[iv][jv] = 1;
                }
                Q.push(std::make_pair(iv, jv));
                dir = k;
                ok++;
            }
        }
        Q.pop();
    }
    numarDeRepeteriMapGemerate++;
}

void drawInitialMatrix(int cursorX, int cursorY) {
    system("cls");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == cursorY && j == cursorX)
                drawTile(255); 
            else
                drawTile(235);
        }
        std::cout << '\n';
    }
    std::cout.flush();

    gotoXY(0, 0);
}

void dig(){
    char ch = _getch();

    if (ch == 's' && playerPosition.Y < ROWS - 1 && mat[playerPosition.X][playerPosition.Y+1] != 0
        && mat[playerPosition.X][playerPosition.Y+1] != 3) {
        if(mat[playerPosition.X][playerPosition.Y+1] != 1 ) {
            gotoXY(55, 5);
            points+= mat[playerPosition.X][playerPosition.Y+1]-1;
            std::cout << "Points: " << points << "\n";
            mat[playerPosition.X][playerPosition.Y+1] = 0;
        }
        mat[playerPosition.X][playerPosition.Y+1] = 0;
        gotoXY(playerPosition.X, playerPosition.Y+1);
        drawTile(235);
    }

    if (ch == 'w' && playerPosition.Y > 0 && mat[playerPosition.X][playerPosition.Y-1] != 0
        && mat[playerPosition.X][playerPosition.Y-1] != 3) {
        if(mat[playerPosition.X][playerPosition.Y-1] != 1) {
            gotoXY(55, 5);
            points+= mat[playerPosition.X][playerPosition.Y-1]-1;
            std::cout << "Points: " << points << "\n";
            mat[playerPosition.X][playerPosition.Y-1] = 0;
        }
        mat[playerPosition.X][playerPosition.Y-1] = 0;
        gotoXY(playerPosition.X, playerPosition.Y-1);
        drawTile(235);
    }

    if (ch == 'a' && playerPosition.X > 0 && mat[playerPosition.X-1][playerPosition.Y] != 0
        && mat[playerPosition.X-1][playerPosition.Y] != 3) {
        if(mat[playerPosition.X-1][playerPosition.Y] != 1) {
            gotoXY(55, 5);
            points+= mat[playerPosition.X-1][playerPosition.Y]-1;
            std::cout << "Points: " << points << "\n";
            mat[playerPosition.X-1][playerPosition.Y] = 0;
        }
        mat[playerPosition.X-1][playerPosition.Y] = 0;
        gotoXY(playerPosition.X-1, playerPosition.Y);
        drawTile(235);
    }

    if (ch == 'd' && playerPosition.X < COLS - 1 && mat[playerPosition.X+1][playerPosition.Y] != 0
        && mat[playerPosition.X+1][playerPosition.Y] != 3) {
        if(mat[playerPosition.X+1][playerPosition.Y] != 1) {
            gotoXY(55, 5);
            points+= mat[playerPosition.X+1][playerPosition.Y]-1;
            std::cout << "Points: " << points << "\n";
            mat[playerPosition.X+1][playerPosition.Y] = 0;
        }
        mat[playerPosition.X+1][playerPosition.Y] = 0;
        gotoXY(playerPosition.X+1, playerPosition.Y);
        drawTile(235);
    }
}

void print_end_game() {
    std::cout << R"(
  _____                        _____                 
 |  __ \                      |  _  |                
 | |  \/ __ _ _ __ ___   ___ | | | |_   _____ _ __  
 | | __ / _` | '_ ` _ \ / _ \| | | \ \ / / _ \ '__| 
 | |_\ \ (_| | | | | | | (_) \ \_/ /\ V /  __/ |    
  \____/\__,_|_| |_| |_|\___/ \___/  \_/ \___|_|    

======================================================
                 GAME OVER - THANK YOU!
======================================================
)";
}

void createLavaLayer() {
    for(int i = 0;i<ROWS;i++){
        gotoXY(i, lavaLayer);
        mat[i][lavaLayer] = 3;
        drawTile(196);
        if(i == playerPosition.X && lavaLayer == playerPosition.Y) {
            exitGame = 1;
        }
    }
    lavaLayer++;
}
void print_win_game () {
    std::cout << R"(
     _,-._
    / \_/ \
   |  _  |
  |  \_|  |
   `-----'
   YOU WIN!
)";
}
void checkForInput() {
    auto start = std::chrono::high_resolution_clock::now();
    gotoXY(63, 5);
    std::cout<< points << " out of "<< numberOfPointsNeededBasedOnDifficulty[dificulty] <<"\n";
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
        char ch = _getch();
        if (ch == 'q' || ch == 'Q') return;
        
        gotoXY(playerPosition.X, playerPosition.Y);
        drawTile(235);

        if (ch == 'w' && playerPosition.Y > 0 && mat[playerPosition.X][playerPosition.Y-1] == 0) playerPosition.Y--;
        if (ch == 's' && playerPosition.Y < ROWS - 1 && mat[playerPosition.X][playerPosition.Y+1] == 0) playerPosition.Y++;
        if (ch == 'a' && playerPosition.X > 0 && mat[playerPosition.X-1][playerPosition.Y] == 0) playerPosition.X--;
        if (ch == 'd' && playerPosition.X < COLS - 1 && mat[playerPosition.X+1][playerPosition.Y] == 0) playerPosition.X++;

        if(ch == 'e') {
            dig();
        }

        gotoXY(playerPosition.X, playerPosition.Y);
        drawTile(255);

        gotoXY(0, ROWS + 1);
        if(elapsed.count() >= timeToCreateLavaLayerBasedOnDifficulty[dificulty]) {
            createLavaLayer();
            start = std::chrono::high_resolution_clock::now();
            if(exitGame) {
                system("cls");
                print_end_game();
                return;
            }
        }
        if(playerPosition.X == exitDoor.X && playerPosition.Y == exitDoor.Y) {
            if(points >= numberOfPointsNeededBasedOnDifficulty[dificulty]) {
                system("cls");
                print_win_game();
                return;
            }
        }
    }
}

void print_start_game() {
    std::cout << R"(
+------------------------------------------------------------+
|                                                            |
|  __  __ _                   _____                _         |
| |  \/  (_)                 / ____|              | |        |
| | \  / |_ _ __   ___ _ __ | |     ___  _ __  ___| |_ ___   |
| | |\/| | | '_ \ / _ \ '_ \| |    / _ \| '_ \/ __| __/ __|  |
| | |  | | | | | |  __/ | | | |___| (_) | | | \__ \ |_\__ \  |
| |_|  |_|_|_| |_|\___|_| |_|\_____\___/|_| |_|___/\__|___/  |
|                                                            |
|                      Select Difficulty:                    |
|                      Pres 1 , 2 or 3 to                    |           
|                      choose difficulty                     |
|                        1  Easy                             |
|                        2  Medium                           |
|                        3  Hard                             |
|                                                            |
+------------------------------------------------------------+

)";
    gotoXY(25, 13);
    while (true) {
        char ch = _getch();
        if (ch == '1') {
            dificulty = 0;
            system("cls");
            break;
        }else if (ch == '2') {
            dificulty = 1;
            system("cls");
            break;
        } else if (ch == '3') {
            dificulty = 2;
            system("cls");
            break;
        } else {
            gotoXY(10, 13);
            std::cout << "Invalid choice! Please select 1, 2, or 3\n";
        }
    
    }
}

void generateExitDoor() {
    gotoXY(exitDoor.X, exitDoor.Y);
    drawTile(196);
}

void generateText() {
    gotoXY(55, 5);
    std::cout << "Points: 0\n";
    gotoXY(55, 6);
    std::cout<< "The rulse are simple:\n";
    gotoXY(55, 7);
    std::cout << "Press 'w', 'a', 's', 'd' to move\n";
    gotoXY(55, 8);
    std::cout << "Press 'e' to dig and then chose a direction to dig , you can see you you are diging if the player disapears\n";
    gotoXY(55, 9);
    std::cout << "Press 'q' to exit the game\n";
    gotoXY(55, 10);
    std::cout << "You need to collect " << numberOfPointsNeededBasedOnDifficulty[dificulty] << " points to win the game\n";
    gotoXY(55, 11);
    std::cout<< "Gems witch look like this: \033[48;5;117m \033[0m give you 1 point\n";
    gotoXY(55, 12);
    std::cout<< "Gems witch look like this: \033[48;5;229m \033[0m give you 2 points\n";
    gotoXY(55, 13);
    std::cout<< "You can see the lava layer at the top of the map , it will rise every " << timeToCreateLavaLayerBasedOnDifficulty[dificulty] << " seconds\n";
    gotoXY(55, 14);
    std::cout<< "The exit door looks like this: \033[48;5;196m \033[0m\n";
    gotoXY(55, 15);
    std::cout<< "And the walls look like this: \033[48;5;94m \033[0m\n";
    gotoXY(55, 16);
    std::cout<< "Good luck and have fun!\n";
}
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

    exitDoor = randomPozitionOnMap(ROWS-1, COLS-1, 1);
    playerPosition = randomPozitionOnMap(ROWS-1, COLS-1,0);

    print_start_game();
    drawInitialMatrix(playerPosition.X, playerPosition.Y);
    for(int i = 0;i< timesToGenerateMapBasedOnDifficulty[dificulty];i++){
        COORD randomPos = randomPozitionOnMap(ROWS-1, COLS-1,0);
        map_generate(randomPos.X, randomPos.Y);
    }
    generateExitDoor();
    generateText();

    checkForInput();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}
