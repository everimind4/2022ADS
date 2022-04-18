#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

void update(vector<int>&, int); // vector에 key를 삽입 혹은 삭제하는 Interface 함수
void insert(vector<int>&, int); // vector에 key를 삽입하는 함수
int find(vector<int>&, int);    // vector에서 key를 찾아서 위치를 반환하는 함수
void remove(vector<int>&, int); // vector에서 key를 삭제하는 함수
int query(vector<int>&, int);    // vector에서 해당 order의 key를 찾아주는 함수

int main(int argc, char* argv[]) {  // main의 argument로 file을 입력
    if (argc != 2) {                // File은 하나만 입력
        cout << "./a.out (filename)" << endl;   // 올바른 형식 출력
    }
    ifstream fin(argv[1]);          // Filename으로 file open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "Could not open the file '" << argv[1] << "' . . ." << endl;
    }                               // Error Message 출력
    string line;                    // File 내용을 한 줄씩 저장
    vector<int> myVector;
    int operand;                    // Operation을 수행할 operand
    chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
    chrono::microseconds duration;   // 시간 차이를 계산하기 위한 변수
    start = chrono::system_clock::now();
    while (fin.peek() != '$') {     // '$'가 나타나기 전까지 File 읽기
        getline(fin, line);         // File을 한 줄씩 읽어서 line에 저장
        operand = stoi(line.substr(2)); // 3번째 문자부터 int로 변환해 key에 저장
        switch (line[0]) {          // 맨 앞 문자를 Token으로 인식
            case '+':               // Token '+'인 경우 Update 수행
            update(myVector, operand);
            break;
            case '?':               // Token '?'인 경우 Query 수행
            cout << query(myVector, operand) << endl;
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

void update(vector<int> &myVector, int key) {   // vector에 key를 삽입 혹은 삭제하는 Interface 함수
    int order = find(myVector, key);
    if (order == -1) {                          // 해당 key의 Element를 찾아보고
        insert(myVector, key);                  // 없으면 추가
    } else {                                    // 있으면
        remove(myVector, order);                // 삭제
    }
}

void insert(vector<int> &myVector, int key) {   // vector에 key를 삽입하는 함수
    myVector.push_back(key);
    sort(myVector.begin(), myVector.end());
}

int find(vector<int> &myVector, int key) {      // Binary Search 구현
    int left, right, locate;
    left = 0;
    right = myVector.size() - 1;

    while (left < right) {
        locate = left + (right - left) / 2;
        if (key == myVector[locate]) {
            return locate;
        } else if (key > myVector[locate]) {
            left = locate + 1;
        } else {
            right = locate;
        }
    }
    return -1;
}

void remove(vector<int> &myVector, int order) { // vector에서 해당 order를 삭제하는 함수
    myVector.erase(myVector.begin() + order);
}

int query(vector<int> &myVector, int order) {    // vector에서 해당 order의 key를 찾아주는 함수
    if (order > myVector.size()) {
        return -1;
    } else {
        return myVector[order - 1];
    }
}