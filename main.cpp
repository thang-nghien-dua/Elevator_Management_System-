#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "Elevator_Management_System.h"

using namespace std;
using namespace std::chrono;

void showMenu() {
    cout << "\n===========================================" << endl;
    cout << "   HE THONG DIEU KHIEN THANG MAY (LOOK)    " << endl;
    cout << "===========================================" << endl;
    cout << "1. Nhap khach thu cong (Manual)" << endl;
    cout << "2. Nap du lieu tu file (Load passengers.txt)" << endl;
    cout << "3. Chay demo (Run Automatic)" << endl;
    cout << "4. Danh gia hieu nang (Benchmark)" << endl;
    cout << "5. Hien thi danh sach (Cho & Trong thang)" << endl;
    cout << "6. Chay 1 buoc (Step-by-Step)" << endl;
    cout << "7. Tim kiem khach theo ID" << endl;
    cout << "8. Xem lich su phuc vu" << endl;
    cout << "0. Thoat" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Lua chon: ";
}

int main() {
    Elevator elevator(800.0, 10);
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, from, to; float weight;
                cout << "Nhap ID, Tang bat dau, Tang den, Trong luong: ";
                cin >> id >> from >> to >> weight;
                elevator.addRequest(Passenger(id, from, to, weight));
                break;
            }
            case 2: {
                elevator.loadRequests("passengers.txt");
                break;
            }
            case 3: {
                cout << "[System] Dang chay kich ban demo..." << endl;
                elevator.runAutomatic();
                break;
            }
            case 4: {
                string files[] = {"test_small.txt", "test_medium.txt", "test_large.txt"};
                struct Result { string name; double time; };
                Result results[3];

                cout << "\nBAT DAU BENCHMARK..." << endl;
                for(int i = 0; i < 3; i++) {
                    elevator.reset();
                    elevator.loadRequests(files[i]);
                    auto start = high_resolution_clock::now();
                    elevator.runAutomatic();
                    auto end = high_resolution_clock::now();

                    results[i] = {files[i], duration<double>(end - start).count()};
                }

                cout << "\n" << setw(20) << "BO DU LIEU" << setw(30) << "THOI GIAN XU LY (s)" << endl;
                cout << string(50, '-') << endl;
                for(int i = 0; i < 3; i++)
                    cout << setw(20) << results[i].name << setw(30) << fixed << setprecision(6) << results[i].time << endl;

                elevator.reset();
                elevator.loadRequests("passengers.txt");
                break;
            }
            case 5:
                cout << "\n--- DANH SACH CHO ---" << endl;
                elevator.getWaitList().display();
                cout << "\n--- DANH SACH TRA KHACH ---" << endl;
                elevator.getDestList().display();
                break;
            case 6:
                elevator.runOneStep();
                cout << "Da thuc hien 1 buoc di chuyen." << endl;
                elevator.displayStatus();
                break;
            case 7: {
                int searchId;
                cout << "Nhap ID khach can tim: ";
                cin >> searchId;
                elevator.searchPassenger(searchId);
                break;
            }
            case 8:
                cout << "\n--- LICH SU PHUC VU KHACH ---" << endl;
                elevator.getServiceHistory().displayList();
                break;
            case 0: cout << "Ket thuc chuong trinh." << endl; break;
            default: cout << "Lua chon khong hop le!" << endl;
        }
    } while (choice != 0);

    return 0;
}
