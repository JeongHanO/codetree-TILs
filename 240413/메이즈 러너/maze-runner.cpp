#include <iostream>
#include <vector>
#include <map>
using namespace std;

int n, m, k;
int board[11][11]; // 미로 정보
int rotate_board[11][11]; // 회전용 미로 정보
int r[11], c[11]; // 참가자 위치
bool escaped[11]; // 탈출 여부
pair<int, int> ext; // 출구 위치
int cnt_move_dist = 0; // 움직인 거리
int cnt_runner; // 참가자 수
int rect_r, rect_c, rect_size; // 사각형의 좌상단 좌표, 사각형 크기

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void input(){
    cin >> n >> m >> k;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cin >> board[i][j];
        }
    }
    for(int i = 1; i <= m; i++){
        cin >> r[i] >> c[i];
    }
    int a, b; cin >> a >> b;
    ext = {a, b};
    cnt_runner = m;
}

void print_board(){
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

void print_runner(){
    for(int i = 1; i <= m; i++){
        if(!escaped[i]) cout << i << "번 : (" <<  r[i] << ", " << c[i] << ")";
        else cout << i << "번 : 탈출";
        cout << endl;
    }
}

// 참가자 이동 함수. 이동 가능한 인원이 없으면 false.
/*
모든 참가자를 순회하면서 이동 시도.
최단거리 방향으로, 상하 우선.
최단 거리 계산이 단순 뺄셈이기 때문에, 어디로 움직이던 1만큼 감소함. -> 그냥 상하 좌우 순으로 탐색하면 됨
탈출시: 참가자 수 감소, 탈출 여부 갱신
*/
void TryMovement(){
    for(int i = 1; i <= m; i++){
        if(escaped[i]) continue; // 탈출한 참가자는 패스
        int cur_dist = abs(ext.first - r[i])+ abs(ext.second - c[i]);
        int tmp_dist;
        //이동 방향 결정
        //cout << i << " 번째 runner (" << r[i] << ", " << c[i] << ")" << endl;
        for(int x = 0; x < 4; x++){
            int nr = r[i] + dx[x];
            int nc = c[i] + dy[x];
            if(nr < 1 || nc < 1 || nr > n || nc > n || board[nr][nc] > 0) continue; // 범위 밖 or 벽
            tmp_dist = abs(ext.first - nr)+ abs(ext.second - nc);
            if(tmp_dist < cur_dist){ // 이동 방향 찾았으면 이동
                cur_dist = tmp_dist;
                r[i] = nr;
                c[i] = nc;
                cnt_move_dist++;
                break;
            }
        }
        // 만약 이동했는데 출구이면
        if(r[i] == ext.first && c[i] == ext.second){
            escaped[i] = true;
            cnt_runner--;
        }
    }
}

// 회전할 사각형 탐색
void SearchRect(){
    int cur_size = 1000;
    int tmp_size;
    for(int i = 1; i <= m; i++){
        if(escaped[i]) continue;
        tmp_size = max(abs(ext.first-r[i]), abs(ext.second-c[i]));
        if(tmp_size < cur_size) cur_size = tmp_size;
    }
    rect_size = cur_size + 1; // 사각형의 크기는 거리보다 한 칸 크기 때문에 +1

    bool is_in = false;
    for(int i = ext.first - cur_size; i <= ext.first && !is_in; i++){
        for(int j = ext.second - cur_size; j <= ext.second && !is_in; j++){
            if(i < 1 || j < 1) continue;
            for(int x = 1; x <= m; x++){
                if(escaped[x]) continue;
                if(r[x] >= i && r[x] < i+rect_size && c[x] >= j && c[x] < j+rect_size){
                    is_in = true;
                    rect_r = i;
                    rect_c = j;
                    break;
                }
            }
        }
    }
}

// 회전시 탈출한 참가자는 패스
/*
시계방향으로 90도 회전, 내부의 벽 내구도 1씩 감소.
- 회전시 출구과 참가자도 같이 회전.
*/
void Rotate(){
    // 벽 내구도 감소
    for(int i = rect_r; i < rect_r+rect_size; i++){
        for(int j = rect_c; j < rect_c+rect_size; j++){
            if(board[i][j] > 0) board[i][j]--;
        }
    }

    map<pair<int, int>, pair<int, int>> rot_m;
    // 벽 회전
    for(int i = 0; i < rect_size; i++){
        for(int j = 0; j < rect_size; j++){
            rotate_board[i][j] = board[rect_r+rect_size-j-1][rect_c+i];
            rot_m[{rect_r+rect_size-j-1, rect_c+i}] = {rect_r+i, rect_c+j};
        }
    }

    // 참가자 회전
    for(int i = 1; i <= m; i++){
        pair<int, int> new_pos;
        if(rot_m.find({r[i],c[i]}) == rot_m.end()) continue;
        new_pos = rot_m[{r[i],c[i]}];
        r[i] = new_pos.first;
        c[i] = new_pos.second;
    }

    ext = rot_m[ext];

    for(int i = 0; i < rect_size; i++){
        for(int j = 0; j < rect_size; j++){
            board[rect_r+i][rect_c+j] = rotate_board[i][j];
        }
    }


}

void TryRotate(){
    SearchRect();
    Rotate();
}

int main() {
    input();
    for(int i = 1; i <= k; i++){
        TryMovement();
        if(cnt_runner == 0) break;
        TryRotate();
    }
    cout << cnt_move_dist << "\n" << ext.first << ' ' << ext.second << endl;
    return 0;
}
/*
최단거리는 ∣x1−x2∣+∣y1−y2∣로 정의
모든 참가자 동시 이동 -> 순서 상관X
칸 단위로 이동X -> 중복되어 움직일 수 있음
움직인 참가자는 어쨋든 표시가 되어야함. 그렇기 때문에 참가자 번호를 이용해서 움직이면 편함.

K초 동안 반복 or 참가자 모두 탈출시 종료
모든 참가자들의 이동 거리 합과 출구 좌표 출력

변수
int board[11][11] 미로 정보(벽 체력)
pair<int, int> ext 출구 위치
int r[11], int c[11] 참가자 위치
int cnt_runner 참가자의 수 카운트

함수
input()
TryMovement() // 참가자 이동
TryRotate() // 미로 회전 준비
    SearchRect() // 최소 정사각형 크기 + 좌상단 위치 탐색
    Rotate() // 실제 회전

1. 참가자 이동
상하좌우로 이동. (상하 우선)
벽이 없어야함
움직일 칸은 현재 칸보다 출구까지의 최단 거리가 가까워야함
못 움직이면 가만히 있음

2. 미로 회전
한 명 이상의 참가자와 출구를 포함하는 가장 작은 정사각형 탐색
- 출구와 참가자가 항상 정사각형의 모서리에 있는 건 아님
같은 크기의 정사각형이 존재 가능할 때, 좌상단 r의 좌표가 작은 것이 우선, r도 같으면 c가 작은 게 우선
시계방향으로 90도 회전, 내부의 벽 내구도 1씩 감소.
- 회전시 출구과 참가자도 같이 회전.
*/