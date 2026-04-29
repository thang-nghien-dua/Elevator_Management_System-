#include "Elevator_Management_System.h"

void Elevator::determineDirection()
{
    bool hasAbove = false;
    bool hasBelow = false;

    Node* curr = destList.getHead();
    while (curr != nullptr)
    {
        if (curr->data.toFloor > currentFloor) hasAbove = true;
        if (curr->data.toFloor < currentFloor) hasBelow = true;
        curr = curr->next;
    }

    curr = waitList.getFront();
    while (curr != nullptr)
    {
        if (curr->data.fromFloor > currentFloor) hasAbove = true;
        if (curr->data.fromFloor < currentFloor) hasBelow = true;
        curr = curr->next;
    }

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
    else
    {
        if (hasAbove && hasBelow)
            direction = UP;
        else if (hasAbove)
            direction = UP;
        else if (hasBelow)
            direction = DOWN;
        else
            direction = IDLE;
    }
}

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

void Elevator::move()
{
    determineDirection();

    if (direction == IDLE)
    {
        cout << "[Thang may] Dang dung yen tai tang " << currentFloor << ".\n";
        return;
    }

    currentFloor += direction;

    if (currentFloor < 1) currentFloor = 1;
    if (currentFloor > 20) currentFloor = 20;

    string dirStr = (direction == UP) ? "LEN" : "XUONG";
    cout << "[Thang may] Di chuyen " << dirStr
         << " -> Tang " << currentFloor << "\n";
}

void Elevator::pickUp()
{
    if (waitList.isEmpty()) return;
    if (!waitList.hasPassengerAt(currentFloor)) return;

    checkOverloadSafety();

    Node* passengers = waitList.popAllAt(currentFloor);

    Node* curr = passengers;
    while (curr != nullptr)
    {
        Node* next = curr->next;

        float newWeight = currentWeight + curr->data.weight;
        if (newWeight <= maxWeight && destList.getPassengerCount() < maxPassengers)
        {
            cout << "[Don khach] ID " << curr->data.id
                 << " tai tang " << currentFloor
                 << " -> muon den tang " << curr->data.toFloor
                 << " (" << curr->data.weight << " kg)\n";

            destList.insertSorted(curr->data);
            currentWeight = destList.totalWeight();
            delete curr;
        }
        else
        {
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

void Elevator::dropOff()
{
    if (destList.isEmpty()) return;
    if (!destList.contains(currentFloor)) return;

    Node* curr = destList.getHead();
    while (curr != nullptr)
    {
        if (curr->data.toFloor == currentFloor)
        {
            serviceHistory.insertNode(curr->data);
            cout << "[Tra khach] ID " << curr->data.id
                 << " da xuong tai tang " << currentFloor
                 << " (" << curr->data.weight << " kg)\n";
        }
        curr = curr->next;
    }

    destList.removeByFloor(currentFloor);
    currentWeight = destList.totalWeight();
    checkOverloadSafety();
}

bool Elevator::isFull()
{
    return (currentWeight >= maxWeight) ||
           (destList.getPassengerCount() >= maxPassengers);
}

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

    destList.display();
    waitList.display();
}

void Elevator::addRequest(Passenger p)
{
    waitList.push(p);
    cout << "[Yeu cau] Khach ID " << p.id
         << " goi thang tu tang " << p.fromFloor
         << " den tang " << p.toFloor
         << " (" << p.weight << " kg)\n";
}

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

void Elevator::runOneStep()
{
    cout << "\n--- Buoc xu ly tai tang " << currentFloor << " ---\n";
    dropOff();
    pickUp();
    move();
}

void Elevator::runAutomatic()
{
    cout << "\n============================================\n";
    cout << "  BAT DAU CHAY TU DONG - THUAT TOAN LOOK\n";
    cout << "============================================\n";

    int step = 0;
    int maxSteps = 200;

    while ((!waitList.isEmpty() || !destList.isEmpty()) && step < maxSteps)
    {
        step++;
        cout << "\n>>>>> BUOC " << step << " <<<<<\n";

        dropOff();
        pickUp();
        move();

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

void Elevator::reset()
{
    waitList.clear();
    destList.clear();
    currentFloor = 1;
    direction = IDLE;
    currentWeight = 0;
    isOverloaded = false;
}

void Elevator::searchPassenger(int id)
{
    cout << "\n--- TIM KIEM KHACH HANG ID: " << id << " ---\n";
    bool found = false;

    Node* curr = waitList.getFront();
    while (curr != nullptr)
    {
        if (curr->data.id == id)
        {
            cout << "[Tim thay] Dang CHO tai tang " << curr->data.fromFloor
                 << " -> muon den tang " << curr->data.toFloor
                 << " (" << curr->data.weight << " kg)\n";
            found = true;
        }
        curr = curr->next;
    }

    curr = destList.getHead();
    while (curr != nullptr)
    {
        if (curr->data.id == id)
        {
            cout << "[Tim thay] Dang TRONG THANG, se xuong tang " << curr->data.toFloor
                 << " (" << curr->data.weight << " kg)\n";
            found = true;
        }
        curr = curr->next;
    }

    curr = serviceHistory.getHead();
    while (curr != nullptr)
    {
        if (curr->data.id == id)
        {
            cout << "[Tim thay] Da PHUC VU: tu tang " << curr->data.fromFloor
                 << " den tang " << curr->data.toFloor
                 << " (" << curr->data.weight << " kg)\n";
            found = true;
        }
        curr = curr->next;
    }

    if (!found)
    {
        cout << "[Khong tim thay] Khong co khach hang voi ID " << id << "\n";
    }
}
