#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct segment {            // Line Segment를 저장할 구조체
    int num;                // Line Segment 번호
    long xs, ys, xe, ye;    // Line Segment 좌표
    // Line Segment 구조체 생성자
    segment(int N, long a, long b, long c, long d)
        : num(N), xs(a), ys(b), xe(c), ye(d) {}
};
struct result { // Line Segment 비교 결과를 저장할 구조체
    int si, sj; // 교차하는 Line Segment 번호를 저장
    // 비교 결과 구조체 생성자
    result(int i, int j) : si(i), sj(j) {}
};
struct compareseg { // Line Segment 정렬을 위한 비교 구조체
    bool operator()(const segment& seg1, const segment& seg2) {
        return seg1.xs > seg2.xs; // 시작점의 X좌표를 기준으로 정렬
    }
};
struct compareres { // 교차하는 Segment 번호 정렬을 위한 비교 구조체
    bool operator()(const result& res1, const result& res2) {
        if (res1.si > res2.si)          // 앞 번호 기준으로 비교
            return true;
        else if (res1.si == res2.si)    // 앞 번호가 같을 경우
            return res1.sj > res2.sj;   // 뒷 번호 기준으로 비교
        else
            return false;
    }
};

bool intersect(const segment&, const segment&); // 교차 여부를 판단할 함수

// Line Segment와 교차하는 Segment 번호를 정렬해 저장할 Priority Queue
priority_queue<segment, vector<segment>, compareseg> pq;
priority_queue<result, vector<result>, compareres> res;
vector<segment> cmp;    // 교차 여부 판단을 위해 사용하는 벡터

int n, xs, ys, xe, ye;  // 파일에서 Segment 개수 및 좌표 값을 입력

int main() {
    ifstream fin("in_segment.txt"); // 입력 파일 Open
    if (!fin.is_open()) {           // 열리지 않았을 경우
        cout << "There is no 'in_segment.txt' file . . ." << endl;
        return -1;                  // 예외처리
    }

    fin >> n;                                   // Segment 개수 입력
    for (int i = 0; i < n; i++) {               // Segment 개수만큼 반복
        fin >> xs >> ys >> xe >> ye;            // Segment 좌표 값 입력
        pq.push(segment(i+1, xs, ys, xe, ye));  // Priority Queue에 저장
    }

    while (!pq.empty()) {                       // Priority Queue에 대해
        segment temp = pq.top();                // 맨 위의 값 Copy
        pq.pop();                               // 가장 앞의 Segment 제거
        while (!pq.empty() && pq.top().xs <= temp.xe) { // 비교 대상 추출
            cmp.push_back(pq.top());                    // 비교 대상 복사
            pq.pop();                                   // 다음 대상 비교
        } // 겹칠 가능성이 있는 Line Segment를 xs 기준으로 추출하는 과정
        while (!cmp.empty()) {                  // 추출한 비교 대상에 대해
            if (intersect(temp, cmp.back())) {  // 교차 여부 판단
                if (temp.num < cmp.back().num)  // 교차하는 Segment 번호 비교
                    res.push(result(temp.num, cmp.back().num));
                else                            // 사전 순으로 결과 저장
                    res.push(result(cmp.back().num, temp.num));
            }
            pq.push(cmp.back());    // 비교한 Segment를 Priority Queue로 복원
            cmp.pop_back();         // 비교 대상에서 삭제
        }
    }

    while (!res.empty()) {          // 교차하는 Segment 탐색 결과에 대해
        result temp = res.top();    // 맨 앞에서부터 차례대로
        cout << temp.si << ' ' << temp.sj << endl;  // Segment 번호 출력
        res.pop();                  // 출력 후 삭제
    }

    return 0;
}

bool intersect(const segment &a, const segment &b) {    // 교차 여부 판단
    long sa1, sa2, sa3, sa4;                            // Signed Area 값 계산
    sa1 = (a.xs*a.ye + a.xe*b.ys + b.xs*a.ys) - (a.ys*a.xe + a.ye*b.xs + b.ys*a.xs);
    sa2 = (a.xs*a.ye + a.xe*b.ye + b.xe*a.ys) - (a.ys*a.xe + a.ye*b.xe + b.ye*a.xs);
    sa3 = (b.xs*b.ye + b.xe*a.ys + a.xs*b.ys) - (b.ys*b.xe + b.ye*a.xs + a.ys*b.xs);
    sa4 = (b.xs*b.ye + b.xe*a.ye + a.xe*b.ys) - (b.ys*b.xe + b.ye*a.xe + a.ye*b.xs);
    if (sa1*sa2 < 0 && sa3*sa4 < 0) // Signed Area 값을 기준으로 교차 여부 판단
        return true;                // 교차 시 True 반환
    else                            // 그렇지 않을 경우
        return false;               // False 반환
}
