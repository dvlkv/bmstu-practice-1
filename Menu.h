//
// Created by Данила Волков on 05.10.2021.
//
#include <utility>
#include <vector>
#include "primitives/dynamic_array.h"

#ifndef KURS_1_PRAKTIKA_MENU_H
#define KURS_1_PRAKTIKA_MENU_H

class BaseMenuItem {
public:
    char* title;
    BaseMenuItem(
            char* title
    );

    virtual void handle();
};


class FunctionMenuItem : public BaseMenuItem {
private:
    void (*handler)();
public:
    char* title;
    FunctionMenuItem(
            char* title,
            void handler()
    );

    void handle() override;
};


struct Menu {
private:
    dynamic_array<BaseMenuItem*> items;
public:
    Menu& add(char* title, void handler());
    Menu& add(char* title, Menu* menu);
    void start();
    Menu();
};

class NestedMenuMenuItem : public BaseMenuItem {
private:
    Menu* menu;
public:
    NestedMenuMenuItem(char *title, Menu& menu);

    void handle() override;
};


#endif //KURS_1_PRAKTIKA_MENU_H
