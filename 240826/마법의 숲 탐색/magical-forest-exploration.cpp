#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int R, C, K, ans = 0;
int board[76][76]; //1~R, 1~C
bool exits[76][76];
vector<pair<int, int>> gol; // 골렘의 출발 열, 출구 방향 정보

// 0: 북 / 1: 동 / 2: 남 / 3: 서
int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

void input() {
	cin >> R >> C >> K;

	int a, b;
	for (int i = 0; i < K; i++) {
		cin >> a >> b;
		gol.push_back({ a, b });
	}
}

// 맵 초기화
void clearBoard() {
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			board[i][j] = 0;
			exits[i][j] = false;
		}
	}
}

void move(int center_x, int center_y) { // 정령 x, y에서 바닥까지 이동
	bool visited[76][76] = { false };
	int depth = center_x;
	queue<pair<int, int>> q;
	q.push({ center_x, center_y });
	visited[center_x][center_y] = true;
	while (!q.empty()) {
		int x = q.front().first;
		int y = q.front().second;
		int now = board[x][y];
		if (x > depth) depth = x;
		q.pop();
		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (nx < 1 || ny < 1 || nx > R || ny > C || board[nx][ny] == 0 || visited[nx][ny]) continue;
			if (exits[x][y] == true) { // 현재 위치가 출구이면
				q.push({ nx, ny });
				visited[nx][ny] = true;
			}
			else if (board[nx][ny] == now) {
				q.push({ nx, ny });
				visited[nx][ny] = true;
			}
		}
	}
	ans += depth;
}

void move_gol(int num) {
	int y = gol[num].first;
	int dir = gol[num].second; // 0,1,2,3은 북, 동, 남, 서쪽

	num++;
	int depth = 0;
	while (1) { // 더이상 움직일 수 없을 때까지 이동
		for (depth; depth < R - 1; ++depth) {
			if (board[depth + 2][y] > 0) { // 아래칸 블럭 아래에 블럭 있음.
				break;
			}
			if (board[depth + 1][y - 1] > 0 || board[depth + 1][y + 1] > 0) { // 좌or우 블럭 아래에 블럭 있음.
				break;
			}
			
		}
		if (depth == R - 1) { // 가장 아래까지 도달
			board[depth][y] = num;
			for (int i = 0; i < 4; i++) {
				board[depth+dx[i]][y+dy[i]] = num;
			}
			exits[depth + dx[dir]][y + dy[dir]] = true;
			ans += R;
			break;
		}
		
		bool flag = false;
		// 왼쪽 아래로 갈 수 있음
		if (y-2 > 0 && depth+2 <= R && board[depth][y - 2] == 0 && board[depth - 1][y - 1] == 0 && board[depth + 1][y - 1] == 0 && board[depth+2][y-1] == 0 && board[depth+1][y-2] == 0 && board[depth+1][y] == 0) {
			y--;
			dir = (dir - 1 + 4) % 4;
			flag = true;
		}
		// 오른쪽 아래로 갈 수 있음
		else if (y+2 <= C && depth + 2 <= R && board[depth][y + 2] == 0 && board[depth - 1][y + 1] == 0 && board[depth + 1][y + 1] == 0 && board[depth + 2][y + 1] == 0 && board[depth + 1][y] == 0 && board[depth + 1][y+2] == 0) {
			y++;
			dir = (dir + 1) % 4;
			flag = true;
		}
		
		// 아래로 내려갈 수 있는지 확인
		// 못 내려가면 범위 확인 & 범위 내부면 기록(골렘 + 출구)
		// 내려갈 수 있으면 while문 진행(아래로 이동)

		// 추가 이동 불가능
		if (!flag){
			// 범위 밖
			if (depth <= 1) { 
				clearBoard();
				return;
			}
			// 범위 내부. 골렘+출구 기록.
			board[depth][y] = num;
			for (int i = 0; i < 4; i++) {
				board[depth + dx[i]][y + dy[i]] = num;
			}
			exits[depth + dx[dir]][y + dy[dir]] = true;
			// 정령 이동
			move(depth, y);
			return;
		}
	}
}

void solve() {
	// 골램 모두 이동
	for (int gol_num = 0; gol_num < gol.size(); gol_num++) {
		move_gol(gol_num);
	}

	cout << ans;
	return;
}

int main() {
	input();
	solve();

	return 0;
}

/*
십자 모양으로 내려와야함.
내려올 수 있는지 확인할 좌표: 중앙 = (x, y) 일 때, (x+1, y), (x, y-1), (x, y+1)

골렘 배열을 순회하며 모든 골렘이 이동하도록 함

더이상 내려갈 수 없을 때, 출구 방향에 해당하는 칸에서 다른 골렘으로 넘어갈 수 있음을 기록.
맵에 각 골렘마다 인덱스를 기록.
출구칸은 다른 배열을 사용해서 기록해도 좋을듯. (출구 칸에서는 인접한 모든 칸 이동 가능)

출구칸은 무조건 현재 타고 있는 골렘의 출구여야함.
-> 출구 칸을 표시할 자료 구조 필요할듯

bfs로 아래로 내려감.

이동 순서
1. 남 -> 출구 유지
2. 서+남 -> 출구 반 시계
3. 동+남 -> 출구 시계

2,3번의 움직임의 경우 서or동으로 '한 칸'만 이동후 남쪽으로 이동.
둘 다 불가능 하면 가만히 있어야함.

골렘이 내려갈 수 있는 칸이 없으면 배열 초기화 후 다음 골렘부터 이동

내려갈 수 없는지 어떻게 판단?
(x+1, y) -> 3보다 작으면 못 내려감
(x, y-1), (x, y+1) -> 2보다 작으면 못 내려감
*/