#include<bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;
char playAgain;
int highestScore;

class Console {
public:
    Console(int width, int height) : width(width), height(height) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        bufferSize = { static_cast<short>(width + 2 + 18), static_cast<short>(height + 2) };  // casting width and height to short
        buffer1.resize((width + 2 + 18+2) * (height + 2), ' ');
        buffer2.resize((width + 2 + 18+2) * (height + 2), ' ');
        currentBuffer = &buffer1;
        nextBuffer = &buffer2;
    }

    void setupWindow() {
        SMALL_RECT windowSize = { 0, 0, static_cast<short>(width + 1 + 18), static_cast<short>(height + 1 ) };  // casting width and height to short
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
        SetConsoleScreenBufferSize(hConsole, { static_cast<short>(width + 2 + 18), static_cast<short>(height + 2) });  // casting width and height to short
    }

    void draw(int x, int y, int fruitX, int fruitY, int score, const vector<int>& tailX, const vector<int>& tailY, int nTail) {
        COORD coord = { 0, 0 };
        DWORD bytesWritten;
        fill(nextBuffer->begin(), nextBuffer->end(), ' ');

        // Drawing the boundary walls
        for (int i = 0; i < width + 2; i++)
            (*nextBuffer)[i] = '#';
        for (int i = 0; i < height; i++) {
            (*nextBuffer)[(i + 1) * (width + 2 + 18)] = '#';
            (*nextBuffer)[(i + 1) * (width + 2 + 18) + width + 1] = '#';
        }
        for (int i = 0; i < width + 2; i++)
            (*nextBuffer)[(height + 1) * (width + 2 + 18) + i] = '#';

        (*nextBuffer)[(y + 1) * (width + 2 + 18) + (x + 1)] = 'O';

        for (int i = 0; i < nTail; i++)
            (*nextBuffer)[(tailY[i] + 1) * (width + 2 + 18) + (tailX[i] + 1)] = 'o';

        (*nextBuffer)[(fruitY + 1) * (width + 2 + 18) + (fruitX + 1)] = 'F';

        string scoreText = "Score: " + to_string(score);
        string highestscore= "Highest Score: " +to_string(highestScore);
        for (int i = 0; i < scoreText.length(); i++) {
            (*nextBuffer)[(height / 2) * (width + 2 + 18) + width + 3 + i] = scoreText[i];
        }
        for (int i = 0; i < highestscore.length(); i++) {
            (*nextBuffer)[((height / 2)+1) * (width + 2 + 18) + width + 3 + i] = highestscore[i];
        }
        WriteConsoleOutputCharacterA(hConsole, nextBuffer->data(), nextBuffer->size(), coord, &bytesWritten);
        swap(currentBuffer, nextBuffer);
        SetConsoleCursorPosition(hConsole, { 0, static_cast<short>(height + 3 )});
    }

    HANDLE hConsole;
    COORD bufferSize;
    vector<char> buffer1, buffer2;
    vector<char>* currentBuffer;
    vector<char>* nextBuffer;
    int width, height;
};

class SnakeGame {
public:
    SnakeGame(int width, int height) : width(width), height(height), console(width, height) {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        score = 0;
        nTail = 0;
        tailX.resize(100);
        tailY.resize(100);
        highestScore = 0;
        appearFood();
    }

    void start() {
        console.setupWindow();
        do {
            gameOver = false;
            dir = STOP;
            x = width / 2;
            y = height / 2;
            tailX[1]=x;
            tailY[1]=y+1;
            tailX[2]=x;
            tailY[2]=y+2;

            appearFood();
            score = 0;
            nTail = 2;

            while (!gameOver) {
                console.draw(x, y, fruitX, fruitY, score, tailX, tailY, nTail);
                input();
                logic();
                Sleep(90);
            }

            // Update highest score if needed
            if (score > highestScore) {
                highestScore = score;
            }

            // Display Game Over message and highest score at the bottom

            // Ask user if they want to restart the game
            system("cls");
            showGameOver();
            cout << "\nDo you want to play again? (y/n): ";
            cin >> playAgain;
            system("cls");

        } while (playAgain == 'y' || playAgain == 'Y');
    }

private:
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    bool gameOver;
    int width, height;
    int x, y, fruitX, fruitY, score;
    vector<int> tailX, tailY;
    int nTail;
    eDirection dir;
    Console console;

    void input() {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
            case 'a':
                // Move LEFT, but ensure it's not the opposite direction of RIGHT
                if (dir != RIGHT) {
                    dir = LEFT;
                }
                break;
            case 'd':
                // Move RIGHT, but ensure it's not the opposite direction of LEFT
                if (dir != LEFT) {
                    dir = RIGHT;
                }
                break;
            case 'w':
                // Move UP, but ensure it's not the opposite direction of DOWN
                if (dir != DOWN) {
                    dir = UP;
                }
                break;
            case 's':
                // Move DOWN, but ensure it's not the opposite direction of UP
                if (dir != UP) {
                    dir = DOWN;
                }
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }

    void logic() {
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;

        // Move the tail
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        // Move the head
        switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
        }

        // Check if the snake hits the wall
        if (x < 0 || x >= width || y < 0 || y >= height) {
            gameOver = true;
        }

        // Check if the snake hits itself
        for (int i = 2; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameOver = true;
            }
        }

        // If the snake eats the food
        if (x == fruitX && y == fruitY) {
            score += 10;
            appearFood(); // Generate new food position
            nTail++;
        }
    }

    void appearFood() {
        do {
            fruitX = rand() % width;
            fruitY = rand() % height;
        } while ((fruitX <= 2 || fruitX >= width - 2 || fruitY <= 2 || fruitY >= height - 2 || // Avoid near walls
                  isFoodOnSnake(fruitX, fruitY)));  // Check if the food position is on the snake body
    }

    bool isFoodOnSnake(int xPos, int yPos) {
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == xPos && tailY[i] == yPos) {
                return true;  // Food overlaps with the snake's body
            }
        }
        return false;  // No overlap with the snake's body
    }

    void showGameOver() {
        // Set cursor to the bottom of the screen
        COORD coord = { 0, static_cast<short>(height + 3) };
        SetConsoleCursorPosition(console.hConsole, coord);

        // Display the game over message, last score, and highest score
        cout << "Game Over!" << endl;
        cout << "Your last score was: " << score << endl;
        cout << "Highest score: " << highestScore << endl;
    }
};

int main() {
    
    SnakeGame game(24, 17);
    game.start();
    return 0;
}
