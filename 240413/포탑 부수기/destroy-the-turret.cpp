#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int n, m, k;
int dmg;
pair<int, int> board[11][11]; // {공격력, 마지막 공격 시기}
bool attacked[11][11];
pair<int, int> weak_pos;
pair<int, int> strong_pos;

int dx[] = { 0, 1, 0, -1 };
int dy[] = { 1, 0, -1, 0 };
int b_dx[] = { 1, 1, 1, 0, -1, -1, -1, 0 };
int b_dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

void input() {
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> board[i][j].first;
            board[i][j].second = 0;
        }
    }
}

void print_board() {
    cout << "==공격력==" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << board[i][j].first << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

bool CheckAble() {
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j].first > 0) cnt++;
        }
    }
    if (cnt < 2) return false;
    return true;
}

// 공격자 선정
void ChooseAttacker() {
    int point = 1000000;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j].first == 0) continue; // 부서진 포탑은 쓰면 안됨
            if (board[i][j].first < point) { // 공격력이 더 낮으면 갱신
                point = board[i][j].first;
                weak_pos = { i, j };
            }
            else if (board[i][j].first == point) { // 공격력 동일
                if (board[i][j].second > board[weak_pos.first][weak_pos.second].second) { // 마지막 공격이 더 최신이면 갱신
                    weak_pos = { i, j };
                }
                else if (board[i][j].second == board[weak_pos.first][weak_pos.second].second) { // 마지막 공격 시기 동일
                    if (weak_pos.first + weak_pos.second < i + j) { // 행+열 값이 더 크면 갱신
                        weak_pos = { i, j };
                    }
                    else if (weak_pos.first + weak_pos.second == i + j) { // 행+열 값 동일
                        if (weak_pos.second < j) weak_pos = { i, j }; // 열이 더 크면 갱신
                    }
                }
            }
        }
    }
    board[weak_pos.first][weak_pos.second].first += n + m;
    dmg = board[weak_pos.first][weak_pos.second].first;
    //cout << "ATTACKER: " << weak_pos.first << ' ' << weak_pos.second << endl;
}

// 피격자 선정
void ChooseBeAttacker() {
    int point = -1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j].first == 0) continue; // 부서진 포탑은 쓰면 안됨
            if (i == weak_pos.first && j == weak_pos.second) continue;
            if (board[i][j].first > point) { // 공격력이 더 높으면 갱신
                point = board[i][j].first;
                strong_pos = { i, j };
            }
            else if (board[i][j].first == point) { // 공격력 동일
                if (board[i][j].second < board[strong_pos.first][strong_pos.second].second) { // 마지막 공격이 더 오래 됐으면 갱신
                    strong_pos = { i, j };
                }
                else if (board[i][j].second == board[strong_pos.first][strong_pos.second].second) { // 마지막 공격 시기 동일
                    if (strong_pos.first + strong_pos.second > i + j) { // 행+열 값이 더 작으면 갱신
                        strong_pos = { i, j };
                    }
                    else if (strong_pos.first + strong_pos.second == i + j) { // 행+열 값 동일
                        if (strong_pos.second > j) strong_pos = { i, j }; // 열이 더 작으면 갱신
                    }
                }
            }
        }
    }
    //cout << "BE ATTACKER: " << strong_pos.first << ' ' << strong_pos.second << endl;

}

// 레이저 공격 시도. 성공시 true 반환.
bool laser(int round) {
    bool visited[11][11] = { {false} };
    pair<int, int> load[11][11];

    queue<pair<int, int>> q;
    q.push({ weak_pos.first, weak_pos.second });
    visited[weak_pos.first][weak_pos.second] = true;

    bool found = false;
    while (!q.empty() && !found) {
        int q_size = q.size();
        for (int x = 0; x < q_size; x++) {
            pair<int, int> pos = q.front();
            q.pop();
            for (int i = 0; i < 4; i++) {
                int nx = pos.first + dx[i];
                int ny = pos.second + dy[i];
                if (nx < 1) nx = n;
                else if (nx > n) nx = 1;
                if (ny < 1) ny = m;
                else if (ny > m) ny = 1;
                if (board[nx][ny].first == 0 || visited[nx][ny]) continue; // 부서진 포탑은 못 지나감
                load[nx][ny] = { pos.first, pos.second };
                visited[nx][ny] = true;
                if (nx == strong_pos.first && ny == strong_pos.second) { // 피격자 찾음
                    board[strong_pos.first][strong_pos.second].first -= dmg;
                    if (board[strong_pos.first][strong_pos.second].first < 0) board[strong_pos.first][strong_pos.second].first = 0;
                    attacked[strong_pos.first][strong_pos.second] = true;
                    found = true;
                    break;
                }
                q.push({ nx, ny });
            }
        }
    }

    if (!found) return false; // 레이저 공격 불가능

    // cout << "==LOAD==" << endl;
    // for(int i = 1; i <= n; i++){
    //     for(int j = 1; j <= n; j++){
    //         cout << "{" << load[i][j].first << ", " << load[i][j].second << "} ";
    //     }
    //     cout << endl;
    // }

    // load에 저장된 좌표를 보면서 weak_pos까지 따라가기 + 데미지
    int r = load[strong_pos.first][strong_pos.second].first;
    int c = load[strong_pos.first][strong_pos.second].second;
    while (!(r == weak_pos.first && c == weak_pos.second)) {
        board[r][c].first -= (dmg / 2);
        attacked[r][c] = true;
        if (board[r][c].first < 0) board[r][c].first = 0;
        int nr = load[r][c].first;
        int nc = load[r][c].second;
        r = nr;
        c = nc;
    }
    return true;
}

// 포탄 공격
void bomb(int round) {
    board[strong_pos.first][strong_pos.second].first -= dmg;
    //cout << "BOMB CENTER: " << strong_pos.first << ' ' << strong_pos.second << endl;
    attacked[strong_pos.first][strong_pos.second] = true;
    if (board[strong_pos.first][strong_pos.second].first < 0) board[strong_pos.first][strong_pos.second].first = 0;
    for (int i = 0; i < 8; i++) {
        int nx = strong_pos.first + b_dx[i];
        int ny = strong_pos.second + b_dy[i];
        if (nx < 1) nx = n;
        else if (nx > n) nx = 1;
        if (ny < 1) ny = m;
        else if (ny > m) ny = 1;
        //cout << "BOMD: " << nx << ' ' << ny << endl;
        if (board[nx][ny].first == 0) continue; // 공격력 0이면 피격X
        if (nx == weak_pos.first && ny == weak_pos.second) continue; // 공격자는 피격X
        board[nx][ny].first -= (dmg / 2);
        attacked[nx][ny] = true;
        if (board[nx][ny].first < 0) board[nx][ny].first = 0;
    }
}

// 공격
void Attack(int round) {
    if (!laser(round)) {
        //cout << "BOMB!!!" << endl;
        bomb(round);
    }
    board[weak_pos.first][weak_pos.second].second = round;
}

// 회복 - 공.피격시 X, 체력0 X
void Heal() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j].first == 0 || attacked[i][j] || (i == weak_pos.first && j == weak_pos.second)) continue; // 공격력0, 공.피격자는 회복X
            board[i][j].first++;
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            attacked[i][j] = false;
        }
    }
}

bool TryAttack(int round) {
    if (!CheckAble()) return false;
    ChooseAttacker();
    ChooseBeAttacker();
    Attack(round);
    Heal();
    return true;
}

void answer() {
    int powerful = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j].first > powerful) {
                powerful = board[i][j].first;
            }
        }
    }
    cout << powerful;
}

int main() {
    input();
    /*cout << "초기 모습" << endl;
    print_board();*/
    for (int i = 1; i <= k; i++) {
        //cout << i << " 번째" << endl;
        if (!TryAttack(i)) break;
        //print_board();
    }
    answer();
    return 0;
}
/*
부서지지 않은 포탑이 하나가 될 때까지 or K번 반복

필요한 변수: 체력, 공격한 마지막 시간, (공격자 위치, 피격자 위치), 공격 or 피격 여부(매 라운드 갱신)

pair<int, int> board에 체력, 공격한 마지막 시간 저장
bool unatk_board에 공.피격 여부 저장 - 공격 함수 내부에 선언.

1. 공격자 선정
- 공격력이 가장 낮은 포탑 -> 가장 최근에 공격한 포탑 -> 행+열이 가장 큰 포탑 -> 열 값이 가장 큰 포탑
좌상단부터 배열 순회하면서 공격력 값을 비교해서 작거나 같으면, 마지막 공격 시기 비교, 행+열 값 비교, 같으면 열 값 비교.
M+N만큼 공격력 상승

2. 공격
레이저는 최단거리 = BFS but 격자가 양쪽이 연결됨
포탄은 공격 대상 + 주변 8개 격자가 양쪽이 연결

2-0. 피격자 선정
- 공격력이 가장 높은 포탑 -> 공격한지 가장 오래된 포탑 -> 행+열이 가장 작은 포탑 -> 열이 가장 작은 포탑
좌상단부터 배열 순회하면서 공격력 값을 비교해서 크면, 마지막 공격 시기 비교, 행+열 값 비교, 같으면 열 값 비교.

선정이 완료되면
2-1. 레이저 공격 시도
    BFS로 탐색. 도달 가능여부 확인. 격자 연결된 것 주의.
    부서진 포탑은 못 지나감.
    도달 가능 시 레이저로 공격.
    경로는 어떻게?
    BFS로 visited에 기록하면서 탐색.
    bfs를 순서대로 탐색하면서 나아가면 되려나?
    우/하/좌/상 순으로 움직임

2-2. 포탄 공격
    피격 대상 + 주변 8칸에 데미지. 격자 연결된 것 주의. 공격자는 영향 안받는 것 주의.
    피격 대상은 공격력만큼, 주변에는 공격력/2만큼 데미지.

3. 부서짐
- 체력 0이하면 부서짐 판정.
부서진 판정을 굳이 넣지 안아도 될듯.

4. 정비
- 공격X, 피격X인 포탑 공격력 1 상승.
공격 또는 피격 대상임을 저장할 변수 필요.
*/