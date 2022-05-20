#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct segment {
    int num;
    long xs, ys, xe, ye;

    segment(int N, long a, long b, long c, long d)
        : num(N), xs(a), ys(b), xe(c), ye(d) {}
};
struct result {
    int si, sj;

    result(int i, int j) : si(i), sj(j) {}
};
struct compareseg {
    bool operator()(const segment& seg1, const segment& seg2) {
        return seg1.xs > seg2.xs;
    }
};
struct compareres {
    bool operator()(const result& res1, const result& res2) {
        if (res1.si > res2.si)
            return true;
        else if (res1.si == res2.si)
            return res1.sj > res2.sj;
        else
            return false;
    }
};

bool intersect(const segment&, const segment&);

priority_queue<segment, vector<segment>, compareseg> pq;
priority_queue<result, vector<result>, compareres> res;
vector<segment> cmp;

int n, xs, ys, xe, ye;

int main() {
    ifstream fin("in_segment.txt");
    if (!fin.is_open()) {
        cout << "There is no 'in_segment.txt' file . . ." << endl;
        return -1;
    }

    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> xs >> ys >> xe >> ye;
        pq.push(segment(i+1, xs, ys, xe, ye));
    }

    while (!pq.empty()) {
        segment temp = pq.top();
        pq.pop();
        while (!pq.empty() && pq.top().xs <= temp.xe) {
            cmp.push_back(pq.top());
            pq.pop();
        }
        while (!cmp.empty()) {
            if (intersect(temp, cmp.back())) {
                if (temp.num < cmp.back().num)
                    res.push(result(temp.num, cmp.back().num));
                else
                    res.push(result(cmp.back().num, temp.num));
            }
            pq.push(cmp.back());
            cmp.pop_back();
        }
    }

    while (!res.empty()) {
        result temp = res.top();
        cout << temp.si << ' ' << temp.sj << endl;
        res.pop();
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
