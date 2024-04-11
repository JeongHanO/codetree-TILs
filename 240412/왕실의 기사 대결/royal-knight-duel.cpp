#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define MAX_L 41
#define MAX_N 31

int l, n, q;
int board[MAX_L][MAX_L];
int r[MAX_N], c[MAX_N], h[MAX_N], w[MAX_N], k[MAX_N];
int life[MAX_N];
int nr[MAX_N], nc[MAX_N];
int dmg[MAX_N];
bool is_moved[MAX_N];

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

bool TryMovement(int idx, int dir){
    for(int i = 1; i <= n; i++){
        dmg[i] = 0;
        is_moved[i] = false;
        nr[i] = r[i];
        nc[i] = c[i];
    }
    queue<int> q;

    q.push(idx);
    is_moved[idx] = true;
    while(!q.empty()){
        int x = q.front();
        q.pop();
        
        nr[x] += dx[dir];
        nc[x] += dy[dir];

        if(nr[x] < 1 || nr[x]+h[x]-1 > l || nc[x] < 1 || nc[x]+w[x]-1 > l)
            return false;

        for(int i = nr[x]; i <= nr[x]+h[x]-1; i++){
            for(int j = nc[x]; j <= nc[x]+w[x]-1; j++){
                if(board[i][j] == 1) dmg[x]++;
                else if(board[i][j] == 2) return false;
            }
        }

        for(int i = 1; i <= n; i++){
            if(is_moved[i] || k[i] <= 0) continue;
            if(r[i] > nr[x]+h[x]-1 || nr[x] > r[i]+h[i]-1) continue; // 이동한 범위보다 아래 또는 위
            if(c[i] > nc[x]+w[x]-1 || nc[x] > c[i]+w[i]-1) continue;

            is_moved[i] = true;
            q.push(i);
        }
    }
    dmg[idx] = 0; // 민 기사는 데미지 X
    return true;
}

void MovePiece(int idx, int dir){
    if(k[idx] <= 0) return;

    if(TryMovement(idx, dir)){
        for(int i = 1; i <= n; i++){
            r[i] = nr[i];
            c[i] = nc[i];
            k[i] -= dmg[i];
        }
    }
}

int main(){
    cin >> l >> n >> q;
    for(int i = 1; i <= l; i++){
        for(int j = 1; j <= l; j++){
            cin >> board[i][j];
        }
    }
    for(int i = 1; i <= n; i++) {
        cin >> r[i] >> c[i] >> h[i] >> w[i] >> k[i];
        life[i] = k[i];
    }
    for(int i = 1; i <= q; i++){
        int idx, dir;
        cin >> idx >> dir;
        MovePiece(idx, dir);
    }

    int ans = 0;
    for(int i =1; i <= n; i++){
        if(k[i] > 0){
            ans += life[i] - k[i];
        }
    }
    cout << ans << endl;
}