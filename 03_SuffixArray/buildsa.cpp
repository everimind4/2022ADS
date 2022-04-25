#include <iostream>
#include <algorithm>

#define MAX_N 10000000

using namespace std;

bool compare(int, int);

int sa[MAX_N], group[MAX_N], nextgroup[MAX_N], l;

int main() {
    string t = "banana$";
    int n = t.size();
    for (int i = 0; i < n; i++) {    // 문자열의 길이만큼 반복
        sa[i] = i;                          // Suffix Array 초기화
        if (i < n - 1)                      // Suffix에 그룹 할당
            group[sa[i]] = t[i]-'a'+1;      // 알파벳 순서를 사용
        else                                // '$'의 경우
            group[n-1] = 0;                 // 가장 앞 그룹 할당
    }

    l = 1;
    while (l <= n) {                        // 정렬 길이가 전체 길이를 넘기 전까지
        sort(sa, sa+n, compare);            // 현재 그룹으로 Suffix를 정렬
        nextgroup[sa[0]] = 0;               // 현재 가장 앞 그룹을 1번 그룹으로 지정
        for (int i = 0; i < n-1; i++)       // 새로 정렬된 Suffix를 그룹핑
            if (compare(sa[i], sa[i+1]))    // 접미사의 그룹을 비교하여
                nextgroup[sa[i+1]] = nextgroup[sa[i]]+1;    // 다르면 다음 그룹 할당
            else                            // 서로 동일한 그룹인 경우
                nextgroup[sa[i+1]] = nextgroup[sa[i]];      // 동일한 그룹 할당
        for (int i = 0; i < n; i++)         // 새로 할당한 그룹을
            group[i] = nextgroup[i];        // 기존 그룹에 Update
        l <<= 1;                            // 비교 대상 Prefix를 Douling
    }
    for (int i = 0; i < n; i++)
        cout << t.substr(sa[i]) << endl;

    return 0;
}

bool compare(int i, int j) {                // Prefix Doubling을 위한 비교 결과 반환 함수
    if (group[i] == group[j])               // 두 부분문자열을 묶어 앞쪽의 순위가 동일한 경우
        return group[i+l] < group[j+l];     // 그 다음 부분문자열의 순위를 비교해 정렬
    else                                    // 앞쪽 부분문자열의 순위가 다를 경우
        return group[i] < group[j];         // 그대로 정렬
}
