#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

int main() {
    int n;
    cin >> n;

    unordered_map<int, int> um;
    for(int i = 0; i < n; i++){
        string s; cin >> s;
        if(s == "add"){
            int a, b; cin >> a >> b;
            um[a] = b;
        }
        else if(s == "remove"){
            int a; cin >> a;
            um.erase(a);
        }
        else{
            int a; cin >> a;
            if(um.find(a) != um.end()){
                cout << um.find(a)->second << endl;
            }
            else{
                cout << "None" << endl;
            }
        }
    }
    return 0;
}