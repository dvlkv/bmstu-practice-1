#include <iostream>
#include "Menu.h"
#include "primitives/dynamic_array.h"
#include "student.hpp"
#include "utils.hpp"
#include "ctime"

auto st = new dynamic_array<Student>();

void read_user_handler() {
    std::cout << "Введите ФИО (макс 25 символов на слово): ";
    char* name = new char[50];
    char* surname = new char[50];
    char* patronymic = new char[50];
    std::cin >> name;
    std::cin >> surname;
    std::cin >> patronymic;

    std::cout << "Введите дату рождения (дд.мм.гггг): ";
    char* date_str = new char[255];
    std::tm tm{};
    while (true) {
        std::cin >> date_str;
        bool is_d = strptime(date_str, "%d.%m.%Y", &tm);
        if (!is_d) {
            std::cout << "Неверное значение. Введите курс заново: ";
        } else {
            break;
        }
    }

    std::cout << "Введите курс (число): ";
    char* grade_str = new char[255];
    int grade;
    while (true) {
        std::cin >> grade_str;
        bool is_n = is_number(grade_str);
        if (is_n) {
            grade = atoi(grade_str);
        }
        if (!is_n || (grade < 1 || grade > 6)) {
            std::cout << "Неверное значение. Введите курс заново: ";
        } else {
            break;
        }
    }

    char* group = new char[10];
    std::cout << "Введите группу (макс 5 символов): ";
    std::cin >> group;

    st->push(Student(name, surname, patronymic, timelocal(&tm), grade, group));
}

void query_handler() {
    print_table(*st);
}

void import_csv_handler() {
    std::cout << "Введите имя файла: ";
    char* fileName = new char[255];
    std::cin >> fileName;

    auto new_students = read_csv(fileName);
    if (new_students == nullptr) {
        std::cout << "Неверный формат файла или файл не существует";
        return;
    }
    delete st;
    st = new_students;
    print_table(*st);
}

void import_bin(char* fileName) {
    FILE* f = fopen(fileName, "r");
    auto new_students = read_from_file(f);
    if (new_students == nullptr) {
        std::cout << "Неверный формат файла или файл не существует";
        return;
    }
    delete st;
    st = new_students;
}

void import_bin_handler() {
    std::cout << "Введите имя файла: ";
    char* fileName = new char[255];
    std::cin >> fileName;

    import_bin(fileName);
    print_table(*st);
}

void export_csv_handler() {
    std::cout << "Введите имя файла: ";
    char* fileName = new char[255];
    std::cin >> fileName;

    std::ofstream outfile(fileName, 2);
    for (int i = 0; i < st->size(); i++) {
        if (i != 0) outfile << "\n";
        outfile << student_to_csv(st->get(i)).str().c_str();
    }
    outfile.close();
}

void export_console_handler() {
    for (int i = 0; i < st->size(); i++) {
        std::cout << student_to_csv(st->get(i)).str().c_str() << "\n";
    }
}

void export_bin_handler() {
    std::cout << "Введите имя файла: ";
    char* fileName = new char[255];
    std::cin >> fileName;

    FILE* f = fopen(fileName, "w+");
    save_to_file(st, f);
    fflush(f);
    fclose(f);
}

int main() {
    import_bin("../keklol.dat");

    Menu menu = Menu();

    Menu importMenu = Menu();
    importMenu
            .add("Импорт из файла (csv)", import_csv_handler)
            .add("Импорт из файла (bin)", import_bin_handler);


    Menu exportMenu = Menu();
    exportMenu.add("Экспорт в консоль", export_console_handler)
            .add("Экспорт в файл (csv)", export_csv_handler)
            .add("Экспорт в файл (bin)", export_bin_handler);

    Menu editMenu = Menu();
    editMenu
            .add("Создать запись", read_user_handler)
            .add("Редактировать запись", query_handler)
            .add("Удалить запись", query_handler)
            .add("Сортировка", query_handler);


    menu
      .add("Просмотр", query_handler)
      .add("Импорт", &importMenu)
      .add("Экспорт", &exportMenu)
      .add("Редактирование", &editMenu)
      .start();

    return 0;
}
