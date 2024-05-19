#include <SFML/Graphics.hpp>
#include <cmath>

#define WIDTH 800
#define HEIGHT 600
#define FPS 60
#define GRID_SIDE_SIZE 10
#define BACKGROUND_COLOR sf::Color::Black

void createWindow() {
    // Создаем окно с размером WIDTH x HEIGHT пикселей и названием "GEMS Game"
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "GEMS Game", sf::Style::Close);

    // Главный цикл программы: выполняется, пока открыто окно
    while (window.isOpen()) {
        // Обработка событий в цикле
        sf::Event event;
        while (window.pollEvent(event)) {
            // Пользователь закрыл окно: закрыть окно
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Очистка окна (с черным фоном)
        window.clear(BACKGROUND_COLOR);


        // Отображение содержимого окна
        window.display();
    }
}
void start_game() {
    // Здесь код для запуска игры
}

void draw_grid() {
    // Здесь код для рисования сетки
}

void draw_menu() {
    // Здесь код для рисования меню
}


int main() {
    createWindow();


    return 0;
}
