#include <iostream>
#include <fstream>
#include <string>
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

ifstream fin("bhin.txt");       // 입력 파일 Open
ofstream fout("bhout.txt");     // 출력 파일 Open

int main() {
    if (!fin.is_open()) {           // 열리지 않았을 경우
        cout << "There is no 'bhin.txt' file . . ." << endl;
        return -1;                  // 예외처리
    }

    string line;                    // 한 줄씩 저장할 string 변수
    while (fin.peek() != EOF) {     // 파일 전체를 읽어들이는 동안
        getline(fin, line);         // 한 줄씩 읽어서 line에 저장
        switch (line[0]) {
            case '+':
                insert(stoi(line.substr(line.substr(2).find(' ')+3)),
                            line.substr(2, line.substr(2).find(' ')));
                break;
            case 'p':
                pop();
                break;
            default:
                break;
        }
    }
}

void insert(int key, string name) {
    node *temp = new node(key, name);
    if (bheap == nullptr || bheap->deg != 0) {
        temp->next = bheap;
        bheap = temp;
    }
    else {
        merge(bheap, temp);
    }
}

void pop() {
    if (bheap == nullptr)
        return;
    int key_min = top(bheap);
    fout << key_min << ' ';
    priority_queue<string, vector<string>, greater<string>> res;
    while (key_min == top(bheap)) {
        node *temp = bheap, *min = bheap, *prev = nullptr, *prev_min = nullptr;
        while (temp != nullptr) {
            if (min->key > temp->key) {
                min = temp;
                prev_min = prev;
            }
            prev = temp;
            temp = temp->next;
        }
        if (prev_min != nullptr)
            prev_min->next = min->next;
        else
            bheap = min->next;
        temp = min->down;
        res.push(min->name);
        delete min;
        while (temp != nullptr) {
            prev = temp;
            temp = temp->next;
            if (bheap == nullptr || bheap->deg != 0) {
                prev->next = bheap;
                bheap = prev;
            } else {
                merge(bheap, prev);
            }
        }
    }
    while (!res.empty()) {
        fout << res.top() << ' ';
        res.pop();
    }
    fout << endl;
}

int top(node* bheap) {
    if (bheap == nullptr)
        return 0;
    int min = 100000;
    node *temp;
    for (temp = bheap; temp != nullptr; temp = temp->next)
        if (min > temp->key)
            min = temp->key;
    return min;
}

void merge(node* a, node* b) {
    node *prev = nullptr;
    while (a != nullptr && a->deg < b->deg) {
        prev = a;
        a = a->next;
    }
    if (a == nullptr) {
        prev->next = b;
        b->next = nullptr;
    } else if (a->deg != b->deg) {
        if (prev != nullptr)
            prev->next = b;
        b->next = a;
    } else {
        b->next = a->next;
        while (b != nullptr && a->deg == b->deg) {
            if (a->key > b->key) {
                if (prev != nullptr)
                    prev->next = b;
                else
                    bheap = b;
                b->deg++;
                a->next = b->down;
                b->down = a;
                a = b;
                b = b->next;
            } else {
                a->deg++;
                a->next = b->next;
                b->next = a->down;
                a->down = b;
                b = a->next;
            }
        }
    }
}
