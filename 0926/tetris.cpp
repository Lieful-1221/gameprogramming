#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <vector>

using namespace std;

// --- 전역 변수 ---
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0,};
int currentShape[4][4];
int nextShape[4][4];
int currentX, currentY;
int prevX, prevY;

// 블록 표시 문자 (두 칸 차지하도록 변경)
const char* BLOCK_CHAR = "■ ";
const char* EMPTY_CHAR = "  ";

// 테트리스 도형 (4x4 배열로 정의)
const int shapes[7][4][4] = {
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}, // I
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}  // L
};

// --- 함수 선언 ---
void gotoxy(int x, int y);
void drawBoard();
void drawShape(int shape[][4], int x, int y, const char* block);
bool checkCollision(int x, int y, int shapeToCheck[4][4]);
void lockShape();
void clearLines();
void generateNewShape();
void rotateShape(bool clockwise);
void gameLoop();
void setConsoleWindowSize(int width, int height);
void hideCursor();

// --- 함수 구현 ---
void gotoxy(int x, int y) {
    COORD Pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setConsoleWindowSize(int width, int height) {
    COORD coord;
    coord.X = width;
    coord.Y = height;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    SMALL_RECT rect;
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = width - 1;
    rect.Bottom = height - 1;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);
}

void drawBoard() {
    // 상단 벽
    gotoxy(0, 0);
    cout << "┌";
    for (int i = 0; i < BOARD_WIDTH; i++) cout << "──";
    cout << "┐";

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        gotoxy(0, y + 1);
        cout << "│";
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 1) {
                cout << BLOCK_CHAR;
            } else {
                cout << EMPTY_CHAR;
            }
        }
        cout << "│";
    }

    // 하단 벽
    gotoxy(0, BOARD_HEIGHT + 1);
    cout << "└";
    for (int i = 0; i < BOARD_WIDTH; i++) cout << "──";
    cout << "┘";

    // 지시문 출력 (위치 조정)
    gotoxy(0, BOARD_HEIGHT + 2);
    cout << "조작: ←→ 이동, ↑↓ 회전, Space 하드 드롭, ESC 종료";
}

void drawShape(int shape[][4], int x, int y, const char* block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] == 1) {
                if (x + j >= 0 && x + j < BOARD_WIDTH && y + i >= 0 && y + i < BOARD_HEIGHT) {
                    gotoxy((x + j) * 2 + 1, y + i + 1);
                    cout << block;
                }
            }
        }
    }
}

bool checkCollision(int newX, int newY, int shapeToCheck[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapeToCheck[y][x] == 1) {
                int boardX = newX + x;
                int boardY = newY + y;

                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return true;
                }
                if (boardY >= 0 && board[boardY][boardX] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void lockShape() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentShape[y][x] == 1) {
                if (currentY + y >= 0 && currentY + y < BOARD_HEIGHT && currentX + x >= 0 && currentX + x < BOARD_WIDTH) {
                    board[currentY + y][currentX + x] = 1;
                }
            }
        }
    }
}

void clearLines() {
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool lineIsFull = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            for (int dropY = y; dropY > 0; dropY--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[dropY][x] = board[dropY - 1][x];
                }
            }
            for(int x = 0; x < BOARD_WIDTH; x++) {
                board[0][x] = 0;
            }
            y++;
        }
    }
}

void generateNewShape() {
    int shapeIndex = rand() % 7;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            currentShape[y][x] = shapes[shapeIndex][y][x];
        }
    }
    currentX = (BOARD_WIDTH / 2) - 2;
    currentY = -3;
    prevX = currentX;
    prevY = currentY;

    if (checkCollision(currentX, currentY, currentShape)) {
        system("cls");
        gotoxy(5, 5);
        cout << "G A M E   O V E R ! ! !";
        gotoxy(5, 7);
        cout << "Press any key to exit...";
        _getch();
        exit(0);
    }
}

void rotateShape(bool clockwise) {
    int temp[4][4];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (clockwise) {
                temp[x][3 - y] = currentShape[y][x];
            } else {
                temp[3 - x][y] = currentShape[y][x];
            }
        }
    }

    if (!checkCollision(currentX, currentY, temp)) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                currentShape[y][x] = temp[y][x];
            }
        }
    }
}

void gameLoop() {
    drawBoard();
    generateNewShape();
    long long lastMoveTime = GetTickCount64();

    while (true) {
        drawShape(currentShape, prevX, prevY, EMPTY_CHAR);

        drawShape(currentShape, currentX, currentY, BLOCK_CHAR);
        prevX = currentX;
        prevY = currentY;

        if (GetTickCount64() - lastMoveTime > 500) {
            if (!checkCollision(currentX, currentY + 1, currentShape)) {
                currentY++;
            } else {
                lockShape();
                clearLines();
                drawBoard();
                generateNewShape();
            }
            lastMoveTime = GetTickCount64();
        }

        if (_kbhit()) {
            int key = _getch();
            drawShape(currentShape, prevX, prevY, EMPTY_CHAR);
            switch (key) {
                case 32: // 스페이스 바
                    while (!checkCollision(currentX, currentY + 1, currentShape)) {
                        drawShape(currentShape, currentX, currentY, EMPTY_CHAR); // 이전 위치 지우기
                        currentY++;
                        drawShape(currentShape, currentX, currentY, BLOCK_CHAR); // 새 위치 그리기
                        Sleep(10); // 깜빡임 방지를 위한 짧은 딜레이
                    }
                    lockShape();
                    clearLines();
                    drawBoard();
                    generateNewShape();
                    lastMoveTime = GetTickCount64();
                    break;
                case 224:
                    key = _getch();
                    switch (key) {
                        case 75:
                            if (!checkCollision(currentX - 1, currentY, currentShape)) currentX--;
                            break;
                        case 77:
                            if (!checkCollision(currentX + 1, currentY, currentShape)) currentX++;
                            break;
                        case 72:
                            rotateShape(false);
                            break;
                        case 80:
                            rotateShape(true);
                            break;
                    }
                    break;
                case 27:
                    exit(0);
                    break;
            }
            drawShape(currentShape, currentX, currentY, BLOCK_CHAR);
            prevX = currentX;
            prevY = currentY;
        }

        Sleep(10);
    }
}

int main() {
    srand(time(0));
    setConsoleWindowSize(BOARD_WIDTH * 2 + 3, BOARD_HEIGHT + 4);
    hideCursor();
    
    gameLoop();
    return 0;
}