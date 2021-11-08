//
// Created by Данила Волков on 05.10.2021.
//

#include <iostream>
#include "Menu.h"


BaseMenuItem::BaseMenuItem(char* title) {
    this->title = title;
}

void BaseMenuItem::handle() {
}


FunctionMenuItem::FunctionMenuItem(char* title, void (*handler)()) : BaseMenuItem(title) {
    this->handler = handler;
}

void FunctionMenuItem::handle() {
    BaseMenuItem::handle();
    this->handler();

    // Draw menu after 2 lines
    std::cout << "\n\n";
}

NestedMenuMenuItem::NestedMenuMenuItem(char* title, Menu& menu) : BaseMenuItem(title) {
    this->menu = &menu;
}
void NestedMenuMenuItem::handle() {
    BaseMenuItem::handle();

    this->menu->start();

    // Draw previous menu clearly
    std::cout << "\x1B[2J\x1B[H";
};


Menu::Menu() {
}

void Menu::start() {
    bool shouldExit = false;
    std::cout << "\x1B[2J\x1B[H";

    while (!shouldExit) {
        for (int i = 1; i <= items.size(); i++) {
            std::cout << i << ". " << items[i-1]->title << "\n";
        }
        std::cout << "0. Выйти\n";
        std::cout << "Введите пункт меню: ";
        std::cout.flush();

        char selected{};
        std::cin >> selected;
        std::cin.clear();

        bool isHandled = false;
        if (selected == '0') {
            shouldExit = true;
            isHandled = true;
        } else {
            for (int i = 1; i <= items.size(); i++) {
                if (selected == i + '0') {
                    std::cout << "\x1B[2J\x1B[H";
                    items[i-1]->handle();
                    isHandled = true;
                }
            }
        }
        if (!isHandled) {
            std::cout << "\x1B[2J\x1B[H";
            std::cout << "Выбран несуществующий пункт меню.\n\n";
        }
    }
}

Menu& Menu::add(char* title, void handler()) {
    auto item = new FunctionMenuItem(title, handler);
    items.push(item);
    return *this;
}

Menu& Menu::add(char *title, Menu* menu) {
    auto item = new NestedMenuMenuItem(title, *menu);
    items.push(item);
    return *this;
}