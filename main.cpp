#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>


//display parameters
#define WIDTH 1600
#define HEIGHT 900
#define FPS 120
#define FONT_FILE "Andale Mono.ttf"
//game parameters
#define GRID_SIZE 10
#define SQUARE_SIZE 70
//bonus parameters
#define POSIBILITY_BONUS 10
#define POSIBILITY_BOMB_DESTROY 25
#define POSIBILITY_BONUS_PAIN 75
//color parameters
#define BACKGROUND_COLOR sf::Color::Black
//color of squares and bonuses
#define RED_COLOR sf::Color::Red
#define GREEN_COLOR sf::Color::Green
#define BLUE_COLOR sf::Color::Blue
#define BONUS_COLOR_WHITE sf::Color::White
#define BONUS_COLOR_YELLOW sf::Color::Yellow
//textures parameters
#define BACKGROUND_TEXTURE_FILE_1 "textures/background_2.jpg"
#define BACKGROUND_TEXTURE_FILE_2 "textures/background_1.jpg"
#define BOMB_DESTROY_TEXTURE_FILE "textures/bomb_destroy.png"
#define PAINT_BOMB_TEXTURE_FILE "textures/paint_bomb.png"
#define SQUARE_TEXTURE_FILE_1 "textures/color_1.png"
#define SQUARE_TEXTURE_FILE_2 "textures/color_2.png"
#define SQUARE_TEXTURE_FILE_3 "textures/color_3.png"

struct Square {
    int x_start, y_start;
    int x_end, y_end;
    int target_y_start;
    bool is_moving;
    sf::Color color;

    Square() : x_start(0), y_start(0), x_end(0), y_end(0), target_y_start(0), is_moving(false),
               color(sf::Color::Black) {}
};

struct Bonus : public Square {
    Bonus() : Square() {}
};

struct Bomb_destroy : public Bonus {
    Bomb_destroy() : Bonus() {}
};

struct Bonus_paint : public Bonus {
    Bonus_paint() : Bonus() {}
};

struct Board {
    std::vector<std::vector<Square>> grid;

    Board() {
        grid.resize(GRID_SIZE, std::vector<Square>(GRID_SIZE));
    }
};

void create_grid(Board &board) {
    srand(static_cast<unsigned>(time(0)));
    std::vector<sf::Color> colors = {RED_COLOR, GREEN_COLOR, BLUE_COLOR};

    int shift_x = (WIDTH - GRID_SIZE * SQUARE_SIZE) / 2;
    int shift_y = (HEIGHT - GRID_SIZE * SQUARE_SIZE) / 2;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            board.grid[i][j].x_start = i * SQUARE_SIZE + shift_x;
            board.grid[i][j].y_start = j * SQUARE_SIZE + shift_y;
            board.grid[i][j].x_end = board.grid[i][j].x_start + SQUARE_SIZE;
            board.grid[i][j].y_end = board.grid[i][j].y_start + SQUARE_SIZE;

            int random_color_index = rand() % colors.size();
            board.grid[i][j].color = colors[random_color_index];
        }
    }
}

void draw_grid( Board &board, sf::RenderWindow &window, sf::Font &font, sf::Texture &color1, sf::Texture &color2,
               sf::Texture &color3,sf::Texture &bomb_destroy_texture, sf::Texture &paint_bomb_texture) {
    int shift_x = (WIDTH - GRID_SIZE * SQUARE_SIZE) / 2;
    int shift_y = (HEIGHT - GRID_SIZE * SQUARE_SIZE) / 2;
    sf::RectangleShape Rectangle(sf::Vector2f(SQUARE_SIZE*GRID_SIZE, SQUARE_SIZE*GRID_SIZE));
    sf::Color color(176, 103, 141, 222);
    Rectangle.setFillColor(color);
    Rectangle.setPosition(WIDTH / 2 - SQUARE_SIZE * GRID_SIZE / 2, HEIGHT / 2 - SQUARE_SIZE * GRID_SIZE / 2);
    window.draw(Rectangle);


    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            const Square &square = board.grid[i][j];
            sf::Sprite squareShape;
            // Set the texture based on the color of the square
            if (square.color == RED_COLOR) {
                squareShape.setTexture(color1);
            } else if (square.color == GREEN_COLOR) {
                squareShape.setTexture(color2);
            } else if (square.color == BLUE_COLOR) {
                squareShape.setTexture(color3);
            }
            else if (square.color == BONUS_COLOR_WHITE) {
                squareShape.setTexture(bomb_destroy_texture);
            }
            else if (square.color == BONUS_COLOR_YELLOW) {
                squareShape.setTexture(paint_bomb_texture);
            }

            // Set the position of the square
            board.grid[i][j].x_start = i * SQUARE_SIZE+ shift_x;
            board.grid[i][j].y_start = j * SQUARE_SIZE+ shift_y;

            squareShape.setPosition(square.x_start, square.y_start);
            squareShape.setScale(static_cast<float>(SQUARE_SIZE) / squareShape.getLocalBounds().width, static_cast<float>(SQUARE_SIZE) / squareShape.getLocalBounds().height);
            window.draw(squareShape);

        }
    }
}

void change_two_squares(Board &board, int x1, int y1, int x2, int y2) {
    sf::Color temp_color = board.grid[x1][y1].color;
    board.grid[x1][y1].color = board.grid[x2][y2].color;
    board.grid[x2][y2].color = temp_color;
}

void update_moving_squares(Board &board, float deltaTime) {
    const float speed = 500.0f; // Speed of the falling squares in pixels per second

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board.grid[i][j].is_moving) {
                int &y_start = board.grid[i][j].y_start;
                int target = board.grid[i][j].target_y_start;

                if (y_start < target) {
                    y_start = std::min(target, y_start + static_cast<int>(speed * deltaTime));
                } else if (y_start > target) {
                    y_start = std::max(target, y_start - static_cast<int>(speed * deltaTime));
                }

                if (y_start == target) {
                    board.grid[i][j].is_moving = false;
                }
            }
        }
    }
}

bool move_squares_down(Board &board, float deltaTime) {
    bool moved = false;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = GRID_SIZE - 1; j >= 0; --j) {
            if (board.grid[i][j].color == sf::Color::Black) {
                for (int k = j - 1; k >= 0; --k) {
                    if (board.grid[i][k].color != sf::Color::Black) {
                        board.grid[i][j].color = board.grid[i][k].color;
                        board.grid[i][j].is_moving = true;
                        board.grid[i][j].target_y_start = board.grid[i][j].y_start;
                        board.grid[i][k].color = sf::Color::Black;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }
    // Update moving squares
    update_moving_squares(board, deltaTime);

    return moved;
}


void apply_bonus(Board &board, int x, int y) {
    if (rand() % 2 == 0) {
        board.grid[x][y].color = BONUS_COLOR_WHITE;
    } else {
        board.grid[x][y].color = BONUS_COLOR_YELLOW;
    }
}

void create_new_squares(Board &board) {
    std::vector<sf::Color> colors = {RED_COLOR, GREEN_COLOR, BLUE_COLOR};

    int shift_y = (HEIGHT - GRID_SIZE * SQUARE_SIZE) / 2;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board.grid[i][j].color == sf::Color::Black) {
                if (rand() % POSIBILITY_BONUS == 0) {
                    apply_bonus(board, i, j);
                } else {
                    int random_color_index = rand() % colors.size();
                    board.grid[i][j].color = colors[random_color_index];
                }
                board.grid[i][j].y_start = shift_y - SQUARE_SIZE;
                board.grid[i][j].is_moving = true;
                board.grid[i][j].target_y_start = j * SQUARE_SIZE + shift_y;

            }
        }
    }
}
void do_bonus_things(Board &board) {

}
bool check_grid(Board &board, int &score) {
    bool found_match = false;
    // Check horizontal matches
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE - 2; ++j) {
            if (board.grid[i][j].color == board.grid[i][j + 1].color &&
                board.grid[i][j].color == board.grid[i][j + 2].color &&
                board.grid[i][j].color != sf::Color::Black) {
                board.grid[i][j].color = sf::Color::Black;
                board.grid[i][j + 1].color = sf::Color::Black;
                board.grid[i][j + 2].color = sf::Color::Black;
                found_match = true;
                score += 3;
            }
        }
    }

    // Check vertical matches
    for (int j = 0; j < GRID_SIZE; ++j) {
        for (int i = 0; i < GRID_SIZE - 2; ++i) {
            if (board.grid[i][j].color == board.grid[i + 1][j].color &&
                board.grid[i][j].color == board.grid[i + 2][j].color &&
                board.grid[i][j].color != sf::Color::Black) {
                board.grid[i][j].color = sf::Color::Black;
                board.grid[i + 1][j].color = sf::Color::Black;
                board.grid[i + 2][j].color = sf::Color::Black;
                found_match = true;
                score += 3;
            }
        }
    }
    return found_match;
}

std::pair<int, int> find_square(const Board &board, int x, int y) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board.grid[i][j].x_start <= x && board.grid[i][j].x_end >= x &&
                board.grid[i][j].y_start <= y && board.grid[i][j].y_end >= y) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

void display_score(sf::RenderWindow &window, int score, sf::Font &font) {
    sf::Text text;

    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(30);
    text.setString("Score: " + std::to_string(score));
    text.setPosition(20, 20);
    window.draw(text);
}

void display_time_left(sf::RenderWindow &window, float time_left, sf::Font &font) {
    sf::Text text;

    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setString("Time left: " + std::to_string(time_left));
    text.setPosition(20, 50);
    window.draw(text);
}

void mouse_pressed_on_square(sf::RenderWindow &window, Board &board, int x1, int y1, int x2, int y2) {
    int found_x1, found_y1;
    std::tie(found_x1, found_y1) = find_square(board, x1, y1);
    std::cout << "First square: " << found_x1 << " " << found_y1 << std::endl;

    int found_x2, found_y2;
    std::tie(found_x2, found_y2) = find_square(board, x2, y2);
    std::cout << "Second square: " << found_x2 << " " << found_y2 << std::endl;

    if (found_x1 == found_x2 && found_y1 == found_y2) {
        std::cout << "Same square" << std::endl;
        return;
    }
    if (!(found_x1 == -1 || found_y1 == -1 || found_x2 == -1 || found_y2 == -1)) {
        if (board.grid[found_x1][found_y1].color != board.grid[found_x2][found_y2].color) {
            if (abs(found_x1 - found_x2) + abs(found_y1 - found_y2) == 1) {
                change_two_squares(board, found_x1, found_y1, found_x2, found_y2);
                std::cout << "Changed" << std::endl;
            }
        }
    }
    return;
}

bool load_texture(sf::Texture &texture, const std::string &filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error loading texture: " << filename << std::endl;
        return false;
    }
    return true;
}
void display_user_interface(sf::RenderWindow &window, sf::Font &font) {
    std::cerr << "Displaying UI" << std::endl;

    sf::Texture background_texture;
    if (!load_texture(background_texture, BACKGROUND_TEXTURE_FILE_1)) {
        std::cerr << "Error loading background texture" << std::endl;
        return;
    }

    window.clear(BACKGROUND_COLOR);

    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);
    background_sprite.setPosition(0, 0);
    background_sprite.setScale(WIDTH / background_sprite.getLocalBounds().width,
                               HEIGHT / background_sprite.getLocalBounds().height);

    window.draw(background_sprite);

    sf::RectangleShape button_start(sf::Vector2f(3 * SQUARE_SIZE, 1 * SQUARE_SIZE));
    button_start.setFillColor(sf::Color::White);
    button_start.setOutlineColor(sf::Color::Black);
    button_start.setOutlineThickness(5);
    button_start.setPosition(WIDTH / 2-4/3*SQUARE_SIZE, HEIGHT / 2);

    sf::Text text_start;
    text_start.setFont(font);
    text_start.setFillColor(sf::Color::Black);
    text_start.setCharacterSize(50);
    text_start.setString("Start");
    text_start.setPosition(WIDTH / 2-4/3*SQUARE_SIZE, HEIGHT / 2);

    sf::RectangleShape button_quit(sf::Vector2f(3 * SQUARE_SIZE, 1 * SQUARE_SIZE));
    button_quit.setFillColor(sf::Color::White);
    button_quit.setOutlineColor(sf::Color::Black);
    button_quit.setOutlineThickness(5);
    button_quit.setPosition(WIDTH / 2-4/3*SQUARE_SIZE, HEIGHT * 3 / 4);

    sf::Text text_quit;
    text_quit.setString("Quit");
    text_quit.setFont(font);
    text_quit.setCharacterSize(50);
    text_quit.setFillColor(sf::Color::Black);
    text_quit.setPosition(WIDTH / 2-4/3*SQUARE_SIZE, HEIGHT * 3 / 4);
    std::cerr << "UI drawn" << std::endl;


    window.draw(button_start);
    window.draw(text_start);
    window.draw(button_quit);
    window.draw(text_quit);
    window.display();

}



void start_game() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "GEMS Game:Nekopara edition", sf::Style::Default);
    window.setFramerateLimit(FPS);
    window.setVerticalSyncEnabled(true);
    window.clear(BACKGROUND_COLOR);

    sf::Font font;
    if (!font.loadFromFile(FONT_FILE)) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    sf::Texture color1, color2, color3, background_texture_1, background_texture_2, bomb_destroy_texture, paint_bomb_texture;
    if (!load_texture(color1, SQUARE_TEXTURE_FILE_1) ||
        !load_texture(color2, SQUARE_TEXTURE_FILE_2) ||
        !load_texture(color3, SQUARE_TEXTURE_FILE_3) ||
        !load_texture(background_texture_1, BACKGROUND_TEXTURE_FILE_1) ||
        !load_texture(background_texture_2, BACKGROUND_TEXTURE_FILE_2)||!load_texture(bomb_destroy_texture, BOMB_DESTROY_TEXTURE_FILE) || !load_texture(paint_bomb_texture, PAINT_BOMB_TEXTURE_FILE)) {
        std::cerr << "Failed to load one or more textures." << std::endl;
        return;
    }

    sf::Sprite background_sprite_1, background_sprite_2;
    background_sprite_1.setTexture(background_texture_1);
    background_sprite_1.setPosition(0, 0);
    background_sprite_1.setScale(WIDTH / background_sprite_1.getLocalBounds().width,
                                 HEIGHT / background_sprite_1.getLocalBounds().height);
    background_sprite_2.setTexture(background_texture_2);
    background_sprite_2.setPosition(0, 0);
    background_sprite_2.setScale(WIDTH / background_sprite_2.getLocalBounds().width,
                                 HEIGHT / background_sprite_2.getLocalBounds().height);

    Board board;
    int score = 0;
    create_grid(board);
    bool firstClick = true;
    bool started = false;
    int x1 = 0, y1 = 0;

    int start_button_x = WIDTH / 2-4/3*SQUARE_SIZE;
    int start_button_y = HEIGHT / 2;
    int quit_button_x = WIDTH / 2-4/3*SQUARE_SIZE;
    int quit_button_y = HEIGHT * 3 / 4;

    sf::Clock clock;
    float deltaTime_t = 0;
    display_user_interface(window, font);
    while (window.isOpen() && !started) {
        sf::Event event;
        while (window.pollEvent(event) && !started) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.x >= start_button_x && event.mouseButton.x <= start_button_x + 3 * SQUARE_SIZE &&
                    event.mouseButton.y >= start_button_y && event.mouseButton.y <= start_button_y + 1 * SQUARE_SIZE) {
                    started = true;
                } else if (event.mouseButton.x >= quit_button_x &&
                           event.mouseButton.x <= quit_button_x + 3 * SQUARE_SIZE &&
                           event.mouseButton.y >= quit_button_y &&
                           event.mouseButton.y <= quit_button_y + 1 * SQUARE_SIZE) {
                    window.close();
                    return;
                }
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (firstClick) {
                    x1 = event.mouseButton.x;
                    y1 = event.mouseButton.y;
                    firstClick = false;
                } else  {
                    int x2 = event.mouseButton.x;
                    int y2 = event.mouseButton.y;
                    mouse_pressed_on_square(window, board, x1, y1, x2, y2);
                    firstClick = true;
                    if (check_grid(board, score)) {
                        while (move_squares_down(board, deltaTime_t) || check_grid(board, score)) {
                            create_new_squares(board);
                            deltaTime_t = clock.restart().asSeconds();
                        }

                    }
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        move_squares_down(board, deltaTime);
        create_new_squares(board);
        window.clear(BACKGROUND_COLOR);
        window.draw(background_sprite_1);
        check_grid(board, score);
        display_score(window, score, font);
        draw_grid(board, window, font, color1, color2, color3, bomb_destroy_texture, paint_bomb_texture);
        window.display();
    }
}

int main() {
    start_game();

    return 0;
}
