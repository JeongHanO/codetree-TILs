#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct Knight{ // x, y, 높이, 너비, 생명, 받은 데미지
    int r;
    int c;
    int h;
    int w;
    int k;
    int d;
};

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

int l, n, q;
int ans = 0;
pair<int, int> board[41][41]; // first: 맵 정보, second: 기사 번호
Knight knights[31];
pair<int, int> orders[101];

bool test; //@@@@

void print_knight(){
    for(int i = 1; i <= l; i++){
        for(int j = 1; j <= l; j++){
            cout << board[i][j].second << ' ';
        }
        cout << endl;
    }
    cout << "===================\n\n";
}

void input(){
    cin >> l >> n >> q;
    // 맵 정보
    for(int i = 1; i <= l; i++){
        for(int j = 1; j <= l; j++){
            int a; cin >> a;
            board[i][j].first = a;
        }
    }
    // 기사 정보
    for(int i = 1; i <= n; i++){
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        knights[i] = {r, c, h, w, k};
        for(int x = r; x < r + h; x++){
            for(int y = c; y < c + w; y++){
                board[x][y].second = i;
            }
        }
    }
    // 명령 정보
    for(int i = 0; i < q; i++){
        int a, b; cin >> a >> b;
        orders[i] = {a, b};
    }
}

// 입력받은 기사를 이동
// 기존에 있던 자리 비우기, 새로운 자리에 입력, 구조체 갱신
void movement(int kni_num, int dir){
    int r = knights[kni_num].r;
    int c = knights[kni_num].c;
    int h = knights[kni_num].h;
    int w = knights[kni_num].w;
    int nr = r + dx[dir]; 
    int nc = c + dy[dir];

    // board 갱신
    for(int i = r; i < r + h; i++){
        for(int j = c; j < c + w; j++){
            board[i][j].second = 0;
        }
    }

    // 기사 정보 갱신
    knights[kni_num].r = nr;
    knights[kni_num].c = nc;

    for(int i = nr; i < nr + h; i++){
        for(int j = nc; j < nc + w; j++){
            board[i][j].second = kni_num;
        }
    }
}

// 기사의 번호를 받아서 범위 내 함정의 개수만큼 체력 감소
void damage(int kni_num){
    // 1~n사이의 범위 잘 확인 할 것. 함정은 1임.
    int r = knights[kni_num].r;
    int c = knights[kni_num].c;
    int h = knights[kni_num].h;
    int w = knights[kni_num].w;
    int k = knights[kni_num].k;

    // 범위 내 함정 갯수 카운트
    int cnt_dmg = 0;
    for(int i = r; i <= l &&  i < r+h; i++){
        for(int j = c; j <= l &&  j < c+w; j++){
            if(board[i][j].first == 1) cnt_dmg++;
        }
    }

    // 데미지 계산, 기사 생존 여부 확인. 사망시 board에서 정보 삭제.
    k -= cnt_dmg;
    if(k <= 0){ // 사망
        knights[kni_num].k = 0;
        for(int i = r; i < r + h; i++){
            for(int j = c; j < c + w; j++){
                board[i][j].second = 0;
            }
        }

    } else{ // 생존
        knights[kni_num].k = k; // 생명력 갱신
        knights[kni_num].d += cnt_dmg; // 누적 데미지 갱신
        // cout << "num: " << kni_num << " / dmg : " << cnt_dmg << endl;
    }
}

// 명령 수행이 가능한지 판단하는 함수
// 연쇄적으로 움직일 기사들을 stack에 넣고, 움직임의 끝이 벽인지 아닌지 확인.
void check_order(int ord_num){
    stack<int> stk;
    
    // 기사 번호, 이동 방향, 기사 좌표
    int ord_kni_num = orders[ord_num].first;
    int dir = orders[ord_num].second;
    
    stk.push(ord_kni_num);
    bool stacked[31] = {false};

    // 스택의 top에 있는 기사의 범위를 보고 판단.
    // 벽이 있거나 범위를 벗어나면 -> return
    // 범위에 다른 기사가 있다면 -> 스택에 추가 + continue
    // 범위에 다른 기사의 "범위"가 포함되는지 봐야함. board에서 범위를 늘려야할듯.
    // 범위를 늘리면 스택에 쌓이는 수가 많아질텐데. 이걸 방지할 방법이 필요할듯.
    // 기사도 벽도 없으면 -> damage + stk.pop()
    while(!stk.empty()){
        int kni_num = stk.top();
        int nr = knights[kni_num].r + dx[dir];
        int nc = knights[kni_num].c + dy[dir];
        int h = knights[kni_num].h;
        int w = knights[kni_num].w;

        // cout << "K.N: " << kni_num << " | "<< nr << ' ' << nc << ' ' << h << ' ' << w  << endl;
        if(nr < 1 || nc < 1 || nr+h-1 > l || nc+w-1> l) {
            return;
        } // 범위 벗어남

        bool found_knight = false; // 범위 내 기사 존재 여부
        
        for(int i = nr; i < nr + h; i++){
            for(int j = nc; j < nc + w; j++){
                if(board[i][j].first == 2) return; //벽에 막힘
                if(board[i][j].second > 0 && !stacked[board[i][j].second]){ // 범위 안에 기사 있음
                    stk.push(board[i][j].second);
                    stacked[board[i][j].second] = true;
                    found_knight = true;
                }
            }
        }

        if(found_knight) continue; // 기사 찾았으면 연쇄작용 받을 기사 먼저 탐색 + 동작
        
        // 범위 안에 기사 없으면 자리 이동 + 데미지 수행
        stk.pop();
        movement(kni_num, dir);
        if(kni_num != ord_kni_num) damage(kni_num); // 민 기사는 데미지 받으면 안됨
    }

}

void sum_damege(){
    for(int i = 1; i <= n; i++){
        if(knights[i].k > 0) ans += knights[i].d;
    }
    cout << ans << endl;
}

void solve(){
    input();
    for(int i = 0; i < q; i++){
        if(knights[orders[i].first].k == 0) continue; // 수행할 기사 없음
        check_order(i);
    }
    sum_damege();
}

int main() {
    solve();
    return 0;
}

/*
solve에서 q번 실행.
1. 명령을 실행할 대상의 존재 판별(라이프 확인)
2. 명령을 실행 -> knights 배열의 인덱스 번호를 호출(기사 번호와 동일)
2-1. 이동 가능 여부 확인(아무도 못 움직일 때. 벽과 범위 판단)
2-2. 연쇄반응의 마지막에 있는 병사부터 한 칸씩 이동 (보드 갱신 & 기사 갱신)
3. 이동 후 데미지
3-1. 범위를 판단해서 함정의 개수 카운트(범위 밖으로 벗어나는지 판단)

이동과 데미지를 어떻게 구현할까?
- 스택을 이용하여 마지막 기사부터 이동.
- 기사가 이동하면 데미지.
*/