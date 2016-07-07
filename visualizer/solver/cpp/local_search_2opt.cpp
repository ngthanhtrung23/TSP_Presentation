#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2011;
const double SOLUTION = 37000;

struct Point {
    double x, y;

    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }

    double len() { return sqrt(x*x + y*y); }
} a[MAXN];

int n;

struct Result {
    double len;
    int id[MAXN];

    void calculate() {
        len = 0;
        for(int i = 1; i <= n; ++i) {
            int u = id[i], v = (i == n) ? id[1] : id[i+1];
            len += (a[u] - a[v]).len();
        }
    }

    void move_2opt(int u, int v) {
        for(int x = u+1, y = v; x < y; ++x, --y)
            swap(id[x], id[y]);
        calculate();
    }
} current, optimal;

void save() {
    cout << (fixed) << setprecision(3) << optimal.len << " 0\n";
    for(int i = 1; i <= n; ++i) {
        cout << optimal.id[i] - 1 << ' ';
    }
    cout << endl;
}

void update(Result &optimal, const Result &current) {
    if (current.len > optimal.len) return ;

    optimal.len = current.len;
    for(int i = 1; i <= n; ++i)
        optimal.id[i] = current.id[i];
    save();
}

bool used[MAXN];

void optimize() {
    while (true) {
        int found = false;
        for(int u = 1; u <= n; ++u)
            for(int v = u+2; v <= n; ++v) {
                Point A = a[ current.id[u] ];
                Point B = a[ current.id[u + 1] ];

                Point X = a[ current.id[v] ];
                Point Y = a[ current.id[v % n + 1] ];

                if ((A - B).len() + (Y - X).len() > (A - X).len() + (B - Y).len()) {
                    current.move_2opt(u, v);
                    found = true;
                }
            }
        if (!found) break;

        cout << optimal.len << " --> " << current.len << endl;
        update(optimal, current);
    }
}

int main(int argc, char** argv) {
    cerr << "HERE" << endl;
    cerr << (fixed) << setprecision(3);

    // READ INPUT
    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    fin.close();

    // INIT RESULT
    cerr << "n = " << n << endl;
    for(int i = 1; i <= n; ++i)
        current.id[i] = i;
    current.calculate();

    cout << current.len << endl;
    optimal = current;

    // OPTIMIZE
    optimize();

    // WRITE OUTPUT
    cerr << "OUTPUT TO: " << argv[2] << endl;
    fstream fout; fout.open(argv[2], fstream :: out);
    fout << (fixed) << setprecision(3) << optimal.len << " 0" << endl;
    for(int i = 1; i <= n; ++i)
        fout << optimal.id[i] - 1 << ' ';
    fout << endl;
    fout.close();
}
