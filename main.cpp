#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#define WIDTH 900
#define HEIGHT 900
#define CELL_NUMBERS 75

const int cellY = HEIGHT / CELL_NUMBERS, cellX = WIDTH / CELL_NUMBERS;
bool pause = false;

std::vector<std::vector<int>> board;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Conway's Game of Life");

int countAliveCells(int ln, int cl, std::vector<std::vector<int>> cboard = board) {
    int count = 0;

    for(int y = ln - 1; y <= ln + 1; y++) {
        for(int x = cl - 1; x <= cl + 1; x++) {
            if(y >= 0 && x >= 0 && y < CELL_NUMBERS && x < CELL_NUMBERS) {
                count += cboard[y][x];
            }
        }
    }

    return count - cboard[ln][cl];
}

const float delay = 0.0005f;
sf::Clock gameclock;
void handleNextGeneration() {
    float elapsed = gameclock.getElapsedTime().asSeconds();

    std::vector<std::vector<int>> temporally = board;
    if(elapsed >= delay) {
        for(int ln = 0; ln < CELL_NUMBERS; ln++) {
            std::vector<int> l = board[ln];
            for(int cl = 0; cl < CELL_NUMBERS; cl++) {
                int cellState = l[cl];
                int count = countAliveCells(ln, cl);

                if(cellState == 1) {
                    if(count < 2 || count > 3) temporally[ln][cl] = 0;
                } else {
                    if(count == 3) temporally[ln][cl] = 1;
                }
            }
        }
    }

    board = temporally;
    gameclock.restart();
}

void togglecell(int ln, int col) {
    int v = board[ln][col];
    if(v == 1) {
        board[ln][col] = 0;
    } else {
        board[ln][col] = 1;
    }
}

int main() {

    for(int i = 0; i < CELL_NUMBERS; ++i) {
        std::vector<int> ln;
        for(int j = 0; j < CELL_NUMBERS; ++j) {
            if((rand() % 100) < 30) ln.push_back(1);
            else ln.push_back(0);
        }
        board.push_back(ln);
    }

    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    pause = !pause;
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int col = floor(event.mouseButton.x / cellX);
                    int ln = floor(event.mouseButton.y / cellY);

                    gameclock.restart();
                    togglecell(ln, col);

                }
            }
        }

        window.clear(sf::Color(50, 50, 50));

        if(!pause) {
            handleNextGeneration();
        } else {
            gameclock.restart();
        }

        float th = 1.f;
        for(int ln = 0; ln < CELL_NUMBERS; ln++) {
            std::vector<int> l = board[ln];
            for(int cl = 0; cl < CELL_NUMBERS; cl++) {
                int col = l[cl];

                if(col == 1) {
                    sf::RectangleShape cell;
                    cell.setPosition(cl * cellX + th, ln * cellY + th);
                    cell.setSize(sf::Vector2f(cellX - th, cellY - th));

                    cell.setFillColor(sf::Color(255, 255, 255));
                    cell.setOutlineThickness(0);

                    window.draw(cell);
                }
            }
        }

        window.display();
    }
}