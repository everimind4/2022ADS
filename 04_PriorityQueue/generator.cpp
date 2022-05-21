#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "./a.out (# of Segment)" << endl;
        return -1;
    }
    int n = atoi(argv[1]);

    ofstream fout("myin.txt");
    fout << n << endl;

    return 0;
}
