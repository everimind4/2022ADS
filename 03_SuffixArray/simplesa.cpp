#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

#define N 10000000              // 문자열 T의 길이 최대값

void buildsa();                 // 접미사 배열을 생성하는 함수
bool compare(int, int);         // 접미사를 비교하는 함수

int sa[N];                      // 접미사 배열을 저장
int n;                          // 문자열 T의 길이를 저장
string t;                       // 문자열 T를 저장

int main() {
    ifstream fin("indata.txt"); // 데이터 입력 파일 open
    if (!fin.is_open()) {       // File이 존재하지 않을 경우
        cout << "There is no 'indata.txt' file . . ." << endl;
        return -1;              // Usage 출력 후 실행 종료
    }

    chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
    chrono::microseconds duration;  // 시간 차이를 계산하기 위한 변수

    fin >> n;                       // 문자열 T의 길이를 입력
    getline(fin, t);                // 질의 횟수 k를 입력에서 제외
    fin >> t;                       // 문자열 T를 입력
    n = t.size();

    start = chrono::system_clock::now();    // 시간 측정 시작
    buildsa();
    finish = chrono::system_clock::now();   // 시간 측정 끝
    duration = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << "접미사 배열 생성 소요 시간 : " << duration.count() << " μs" << endl;
    return 0;
}

void buildsa() {                            // 접미사 배열 생성
    for (int i = 0; i < n; i++)             // 문자열의 길이만큼 반복
        sa[i] = i;                          // 접미사 배열 초기화
    sort(sa, sa+n, compare);                // 접미사 배열 정렬
}

bool compare(int i, int j) {                // 접미사를 비교하는 함수
    if (t.substr(i) < t.substr(j))          // 접미사를 사전순으로 비교
        return true;                        // 더 작은 경우 앞으로 이동
    else                                    // 더 큰 경우
        return false;                       // 뒤로 이동
}
