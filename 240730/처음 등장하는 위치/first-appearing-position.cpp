#include <iostream>
#include <map>
using namespace std;

int main() {
    int n;
    cin >> n;
    map<int, int> m;
    for(int i = 0; i < n ;i++){
        int a; cin >> a;
        if(m.find(a) == m.end()){
            m[a] = i+1;
        }
    }
    for(auto it: m){
        cout << it.first << ' ' << it.second << endl;
    }
    return 0;
}