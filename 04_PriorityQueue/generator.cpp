#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <vector>

using namespace std;

int count, start;   // 생성한 Segment 개수, 시작 위치를 위한 변수
mt19937 gen(chrono::high_resolution_clock:: // 난수 시드 초기화
        now().time_since_epoch().count());  // 현재 시간으로 초기화
uniform_int_distribution<int> xlen(1, 10);  // Segment의 x축 길이 설정
uniform_int_distribution<int> ylen(1, 5);   // Segment의 y축 길이 설정
uniform_int_distribution<int> coin(0, 1);   // Coin Flip
uniform_int_distribution<int> jump(5, 50);  // 시작 위치 변경

struct segment {        // Line Segment를 저장할 구조체
    int xs, ys, xe, ye; // Line Segment 좌표값
    // Line Segment 생성자
    segment(int a, int b, int c, int d)
        : xs(a), ys(b), xe(c), ye(d) {}
};

vector<segment> segments;   // Line Segment를 저장할 Vector

int main(int argc, char* argv[]) {
    if (argc != 2) {    // 생성할 Segment 개수만 입력됐는지 검사
        cout << "./a.out (# of Segment)" << endl;
        return -1;      // 그렇지 않은 경우 Usage 출력하고 종료
    }
    int n = atoi(argv[1]);      // 입력된 Segment 개수 저장

    ofstream fout("myin.txt");  // 출력 파일 생성
    fout << n << endl;          // Segment 개수 출력

    int xs, ys, xe, ye;                     // Segment 좌표 저장
    while (count < n) {                     // Segment 개수만큼 반복
        xs = start / 100;                   // 시작 위치 x 좌표값 지정
        ys = start % 100 * 2 + 11;          // 시작 위치 y 좌표값 지정
        xe = xs + xlen(gen);                // 끝나는 위치 x 좌표값 지정
        if (coin(gen))                      // 1이 나온 경우
            ye = ys + ylen(gen) * 2 + 1;    // 끝나는 위치의 y 좌표값이 증가함
        else                                // 0이 나온 경우
            ye = ys - ylen(gen) * 2 - 1;    // 끝나는 위치의 y 좌표값이 감소함
        start += jump(gen);                 // 시작 위치를 다음으로 변경 (중복 제거)
        segments.push_back(segment(xs, ys, xe, ye));
        count++;                            // 생성한 Segment 개수 1 증가
    }

    while (!segments.empty()) {             // 모든 Segment에 대해
        uniform_int_distribution<int> pick(0, segments.size() - 1);
        int numseg = pick(gen);             // 무작위로 Segment를 하나 추출
        fout << segments[numseg].xs << ' '  // 좌표 값을 차례대로 파일에 출력
            << segments[numseg].ys << ' '
            << segments[numseg].xe << ' '
            << segments[numseg].ye << endl;
        segments.erase(segments.begin() + numseg);  // 출력한 Segment를 삭제
    }

    return 0;
}
