#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
    int n;
    cin >> n;
    string s;
    map<string, int> m;
    for(int i = 0; i < n ;i++){
        cin >> s;
        if(m.find(s) == m.end()){
            m[s] = 1;
        }else{
            m[s]++;
        }
    }
    for(auto it: m){
        string word = it.first;
        double rat = double(it.second) / n * 100;
        cout << fixed;
        cout.precision(4);
        cout << word << ' ' << rat << endl;
    }
    return 0;
}