#include <iostream>
#include <random>
#include <chrono>

using namespace std;

mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count()); // 현재 시간으로 난수 생성 시드 초기화
uniform_int_distribution<int> dis(0, 9999); // int형 난수 생성 범위 0~9999로 설정

typedef int level_t; // 새로 추가할 Node의 Level을 위한 int type define

level_t set_level(); // 새로 추가할 노드의 Level을 정하는 함수

struct skipnode
{
    skipnode *next; // 다음 Node를 가리킬 Pointer
    skipnode *down; // 아래 Node를 가리킬 Pointer
    int key;        // Sort 대상이 되는 Key
    // level_t level;
    
    skipnode(int insert_key = -1) {
        key = insert_key;
        // level = 0;
        next = nullptr;
        down = nullptr;
    }
};

class Skiplist {
    skipnode *head;
    int level_max;
    int total;
    skipnode **latest;

    void insert(int);
    void remove(int);

    public:
    Skiplist() {
        head = new skipnode;
        level_max = 0;
        total = 0;
        latest = nullptr;
    }
    ~Skiplist() {
        skipnode *next = head->next;
        skipnode *down = head->down;
        for (int i = 0; i < level_max; i++) {
            while (head != nullptr) {
                head->next = nullptr;
                head->down = nullptr;
                delete head;
                head = next;
                if (next != nullptr) {
                    next = next->next;
                }
            }
            head = down;
            if (down != nullptr) {
                down = down->down;
            }
        }
        if (latest != nullptr) {
            delete[] latest;
        }
    }

    bool search(int);
    void update(int);
    int query(int);
};

int main() {
    Skiplist mySkip;

    return 0;
}

level_t set_level() { // 난수를 생성하여 새로운 노드의 층수를 결정하는 함수
    level_t level = 0;
    while (dis(gen) % 2) {
        level++;
    }
    return level;
}

void Skiplist::insert(int key) {
    level_t level = set_level();
    if (level > level_max) {
        skipnode *newHead;
        for (int i = 0; i < level - level_max; i++) {
            newHead = new skipnode;
            newHead->down = head;
            head = newHead;
        }
        delete[] latest;
        level_max = level;
        latest = new skipnode*[level_max + 1];
        search(key);
    }
    skipnode *newNode;
    skipnode *newNodeabove = nullptr;
    for (int i = level; i >= 0; i--) {
        newNode = new skipnode(key);
        if (newNodeabove != nullptr) {
            newNodeabove->down = newNode;
        }
        // newNode->level = level;
        newNode->next = latest[level_max - i]->next;
        latest[level_max - i]->next = newNode;
        newNodeabove = newNode;
    }
    total++;
}

void Skiplist::remove(int key) {
    skipnode *next;
    for (int i = 0; i < level_max + 1; i++) {
        if (latest[i]->next != nullptr && key == latest[i]->next->key) {
            next = latest[i]->next->next;
            latest[i]->next->down = nullptr;
            delete latest[i]->next;
            latest[i]->next = next;
        }
    }
    while (head->next == nullptr) {
        next = head->down;
        head->down = nullptr;
        delete head;
        head = next;
        level_max--;
    }
    total--;
}

bool Skiplist::search(int key) {
    bool result = false;
    skipnode *next = head;
    skipnode *down = head->down;
    for (int i = level_max; i >= 0; i--) {
        while (next->next != nullptr && key >= next->next->key) {
            if (key == next->next->key) {
                if (!result) {
                    result = !result;
                }
                break;
            }
            next = next->next;
        }
        
        if (latest != nullptr) {
            latest[level_max - i] = next;
        }
        next = down;
        if (down != nullptr) {
            down = down->down;
        }
    }
    return result;
}

void Skiplist::update(int key) {
    latest = new skipnode*[level_max + 1];
    if (!search(key)) {
        insert(key);
    } else {
        remove(key);
    }
    delete[] latest;
    latest = nullptr;
}

int Skiplist::query(int order) {
    if (order > total) {
        return -1;
    }
    int count = 0;
    skipnode *findnode = head;
    while (findnode->down != nullptr) {
        findnode = findnode->down;
    }
    while (count++ != order) {
        findnode = findnode->next;
    }
    return findnode->key;
}
