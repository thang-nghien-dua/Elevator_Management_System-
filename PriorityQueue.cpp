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
// PEEK: Xem khách ưu tiên nhất (không xóa)
// ==========================================
Passenger PriorityQueue::peek()
{
    if (front == nullptr)
    {
        return Passenger(0, 0, 0, 0);
    }
    return front->data;
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

// ==========================================
// DISPLAY: Hiển thị danh sách khách đang chờ
// ==========================================
void PriorityQueue::display()
{
    if (front == nullptr)
    {
        cout << "Hang doi trong! Khong co khach nao dang cho.\n";
        return;
    }

    cout << "\n--- DANH SACH KHACH DANG CHO THANG MAY ---\n";
    cout << "So luong: " << count << " khach\n";

    Node* temp = front;
    int stt = 1;
    while (temp != nullptr)
    {
        cout << stt << ". ID: " << temp->data.id
             << " | Tang goi: " << temp->data.fromFloor
             << " | Tang den: " << temp->data.toFloor
             << " | Nang: " << temp->data.weight << " kg"
             << endl;
        temp = temp->next;
        stt++;
    }
    cout << "-------------------------------------------\n";
}

// ==========================================
// HAS PASSENGER AT: Kiểm tra có khách chờ ở tầng này không
// ==========================================
bool PriorityQueue::hasPassengerAt(int floor)
{
    Node* temp = front;
    while (temp != nullptr)
    {
        if (temp->data.fromFloor == floor)
            return true;
        temp = temp->next;
    }
    return false;
}

// ==========================================
// POP ALL AT: Lấy tất cả khách chờ tại tầng (xóa khỏi queue)
// ==========================================
Node* PriorityQueue::popAllAt(int floor)
{
    Node* result = nullptr;
    Node* resultTail = nullptr;

    // Xóa từ đầu
    while (front != nullptr && front->data.fromFloor == floor)
    {
        Node* temp = front;
        front = front->next;
        temp->next = nullptr;
        count--;

        if (result == nullptr)
        {
            result = temp;
            resultTail = temp;
        }
        else
        {
            resultTail->next = temp;
            resultTail = temp;
        }
    }

    // Xóa từ giữa/cuối
    Node* curr = front;
    while (curr != nullptr && curr->next != nullptr)
    {
        if (curr->next->data.fromFloor == floor)
        {
            Node* temp = curr->next;
            curr->next = temp->next;
            temp->next = nullptr;
            count--;

            if (result == nullptr)
            {
                result = temp;
                resultTail = temp;
            }
            else
            {
                resultTail->next = temp;
                resultTail = temp;
            }
        }
        else
        {
            curr = curr->next;
        }
    }

    return result;
}