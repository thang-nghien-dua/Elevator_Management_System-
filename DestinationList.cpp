#include "Elevator_Management_System.h"
#include <iostream>

using namespace std;

// INSERT SORTED: Chèn Passenger vào danh sách theo thứ tự tầng

void DestinationList::insertSorted(Passenger p)
{
    Node* newNode = new Node(p);

    // Nếu danh sách rỗng
    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    // Nếu chèn vào đầu (tầng nhỏ hơn head)
    if (p.toFloor < head->data.toFloor)
    {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Tìm vị trí phù hợp
    Node* current = head;
    while (current->next != nullptr &&
        current->next->data.toFloor <= p.toFloor)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}


// REMOVE BY FLOOR: Xóa tất cả hành khách có tầng đích = floor

void DestinationList::removeByFloor(int floor)
{
    // Xóa ở đầu danh sách
    while (head != nullptr && head->data.toFloor == floor)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    // Xóa các node phía sau
    Node* current = head;
    while (current != nullptr && current->next != nullptr)
    {
        if (current->next->data.toFloor == floor)
        {
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
        else
        {
            current = current->next;
        }
    }
}


// CONTAINS: Kiểm tra có khách nào xuống tại tầng này không

bool DestinationList::contains(int floor)
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.toFloor == floor)
            return true;

        current = current->next;
    }

    return false;
}

void displayDestinationList(Node* head)
{
    if (head == nullptr)
    {
        cout << "Danh sach tra khach rong!\n";
        return;
    }

    cout << "Danh sach diem tra (floor):\n";

    Node* current = head;
    while (current != nullptr)
    {
        cout << "ID: " << current->data.id
            << " | To Floor: " << current->data.toFloor
            << " | Weight: " << current->data.weight << endl;

        current = current->next;
    }
}