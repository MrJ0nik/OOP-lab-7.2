#include "Array.h" 
#include <iostream> 
#include <Windows.h> 
#include <algorithm> 
using namespace std; 

typedef Array::value_type* TArray; // ��� ��� ��������� �� �������� Array

// ������� ����-�������� (���������)
template <typename T>
class Predicate {
public:
    virtual bool operator()(const T& value) const = 0; // ����������� ��������()
    virtual ~Predicate() {} // ³��������� ����������
};

// �������� ��� ��'����� �����
template <typename T>
class NegativeValuePredicate : public Predicate<T> {
public:
    bool operator()(const T& value) const override {
        return value < 0; // ������� true, ���� ����� ��'����
    }
};

// �������� ��� �������� "����� ��"
template <typename T>
class GreaterThanPredicate : public Predicate<T> {
private:
    T threshold; // ����
public:
    GreaterThanPredicate(T threshold) : threshold(threshold) {} // �����������
    bool operator()(const T& value) const override {
        return value > threshold; // ������� true, ���� ����� ������
    }
};

// ������� ������������ ��������
// �������� ��������, �� � ���� � ������ � ���� ������ (������)
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_xor(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result++ = *first1++; // ������ � �������, ���� �����
        }
        else if (*first2 < *first1) {
            *result++ = *first2++; // ������ � �������
        }
        else {
            ++first1; ++first2; // ���������� �����
        }
    }
    return std::copy(first2, last2, std::copy(first1, last1, result)); // ������ �������
}

// ���������� �������� � ����������
// �������� ���� � ��������, �� ������������� ����
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

// ������ ����� ������� �����������
template <typename InputIt, typename OutputIt>
OutputIt duplicate(InputIt first, InputIt last, OutputIt result) {
    while (first != last) {
        *result++ = *first;
        *result++ = *first;
        ++first;
    }
    return result;
}

// ������ ���� � ��������, �� ������������� ��������
// ���� � ���� ��������� ���� ���
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

// �������� �������� ������
void print_array(const Array& arr) {
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    SetConsoleCP(1251); // ��������� ���� �����
    SetConsoleOutputCP(1251); // ��������� ���� ������

    srand((unsigned)time(NULL)); // ����������� rand()

    int n;
    cout << "������ ����� ������: ";
    cin >> n; // ��� ������ ������

    TArray a = new Array::value_type[n]; // ��������� ����� a
    TArray b = new Array::value_type[n]; // ��������� ����� b

    for (int i = 0; i < n; i++) {
        a[i] = -50 + rand() % 101; // �������� [-50, 50]
        b[i] = -50 + rand() % 101;
    }

    sort(a, a + n); // ����������
    sort(b, b + n);

    cout << "������ �����:" << endl;
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;

    cout << "������ �����:" << endl;
    for (int i = 0; i < n; i++) cout << b[i] << " ";
    cout << endl;

    Array arrayA(n), arrayB(n); // ������-����������
    for (int i = 0; i < n; i++) {
        arrayA[i] = a[i];
        arrayB[i] = b[i];
    }

    Array resultXor(2 * n); // ��������� ������������ ��������
    auto end = set_xor(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXor.begin());
    resultXor.resize(end - resultXor.begin()); // �������� �������

    cout << "��������� set_xor (���������� �������� ������):" << endl;
    print_array(resultXor);

    Array resultXorIfNeg(2 * n); // ��������� � ��'������
    NegativeValuePredicate<Array::value_type> negPred;
    end = set_xor_if(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXorIfNeg.begin(), negPred);
    resultXorIfNeg.resize(end - resultXorIfNeg.begin());

    cout << "��������� set_xor_if (����� ��'��� �����):" << endl;
    print_array(resultXorIfNeg);

    Array resultXorIfGt10(2 * n); // ��������� > 10
    GreaterThanPredicate<Array::value_type> gtPred(10);
    end = set_xor_if(arrayA.begin(), arrayA.end(), arrayB.begin(), arrayB.end(), resultXorIfGt10.begin(), gtPred);
    resultXorIfGt10.resize(end - resultXorIfGt10.begin());

    cout << "��������� set_xor_if (����� > 10):" << endl;
    print_array(resultXorIfGt10);

    Array duplicated(2 * n); // ����������� �����
    end = duplicate(arrayA.begin(), arrayA.end(), duplicated.begin());
    duplicated.resize(end - duplicated.begin());
    cout << "����������� ������ ����� (duplicate):" << endl;
    print_array(duplicated);

    Array duplicatedNeg(3 * n); // ��������� ���� ��'���
    end = duplicate_if(arrayA.begin(), arrayA.end(), duplicatedNeg.begin(), negPred);
    duplicatedNeg.resize(end - duplicatedNeg.begin());
    cout << "��������� ���� ��'��� (duplicate_if):" << endl;
    print_array(duplicatedNeg);

    delete[] a; // ��������� ���'��
    delete[] b;

    cout << "\n�������� Enter ��� ���������� ��������...";
    cin.get(); // ���������� Enter
    cin.get();

    return 0;
}
