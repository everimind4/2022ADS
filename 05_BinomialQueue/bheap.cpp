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

void merge(node*, node*);
void insert(int);
void pop();

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

void merge(node* a, node* b) {

}

void insert(int key) {

}

void pop() {

}
