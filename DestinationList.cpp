#include "Elevator_Management_System.h"

DestinationList::~DestinationList()
{
    clear();
}

void DestinationList::clear()
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    passengerCount = 0;
}

void DestinationList::insertSorted(Passenger p)
{
    Node* newNode = new Node(p);

    if (head == nullptr)
    {
        head = newNode;
        passengerCount++;
        return;
    }

    if (p.toFloor < head->data.toFloor)
    {
        newNode->next = head;
        head = newNode;
        passengerCount++;
        return;
    }

    Node* current = head;
    while (current->next != nullptr &&
        current->next->data.toFloor <= p.toFloor)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    passengerCount++;
}

float DestinationList::removeByFloor(int floor)
{
    float removedWeight = 0;

    while (head != nullptr && head->data.toFloor == floor)
    {
        Node* temp = head;
        removedWeight += temp->data.weight;
        head = head->next;
        delete temp;
        passengerCount--;
    }

    Node* current = head;
    while (current != nullptr && current->next != nullptr)
    {
        if (current->next->data.toFloor == floor)
        {
            Node* temp = current->next;
            removedWeight += temp->data.weight;
            current->next = temp->next;
            delete temp;
            passengerCount--;
        }
        else
        {
            current = current->next;
        }
    }

    return removedWeight;
}

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

int DestinationList::getNextStop()
{
    if (head == nullptr)
        return -1;

    return head->data.toFloor;
}

void DestinationList::display()
{
    if (head == nullptr)
    {
        cout << "Danh sach tra khach rong!\n";
        return;
    }

    cout << "\n--- DANH SACH KHACH TRONG THANG MAY ---\n";
    cout << "So khach: " << passengerCount << "\n";

    Node* current = head;
    int stt = 1;
    while (current != nullptr)
    {
        cout << stt << ". ID: " << current->data.id
             << " | Xuong tang: " << current->data.toFloor
             << " | Nang: " << current->data.weight << " kg"
             << endl;

        current = current->next;
        stt++;
    }
    cout << "---------------------------------------\n";
}

float DestinationList::totalWeight()
{
    float total = 0;
    Node* current = head;
    while (current != nullptr)
    {
        total += current->data.weight;
        current = current->next;
    }
    return total;
}