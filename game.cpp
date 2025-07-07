#include <iostream>
#include <windows.h>
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;

using namespace std;
void printmaze();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void enemyfire();
void updatebullets();
void spawnfruit();
void updatefruit();
void updateplayerbullets();
void playerfire();
void printStats();
void setupNextLevel();
void checkLevelCompletion();

const int rows = 20;
const int column = 20;
const int maxBullets = 100;
const int maxPlayerBullets = 10;

char maze[rows][column] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};

int planeX = 18, planeY = 10;
int bulletX[maxBullets], bulletY[maxBullets];
bool bulletactive[maxBullets];
int bulletcount = 0;
int framecounter = 0;
int lastfiredenemy = 0;
int enemyShotsFired = 0;
int health = 3;
int score = 0;
int playerBulletX[maxPlayerBullets], playerBulletY[maxPlayerBullets];
bool playerBulletActive[maxPlayerBullets];
int playerBulletCount = 0;

bool fruitActive = false;
int fruitX, fruitY;
const int fruitSpawnThreshold = 10;
int level = 1;
int enemiesRemaining = 38;
int currentFireDelay = 20;
bool levelComplete = false;



int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (health > 0) {
        
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(hConsole, cursorPosition);

        maze[planeX][planeY] = 'A';
        
        if (!levelComplete) {
            enemyfire();
            updatebullets();
            checkLevelCompletion();
        } else {
            if (level < 5) {
                setupNextLevel();
                levelComplete = false;
            } else {
                system("cls");
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(hConsole, cursorPosition);

                cout << "CONGRATULATIONS! YOU WON!\n";
                cout << "Final Score: " << score << endl;
                return 0;
            }
        }    
        playerfire();
        updateplayerbullets();
        
        if (enemyShotsFired >= fruitSpawnThreshold && !fruitActive) {
            spawnfruit();
            enemyShotsFired = 0;
        }
        updatefruit();
        printmaze();
        printStats();

        if (GetAsyncKeyState(VK_LEFT)) {
            moveLeft();
        }
        else if (GetAsyncKeyState(VK_RIGHT)) {
            moveRight();
        }
        else if (GetAsyncKeyState(VK_UP)) {
            moveUp();
        }
        else if (GetAsyncKeyState(VK_DOWN)) {
            moveDown();
        }
        
        framecounter++;
        Sleep(50);
    }
    system("cls");
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(hConsole, cursorPosition);

    cout << "GAME OVER!\n";
    cout << "Final Score: " << score << endl;
    return 0;
}
void enemyfire() {
    if (framecounter % currentFireDelay == 0) {
        int enemiesToFire = 2 + level; 
        if (level > 2) {
            for (int i = 0; i < enemiesToFire && bulletcount < maxBullets; i++) {
                int col = 1 + (framecounter + i) % (column - 2);
                if (maze[1][col] == 'V') {
                    bulletX[bulletcount] = 2;
                    bulletY[bulletcount] = col;
                    bulletactive[bulletcount] = true;
                    bulletcount++;
                    enemyShotsFired++;
                }
            }
        } else {
            int start = (lastfiredenemy + 1) % (column - 2);
            if (start == 0) start = 1;
            
            for (int j = start, fired = 0; j < column - 1 && fired < enemiesToFire; j++) {
                if (maze[1][j] == 'V' && bulletcount < maxBullets) {
                    bulletX[bulletcount] = 2;
                    bulletY[bulletcount] = j;
                    bulletactive[bulletcount] = true;
                    bulletcount++;
                    lastfiredenemy = j;
                    fired++;
                    enemyShotsFired++;
                }
            }
        }
    }
}
void printmaze() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < column; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
}
void moveLeft() {
    if (planeY > 1 && maze[planeX][planeY - 1] != '#') {
        maze[planeX][planeY] = ' ';
        planeY--;
        maze[planeX][planeY] = 'A';
    }
}
void moveRight() {
    if (planeY < column - 2 && maze[planeX][planeY + 1] != '#') {
        maze[planeX][planeY] = ' ';
        planeY++;
        maze[planeX][planeY] = 'A';
    }
}
void moveUp() {
    if (planeX > 1 && maze[planeX - 1][planeY] != '#') {
        maze[planeX][planeY] = ' ';
        planeX--;
        maze[planeX][planeY] = 'A';
    }
}
void moveDown() {
    if (planeX < rows - 2 && maze[planeX + 1][planeY] != '#') {
        maze[planeX][planeY] = ' ';
        planeX++;
        maze[planeX][planeY] = 'A';
    }
}
void updatebullets() {
    for (int i = 0; i < bulletcount; i++) {
        if (bulletactive[i]) {
            if (maze[bulletX[i]][bulletY[i]] == '.') {
                maze[bulletX[i]][bulletY[i]] = ' ';
            }

            bulletX[i]++;

            if (bulletX[i] < rows - 1) {
                if (bulletX[i] == planeX && bulletY[i] == planeY) {
                    health--;
                    bulletactive[i] = false;
                }
                else {
                    maze[bulletX[i]][bulletY[i]] = '.';
                }
            }
            else {
                bulletactive[i] = false;
            }
        }
    }
}
void printStats() {
    cout << "Health: " << health << " | Score: " << score << " | Level: " << level << endl;
    cout << "Enemies Remaining: " << enemiesRemaining << endl;
}
void playerfire() {
    if (GetAsyncKeyState(VK_SPACE) && playerBulletCount < maxPlayerBullets) {
        for (int i = 0; i < maxPlayerBullets; i++) {
            if (!playerBulletActive[i]) {
                playerBulletX[i] = planeX - 1;
                playerBulletY[i] = planeY;
                playerBulletActive[i] = true;
                playerBulletCount++;
                break;
            }
        }
    }
}

void updateplayerbullets() {
    for (int i = 0; i < maxPlayerBullets; i++) {
        if (playerBulletActive[i]) {
            if (maze[playerBulletX[i]][playerBulletY[i]] == '^') {
                maze[playerBulletX[i]][playerBulletY[i]] = ' ';
            }

            playerBulletX[i]--;

            if (playerBulletX[i] > 0) {
                if (maze[playerBulletX[i]][playerBulletY[i]] == 'V') {
                    maze[playerBulletX[i]][playerBulletY[i]] = ' ';
                    playerBulletActive[i] = false;
                    playerBulletCount--;
                    score += 10;
                    enemiesRemaining--;
                }
                else {
                    maze[playerBulletX[i]][playerBulletY[i]] = '^';
                }
            }
            else {
                playerBulletActive[i] = false;
                playerBulletCount--;
            }
        }
    }
}

void spawnfruit() {
    fruitY = (framecounter % (column - 2)) + 1;
    fruitX = 2;

    while (maze[fruitX][fruitY] == 'V' && fruitY < column - 2) {
        fruitY++;
    }
    
    fruitActive = true;
    maze[fruitX][fruitY] = 'F';
}

void updatefruit() {
    if (fruitActive) {
        if (planeX == fruitX && planeY == fruitY) {
            score += 50;
            if (health < 3) {
    health++;
}
            fruitActive = false;
            maze[fruitX][fruitY] = ' ';
        }
        else {
            maze[fruitX][fruitY] = ' ';
            fruitX++;
            
            if (fruitX < rows - 1) {
                maze[fruitX][fruitY] = 'F';
            }
            else {
                fruitActive = false;
            }
        }
    }
}
void checkLevelCompletion() {
    enemiesRemaining = 0;
    for (int j = 1; j < column - 1; j++) {
        if (maze[1][j] == 'V') enemiesRemaining++;
    }
    
    if (enemiesRemaining == 0) {
        levelComplete = true;
        score += 100 * level; 
    }
}
void setupNextLevel() {
    level++;
    currentFireDelay = 20 - (level * 2);
    if (currentFireDelay < 5) currentFireDelay = 5;
    for (int j = 1; j < column - 1; j++) {
        maze[1][j] = 'V';
    }
    enemiesRemaining = column - 2;
    for (int i = 0; i < maxBullets; i++) bulletactive[i] = false;
    for (int i = 0; i < maxPlayerBullets; i++) playerBulletActive[i] = false;
    bulletcount = 0;
    playerBulletCount = 0;
    fruitActive = false;
    enemyShotsFired = 0;
    planeX = 18;
    planeY = 10;
}
