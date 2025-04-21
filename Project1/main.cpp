#include <iostream>   
#include <cstdlib>    
#include <ctime>      
#include <Windows.h>  

using namespace std;  

// ��������� ����� ���������������� ������
struct Node {
    int data;      // ��� ����� (���� �����)
    Node* next;    // �������� �� ��������� �����
    Node(int val) : data(val), next(nullptr) {}  // ����������� ��� �����������
};

// ������� ��� ��������� ������ �� �����
void printList(Node* head) {
    cout << "������: ";
    // ��������� �� ��� ������ ������
    while (head != nullptr) {
        cout << head->data << " ";  // �������� ��� ��������� �����
        head = head->next;          // ���������� �� ���������� �����
    }
    cout << endl;  
}

// ������� ��� ��������� ������ � ����������� �������
Node* createList(int size) {
    if (size <= 0) return nullptr;  

    
    Node* head = new Node(rand() % 100 - 50);
    Node* current = head;  // �������� ��� ����������� �� ������

    // ������ ����� �����
    for (int i = 1; i < size; ++i) {
        current->next = new Node(rand() % 100 - 50);  // ��������� ����� �����
        current = current->next;  // ���������� �������� �� ����� �����
    }
    return head;  
}

// ������� ��� ��������� ��������� ������������� ������
Node* createSortedList() {
    
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(2);
    head->next->next->next = new Node(5);
    return head;
}

// ������� ��� ��������, �� ������ ������������� �� �����������
bool isNonDecreasing(Node* head) {
    // ������ � 0 ��� 1 �������� ������ �������������
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    Node* current = head;  // �������� ��� ����������� �� ������
    // ���������� �� ����� ����
    while (current->next != nullptr) {
        // ���� �������� ������� ������ �� ��������� - ������ �� �������������
        if (current->data > current->next->data) {
            return false;
        }
        current = current->next;  // ���������� �� ���������� �����
    }
    return true;  
}

// ������� ��� ��������� ������ �� ��������� ���'��
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;  
        head = head->next;  // ���������� �� ����������
        delete temp;        
    }
}

int main() {
    SetConsoleOutputCP(1251);  
    SetConsoleCP(1251);       
    srand(static_cast<unsigned>(time(nullptr)));  // ����������� ���������� ���������� �����

    // ���� 1: ������ � ����������� �������
    int n;
    cout << "������ ����� ������ n = ";
    cin >> n;  

    Node* randomList = createList(n);  // ��������� ���������� ������
    cout << "��������� ���������� ";
    printList(randomList);  // �������� ����

    // ���������� �� ������������� 
    if (isNonDecreasing(randomList)) {
        cout << "���������: ������������� �� �����������.\n\n";
    }
    else {
        cout << "���������: �� ������������� �� �����������.\n\n";
    }

    // ���� 2:  (�������� �������)
    Node* sortedList = createSortedList();  
    cout << "�������� ������������ ";
    printList(sortedList); 

   
    if (isNonDecreasing(sortedList)) {
        cout << "���������: ������������� �� �����������.\n\n";
    }
    else {
        cout << "���������: �� ������������� �� �����������.\n\n";
    }

    // �������� ���'�� (��������� ���� ������)
    deleteList(randomList);
    deleteList(sortedList);

    return 0;  
}