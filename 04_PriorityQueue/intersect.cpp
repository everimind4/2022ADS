#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct segment {
    int num;
    long xs;
    long ys;
    long xe;
    long ye;

    segment(int N, long a, long b, long c, long d)
         : num(N), xs(a), ys(b), xe(c), ye(d) {}
};

struct compare {
    bool operator()(const segment& seg1, const segment& seg2) {
        return seg1.xs > seg2.xs;
    }
};

bool intersect(const segment&, const segment&);

priority_queue<segment, vector<segment>, compare> pq;
int n, xs, ys, xe, ye;

int main() {
    ifstream fin("in_segment.txt");
    if (!fin.is_open()) {
        cout << "There is no 'indata.txt' file . . ." << endl;
        return -1;
    }

    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> xs >> ys >> xe >> ye;
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

bool intersect(const segment &a, const segment &b) {
    long sa1, sa2, sa3, sa4;
    sa1 = (a.xs*a.ye + a.xe*b.ys + b.xs*a.ys) - (a.ys*a.xe + a.ye*b.xs + b.ys*a.xs);
    sa2 = (a.xs*a.ye + a.xe*b.ye + b.xe*a.ys) - (a.ys*a.xe + a.ye*b.xe + b.ye*a.xs);
    sa3 = (b.xs*b.ye + b.xe*a.ys + a.xs*b.ys) - (b.ys*b.xe + b.ye*a.xs + a.ys*b.xs);
    sa4 = (b.xs*b.ye + b.xe*a.ye + a.xe*b.ys) - (b.ys*b.xe + b.ye*a.xe + a.ye*b.xs);
    if (sa1*sa2 < 0 && sa3*sa4 < 0)
        return true;
    else
        return false;
}
