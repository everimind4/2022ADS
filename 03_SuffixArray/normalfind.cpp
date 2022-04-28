#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

#define N 10000000

int search(string); // 문자열 T에서 패턴을 찾는 함수

int n, k, s, m;     // 문자열 T의 길이, 질의 횟수, 패턴의 시작 위치, 길이
string t, p;    // 문자열 T와 패턴 문자열을 저장

chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
chrono::microseconds duration;  // 시간 차이를 계산하기 위한 변수

int main() {
    ifstream fin("indata.txt");     // 데이터 입력 파일 open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "There is no 'indata.txt' file . . ." << endl;
        return -1;                  // Usage 출력 후 실행 종료
    }
    ofstream fout("outdata.txt");   // 결과 출력 파일 open

    fin >> n >> k >> t;             // 파일에서 해당 변수들의 값을 Read
    n = t.size();

    int count;
    for (int i = 0; i < k; i++) {               // k번 반복하며
        fin >> s >> m;                          // 패턴의 시작 위치와 길이 입력
        p = t.substr(s, m);                     // 시작 위치와 길이로 패턴 추출
        start = chrono::system_clock::now();    // 시간 측정 시작
        count = search(p);                      // 패턴 문자열 탐색 후 횟수 저장
        finish = chrono::system_clock::now();   // 시간 측정 끝
        duration += chrono::duration_cast<chrono::microseconds>(finish - start);
        fout << count << endl;                  // 패턴 등장 횟수를 파일에 출력
    }
    cout << "패턴 문자열 탐색 소요 시간 : " << duration.count() << " μs" << endl;
    return 0;
}

int search(string p) {                      // 문자열 T에서 패턴 P를 탐색해 등장 횟수를 반환하는 함수
    int tmp, pos = 0, count = 0;            // 탐색 성공 여부, 탐색 위치, 탐색 횟수를 저장할 변수
    while ((tmp = t.find(p, pos)) != -1) {  // 문자열 T에서 P를 탐색
        count++;                            // 탐색에 성공한 경우 탐색 횟수 1 증가 
        pos = tmp + 1;                      // 탐색한 다음 위치부터 탐색
    }
    return count;                           // 탐색 횟수를 반환
}
