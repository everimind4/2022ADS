#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

#define N 10000000                  // 문자열 T의 길이 최대값

void buildsa();                     // 접미사 배열을 생성하는 함수
bool compare(int, int);             // 그룹 비교를 수행하는 함수
void buildlcp();                    // LCP 배열을 생성하는 함수
void buildlr(int, int);             // LCP_LR 배열을 생성하는 함수

// 접미사 배열, LCP 배열 저장 및 그룹을 위한 변수
int sa[N], sa_i[N], lcp[N], lcp_lr[N], group[N], nextgroup[N], l;
int n, k, s, m; // 문자열 T의 길이, 질의 횟수, 패턴의 시작 위치, 길이
string t, p;    // 문자열 T와 패턴 문자열을 저장

int main() {
    ifstream fin("indata.txt");     // indata.txt 파일 open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "There is no 'indata.txt' file . . ." << endl; // Error 출력
        return -1;                  // 실행 종료
    }

    chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
    chrono::microseconds duration;  // 시간 차이를 계산하기 위한 변수

    fin >> n >> k >> t;             // 파일에서 해당 변수들의 값을 Read

    buildsa();                      // 접미사 배열 생성

    for (int i = 0; i < n; i++)
        cout << t.substr(sa[i]) << endl;

    start = chrono::system_clock::now();    // 시간 측정 시작
    for (int i = 0; i < k; i++) {           // k번 반복하며
        fin >> s >> m;
        p = t.substr(s, m);
        // 접미사 배열 탐색 코드 작성
    }
    finish = chrono::system_clock::now();   // 시간 측정 끝
    duration = chrono::duration_cast<chrono::microseconds>(finish - start);

    cout << "실행에 걸린 시간 : " << duration.count() << " μs" << endl;
    return 0;
}

void buildsa() {                            // 접미사 배열 생성
    for (int i = 0; i < n; i++) {           // 문자열의 길이만큼 반복
        sa[i] = i;                          // 접미사 배열 초기화
        if (i < n - 1)                      // 접미사에 그룹 할당
            group[sa[i]] = t[i]-'a'+1;      // 알파벳 순서를 사용
        else                                // '$'의 경우
            group[n-1] = 0;                 // 가장 앞 그룹 할당
    }

    l = 1;
    while (l <= n) {                        // 정렬 길이가 전체 길이를 넘기 전까지
        sort(sa, sa+n, compare);            // 현재 그룹으로 접미사를 정렬
        nextgroup[sa[0]] = 0;               // 현재 가장 앞 그룹을 1번 그룹으로 지정
        for (int i = 0; i < n-1; i++)       // 새로 정렬된 접미사를 그룹핑
            if (compare(sa[i], sa[i+1]))    // 부분문자열의 순위를 비교하여
                nextgroup[sa[i+1]] = nextgroup[sa[i]]+1;    // 다르면 다음 그룹 할당
            else                            // 서로 순위가 동일한 경우
                nextgroup[sa[i+1]] = nextgroup[sa[i]];      // 동일한 그룹 할당
        for (int i = 0; i < n; i++)         // 새로 할당한 그룹을
            group[i] = nextgroup[i];        // 기존 그룹에 Update
        l <<= 1;                            // 비교 길이를 Doubling
    }
}

bool compare(int i, int j) {                // Prefix Doubling을 위한 비교 결과 반환 함수
    if (group[i] == group[j])               // 두 부분문자열을 묶어 앞쪽의 순위가 동일한 경우
        return group[i+l] < group[j+l];     // 그 다음 부분문자열의 순위를 비교하여 결과 반환
    else                                    // 앞쪽 부분문자열의 순위가 다를 경우
        return group[i] < group[j];         // 앞쪽 부분문자열의 순위 비교 결과를 반환
}
