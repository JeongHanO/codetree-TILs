#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
    unordered_map<int, int> m;
    int n, m1;
    cin >> n >> m1;
    for(int i = 0; i < n; i++){
        int a; cin >> a;
        if(m.find(a) == m.end()) m[a] = 1;
        else m[a]++;
    }
    for(int i = 0; i < m1; i++){
        int a; cin >> a;
        cout << m[a] << ' ';
    }
    return 0;
}