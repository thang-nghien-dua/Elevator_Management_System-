#include "Elevator_Management_System.h"

// ==========================================
// DETERMINE DIRECTION: Thuật toán LOOK chọn hướng
// - Ưu tiên tiếp tục hướng hiện tại nếu còn khách
// - Đổi hướng nếu không còn khách theo hướng hiện tại
// - IDLE nếu không còn ai
// ==========================================
void Elevator::determineDirection()
{
    bool hasAbove = false;
    bool hasBelow = false;

    // Kiểm tra destList (khách đang trong thang)
    if (!destList.isEmpty())
    {
        // Duyệt destList xem có tầng nào trên/dưới không
        // Dùng getNextStop() để xác định — danh sách đã sắp xếp tăng dần
        int nextStop = destList.getNextStop();
        if (nextStop > currentFloor) hasAbove = true;
        if (nextStop < currentFloor) hasBelow = true;
        if (nextStop == currentFloor)
        {
            // Tầng hiện tại có khách xuống, giữ nguyên hướng
            return;
        }
    }

    // Kiểm tra waitList (khách đang chờ bên ngoài)
    // Cập nhật priority theo vị trí hiện tại
    waitList.updatePriority(currentFloor);
    if (!waitList.isEmpty())
    {
        Passenger next = waitList.peek();
        if (next.fromFloor > currentFloor) hasAbove = true;
        if (next.fromFloor < currentFloor) hasBelow = true;
        if (next.fromFloor == currentFloor)
        {
            // Có khách ngay tầng này
            return;
        }
    }

    // Thuật toán LOOK: ưu tiên tiếp tục hướng hiện tại
    if (direction == UP)
    {
        if (hasAbove)
            direction = UP;
        else if (hasBelow)
            direction = DOWN;
        else
            direction = IDLE;
    }
    else if (direction == DOWN)
    {
        if (hasBelow)
            direction = DOWN;
        else if (hasAbove)
            direction = UP;
        else
            direction = IDLE;
    }
    else // IDLE
    {
        if (hasAbove && hasBelow)
        {
            // Chọn hướng có khách gần hơn
            direction = UP; // mặc định đi lên trước
        }
        else if (hasAbove)
            direction = UP;
        else if (hasBelow)
            direction = DOWN;
        else
            direction = IDLE;
    }
}

// ==========================================
// CHECK OVERLOAD SAFETY: Kiểm tra an toàn trọng tải
// ==========================================
void Elevator::checkOverloadSafety()
{
    currentWeight = destList.totalWeight();

    if (currentWeight > maxWeight)
    {
        isOverloaded = true;
        cout << "[CANH BAO] Thang may QUA TAI! ("
             << currentWeight << "/" << maxWeight << " kg)\n";
        cout << "[CANH BAO] Thang may se khong don them khach!\n";
    }
    else
    {
        isOverloaded = false;
    }
}

// ==========================================
// MOVE: Di chuyển thang máy 1 tầng theo hướng hiện tại
// ==========================================
void Elevator::move()
{
    // Xác định hướng trước khi di chuyển
    determineDirection();

    if (direction == IDLE)
    {
        cout << "[Thang may] Dang dung yen tai tang " << currentFloor << ".\n";
        return;
    }

    // Di chuyển 1 tầng
    currentFloor += direction;

    // Giới hạn tầng (1 đến 20)
    if (currentFloor < 1) currentFloor = 1;
    if (currentFloor > 20) currentFloor = 20;

    string dirStr = (direction == UP) ? "LEN" : "XUONG";
    cout << "[Thang may] Di chuyen " << dirStr
         << " -> Tang " << currentFloor << "\n";
}

// ==========================================
// PICKUP: Đón khách tại tầng hiện tại từ waitList
// ==========================================
void Elevator::pickUp()
{
    if (waitList.isEmpty()) return;
    if (!waitList.hasPassengerAt(currentFloor)) return;

    // Kiểm tra quá tải trước
    checkOverloadSafety();

    // Lấy tất cả khách chờ tại tầng hiện tại
    Node* passengers = waitList.popAllAt(currentFloor);

    Node* curr = passengers;
    while (curr != nullptr)
    {
        Node* next = curr->next;

        // Kiểm tra có thể đón thêm không (trọng tải + số người)
        float newWeight = currentWeight + curr->data.weight;
        if (newWeight <= maxWeight && destList.getPassengerCount() < maxPassengers)
        {
            cout << "[Don khach] ID " << curr->data.id
                 << " tai tang " << currentFloor
                 << " -> muon den tang " << curr->data.toFloor
                 << " (" << curr->data.weight << " kg)\n";

            // Thêm vào destList (danh sách trả khách)
            destList.insertSorted(curr->data);
            currentWeight = destList.totalWeight();
            delete curr;
        }
        else
        {
            // Quá tải — đưa khách lại vào waitList
            cout << "[Tu choi] Khong the don khach ID " << curr->data.id
                 << " (Qua tai hoac day thang)\n";
            curr->next = nullptr;
            waitList.push(curr->data);
            delete curr;
        }

        curr = next;
    }

    checkOverloadSafety();
}

// ==========================================
// DROPOFF: Trả khách tại tầng hiện tại
// ==========================================
void Elevator::dropOff()
{
    if (destList.isEmpty()) return;
    if (!destList.contains(currentFloor)) return;

    float removedWeight = destList.removeByFloor(currentFloor);
    currentWeight -= removedWeight;
    if (currentWeight < 0) currentWeight = 0;

    cout << "[Tra khach] Da tra khach tai tang " << currentFloor
         << " (giam " << removedWeight << " kg)\n";

    // Cập nhật trạng thái quá tải
    checkOverloadSafety();
}

// ==========================================
// IS FULL: Kiểm tra thang máy có đầy/quá tải không
// ==========================================
bool Elevator::isFull()
{
    return (currentWeight >= maxWeight) ||
           (destList.getPassengerCount() >= maxPassengers);
}

// ==========================================
// DISPLAY STATUS: Hiển thị trạng thái thang máy
// ==========================================
void Elevator::displayStatus()
{
    cout << "\n========================================\n";
    cout << "       TRANG THAI THANG MAY\n";
    cout << "========================================\n";
    cout << "Tang hien tai : " << currentFloor << "\n";

    string dirStr;
    if (direction == UP) dirStr = "DI LEN";
    else if (direction == DOWN) dirStr = "DI XUONG";
    else dirStr = "DUNG YEN";
    cout << "Huong di      : " << dirStr << "\n";

    cout << "Trong luong   : " << currentWeight << "/" << maxWeight << " kg\n";
    cout << "So khach      : " << destList.getPassengerCount() << "/" << maxPassengers << "\n";
    cout << "Trang thai    : " << (isOverloaded ? "QUA TAI!" : "Binh thuong") << "\n";
    cout << "Khach cho     : " << waitList.size() << " nguoi\n";
    cout << "========================================\n";

    // Hiển thị chi tiết
    destList.display();
    waitList.display();
}

// ==========================================
// ADD REQUEST: Thêm yêu cầu gọi thang máy
// ==========================================
void Elevator::addRequest(Passenger p)
{
    waitList.push(p);
    cout << "[Yeu cau] Khach ID " << p.id
         << " goi thang tu tang " << p.fromFloor
         << " den tang " << p.toFloor
         << " (" << p.weight << " kg)\n";
}

// ==========================================
// LOAD REQUESTS: Đọc yêu cầu từ file
// ==========================================
void Elevator::loadRequests(const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Loi: Khong the mo file " << filename << endl;
        return;
    }

    int id, fromFloor, toFloor;
    float weight;
    int loaded = 0;

    while (inFile >> id >> fromFloor >> toFloor >> weight)
    {
        waitList.push(Passenger(id, fromFloor, toFloor, weight));
        loaded++;
    }
    inFile.close();

    cout << "[System] Da nap " << loaded << " yeu cau tu file: " << filename << "\n";
}

// ==========================================
// RUN ONE STEP: Chạy 1 bước (trả khách -> đón khách -> di chuyển)
// ==========================================
void Elevator::runOneStep()
{
    cout << "\n--- Buoc xu ly tai tang " << currentFloor << " ---\n";

    // Bước 1: Trả khách trước (nếu có)
    dropOff();

    // Bước 2: Đón khách (nếu có)
    pickUp();

    // Bước 3: Di chuyển đến tầng tiếp theo
    move();
}

// ==========================================
// RUN AUTOMATIC: Chạy tự động đến khi hết tất cả khách
// ==========================================
void Elevator::runAutomatic()
{
    cout << "\n============================================\n";
    cout << "  BAT DAU CHAY TU DONG - THUAT TOAN LOOK\n";
    cout << "============================================\n";

    int step = 0;
    int maxSteps = 200; // giới hạn vòng lặp tránh loop vô hạn

    while ((!waitList.isEmpty() || !destList.isEmpty()) && step < maxSteps)
    {
        step++;
        cout << "\n>>>>> BUOC " << step << " <<<<<\n";

        // Trả khách tại tầng hiện tại
        dropOff();

        // Đón khách tại tầng hiện tại
        pickUp();

        // Di chuyển
        move();

        // Hiển thị trạng thái ngắn gọn
        cout << "[Tang: " << currentFloor
             << " | Trong thang: " << destList.getPassengerCount()
             << " | Cho: " << waitList.size()
             << " | Weight: " << currentWeight << "/" << maxWeight << " kg]\n";
    }

    cout << "\n============================================\n";
    if (step >= maxSteps)
        cout << "  DA DAT GIOI HAN BUOC (" << maxSteps << ")\n";
    else
        cout << "  HOAN THANH! Da xu ly het " << step << " buoc.\n";
    cout << "============================================\n";

    displayStatus();
}
