#include <iostream>
#include "Menu.h"
#include "primitives/dynamic_array.h"
#include "student.hpp"
#include "utils.hpp"
#include "ctime"

auto st = new dynamic_array<Student>();

int read_date() {
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
    return timelocal(&tm);
}

int read_grade() {
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
    return grade;
}

void read_user_handler() {
    std::cout << "Введите ФИО (макс 25 символов на слово): ";
    char* name = new char[50];
    char* surname = new char[50];
    char* patronymic = new char[50];
    std::cin >> name;
    std::cin >> surname;
    std::cin >> patronymic;

    std::cout << "Введите дату рождения (дд.мм.гггг): ";
    int date = read_date();

    std::cout << "Введите курс (число): ";
    int grade = read_grade();

    char* group = new char[20];
    std::cout << "Введите группу (макс 10 символов): ";
    std::cin >> group;

    st->push(Student(name, surname, patronymic, date, grade, group));
}

void query_handler() {
    auto filtered = dynamic_array<Student>();
    for (int i = 0; i < st->size(); i++) {
        auto student = st->get(i);
        time_t bd = student.birthdate;
        auto bd_time = localtime(&bd);
        if (bd_time->tm_mon == 3) {
            filtered.push(student);
        }
    }
    print_table(filtered);
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

void edit_user_handler() {
    print_table(*st);
    int n;
    while (true) {
        std::cout << "Введите номер студента: ";
        std::cin >> n;
        if (n <= 0 || n > st->size()) {
            std::cout << "Введен неверный номер студента.\n";
            continue;
        }
        break;
    }
    n--;
    while (true) {
        std::cout << "Выберите поле для редактирования:\n";
        std::cout << "1. Дата рождения\n";
        std::cout << "2. Курс\n";
        std::cout << "3. Имя\n";
        std::cout << "4. Фамилия\n";
        std::cout << "5. Отчество\n";
        std::cout << "6. Группа\n";
        std::cout << "0. Отменить\n\n";
        std::cout << "Выберите вариант: ";
        int option;
        std::cin >> option;
        if (option < 0 || option > 6) {
            std::cout << "Выбран не верный параметр.\n\n\n";
            continue;
        }
        if (option == 0) {
            break;
        }
        std::cout << "Введите новое значение: ";
        switch (option) {
            case 1:
                st->storage[n].birthdate = read_date();
                break;
            case 2:
                st->storage[n].grade = read_grade();
                break;
            case 3:
                std::cin >> st->storage[n].name;
                break;
            case 4:
                std::cin >> st->storage[n].surname;
                break;
            case 5:
                std::cin >> st->storage[n].patronymic;
                break;
            case 6:
                std::cin >> st->storage[n].group;
        }
        break;
    }
    dynamic_array<Student> affected{};
    affected.push(st->get(n));
    print_table(affected);
}

void remove_user_handler() {
    print_table(*st);
    std::cout << "Введите номер пользователя: ";
    unsigned int n;
    std::cin >> n;
    st->remove(n-1);
    print_table(*st);
}

int compare_students_bd(Student a, Student b) {
    return a.birthdate - b.birthdate;
}

int compare_students_grade(Student a, Student b) {
    return a.grade - b.grade;
}

int compare_students_name(Student a, Student b) {
    return strcmp(a.name, b.name);
}

int compare_students_surname(Student a, Student b) {
    return strcmp(a.surname, b.surname);
}

int compare_students_patronymic(Student a, Student b) {
    return strcmp(a.patronymic, b.patronymic);
}

void sort_users_handler() {
    int (*compare)(Student, Student);
    while (true) {
        std::cout << "Варианты сортировки:\n";
        std::cout << "1. Дата рождения\n";
        std::cout << "2. Курс\n";
        std::cout << "3. По имени\n";
        std::cout << "4. По фамилии\n";
        std::cout << "5. По отчеству\n";
        std::cout << "0. Отменить\n\n";
        std::cout << "Выберите вариант: ";
        int option;
        std::cin >> option;
        if (option < 0 || option > 5) {
            std::cout << "Выбран не верный параметр.\n\n\n";
            continue;
        }
        if (option == 0) {
            break;
        }
        switch (option) {
            case 1:
                compare = compare_students_bd;
                break;
            case 2:
                compare = compare_students_grade;
                break;
            case 3:
                compare = compare_students_name;
                break;
            case 4:
                compare = compare_students_surname;
                break;
            case 5:
                compare = compare_students_patronymic;
                break;
        }
        break;
    }
    if (compare == nullptr) {
        return;
    }
    st->sort(compare);
    print_table(*st);
}

int main() {
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
            .add("Редактировать запись", edit_user_handler)
            .add("Удалить запись", remove_user_handler)
            .add("Сортировка", sort_users_handler);


    menu
      .add("Запрос", query_handler)
      .add("Импорт", &importMenu)
      .add("Экспорт", &exportMenu)
      .add("Редактирование", &editMenu)
      .start();

    return 0;
}
