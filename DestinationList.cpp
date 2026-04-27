#include "Elevator_Management_System.h"

// ==========================================
// INSERT SORTED: Chèn Passenger vào danh sách theo thứ tự tầng
// ==========================================
void DestinationList::insertSorted(Passenger p)
{
    Node* newNode = new Node(p);

    // Nếu danh sách rỗng
    if (head == nullptr)
    {
        head = newNode;
        passengerCount++;
        return;
    }

    // Nếu chèn vào đầu (tầng nhỏ hơn head)
    if (p.toFloor < head->data.toFloor)
    {
        newNode->next = head;
        head = newNode;
        passengerCount++;
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
    passengerCount++;
}

// ==========================================
// REMOVE BY FLOOR: Xóa tất cả hành khách có tầng đích = floor
// Trả về tổng trọng lượng đã trả
// ==========================================
float DestinationList::removeByFloor(int floor)
{
    float removedWeight = 0;

    // Xóa ở đầu danh sách
    while (head != nullptr && head->data.toFloor == floor)
    {
        Node* temp = head;
        removedWeight += temp->data.weight;
        head = head->next;
        delete temp;
        passengerCount--;
    }

    // Xóa các node phía sau
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

// ==========================================
// CONTAINS: Kiểm tra có khách nào xuống tại tầng này không
// ==========================================
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

// ==========================================
// GET NEXT STOP: Lấy tầng tiếp theo cần dừng
// ==========================================
int DestinationList::getNextStop()
{
    if (head == nullptr)
        return -1;

    return head->data.toFloor;
}

// ==========================================
// DISPLAY: Hiển thị danh sách tầng cần trả khách
// ==========================================
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

// ==========================================
// TOTAL WEIGHT: Tính tổng trọng lượng trong thang
// ==========================================
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