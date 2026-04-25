#include "Elevator_Management_System.h"
#include <cmath> // dùng abs()

// ==========================================
// PUSH: Thêm khách vào queue (chưa sắp xếp)
// ==========================================
void PriorityQueue::push(Passenger p)
{
    Node* newNode = new Node(p);

    // Nếu queue rỗng
    if (front == nullptr)
    {
        front = newNode;
    }
    else
    {
        // thêm vào cuối (FIFO trước, sau sẽ sort)
        Node* temp = front;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    count++;
}

// ==========================================
// POP: Lấy khách ưu tiên nhất (đầu danh sách)
// ==========================================
Passenger PriorityQueue::pop()
{
    if (front == nullptr)
    {
        cout << "Queue rong!\n";
        return Passenger(0, 0, 0, 0);
    }

    Node* temp = front;
    Passenger p = temp->data;

    front = front->next;
    delete temp;

    count--;
    return p;
}

// ==========================================
// UPDATE PRIORITY (QUAN TRỌNG)
// Sắp xếp lại theo khoảng cách gần nhất
// ==========================================
void PriorityQueue::updatePriority(int currentFloor)
{
    if (front == nullptr || front->next == nullptr)
        return;

    // Dùng bubble sort trên linked list (dễ hiểu)
    bool swapped;
    do
    {
        swapped = false;
        Node* curr = front;

        while (curr->next != nullptr)
        {
            int dist1 = abs(curr->data.fromFloor - currentFloor);
            int dist2 = abs(curr->next->data.fromFloor - currentFloor);

            // nếu node sau gần hơn → đổi chỗ
            if (dist1 > dist2)
            {
                Passenger temp = curr->data;
                curr->data = curr->next->data;
                curr->next->data = temp;

                swapped = true;
            }

            curr = curr->next;
        }

    } while (swapped);
}