#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct segment {
    int num;
    long xs;
    long xe;
    long ys;
    long ye;

    segment(int N, long a, long b, long c, long d)
         : num(N), xs(a), xe(b), ys(c), ye(d) {}
};

struct compare {
    bool operator()(const segment& seg1, const segment& seg2) {
        return seg1.xs > seg2.xs;
    }
};

priority_queue<segment, vector<segment>, compare> pq;

int n, xs, xe, ys, ye;

int main() {
    ifstream fin("in_segment.txt");
    if (!fin.is_open()) {
        cout << "There is no 'indata.txt' file . . ." << endl;
        return -1;
    }
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> xs >> xe >> ys >> ye;
        pq.push(segment(i+1, xs, xe, ys, ye));
    }
    while (!pq.empty()) {
        segment temp = pq.top();
        cout << temp.num << ' '
            << temp.xs << ' '
            << temp.xe << ' '
            << temp.ys << ' '
            << temp.ye << endl;
        pq.pop();
    }
    return 0;
}
