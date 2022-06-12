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
        switch (line[0]) {          // 첫 문자를 읽어서 Operation 구분자로 사용
            case '+':               // '+'인 경우
                insert(stoi(line.substr(line.substr(2).find(' ')+3)),
                            line.substr(2, line.substr(2).find(' ')));
                break;              // key와 name을 Binomial Queue에 입력
            case 'p':               // 'p'인 경우
                pop();              // Pop operation 호출
                break;
            default:
                break;
        }
    }
}

void insert(int key, string name) {     // Insert Operation
    node *temp = new node(key, name);   // 새로운 Node 생성
    if (bheap == nullptr                // BQ가 비어 있거나
            || bheap->deg != 0) {       // 바로 삽입 가능한 경우
        temp->next = bheap;             // 새로 생성한 Node를
        bheap = temp;                   // 맨 앞에 추가
    } else {                            // 그렇지 않은 경우
        merge(bheap, temp);             // Merge Operation 호출
    }
}

void pop() {                            // Pop Operation
    if (bheap == nullptr)               // BQ가 비어 있으면
        return;                         // 아무 동작도 하지 않음
    int key_min = top(bheap);           // Pop이 요청된 Key의 Min을 기록
    fout << key_min << ' ';             // 파일에 출력
    priority_queue<string, vector<string>,
        greater<string>> res;           // Pop 대상의 Name을 기록할 PQ
    while (key_min == top(bheap)) {     // Key가 Min인 모든 Node를 찾아서
        node *temp = bheap, *min = bheap, *prev = nullptr,
             *prev_min = nullptr;       // Pop 처리를 위한 Node Pointers
        while (temp != nullptr) {       // Min Node 탐색
            if (min->key > temp->key) { // 현재까지 찾은 Min과 대소비교
                min = temp;             // 더 작은 값을 Min으로 변경
                prev_min = prev;        // 현재 Min의 앞 Node를 저장
            }
            prev = temp;                // 이전 Node 기록
            temp = temp->next;          // 다음 Node 탐색
        }
        if (prev_min != nullptr)        // 이전 Node가 있는 경우
            prev_min->next = min->next; // Min을 List에서 제거
        else                            // Min이 맨 앞인 경우
            bheap = min->next;          // Min의 Next를 Head로 변경
        temp = min->down;               // Min의 Child를 Merge하기 위해 저장
        res.push(min->name);            // Min의 Name을 저장
        delete min;                     // Min 할당 해제
        while (temp != nullptr) {       // Min의 Child를 모두 순회
            prev = temp;                // 이전 Node 기록
            temp = temp->next;          // 다음 Node 탐색
            if (bheap == nullptr ||     // BQ가 비어 있거나
                    bheap->deg != 0) {  // 맨 앞의 degree가 0이 아니면
                prev->next = bheap;     // 바로 추가
                bheap = prev;           // 추가된 Node를 Head로 변경
            } else {                    // 그렇지 않으면
                merge(bheap, prev);     // Merge Operation 호출
            }
        }
    }
    while (!res.empty()) {              // Pop 대상 Node 전체에 대해
        fout << res.top() << ' ';       // Name을 파일에 출력
        res.pop();                      // 기록한 Node 삭제
    }
    fout << endl;                       // Pop 대상 출력 완료
}

int top(node* bheap) {                  // 현재 Min의 Key를 출력
    if (bheap == nullptr)               // BQ가 비어 있으면
        return 0;                       // 바로 실행 종료
    int min = 100000;                   // Min을 Key 최대값으로 설정
    node *temp;                         // Node Pointer
    for (temp = bheap; temp != nullptr; temp = temp->next)  // 전체를 순회
        if (min > temp->key)            // 대소비교를 수행해
            min = temp->key;            // 더 작은 값을 Min으로 변경
    return min;                         // Min을 Return
}

void merge(node* a, node* b) {          // Merge Operation (a <- b)
    node *prev = nullptr;               // 이전 Node를 기록할 Pointer
    while (a != nullptr && a->deg < b->deg) {   // Merge 위치를 탐색
        prev = a;                               // 이전 위치를 저장
        a = a->next;                            // 다음 위치 탐색
    }
    if (a == nullptr) {                 // a의 마지막에 추가하는 경우
        prev->next = b;                 // b를 a의 마지막에 추가
        b->next = nullptr;              // b의 Next를 Nullify
    } else if (a->deg != b->deg) {      // a의 중간에 추가하는 경우
        if (prev != nullptr)            // a가 맨 앞이 아니면
            prev->next = b;             // b를 a의 앞에 삽입하고
        b->next = a;                    // b의 next를 a로 설정
    } else {                            // degree가 같은 Node가 있는 경우
        b->next = a->next;              // b를 a 다음에 우선 연결한 후
        while (b != nullptr && a->deg == b->deg) {  // b를 a에 Merge
            if (a->key > b->key) {      // a가 child, b가 root
                if (prev != nullptr)    // a가 맨 앞이 아니면
                    prev->next = b;     // b를 a의 앞에 삽입하고
                else                    // a가 맨 앞인 경우
                    bheap = b;          // b를 Head로 변경
                b->deg++;               // b의 degree를 1 증가
                a->next = b->down;      // b의 down을 a의 next로 설정
                b->down = a;            // a를 b의 down으로 설정
                a = b;                  // b를 a에 연결
                b = b->next;            // 다음 merge를 위한 Pointer 변경
            } else {                    // a가 root, b가 child
                a->deg++;               // a의 degree를 1 증가
                a->next = b->next;      // b의 next를 a의 next로 설정
                b->next = a->down;      // a의 down을 b의 next로 설정
                a->down = b;            // b를 a의 down으로 설정
                b = a->next;            // 다음 merge를 위한 Pointer 변경
            }
        }
    }
}
