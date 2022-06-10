#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct node {
    int key;
    int deg;
    string name;
    node *down, *next;

    node(int key, string name) :
    key(key), deg(0), name(name),
    down(nullptr), next(nullptr) {};
};

void insert(int, string);
void pop();
int top(node*);
void merge(node*, node*);

node *bheap = nullptr;

int main() {
    ifstream fin("bhin.txt");       // 입력 파일 Open
    if (!fin.is_open()) {           // 열리지 않았을 경우
        cout << "There is no 'bhin.txt' file . . ." << endl;
        return -1;                  // 예외처리
    }
    ofstream fout("bhout.txt");     // 출력 파일 Open

    string line;                    // 한 줄씩 저장할 string 변수
    while (fin.peek() != EOF) {     // 파일 전체를 읽어들이는 동안
        getline(fin, line);         // 한 줄씩 읽어서 line에 저장
        cout << line << endl;
    }
}

void insert(int key, string name) {
    node *temp = new node(key, name);
    if (bheap == nullptr)
        bheap = temp;
    else
        merge(bheap, temp);
}

void pop() {
    node *temp, *down, *prev = nullptr;
    int min = top(bheap);
    priority_queue<string, vector<string>, greater<string>> res;
    while (top(bheap) == min) {
        for (temp = bheap; temp != nullptr; temp = temp->next) {
            if (min == temp->key) {
                res.push(temp->name);
                if (prev != nullptr)
                    prev->next = temp->next;
                down = temp->down;
                delete(temp);
                merge(bheap, down);
            } else {
                prev = temp;
                temp = temp->next;
            }
        }
    }
}

int top(node* bheap) {
    if (bheap == nullptr)
        return;
    node *temp;
    int min = 100000;
    for (temp = bheap; temp != nullptr; temp = temp->next)
        if (min > temp->key)
            min = temp->key;
    return min;
}

void merge(node* a, node* b) {

}
