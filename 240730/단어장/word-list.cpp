#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    int n; cin >> n;
    string s;
    map<string, int> m;
    for(int i = 0 ; i < n; i++){
        cin >> s;
        if(m.find(s) == m.end()){
            m[s] = 1;
        }
        else{
            m[s]++;
        }
    }
    for(auto it: m){
        cout << it.first << ' ' << it.second << endl;
    }
    return 0;
}