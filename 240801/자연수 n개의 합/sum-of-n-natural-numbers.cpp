#include <iostream>
using namespace std;

int main() {
    long long s;
    cin >> s;
    long long l = 0, r = 10000000000;
    int n =0;
    while(l <= r){
        long long mid = (l+r)/2;
        if(mid*(mid+1)/2 <= s){
            n = mid;
            l = mid +1;
        }else r = mid -1;
    }
    cout << n;
    return 0;
}