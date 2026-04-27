#include "Elevator_Management_System.h"

// ==========================================
// LinkedList: Destructor
// ==========================================
LinkedList::~LinkedList()
{
    Node* current = head;
    while (current != nullptr)
    {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// ==========================================
// LinkedList: Thêm hành khách vào cuối danh sách
// ==========================================
void LinkedList::insertNode(Passenger p)
{
    Node* newNode = new Node(p);
    if (head == nullptr)
    {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// ==========================================
// LinkedList: Xóa hành khách theo ID
// ==========================================
bool LinkedList::removeNode(int id)
{
    if (head == nullptr)
        return false;

    // Nếu người cần xóa nằm ở đầu danh sách
    if (head->data.id == id)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    // Tìm người cần xóa ở giữa hoặc cuối danh sách
    Node* current = head;
    while (current->next != nullptr && current->next->data.id != id)
    {
        current = current->next;
    }

    if (current->next == nullptr)
        return false; // Không tìm thấy ID

    Node* temp = current->next;
    current->next = current->next->next;
    delete temp;
    return true;
}

// ==========================================
// LinkedList: Duyệt và in danh sách hành khách
// ==========================================
void LinkedList::displayList()
{
    if (head == nullptr)
    {
        cout << "Danh sach hanh khach trong!" << endl;
        return;
    }
    Node* temp = head;
    cout << "\n--- DANH SACH HANH KHACH ---" << endl;
    while (temp != nullptr)
    {
        cout << "ID: " << temp->data.id
             << "\t| Tu tang: " << temp->data.fromFloor
             << "\t| Den tang: " << temp->data.toFloor
             << "\t| Nang: " << temp->data.weight << " kg" << endl;
        temp = temp->next;
    }
    cout << ".......................\n"
         << endl;
}

// ==========================================
// LinkedList: Ghi dữ liệu hiện tại ra file .txt
// ==========================================
void LinkedList::saveToFile(const string& filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Loi: Khong the tao file " << filename << endl;
        return;
    }

    Node* temp = head;
    // Cấu trúc lưu: ID fromFloor toFloor weight
    while (temp != nullptr)
    {
        outFile << temp->data.id << " "
                << temp->data.fromFloor << " "
                << temp->data.toFloor << " "
                << temp->data.weight << "\n";
        temp = temp->next;
    }
    outFile.close();
    cout << "[System] Da ghi du lieu ra file: " << filename << endl;
}

// ==========================================
// LinkedList: Đọc dữ liệu từ file .txt và thêm vào danh sách
// ==========================================
void LinkedList::loadFromFile(const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Loi: Khong the mo file " << filename << " (File co the chua ton tai)." << endl;
        return;
    }

    int id, fromFloor, toFloor;
    float weight;
    while (inFile >> id >> fromFloor >> toFloor >> weight)
    {
        insertNode(Passenger(id, fromFloor, toFloor, weight));
    }
    inFile.close();
    cout << "[System] Da doc du lieu tu file: " << filename << endl;
}
