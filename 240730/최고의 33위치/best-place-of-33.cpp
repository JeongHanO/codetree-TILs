#include <iostream>
using namespace std;

int n;
int board[21][21];

int main() {
    cin >> n;
    for(int i =0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> board[i][j];
        }
    }

    int ans =0;
    for(int i = 0; i <= n-3; i++){
        for(int j = 0; j <= n-3; j++){
            int cnt = 0;
            for(int x = i; x < i+3; x++){
                for(int y = j; y < j+3; y++){
                    cnt += board[x][y];
                }
            }
            if(ans < cnt) ans = cnt;
        }
    }
    cout << ans;
    return 0;
}