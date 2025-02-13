#include <iostream>
#include <termios.h> // for terminal settings (Linux/macOS)
#include <unistd.h>  // for usleep()
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // for time()
#include <vector>
#include <fcntl.h>   // for fcntl()
using namespace std;

const int GRID_WIDTH = 40; // width of the grid (40 columns)
const int GRID_HEIGHT = 20; // height of the grid (20 rows)
const char SNAKE_BODY = 'O';
const char FOOD = '$';
const char EMPTY = ' ';
int highScore = 0;
// Direction enum for snake movement
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

// Function to get keyboard input without blocking
template <typename T>
void disableEcho(T &oldt) {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

template <typename T>
void enableEcho(T &oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

class Food {
public:
    int x, y;

    Food(int gridWidth, int gridHeight) {
        vector< pair<int, int> > v;
        respawn(gridWidth, gridHeight, v); // Initialize with an empty snake body
    }

    void respawn(int gridWidth, int gridHeight, const vector< pair<int, int> > &snakeBody) {
        bool validPosition = false;
        while (!validPosition) {
            x = rand() % (gridWidth - 2) + 1; // Adjust for new width
            y = rand() % (gridHeight - 2) + 1; // Adjust for new height
            validPosition = true;
            for (const auto &bodyPart : snakeBody) {
                if (bodyPart.first == x && bodyPart.second == y) {
                    validPosition = false;
                    break;
                }
            }
        }
    }
};

class Snake {
public:
    vector< pair<int, int> > body;
    Direction dir;

    Snake() {
        dir = STOP;
        body.push_back(make_pair(GRID_WIDTH / 2, GRID_HEIGHT / 2));
        body.push_back(make_pair(GRID_WIDTH / 2, GRID_HEIGHT / 2-1));
        body.push_back(make_pair(GRID_WIDTH / 2, GRID_HEIGHT / 2-2)); // Start at center of grid
    }

    void move() {
        pair<int, int> head = body[0];
        switch (dir) {
        case UP:    head.second--; break;
        case DOWN:  head.second++; break;
        case LEFT:  head.first--; break;
        case RIGHT: head.first++; break;
        default: break;
        }

        // Move the snake's body
        body.insert(body.begin(), head);
        body.pop_back();
    }

    void grow() {
        body.push_back(body.back());
    }

    bool hasCollided() {
        // Check collision with self
        for (size_t i = 3; i < body.size(); ++i) {
            if (body[i] == body[0]) return true;
        }

        // Check collision with boundaries
        return body[0].first < 0 || body[0].second < 0 ||
               body[0].first >= GRID_WIDTH || body[0].second >= GRID_HEIGHT;
    }
};

class Game {
private:
    Snake snake;
    Food food;
    int score;
    int foodEaten;
    bool gameOver;
    int sleepDuration;

public:
    Game() : food(GRID_WIDTH, GRID_HEIGHT), score(0), foodEaten(0), gameOver(false), sleepDuration(200000) {
        food.respawn(GRID_WIDTH, GRID_HEIGHT, snake.body); // Ensure food is placed at game start
    }

    void draw() {
        system("clear");

        // Print Game Title and Instructions
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "\033[1;32m             SNAKE GAME\033[0m" << endl;
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "Controls: 'W' - Up | 'S' - Down | 'A' - Left | 'D' - Right | 'X' - Exit" << endl;
        cout << "\033[1;35m--------------------------------------\033[0m" << endl;

        // Draw the grid
        for (int i = 0; i < GRID_HEIGHT; i++) {
            for (int j = 0; j < GRID_WIDTH; j++) {
                if (i == 0 || i == GRID_HEIGHT - 1 || j == 0 || j == GRID_WIDTH - 1) {
                    cout << "\033[1;34m#\033[0m"; // Boundary
                } else {
                    bool isSnake = false;
                    for (const auto &bodyPart : snake.body) {
                        if (bodyPart.first == j && bodyPart.second == i) {
                            cout << "\033[1;32m"<< SNAKE_BODY<< "\033[0m" ; // Snake in green
                            isSnake = true;
                            break;
                        }
                    }
                    if (food.x == j && food.y == i) {
                        cout << "\033[1;35m" <<  FOOD << "\033[0m"; // Food in yellow
                    } else if (!isSnake) {
                        cout << EMPTY;
                    }
                }
            }
            cout << endl;
        }

        // Display score in a stylish way
        cout <<"Score : " << score  << endl;
        cout << "\nHighest Score: " << highScore << endl<<endl;
    }

    void input() {
        if (kbhit()) {
            char key;
            cin >> key;
            switch (key) {
            case 'w': if (snake.dir != DOWN) snake.dir = UP; break;
            case 's': if (snake.dir != UP) snake.dir = DOWN; break;
            case 'a': if (snake.dir != RIGHT) snake.dir = LEFT; break;
            case 'd': if (snake.dir != LEFT) snake.dir = RIGHT; break;
            case 'x': gameOver = true; break; // Exit the game
            }
        }
    }

    void logic() {
        // Check if snake eats the food
        if (snake.body[0].first == food.x && snake.body[0].second == food.y) {
            score += 10;
            foodEaten++;
            snake.grow();
            food.respawn(GRID_WIDTH, GRID_HEIGHT, snake.body);

            // Increase speed after every 5 food items eaten
            if (foodEaten % 6 == 0) {
                sleepDuration = max(50000, sleepDuration - 50000); // Decrease sleep duration but not below 50ms
            }
        }
        snake.move();
        // Check for collisions
        if (snake.hasCollided()) {
            gameOver = true;
        }
    }

    void run() {
        struct termios oldt;
        disableEcho(oldt);

        while (!gameOver) {
            draw();
            input();
            logic();
            usleep(sleepDuration); // Adjust speed to make the snake faster
        }

        enableEcho(oldt);
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }
    int getScore() const {
        return score;
    }
};

int main() {
    
    char choice;
    do {
        srand(time(0)); // Initialize random seed only once
        Game game;
        game.run();
        if (game.getScore() > highScore) {
            highScore = game.getScore(); // Update the high score if the current score is higher
        }
        system("clear");
        cout << "\nCurrent Score: " << game.getScore() << endl;
        cout << "\nHighest Score: " << highScore << endl<<endl;
        cout << "Do you want to restart the game? (y/n): ";
        cin >> choice;
        cout << endl;
    } while (choice == 'y');
    system("clear");
    cout << "\nHighest Score: " << highScore << endl<<endl;
    cout << "\033[1;33m--------------------------------------\033[0m" << endl;
    cout<<  "\033[1;32m                GAME OVER\033[0m"<<endl;
    cout << "\033[1;33m--------------------------------------\033[0m" << endl;
    
    return 0;
}
