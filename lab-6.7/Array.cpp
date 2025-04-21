#include "Array.h" 
#include <iostream> 
#include <stdexcept> 
#include <exception> 
using namespace std; 

// Конструктор: ініціалізація масиву фіксованого розміру
Array::Array(const Array::size_type& n) throw(bad_alloc, invalid_argument)
{
    First = 0;
    Count = Size = n;
    elems = new value_type[Size]; // Виділення пам'яті
    for (UINT i = 0; i < Size; i++)
        elems[i] = 0; // Ініціалізація значеннями 0
}

// Конструктор з діапазону ітераторів
Array::Array(const iterator first, const iterator last) throw(bad_alloc, invalid_argument)
{
    First = 0;
    if (first <= last) {
        Count = Size = (last - first) + 1;
        elems = new value_type[Size];
        for (UINT i = 0; i < Size; ++i)
            elems[i] = 0; // Ініціалізація нулями
    }
    else
        throw invalid_argument("!!!"); // Некоректний діапазон
}

// Конструктор за початковим та кінцевим індексом
Array::Array(const size_type first, const size_type last) throw(bad_alloc, invalid_argument)
{
    if (first <= last) {
        First = first;
        Count = Size = (last - first) + 1;
        elems = new value_type[Size];
        for (UINT i = 0; i < Size; ++i)
            elems[i] = 0;
    }
    else
        throw invalid_argument("!!!"); // Некоректний інтервал
}

// Копіюючий конструктор
Array::Array(const Array& t) throw(bad_alloc)
    : Size(t.Size), Count(t.Count), First(t.First), elems(new value_type[Size])
{
    for (UINT i = 0; i < Size; ++i)
        elems[i] = t.elems[i]; // Копіювання значень
}

// Оператор присвоювання
Array& Array::operator =(const Array& t)
{
    Array tmp(t); // Створення копії
    swap(tmp); // Обмін з поточним об'єктом
    return *this;
}

// Деструктор
Array::~Array()
{
    delete[]elems; // Звільнення пам'яті
    elems = 0;
}

// Видалення останнього елемента
void Array::pop_back()
{
    if (Count > 0)
        --Count;
}

// Заповнення масиву значенням v
void Array::assign(const value_type& v)
{
    for (size_type i = 0; i < Count; ++i)
        elems[i] = v;
}

// Додавання елемента в кінець
void Array::push_back(const value_type& v)
{
    if (Count == Size)
        resize(Size * 2); // Збільшуємо розмір
    elems[Count++] = v;
}

// Доступ з перевіркою меж
Array::reference Array::operator [](size_type index) throw(out_of_range)
{
    if ((First <= index) && (index < First + Size))
        return elems[index - First];
    else
        throw out_of_range("Index out of range!");
}

// Константний доступ з перевіркою меж
Array::const_reference Array::operator [](size_type index) const throw(out_of_range)
{
    if ((First <= index) && (index < First + Size))
        return elems[index - First];
    else
        throw out_of_range("Index out of range!");
}

// Зміна розміру масиву
void Array::resize(size_type newsize) throw(bad_alloc)
{
    if (newsize > capacity())
    {
        value_type* data = new value_type[newsize];
        for (size_type i = 0; i < Count; ++i)
            data[i] = elems[i];
        delete[] elems;
        elems = data;
        Size = newsize;
    }
}

// Обмін з іншим масивом
void Array::swap(Array& other)
{
    std::swap(elems, other.elems);
    std::swap(Size, other.Size);
}

// Поточна місткість
Array::size_type Array::capacity() const
{
    return Size;
}

// Поточний розмір (кількість елементів)
Array::size_type Array::size() const
{
    return Count;
}

// Перевірка чи масив порожній
bool Array::empty() const
{
    return Count == 0;
}

// Вивід в консоль
ostream& operator <<(ostream& out, const Array& tmp)
{
    for (size_t j = 0; j < tmp.Count; j++)
        out << tmp[j] << " ";
    out << endl;
    return out;
}

// Зчитування з потоку
istream& operator>>(std::istream& in, Array& tmp) {
    for (size_t j = 0; j < tmp.Count; j++) {
        in >> tmp[j];
    }
    return in;
}