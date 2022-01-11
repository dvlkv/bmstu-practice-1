//
// Created by Данила Волков on 24.12.2021.
//

#include "student.hpp"
#include "primitives/dynamic_array.h"
#include <string>
#include <codecvt>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>

bool is_number(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

int real_len(const char* str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(str);
    return wide.length();
}

template <typename T>
std::string pad(T value, int n) {
    std::stringstream tmp{};
    tmp << value;
    int mx = n - real_len(tmp.str().c_str());
    for (int i = 0; i < mx; i++) tmp << " ";
    return tmp.str();
}

void print_table(dynamic_array<Student> s) {
    int max_row_sizes[7]{2, 2, 2, 2, 15, 5, 8};
    for (int i = 0; i < s.size(); i++) {
        max_row_sizes[1] = std::max((int)real_len(s[i].surname) + 2, max_row_sizes[1]);
        max_row_sizes[2] = std::max((int)real_len(s[i].name) + 2, max_row_sizes[2]);
        max_row_sizes[3] = std::max((int)real_len(s[i].patronymic) + 2, max_row_sizes[3]);
        max_row_sizes[6] = std::max((int)real_len(s[i].group) + 2, max_row_sizes[6]);
    }
    std::stringstream header{};
    header << "| "<< pad("№", max_row_sizes[0]);
    header << "| "<< pad("Фамилия", max_row_sizes[1]);
    header << "| "<< pad( "Имя", max_row_sizes[2]);
    header << "| "<< pad( "Отчество", max_row_sizes[3]);
    header << "| "<< pad( "Дата рождения", max_row_sizes[4]);
    header << "| "<< pad( "Курс", max_row_sizes[5]);
    header << "| "<< pad( "Группа", max_row_sizes[6]) << "|";
    for (int i = 0; i < real_len(header.str().c_str()); i++) std::cout << "-";
    std::cout << '\n';
    std::cout << header.str() << '\n';
    for (int i = 0; i < real_len(header.str().c_str()); i++) std::cout << "-";
    std::cout << '\n';
    for (int i = 0; i < s.size(); i++) {
        auto user = s[i];
        time_t kek = user.birthdate;
        auto *bd = localtime(&kek);

        std::cout << "| "<< pad(i+1, max_row_sizes[0]);
        std::cout << "| "<< pad(user.surname, max_row_sizes[1]);
        std::cout << "| "<< pad(user.name, max_row_sizes[2]);
        std::cout << "| "<< pad(user.patronymic, max_row_sizes[3]);
        std::cout << "| "<< pad(std::put_time(bd, "%d.%m.%Y"), max_row_sizes[4]);
        std::cout << "| "<< pad(user.grade, max_row_sizes[5]);
        std::cout << "| "<< pad(user.group, max_row_sizes[6]); std::cout << "|\n";
    }
    for (int i = 0; i < real_len(header.str().c_str()); i++) std::cout << "-";
    std::cout << '\n';
}

std::stringstream student_to_csv(Student student) {
    std::stringstream tmp{};
    time_t kek = student.birthdate;
    auto *bd = localtime(&kek);
    tmp << student.surname << "," << student.name << ",";
    tmp << student.patronymic << "," << std::put_time(bd, "%d.%m.%Y") << ",";
    tmp << student.grade << ",";
    tmp << student.group;

    return tmp;
}

dynamic_array<Student>* read_csv(char* file) {
    std::ifstream infile(file);
    if (!infile.good()) {
        return nullptr;
    }

    std::string line;
    auto result = new dynamic_array<Student>();
    while (std::getline(infile, line))
    {
        std::stringstream tmp{};
        char* cresult[6]{
            new char[255],
            new char[255],
            new char[255],
            new char[255],
            new char[255],
            new char[255]
        };
        int j = 0;
        for (int i = 0; i < line.length(); i++) {
            if (i == line.length() - 1) {
                tmp << line[i];
            }
            if (line[i] == ',' || i == line.length() - 1) {
                if (j == 6) {
                    std::cout << "invalid csv";
                    return nullptr;
                }
                auto part = tmp.str().c_str();
                memcpy(cresult[j], part, strlen(part));
                tmp = std::stringstream{};
                j++;
            } else {
                tmp << line[i];
            }
        }

        std::tm tm{};
        bool is_d = strptime(cresult[3], "%d.%m.%Y", &tm);
        if (!is_d) {
            return nullptr;
        }

        result->push(Student(
                cresult[0],
                cresult[1],
                cresult[2],
                timelocal(&tm),
                atoi(cresult[4]),
                cresult[5]
        ));
    }
    return result;
}

void write_int(int val, FILE* fd) {
    fwrite(&val, sizeof(int), 1, fd);
}
int read_int(FILE* fd) {
    int val{};
    fread(&val, sizeof(int), 1, fd);
    return val;
}
void write_string(char* str, FILE* fd) {
    int size = strlen(str);
    write_int(size, fd);
    fwrite(str, strlen(str), 1, fd);
}
char* read_string(FILE* fd) {
    int size{};
    fread(&size, sizeof(int), 1, fd);
    char* str = new char[size];
    fread(str, size, 1, fd);
    return str;
}

void save_to_file(dynamic_array<Student>* students, FILE* fd) {
    int size = students->size();
    write_int(size, fd);
    for (int i = 0; i < students->size(); i++) {
        auto student = students->get(i);
        write_string(student.surname, fd);
        write_string(student.name, fd);
        write_string(student.patronymic, fd);
        write_int(student.birthdate, fd);
        write_int(student.grade, fd);
        write_string(student.group, fd);
    }
}

dynamic_array<Student>* read_from_file(FILE* fd) {
    int size = read_int(fd);
    auto res = new dynamic_array<Student>(size);
    for (int i = 0; i < size; i++) {
        auto surname = read_string(fd);
        auto name = read_string(fd);
        auto patronymic = read_string(fd);
        auto birthdate = read_int(fd);
        auto grade = read_int(fd);
        auto group = read_string(fd);
        res->push(Student(surname, name, patronymic, birthdate, grade, group));
    }
    return res;
}