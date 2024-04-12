#include <iostream>
#include <vector>
using namespace std;

#define MAX_N 51
#define MAX_P 31

int n, m, p, c, d;
int board[MAX_N][MAX_N];
int sr[MAX_P], sc[MAX_P], stuned[MAX_P]; //산타 위치, 기절해 있는 라운드
int cnt_survive; // 탈락된 산타 수
bool fail[MAX_P]; // 산타 탈락 여부
int score[MAX_P]; // 산타 점수
pair<int, int> r_pos; // 루돌프 위치

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

// 연쇄 작용 구현
void InterAction(int x, int y, int dir_x, int dir_y, int santa_num){
    int flown_num = santa_num;
    int nx = x, ny = y;
    while(1){
        sr[flown_num] = nx;
        sc[flown_num] = ny;
        if(nx < 1 || ny < 1 || nx > n || ny > n){ // 범위 밖
            fail[flown_num] = true;
            cnt_survive--;
            break;
        }
        if(board[nx][ny] == 0){ //날아간 자리에 산타가 없음
            board[nx][ny] = flown_num;
            break;
        }
        int tmp_num = board[nx][ny]; // 밀릴 산타 번호
        board[nx][ny] = flown_num;
        nx += dir_x; ny += dir_y; // 밀린 산타가 갈 자리
        flown_num = tmp_num;
    }
    return;
}

// 1. 루돌프 움직임 (가장 가까운 탈락하지 않은 산타) - 대각선도 1칸.
// 1-1. 가장 가까운 산타 탐색. -> r,c배열 순회하면서 값 구하기.
// 1-2. 충돌 확인 + 상호작용(밀림) + 탈락 확인(범위 확인)
void r_move(int k){
    int dist = 100000;
    pair<int, int> target = {0, 0};
    for(int i = 1; i <= p; i++){
        if(fail[i]) continue; // 탈락한 산타는 패스
        int tmp_dist = (r_pos.first - sr[i])*(r_pos.first - sr[i]) + (r_pos.second - sc[i])*(r_pos.second - sc[i]);
        if(tmp_dist < dist){ // 더 가까운 산타 발견시 목표, 거리 갱신
            dist = tmp_dist;
            target = {sr[i], sc[i]};
        } 
        else if(tmp_dist == dist){ // 기존 목표와 같은 거리의 산타 발견시 좌표 비교 후 갱신
            if(target.first < sr[i]) target = {sr[i], sc[i]};
            else if(target.first == sr[i] && target.second < sc[i]) target = {sr[i], sc[i]};
        }
    }
    // 가장 가까운 산타와 가까워지는 방향으로 이동. 산타좌표 - 루돌프좌표를 해서 방향 찾으면 될듯.
    int x = 1, y = 1;
    if(target.first < r_pos.first) x = -1;
    else if(target.first == r_pos.first) x = 0;
    if(target.second < r_pos.second) y = -1;
    else if(target.second == r_pos.second) y = 0;

    int nx = r_pos.first + x;
    int ny = r_pos.second + y;

    // 움직일 칸에 산타가 있다면
    if(board[nx][ny] > 0){
        score[board[nx][ny]] += c;
        stuned[board[nx][ny]] = k+1;
        //@@@@@@@ if문 필요 없을지도? InterAction만 있어도 될 지도?
        if(nx+c*x < 1 || ny+c*y < 1 || nx+c*x > n|| ny+c*y > n) { // 산타가 범위 밖으로 날라가면
            cnt_survive--; // 생존 산타 수 감소
            fail[board[nx][ny]] = true; // 탈락
        }
        else{
            InterAction(nx+c*x, ny+c*y, x, y, board[nx][ny]);
        }
    }
    board[nx][ny] = 9; // 루돌프
    board[r_pos.first][r_pos.second] = 0;
    r_pos.first = nx;
    r_pos.second = ny;
}

// 2. 산타 번호 순대로움직임 (기절X, 탈락X / 둘 다 못 움직임)
// 2-1. 루돌프에게 가까워지는 방향 탐색 (상우하좌)
// 2-2. 움직임 가능 여부 확인 (빈 칸 or 루돌프 칸으로만 이동 가능)
//   - 가까워지는 방법X이면 움직임 X
// 2-3. 충돌 확인 + 상호작용(밀림)  + 탈락 확인(범위 확인)
void s_move(int k){
    for(int i = 1; i <= p; i++){
        if(fail[i] || stuned[i] >= k) continue; // 탈락 or 기절 턴
        // 따져볼 것: 루돌프와의 상대적인 위치 + 이동 가능 여부
        int next_x = 0, next_y = 0, nx, ny;
        int dist = (r_pos.first - sr[i])*(r_pos.first - sr[i]) + (r_pos.second - sc[i])*(r_pos.second - sc[i]);
        int tmp_dist; 
        for(int j = 0; j < 4; j++){
            nx = sr[i] + dx[j];
            ny = sc[i] + dy[j];
            tmp_dist = (r_pos.first - nx)*(r_pos.first - nx) + (r_pos.second - ny)*(r_pos.second - ny);
            if(tmp_dist < dist){ // 거리가 감소했을 때. 빈칸 or 루돌프 일 때만 작용. '가장 가까운'거리 탐색.
                if (board[nx][ny] ==9){ // 루돌프이면 바로 충돌
                    score[i] += d;
                    stuned[i] = k+1;
                    board[sr[i]][sc[i]] = 0;
                    InterAction(nx-dx[j]*d, ny-dy[j]*d, -dx[j], -dy[j], i);
                    break;
                }
                else if(board[nx][ny] == 0){ // 빈칸이면 이동 가능
                    dist = tmp_dist;
                    next_x = nx; 
                    next_y = ny;
                }
            }
        }

        if(next_x != 0 || next_y != 0){ // 이동 가능한 가까워지는 칸 존재
            board[sr[i]][sc[i]] = 0;
            sr[i] = next_x;
            sc[i] = next_y;
            board[sr[i]][sc[i]] = i;
        }
    }
}

void add_score(){
    for(int i = 1; i <= p; i++){
        if(fail[i]) continue; // 탈락 or 기절 턴
        score[i]++;
    }
}

void print(){
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    cin >> n >> m >> p >> c >> d;
    cnt_survive = p;
    int x, y;
    cin >> x >> y;
    r_pos = {x, y};
    board[x][y] = 9; // 루돌프

    for(int i = 1; i <= p; i++){
        int s_num;
        cin >> s_num >> x >> y;
        sr[s_num] = x;
        sc[s_num] = y;
        board[x][y] = s_num;
    }

    // cout << "초기 상태" << endl;
    // print();
    for(int i = 1; i <= m; i++){
        // cout << "===== i: " << i << " ======"<< endl;
        r_move(i);
        // cout << "  AFTER DEER MOVE" << endl;
        // print();
        if(cnt_survive == 0)break;
        s_move(i);
        // cout << "  AFTER SANTA MOVE" << endl;
        if(cnt_survive == 0)break;
        // print();
        add_score();
    }

    for(int i = 1; i <= p; i++){
        cout << score[i] << ' ';
    }


    return 0;
}

/*
루돌프는 8방향. / r이 큰 순, 같으면 c가 큰 순
산타는 4방향. / 상우하좌 순

m개의 턴 동안 진행
1. 루돌프 움직임 (가장 가까운 탈락하지 않은 산타) - 대각선도 1칸.
1-1. 가장 가까운 산타 탐색. -> r,c배열 순회하면서 값 구하기.
1-2. 충돌 확인 + 상호작용(밀림) + 탈락 확인(범위 확인)

2. 산타 번호 순대로움직임 (기절X, 탈락X / 둘 다 못 움직임)
2-1. 움직임 가능 여부 확인 (빈 칸 or 루돌프 칸으로만 이동 가능)
2-2. 루돌프에게 가까워지는 방향으로 이동 (가까워지는 방향 찾기) 
    - 가까워지는 방법X이면 움직임 X
2-3. 충돌 확인 + 상호작용(밀림)  + 탈락 확인(범위 확인)

3. 탈락X 산타 점수 1점 추가
*/