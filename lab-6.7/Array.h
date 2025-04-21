#pragma once
#include <exception>
#include <iosfwd>
#include <stdexcept>
using namespace std;

class Array
{
public:
    // типи
    typedef unsigned int UINT;
    typedef double value_type;
    typedef double* iterator;
    typedef const double* const_iterator;
    typedef double& reference;
    typedef const double& const_reference;
    typedef std::size_t size_type;

private:
    static const size_type minsize = 10; // мінімальний розмір масиву
    size_type Size; // виділено пам'яті для елементів
    size_type Count; // кількість елементів в масиві
    size_type First; // значення індексу першого елемента в масиві
    value_type* elems; // вказівник на дані

public:
    // конструктори/копіювання/деструктор
    Array(const size_type& n = minsize) throw(bad_alloc, invalid_argument);
    Array(const Array&) throw(bad_alloc);
    Array(const iterator first, const iterator last) throw(bad_alloc, invalid_argument);
    Array(const size_type first, const size_type last) throw(bad_alloc, invalid_argument);
    ~Array();
    Array& operator=(const Array&);

    // ітератори
    iterator begin() { return elems; }
    const_iterator begin() const { return elems; }
    iterator end() { return elems + Count; }
    const_iterator end() const { return elems + Count; }

    // розміри
    size_type size() const; // поточний розмір
    bool empty() const; // якщо є елементи
    size_type capacity() const; // потенційний розмір
    void resize(size_type newsize) throw(bad_alloc); // змінити розмір

    // доступ до елементів
    reference operator[](size_type) throw(out_of_range);
    const_reference operator[](size_type) const throw(out_of_range);
    reference front() { return elems[0]; }
    const_reference front() const { return elems[0]; }
    reference back() { return elems[size() - 1]; }
    const_reference back() const { return elems[size() - 1]; }

    // методи-модифікатори
    void push_back(const value_type& v); // додати елемент в кінець
    void pop_back(); // видалити останній елемент
    void clear() { Count = 0; } // очистити масив
    void swap(Array& other); // поміняти з другим масивом
    void assign(const value_type& v); // заповнити масив

    // дружні функції вводу/виводу
    friend ostream& operator <<(ostream& out, const Array& a);
    friend istream& operator >>(istream& in, Array& a);
};