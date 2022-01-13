
//
// Created by Данила Волков on 23.10.2021.
//

#include <algorithm>

#ifndef KURS_1_PRAKTIKA_DYNAMIC_ARRAY_H
#define KURS_1_PRAKTIKA_DYNAMIC_ARRAY_H


template<typename T>
class dynamic_array {
private:
    unsigned int space;
    unsigned int _size;
public:
    T* storage;
    dynamic_array();
    dynamic_array(dynamic_array<T>&& ref) noexcept;
    dynamic_array(const dynamic_array<T>& ref);
    explicit dynamic_array(unsigned int size);
    ~dynamic_array();

    unsigned int size();
    void push(T value);
    void resize(unsigned int n);
    void sort(int handler(T a, T b));
    void remove(unsigned int n);
    T& get(unsigned int n);


    T& operator[](unsigned int n);
};


template<typename T>
dynamic_array<T>::dynamic_array(unsigned int size) : space(size), _size(0), storage(new T[size]) {
}
template<typename T>
dynamic_array<T>::dynamic_array() : space(1), _size(0), storage(new T[1]) {
}
template<typename T>
dynamic_array<T>::dynamic_array(dynamic_array<T>&& obj) noexcept {
    this->storage = obj.storage;
    this->space = obj.space;
    this->_size = obj._size;
}
template<typename T>
dynamic_array<T>::~dynamic_array() {
    delete[] this->storage;
}

template<typename T>
dynamic_array<T>::dynamic_array(const dynamic_array<T> &obj) {
    this->storage = new T[obj.space];
    memcpy(this->storage, obj.storage, obj.space * sizeof(T));

    this->space = obj.space;
    this->_size = obj._size;
}


template<typename T>
unsigned int dynamic_array<T>::size() {
    return this->_size;
}

template<typename T>
void dynamic_array<T>::push(T value) {
    if (this->_size == this->space) {
        this->resize(this->space * 2);
    }

    this->storage[this->_size++] = value;
}

template<typename T>
T& dynamic_array<T>::get(unsigned int i) {
    return this->storage[i];
}

template<typename T>
void dynamic_array<T>::resize(unsigned int n) {
    this->storage = (T*)realloc((void*)this->storage, n * sizeof(T));
    this->space = n;
}

template<typename T>
T& dynamic_array<T>::operator[](unsigned int n) {
    return this->storage[n];
}

template<typename T>
void dynamic_array<T>::sort(int compare(T, T)) {
    for (int i = 0; i < this->_size; i++) {
        for (int  j = 0; j < this->_size - i - 1; j++) {
            int cmp_result = compare(this->storage[j], this->storage[j+1]);
            if (cmp_result > 0) {
                auto tmp = this->storage[j];
                this->storage[j] = this->storage[j+1];
                this->storage[j+1] = tmp;
            }
        }
    }
}

template<typename T>
void dynamic_array<T>::remove(unsigned int n) {
    if (n >= this->_size) {
        return;
    }

    for (unsigned int i = n+1; i < this->_size; i++) {
        this->storage[i-1] = this->storage[i];
    }
    this->_size--;
}


#endif //KURS_1_PRAKTIKA_DYNAMIC_ARRAY_H