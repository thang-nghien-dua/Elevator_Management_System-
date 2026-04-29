#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

enum Direction {
    UP = 1,
    DOWN = -1,
    IDLE = 0
};

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

struct Node {
    Passenger data;
    Node* next;

    Node(Passenger p) : data(p), next(nullptr) {}
};

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

class PriorityQueue {
private:
    Node* front;
    int count;

public:
    PriorityQueue() : front(nullptr), count(0) {}
    ~PriorityQueue();

    void push(Passenger p);
    Passenger pop();
    void updatePriority(int currentFloor);
    void display();
    Passenger peek();
    bool hasPassengerAt(int floor);
    Node* popAllAt(int floor);
    bool isEmpty() { return front == nullptr; }
    int size() { return count; }
    void clear();
    Node* getFront() { return front; }
};

class DestinationList {
private:
    Node* head;
    int passengerCount;

public:
    DestinationList() : head(nullptr), passengerCount(0) {}
    ~DestinationList();

    void insertSorted(Passenger p);
    float removeByFloor(int floor);
    bool contains(int floor);
    int getNextStop();
    void display();
    float totalWeight();
    int getPassengerCount() { return passengerCount; }
    bool isEmpty() { return head == nullptr; }
    void clear();
    Node* getHead() { return head; }
};

class Elevator {
private:
    int currentFloor;
    int direction;
    float currentWeight;
    float maxWeight;
    int maxPassengers;
    bool isOverloaded;

    PriorityQueue waitList;
    DestinationList destList;
    LinkedList serviceHistory;

    void determineDirection();
    void checkOverloadSafety();

public:
    Elevator(float _maxWeight = 800.0, int _maxPassengers = 10)
        : currentFloor(1), direction(IDLE), currentWeight(0),
        maxWeight(_maxWeight), maxPassengers(_maxPassengers), isOverloaded(false) {
    }

    void move();
    void pickUp();
    void dropOff();
    bool isFull();
    void displayStatus();
    void addRequest(Passenger p);
    void loadRequests(const string& filename);
    void runOneStep();
    void runAutomatic();
    void reset();
    void searchPassenger(int id);

    int getCurrentFloor() { return currentFloor; }
    int getDirection() { return direction; }
    float getCurrentWeight() { return currentWeight; }
    bool getIsOverloaded() { return isOverloaded; }
    PriorityQueue& getWaitList() { return waitList; }
    DestinationList& getDestList() { return destList; }
    LinkedList& getServiceHistory() { return serviceHistory; }
};

#endif