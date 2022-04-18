#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void update(vector<int>&, int); // vector에 key를 삽입 혹은 삭제하는 Interface 함수
void insert(vector<int>&, int); // vector에 key를 삽입하는 함수
int find(vector<int>&, int);    // vector에서 key를 찾아서 위치를 반환하는 함수
void remove(vector<int>&, int); // vector에서 key를 삭제하는 함수
int rank(vector<int>&, int);    // vector에서 해당 order의 key를 찾아주는 함수

int main() {
    vector<int> myVector = { 4, 7, 2, 5, 10, 8, 1, 6, 3 };
    cout << "Before Sort" << endl;
    for (int i = 0; i < myVector.size(); i++) {
        cout << myVector[i] << ' ';
    }
    cout << endl;
    sort(myVector.begin(), myVector.end());
    cout << "After Sort" << endl;
    for (int i = 0; i < myVector.size(); i++) {
        cout << myVector[i] << ' ';
    }
    cout << endl;

    update(myVector, 8);

    cout << "After Delete" << endl;
    for (int i = 0; i < myVector.size(); i++) {
        cout << myVector[i] << ' ';
    }
    cout << endl;
    
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

int rank(vector<int> &myVector, int order) {    // vector에서 해당 order의 key를 찾아주는 함수
    if (order > myVector.size()) {
        return -1;
    } else {
        return myVector[order - 1];
    }
}