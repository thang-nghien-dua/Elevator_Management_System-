#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Định nghĩa các trạng thái hướng đi cho chuyên nghiệp
enum Direction {
    UP = 1,
    DOWN = -1,
    IDLE = 0
};

// ==========================================
// 1. Thực thể Hành khách
// ==========================================
struct Passenger {
    int id;
    int fromFloor;
    int toFloor;
    float weight;

    Passenger(int _id, int _from, int _to, float _w)
        : id(_id), fromFloor(_from), toFloor(_to), weight(_w) {
    }
};

// ==========================================
// 2. Nút trong danh sách liên kết
// ==========================================
struct Node {
    Passenger data;
    Node* next;

    Node(Passenger p) : data(p), next(nullptr) {}
};

// ==========================================
// 3. Hàng đợi ưu tiên (Quản lý khách đang đợi ngoài)
// ==========================================
class PriorityQueue {
private:
    Node* front;
    int count;

public:
    PriorityQueue() : front(nullptr), count(0) {}

    // Thêm khách vào hàng đợi ưu tiên
    void push(Passenger p);

    // Lấy khách hàng phù hợp nhất ra
    Passenger pop();

    // Cập nhật lại độ ưu tiên dựa trên vị trí thang máy (Priority Scheduling)
    void updatePriority(int currentFloor);

    bool isEmpty() { return front == nullptr; }
    int size() { return count; }
};

// ==========================================
// 4. Danh sách điểm trả (Quản lý khách trong thang)
// ==========================================
class DestinationList {
private:
    Node* head;

public:
    DestinationList() : head(nullptr) {}

    // Chèn tầng đến vào danh sách (đã sắp xếp theo chiều di chuyển)
    void insertSorted(Passenger p);

    // Xóa khách khi đến tầng trả
    void removeByFloor(int floor);

    // Kiểm tra tầng hiện tại có khách muốn xuống không
    bool contains(int floor);

    bool isEmpty() { return head == nullptr; }
};

// ==========================================
// 5. Bộ điều khiển Thang máy (Lớp chính)
// ==========================================
class Elevator {
private:
    int currentFloor;
    int direction;      // Sử dụng enum Direction
    float currentWeight;
    float maxWeight;
    bool isOverloaded;  // Trạng thái quá tải

    PriorityQueue waitList;    // Danh sách chờ bên ngoài
    DestinationList destList;  // Danh sách trả bên trong

    // Hàm nội bộ xử lý logic
    void determineDirection();   // Thuật toán chọn hướng (LOOK)
    void checkOverloadSafety();  // Kiểm tra an toàn trọng tải

public:
    Elevator(float _maxWeight)
        : currentFloor(1), direction(IDLE), currentWeight(0),
        maxWeight(_maxWeight), isOverloaded(false) {
    }

    // Các hành động chính
    void move();     // Di chuyển thang máy qua từng tầng
    void pickUp();   // Đón khách từ waitList
    void dropOff();  // Trả khách vào destList

    bool isFull();   // Kiểm tra có bị quá tải không
    void displayStatus(); // In trạng thái ra màn hình (GUI/Console)
};

#endif