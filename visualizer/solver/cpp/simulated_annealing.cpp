#include <bits/stdc++.h>

#define FOR(i,a,b) for(int i=(a),_b=(b); i <= _b; ++i)
#define REP(i,a) for(int i=0,_a=(a); i < _a; ++i)
#define DEBUG(x) { cout << #x << " = " << x << endl; }
using namespace std;

const int MAXN = 2011;
const double START_TEMP = 1000;
const double COOL_DOWN = 0.997;
const int NTURN = 10000;

int NLOCAL = 1000;

struct Point {
    double x, y;
    Point() { x = y = 0; }
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }
};

int n;
Point a[MAXN];
double cost[MAXN][MAXN];

void read(string s) {
    fstream fin; fin.open(s.c_str(), fstream :: in);
    fin >> n;
    REP(i,n) fin >> a[i].x >> a[i].y;
    fin.close();

    REP(i,n) REP(j,n) {
        double dx = a[i].x - a[j].x;
        double dy = a[i].y - a[j].y;
        cost[i][j] = sqrt(dx*dx + dy*dy);
    }
}

struct Solution {
    int id[MAXN];
    double len;

    void initRandom() {
        REP(i,n) id[i] = i;
        REP(i,n) {
            swap(id[i], id[rand()%(i+1)]);
        }
        calculate();
    }

    void calculate() {
        len = 0;
        REP(i,n) {
            int j = (i == n-1) ? 0 : i+1;
            len += cost[id[i]][id[j]];
        }
    }

    void print() {
    }

    void load() {
        REP(i,n) id[i] = i;
        calculate();
    }

    void move_2opt(int u, int v) {
        for(int x = u+1, y = v; x < y; ++x, --y)
            swap(id[x], id[y]);
        calculate();
    }

    void localSearch(double t) {
        REP(turn,NLOCAL) {
            int u = rand() % (n-1);
            int v = rand() % (n-1);
            if (u == v) continue;

            double delta = (cost[id[u]][id[u+1]] + cost[id[v]][id[v+1]])
                    - (cost[id[u]][id[v]] + cost[id[u+1]][id[v+1]]);

            if (delta > 0) {
                move_2opt(u, v);
            }
            else {
                if ((double)rand() / RAND_MAX < exp(delta / t)) {
                    move_2opt(u, v);
                }
            }
        }
    }

    bool optimize() {
        bool found = false;
        REP(u,n-1) FOR(v,u+2,n-1) {
            double delta = (cost[id[u]][id[u+1]] + cost[id[v]][id[v+1]])
                    - (cost[id[u]][id[v]] + cost[id[u+1]][id[v+1]]);

            if (delta > 0) {
                found = true;
                move_2opt(u, v);
            }
        }
        return found;
    }
} sol, best;

void solve() {
    sol.load();
    best = sol;
    best.len = 1e20;
    double t = START_TEMP;
    cout << (fixed) << setprecision(4);
    REP(turn,NTURN) {
        sol.localSearch(t);
        if (sol.len < best.len) {
            best = sol;
        }
        cout << sol.len << endl;
        t = t * COOL_DOWN;
    }
    DEBUG(t);
    while (best.optimize()) {}
    best.print();
}

int main(int argc, char** argv) {
    cerr << "SIMULATED ANNEALING" << endl;
    srand(time(NULL));
    read(string(argv[1]));
    solve();
    fstream fout; fout.open(argv[2], fstream :: out);
    fout << (fixed) << setprecision(4) << best.len << ' ' << 0 << endl;
    REP(i,n) fout << best.id[i] << ' ';
    fout << endl;
    fout.close();
}
