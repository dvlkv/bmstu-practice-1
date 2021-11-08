#include <iostream>
#include <iomanip>
#include "Menu.h"
#include "primitives/dynamic_array.h"

struct User {
    char* name;
    char* surname;
    char* patronymic;


    User() {};
    User(char* n, char* s, char* p) : name(n), surname(s), patronymic(p) {}
};

auto st = dynamic_array<User>();

void read_user_handler() {
    std::cout << "Введите имя, затем фамилию и отчество (макс 25 символов на слово): ";
    char* name = new char[25];
    char* surname = new char[25];
    char* patronymic = new char[25];
    std::cin >> name;
    std::cin >> surname;
    std::cin >> patronymic;

    st.push(User(name, surname, patronymic));
}

void query_handler() {
    for (int i = 0; i < st.size(); i++) {
        auto user = st[i];
        std::cout << "| " << std::setw(25) << std::left << user.name;
        std::cout << "| " << std::setw(25) << std::left << user.surname;
        std::cout << "| " << std::setw(25) << std::left << user.patronymic << "|\n";
    }
}

int main() {
    Menu menu = Menu();

    Menu importMenu = Menu();
    importMenu
            .add("Импорт из файла (csv)", query_handler)
            .add("Импорт из файла (backup)", query_handler);


    Menu exportMenu = Menu();
    exportMenu.add("Экспорт в консоль", query_handler)
            .add("Экспорт в файл (csv)", query_handler)
            .add("Создание backup", query_handler);

    Menu editMenu = Menu();
    editMenu
            .add("Создать запись", read_user_handler)
            .add("Редактировать запись", query_handler)
            .add("Удалить запись", query_handler);


    menu
      .add("Импорт", &importMenu)
      .add("Экспорт", &exportMenu)
      .add("Редактирование", &editMenu)
      .start();

    return 0;
}
