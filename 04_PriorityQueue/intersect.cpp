#include <iostream>
#include <queue>

using namespace std;

struct segment {
    int num;
    long xs;
    long xe;
    long ys;
    long ye;
}

struct compare {
    bool operator()(const segment& seg1, const segment& seg2) {
        return seg1.xs > seg2.xs;
    }
}

priority_queue<segment, vector<segment>, compare> pq;

int main() {
    return 0;
}
