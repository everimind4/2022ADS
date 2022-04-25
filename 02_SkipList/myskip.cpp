#include <iostream>
#include <fstream>
#include <string>
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
    
    skipnode(int insert_key = -1) { // Node 생성자
        key = insert_key;           // 입력된 Key로 Node를 초기화
        // level = 0;
        next = nullptr;
        down = nullptr;
    }
};

class Skiplist {
    skipnode *head;     // Tower의 최상위 Level로, 탐색이 시작되는 위치
    int level_max;      // 현재 최대 Level을 기록
    int total;          // SkipList에 저장된 Element의 총 개수를 저장
    skipnode **latest;  // Level별로 가장 최근의 Search에서 탐색한 Node들을 저장하는 List

    void insert(int);   // Token '+'가 입력되었을 때 해당 Key의 Node가 없는 경우 호출되어 새로운 Node를 추가
    void remove(int);   // Token '-'가 입력되었을 때 해당 Key의 Node가 없는 경우 호출되어 기존의 Node를 삭제

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

    bool search(int);   // 입력된 Key로 search를 수행하는 함수
    void update(int);   // Token '+'가 입력되면 호출되는 Interface 함수
    int query(int);     // Token '?'가 입력되면 호출되는 Interface 함수
};

int main(int argc, char* argv[]) {  // main의 argument로 file을 입력
    if (argc != 2) {                // File은 하나만 입력
        cout << "./a.out (filename)" << endl;   // 올바른 형식 출력
        return -1;
    }
    ifstream fin(argv[1]);          // Filename으로 file open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "Could not open the file '" << argv[1] << "' . . ." << endl;
        return -2;
    }                               // Error Message 출력
    string line;                    // File 내용을 한 줄씩 저장
    Skiplist mySkip;                // Skip List를 저장할 Class 객체
    int key;                        // Update할 Key를 저장
    chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
    chrono::microseconds duration;   // 시간 차이를 계산하기 위한 변수
    start = chrono::system_clock::now();
    while (fin.peek() != '$') {     // '$'가 나타나기 전까지 File 읽기
        getline(fin, line);         // File을 한 줄씩 읽어서 line에 저장
        key = stoi(line.substr(2)); // 3번째 문자부터 int로 변환해 key에 저장
        switch (line[0]) {          // 맨 앞 문자를 Token으로 인식
            case '+':               // Token '+'인 경우
            mySkip.update(key);     // Update 수행
            break;
            case '?':               // Token '?'인 경우
            cout << mySkip.query(key) << endl;  // Query 수행
            break;
            default:                // 잘못된 Token을 읽은 경우
            cout << "Invalid token '" << line[0] << "'" << endl;
            break;
        }
    }
    finish = chrono::system_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << "실행에 걸린 시간 : " << duration.count() << " μs" << endl;
    return 0;
}

level_t set_level() {       // 난수를 생성하여 새로운 노드의 층수를 결정하는 함수
    level_t level = 0;      // Level을 저장할 변수
    while (dis(gen) % 2) {  // 난수를 2로 Modulation하여 1이 나온 경우
        level++;            // Level을 1 증가
    }                       // 0이 나온 경우
    return level;           // 지금까지의 결과를 Return
}

void Skiplist::insert(int key) {                        // 새로운 Node를 추가하는 Operation
    level_t level = set_level();                        // 새로 생성할 Node의 Level 지정
    if (level > level_max) {                            // 새로 생성한 Node의 Level이 현재 level_max보다 큰 경우
        skipnode *newHead;                              // 새로운 Head를 가리킬 Pointer
        for (int i = 0; i < level - level_max; i++) {   // Head가 늘어나는 길이만큼 반복
            newHead = new skipnode;                     // 새로운 Node를 할당
            newHead->down = head;                       // down이 현재 head를 가리키도록 하고
            head = newHead;                             // head를 새로운 head로 교체
        }
        delete[] latest;                                // 가장 최근에 탐색한 위치를 Reset
        level_max = level;                              // 최대 Level을 Update한 후
        latest = new skipnode*[level_max + 1];          // 메모리를 할당하고
        search(key);                                    // 다시 탐색을 수행
    }
    skipnode *newNode;                                  // 새로 추가할 Node를 가리킬 포인터
    skipnode *newNodeabove = nullptr;                   // 새로 추가할 Node의 위쪽 Node를 가리킬 포인터
    for (int i = level; i >= 0; i--) {                  // 새로 추가할 Node의 Level만큼 반복
        newNode = new skipnode(key);                    // 새로 추가할 Node를 위한 메모리 할당
        if (newNodeabove != nullptr) {                  // 위쪽 Node가 비어 있지 않다면
            newNodeabove->down = newNode;               // 위쪽 Node의 down이 새로운 Node를 가리키도록 하고
        }
        // newNode->level = level;
        newNode->next = latest[level_max - i]->next;    // 마지막으로 탐색한 위치의 next를 현재 Node의 next에 저장
        latest[level_max - i]->next = newNode;          // 마지막으로 탐색한 위치의 next에 현재 Node 저장
        newNodeabove = newNode;                         // 방금 추가한 Node를 NewNodeabove에 저장
    }
    total++;                                            // 전체 Element 개수 1 증가
}

void Skiplist::remove(int key) {                        // 기존 Node를 삭제하는 Operation
    skipnode *next;                                     // 삭제 Operation을 위한 Node 포인터
    for (int i = 0; i < level_max + 1; i++) {           // 최대 Level 수만큼 반복
        if (latest[i]->next != nullptr && key == latest[i]->next->key) { // 삭제할 Node를 찾은 경우
            next = latest[i]->next->next;               // 삭제할 Node의 next 주소를 임시로 저장
            latest[i]->next->down = nullptr;            // 삭제할 Node의 down을 Nullifying
            delete latest[i]->next;                     // 삭제할 Node의 메모리 할당 해제
            latest[i]->next = next;                     // 삭제할 Node를 가리키는 Pointer가 그 다음 Node를 가리키도록 함
        }
    }
    while (head->next == nullptr) {                     // Delete 대상 Node가 유일한 최고층 Node였다면
        next = head->down;                              // next에 head의 down을 임시로 저장해 두고
        head->down = nullptr;                           // head의 Down을 Nullifying
        delete head;                                    // head를 Delete
        head = next;                                    // head를 한 층 낮추고
        level_max--;                                    // 최대 Level 1 감소
    }
    total--;                                            // 전체 Element 개수 1 감소
}

bool Skiplist::search(int key) {                        // 해당 Key를 찾으며 탐색한 마지막 Node 위치를 저장하는 함수
    bool result = false;                                // 탐색 결과를 나타내는 Boolean 변수
    skipnode *next = head;                              // 탐색에 사용되는 Pointer
    skipnode *down = head->down;                        // 탐색에 사용되는 Pointer
    for (int i = level_max; i >= 0; i--) {              // Level 수만큼 반복
        while (next->next != nullptr && key >= next->next->key) {   // 다음 Element가 존재하고 탐색하려는 Key보다 크거나 같으면
            if (key == next->next->key) {               // 같은 경우 탐색에 성공했으므로
                if (!result) {                          // 이전에 탐색한 적이 없는 경우
                    result = !result;                   // 탐색 여부를 변경
                }
                break;                                  // 현재 레벨 탐색 중단
            }
            next = next->next;                          // 찾지 못한 경우 다음 위치 탐색
        }
        if (latest != nullptr) {                        // latest가 nullptr가 아니면 (Search만 수행 시 Nullptr)
            latest[level_max - i] = next;               // 탐색한 위치를 저장
        }
        next = down;                                    // 다음 Level로 내려감
        if (down != nullptr) {                          // 현재 Level의 아래 Level이 Null이 아니면
            down = down->down;                          // 다음 Level 위치 저장
        }
    }
    return result;                                      // 탐색 결과 보고
}

void Skiplist::update(int key) {                        // Token '+'를 처리하는 Interface
    latest = new skipnode*[level_max + 1];              // insert 혹은 remove를 위해 마지막 탐색 위치를 저장
    if (!search(key)) {                                 // Search를 수행하며 각 Level의 마지막 탐색 위치 저장
        insert(key);                                    // 해당 Key로 Search 실패한 경우 Insert 수행
    } else {                                            // 해당 Key로 Search 성공한 경우
        remove(key);                                    // Remove 수행
    }
    delete[] latest;                                    // 마지막 탐색 위치 삭제
    latest = nullptr;                                   // Nullifying
}

int Skiplist::query(int order) {                        // Token '?'를 처리하는 Interface
    if (order > total) {                                // 현재 저장된 개수보다 많은 위치를 Query하면
        return -1;                                      // -1 Return
    }                                                   // 그렇지 않은 경우
    int count = 0;                                      // 탐색한 Element 개수를 저장할 변수
    skipnode *findnode = head;                          // 탐색할 위치를 가리킬 Pointer
    while (findnode->down != nullptr) {                 // 가장 아래층에 도달할 때까지
        findnode = findnode->down;                      // Head의 아래쪽으로 이동
    }
    while (count++ != order) {                          // 탐색할 order에 도달할 때까지
        findnode = findnode->next;                      // 다음 Node로 이동
    }
    return findnode->key;                               // 찾아낸 Key Return
}
