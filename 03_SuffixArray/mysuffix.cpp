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
int buildlr(int, int);              // LCP_LR 배열을 생성하는 함수
int search(string);                 // 문자열 T에서 패턴을 찾는 함수

// 접미사 배열, LCP 배열 저장 및 그룹을 위한 변수
int sa[N], sa_i[N], lcp[N], lcp_lr[N], group[N], nextgroup[N], l;
int n, k, s, m; // 문자열 T의 길이, 질의 횟수, 패턴의 시작 위치, 길이
string t, p;    // 문자열 T와 패턴 문자열을 저장

chrono::system_clock::time_point start, finish; // 시간 측정을 위한 변수
chrono::microseconds duration1, duration2;  // 시간 차이를 계산하기 위한 변수

int main() {
    ifstream fin("indata.txt");     // 데이터 입력 파일 open
    if (!fin.is_open()) {           // File이 존재하지 않을 경우
        cout << "There is no 'indata.txt' file . . ." << endl;
        return -1;                  // Usage 출력 후 실행 종료
    }
    ofstream fout("outdata.txt");   // 결과 출력 파일 open

    fin >> n >> k >> t;             // 파일에서 해당 변수들의 값을 Read
    n = t.size();

    start = chrono::system_clock::now();    // 시간 측정 시작
    buildsa();                              // 접미사 배열 생성
    buildlcp();                             // LCP 배열 생성
    buildlr(0, n-1);                        // LCP_LR 배열 생성
    finish = chrono::system_clock::now();   // 시간 측정 끝
    duration1 = chrono::duration_cast<chrono::microseconds>(finish - start);

    int count;
    for (int i = 0; i < k; i++) {               // k번 반복하며
        fin >> s >> m;                          // 패턴의 시작 위치와 길이 입력
        p = t.substr(s, m);                     // 시작 위치와 길이로 패턴 추출
        start = chrono::system_clock::now();    // 시간 측정 시작
        count = search(p);                      // 패턴 문자열 탐색 후 횟수 저장
        finish = chrono::system_clock::now();   // 시간 측정 끝
        duration2 += chrono::duration_cast<chrono::microseconds>(finish - start);
        fout << count << endl;                  // 패턴 등장 횟수를 파일에 출력
    }

    cout << "접미사 배열 생성 소요 시간 : " << duration1.count() << " μs" << endl;
    cout << "패턴 문자열 탐색 소요 시간 : " << duration2.count() << " μs" << endl;
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
    if (group[i] != group[j])               // 특정 부분문자열의 순위가 다른 경우
        return group[i] < group[j];         // 순위 비교 결과를 바로 반환
    else                                    // 해당 부분문자열의 순위가 같은 경우
        return group[i+l] < group[j+l];     // 그 다음 부분문자열의 순위를 비교하여 결과 반환
}

void buildlcp() {                           // LCP 배열을 생성하는 함수
    for (int i = 0; i < n; i++)             // 접미사 배열의 Inverse 생성
        sa_i[sa[i]] = i;                    // 접미사 배열의 값을 Index로 하여 순번 저장
    int l = 0;                              // LCP 값을 저장할 변수
    string a, b;                            // 비교할 부분문자열을 저장
    for (int i = 0; i < n; ++i) {           // LCP 배열을 생성하기 위해
        a = t.substr(sa[sa_i[i-1]]);        // 원래 문자열의 i번째문자부터 시작하는 접미사와
        b = t.substr(sa[sa_i[i-1]+1]);      // 사전 순으로 정렬한 그 다음 접미사를 가져와 저장
        while (a[l] == b[l])                // 두 문자열이 일치하는 동안
            l++;                            // 비교 길이를 증가시키며 LCP 계산
        lcp[sa_i[i-1]] = l;                 // 계산한 LCP 값을 LCP 배열의 해당 위치에 저장
        l--;                                // 다음에 비교할 문자열은 l-1개의 문제를 제외하고 비교
    }
}

int buildlr(int l, int r) {                 // LCP_LR 배열을 생성하는 함수 (재귀적으로 생성)
    int m = l+((r-l)>>1);                   // 이진 탐색의 중간 위치를 저장할 변수
    int lcp_l, lcp_r;                       // LCP_L, LCP_R 값을 저장할 변수
    if (l == m-1)                           // 범위의 가장 왼쪽 접미사와 비교 대상 접미사가 인접한 경우
        lcp_l = lcp[l];                     // 왼쪽 접미사와 비교 대상 접미사의 LCP 길이를 저장
    else                                    // 그렇지 않은 경우
        lcp_l = buildlr(l, m);              // 인접한 범위가 나타날 때까지 재귀적으로 함수 호출
    if (r == m+1)                           // 범위의 가장 오른쪽 접미사와 비교 대상 접미사가 인접한 경우
        lcp_r = lcp[m];                     // 오른쪽 접미사와 비교 대상 접미사의 LCP 길이를 저장
    else                                    // 그렇지 않은 경우
        lcp_r = buildlr(m, r);              // 인접한 범위가 나타날 때까지 재귀적으로 함수 호출
    lcp_lr[m] = lcp_l < lcp_r ? lcp_l : lcp_r;  // LCP_L과 LCP_R의 최소값을 LCP_LR 배열에 저장한 후
    return lcp_lr[m];                           // 다른 LCP_LR 값 계산에 사용하기 위해 해당 값 반환
}

int search(string p) {                      // 문자열 T에서 패턴 P를 탐색해 등장 횟수를 반환하는 함수
    int is, ie;                             // 탐색할 패턴을 접두사로 하는 접미사의 시작과 끝 위치 저장
    int l, r, m;                            // 이진 탐색 범위와 중간 위치를 저장할 변수
    l = 0, r = n-1;                         // 이진 탐색 범위 초기화
    p += '#';                               // 패턴 문자열의 뒤에 모든 알파벳보다 앞에 위치한 '#' 추가
    while (l != r-1) {                      // 탐색 범위를 좁혀 가며
        m = l+((r-l)>>1);                   // 탐색 범위의 중앙 위치를 계산
        if (p.substr(lcp_lr[m]) > t.substr(sa[m]).substr(lcp_lr[m]))    // 패턴 문자열과 탐색 문자열 비교
            l = m;                          // 패턴 문자열이 더 뒤에 있는 경우 오른쪽 범위 탐색
        else                                // 패턴 문자열이 더 앞에 있는 경우
            r = m;                          // 왼쪽 범위 탐색
    }
    is = r;                                 // 탐색이 완료되었으므로 시작 위치를 기록
    l = 0, r = n-1;                         // 이진 탐색 범위 다시 초기화
    p.erase(p.size()-1);                    // 패턴 문자열의 마지막 문자 제거
    p += '~';                               // 패턴 문자열의 뒤에 모든 알파벳보다 뒤에 위치한 '~' 추가
    while (r != l+1) {                      // 탐색 범위를 좁혀 가며
        m = l+((r-l)>>1);                   // 탐색 범위의 중앙 위치를 계산
        if (p.substr(lcp_lr[m]) > t.substr(sa[m]).substr(lcp_lr[m]))    // 패턴 문자열과 탐색 문자열 비교
            l = m;                          // 패턴 문자열이 더 뒤에 있는 경우 오른쪽 범위 탐색
        else                                // 패턴 문자열이 더 앞에 있는 경우
            r = m;                          // 왼쪽 범위 탐색
    }
    ie = l;                                 // 탐색이 완료되었으므로 마지막 위치를 기록
    if (p > t.substr(sa[n-1]))              // 패턴이 사전 순서로 마지막 접미사까지 등장하는 경우
        ie++;                               // ie를 맨 마지막 위치로 지정하여 예외를 처리
    return ie-is+1;                         // (끝 위치) - (시작 위치) + 1 = 등장 횟수를 반환
}
