#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

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

    Passenger() : id(0), fromFloor(0), toFloor(0), weight(0) {}

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
// 3. Linked List cơ bản (Quản lý hành khách)
// ==========================================
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    void insertNode(Passenger p);
    bool removeNode(int id);
    void displayList();
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
    Node* getHead() { return head; }
};

// ==========================================
// 4. Hàng đợi ưu tiên (Quản lý khách đang đợi ngoài)
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

    // Hien thi danh sach khach dang cho
    void display();

    // Lấy khách phù hợp nhất theo hướng hiện tại (không xóa)
    Passenger peek();

    // Kiểm tra có khách chờ ở tầng này không
    bool hasPassengerAt(int floor);

    // Lấy tất cả khách chờ tại tầng (xóa khỏi queue)
    // Trả về linked list tạm
    Node* popAllAt(int floor);

    bool isEmpty() { return front == nullptr; }
    int size() { return count; }
};

// ==========================================
// 5. Danh sách điểm trả (Quản lý khách trong thang)
// ==========================================
class DestinationList {
private:
    Node* head;
    int passengerCount;

public:
    DestinationList() : head(nullptr), passengerCount(0) {}

    // Chèn tầng đến vào danh sách (đã sắp xếp theo chiều di chuyển)
    void insertSorted(Passenger p);

    // Xóa khách khi đến tầng trả — trả về tổng weight đã trả
    float removeByFloor(int floor);

    // Kiểm tra tầng hiện tại có khách muốn xuống không
    bool contains(int floor);

    // Lay tang tiep theo can dung
    int getNextStop();

    // Hien thi danh sach tang can tra khach
    void display();

    // Tính tổng trọng lượng trong thang
    float totalWeight();

    int getPassengerCount() { return passengerCount; }

    bool isEmpty() { return head == nullptr; }
};

// ==========================================
// 6. Bộ điều khiển Thang máy (Lớp chính)
// ==========================================
class Elevator {
private:
    int currentFloor;
    int direction;      // Sử dụng enum Direction
    float currentWeight;
    float maxWeight;
    int maxPassengers;
    bool isOverloaded;  // Trạng thái quá tải

    PriorityQueue waitList;    // Danh sách chờ bên ngoài
    DestinationList destList;  // Danh sách trả bên trong

    // Hàm nội bộ xử lý logic
    void determineDirection();   // Thuật toán chọn hướng (LOOK)
    void checkOverloadSafety();  // Kiểm tra an toàn trọng tải

public:
    Elevator(float _maxWeight = 800.0, int _maxPassengers = 10)
        : currentFloor(1), direction(IDLE), currentWeight(0),
        maxWeight(_maxWeight), maxPassengers(_maxPassengers), isOverloaded(false) {
    }

    // Các hành động chính
    void move();       // Di chuyển thang máy qua từng tầng
    void pickUp();     // Đón khách từ waitList
    void dropOff();    // Trả khách tại destList

    bool isFull();     // Kiểm tra có bị quá tải không
    void displayStatus(); // In trạng thái ra màn hình

    // Hàm tiện ích cho main
    void addRequest(Passenger p);             // Thêm yêu cầu vào waitList
    void loadRequests(const string& filename); // Đọc yêu cầu từ file
    void runOneStep();                         // Chạy 1 bước (move + pickup + dropoff)
    void runAutomatic();                       // Chạy tự động đến hết khách

    // Getters
    int getCurrentFloor() { return currentFloor; }
    int getDirection() { return direction; }
    float getCurrentWeight() { return currentWeight; }
    bool getIsOverloaded() { return isOverloaded; }
    PriorityQueue& getWaitList() { return waitList; }
    DestinationList& getDestList() { return destList; }
};

#endif