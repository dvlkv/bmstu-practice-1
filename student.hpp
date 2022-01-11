//
// Created by Данила Волков on 24.12.2021.
//

#ifndef STUDENT_STRUCT
#define STUDENT_STRUCT

#pragma pack(push, 1)
struct Student {
    char* name;
    char* surname;
    char* patronymic;
    int birthdate;
    int grade;
    char* group;

    Student() = default;
    Student(char* s, char* n, char* p, int bd, int y, char* g) :
            name(n), surname(s), patronymic(p), birthdate(bd), grade(y), group(g) {}
};
#pragma pack(pop)

#endif