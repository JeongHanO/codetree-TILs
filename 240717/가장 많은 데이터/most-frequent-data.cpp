#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

int main() {
    unordered_map<string, int> m;
    int n; cin >> n;
    int ans = 0;
    for(int i = 0; i < n; i++){
        string s; cin >> s;
        if(m.find(s) == m.end()){
            m[s] = 1;
        }
        else{
            m[s]++;
            ans = max(ans, m[s]);
        }
    }
    cout << ans << endl;
    return 0;
}