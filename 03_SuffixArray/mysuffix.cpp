#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {                // File은 하나만 입력
        cout << "./a.out (filename)" << endl;   // 올바른 형식 출력
        return -1;
    }
    ifstream fin(argv[1]);          // Filename으로 file open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "Could not open the file '" << argv[1] << "' . . ." << endl;
        return -2;
    }                               // Error Message 출력
    chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
    chrono::microseconds duration;  // 시간 차이를 계산하기 위한 변수
    int n, k, i, m;                 // 문자열 T의 길이, 질의 횟수, 패턴의 시작 위치, 길이
    string t, p;                    // 문자열 T와 패턴 문자열을 저장
    fin >> n >> k >> t;             // 파일에서 해당 변수들의 값을 Read
    // Suffix Array 구축 코드 작성
    start = chrono::system_clock::now();    // 시간 측정 시작
    for (int j = 0; j < k; j++) {   // k번 반복하며
        fin >> i >> m;
        cout << t.substr(i, m) << endl;
        // Suffix Array 탐색 코드 작성
    }
    finish = chrono::system_clock::now();   // 시간 측정 끝 
    duration = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << "실행에 걸린 시간 : " << duration.count() << " μs" << endl;
    return 0;
}
