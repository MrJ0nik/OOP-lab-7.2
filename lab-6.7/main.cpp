#include "Array.h" 
#include <iostream> 
#include <Windows.h> 
#include <algorithm> 
using namespace std; 

typedef Array::value_type* TArray; // Тип для вказівника на елементи Array

// Базовий клас-предикат (інтерфейс)
template <typename T>
class Predicate {
public:
    virtual bool operator()(const T& value) const = 0; // Абстрактний оператор()
    virtual ~Predicate() {} // Віртуальний деструктор
};

// Предикат для від'ємних чисел
template <typename T>
class NegativeValuePredicate : public Predicate<T> {
public:
    bool operator()(const T& value) const override {
        return value < 0; // Повертає true, якщо число від'ємне
    }
};

// Предикат для перевірки "більше ніж"
template <typename T>
class GreaterThanPredicate : public Predicate<T> {
private:
    T threshold; // Поріг
public:
    GreaterThanPredicate(T threshold) : threshold(threshold) {} // Ініціалізатор
    bool operator()(const T& value) const override {
        return value > threshold; // Повертає true, якщо більше порогу
    }
};

// Функція симетричного віднімання
// Виводить елементи, що є лише в одному з двох масивів (множин)
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_xor(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result++ = *first1++; // Додаємо з першого, якщо менше
        }
        else if (*first2 < *first1) {
            *result++ = *first2++; // Додаємо з другого
        }
        else {
            ++first1; ++first2; // Пропускаємо спільні
        }
    }
    return std::copy(first2, last2, std::copy(first1, last1, result)); // Додаємо залишки
}

// Симетричне віднімання з предикатом
// Виводить лише ті елементи, що задовольняють умові
template <typename InputIterator1, typename InputIterator2,
    typename OutputIterator, typename Predicate>
OutputIterator set_xor_if(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    OutputIterator result, Predicate pred) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            if (pred(*first1)) *result++ = *first1;
            ++first1;
        }
        else if (*first2 < *first1) {
            if (pred(*first2)) *result++ = *first2;
            ++first2;
        }
        else {
            ++first1; ++first2;
        }
    }
    while (first1 != last1) {
        if (pred(*first1)) *result++ = *first1;
        ++first1;
    }
    while (first2 != last2) {
        if (pred(*first2)) *result++ = *first2;
        ++first2;
    }
    return result;
}

// Дублює кожен елемент послідовності
template <typename InputIt, typename OutputIt>
OutputIt duplicate(InputIt first, InputIt last, OutputIt result) {
    while (first != last) {
        *result++ = *first;
        *result++ = *first;
        ++first;
    }
    return result;
}

// Дублює лише ті елементи, що задовольняють предикат
// Інші — лише копіюються один раз
template <typename InputIt, typename OutputIt, typename Predicate>
OutputIt duplicate_if(InputIt first, InputIt last, OutputIt result, Predicate pred) {
    while (first != last) {
        *result++ = *first;
        if (pred(*first)) {
            *result++ = *first;
        }
        ++first;
    }
    return result;
}

// Виводить елементи масиву
void print_array(const Array& arr) {
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    SetConsoleCP(1251); // Установка коду вводу
    SetConsoleOutputCP(1251); // Установка коду виводу

    srand((unsigned)time(NULL)); // Ініціалізація rand()

    int n;
    cout << "Введіть розмір масивів: ";
    cin >> n; // Ввід розміру масивів

    TArray a = new Array::value_type[n]; // Динамічний масив a
    TArray b = new Array::value_type[n]; // Динамічний масив b

    for (int i = 0; i < n; i++) {
        a[i] = -50 + rand() % 101; // Значення [-50, 50]
        b[i] = -50 + rand() % 101;
    }

    sort(a, a + n); // Сортування
    sort(b, b + n);

    cout << "Перший масив:" << endl;
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;

    cout << "Другий масив:" << endl;
    for (int i = 0; i < n; i++) cout << b[i] << " ";
    cout << endl;

    Array arrayA(n), arrayB(n); // Масиви-контейнери
    for (int i = 0; i < n; i++) {
        arrayA[i] = a[i];
        arrayB[i] = b[i];
    }

    Array resultXor(2 * n); // Результат симетричного віднімання
    auto end = set_xor(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXor.begin());
    resultXor.resize(end - resultXor.begin()); // Обрізання зайвого

    cout << "Результат set_xor (симетричне віднімання множин):" << endl;
    print_array(resultXor);

    Array resultXorIfNeg(2 * n); // Результат з від'ємними
    NegativeValuePredicate<Array::value_type> negPred;
    end = set_xor_if(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXorIfNeg.begin(), negPred);
    resultXorIfNeg.resize(end - resultXorIfNeg.begin());

    cout << "Результат set_xor_if (тільки від'ємні числа):" << endl;
    print_array(resultXorIfNeg);

    Array resultXorIfGt10(2 * n); // Результат > 10
    GreaterThanPredicate<Array::value_type> gtPred(10);
    end = set_xor_if(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXorIfGt10.begin(), gtPred);
    resultXorIfGt10.resize(end - resultXorIfGt10.begin());

    cout << "Результат set_xor_if (числа > 10):" << endl;
    print_array(resultXorIfGt10);

    Array duplicated(2 * n); // Дубльований масив
    end = duplicate(arrayA.begin(), arrayA.end(), duplicated.begin());
    duplicated.resize(end - duplicated.begin());
    cout << "Дубльований перший масив (duplicate):" << endl;
    print_array(duplicated);

    Array duplicatedNeg(3 * n); // Дубльовані лише від'ємні
    end = duplicate_if(arrayA.begin(), arrayA.end(), duplicatedNeg.begin(), negPred);
    duplicatedNeg.resize(end - duplicatedNeg.begin());
    cout << "Дубльовані лише від'ємні (duplicate_if):" << endl;
    print_array(duplicatedNeg);

    delete[] a; // Звільнення пам'яті
    delete[] b;

    cout << "\nНатисніть Enter для завершення програми...";
    cin.get(); // Очікування Enter
    cin.get();

    return 0;
}
