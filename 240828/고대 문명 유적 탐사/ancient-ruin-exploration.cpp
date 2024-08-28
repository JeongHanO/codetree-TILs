#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

int K, M;
int board[5][5];
queue<int> relicQ;

int dx[] = { 1, -1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };

void input() {
	cin >> K >> M;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> board[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int a; cin >> a;
		relicQ.push(a);
	}
}

int cntRelic() {
	int cntMax = 0;
	bool visited[5][5] = { false };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (!visited[i][j]) {
				int cnt = 1;
				visited[i][j] = true;
				queue<pair<int, int>> q;
				q.push({ i, j });
				while (!q.empty()) {
					int x = q.front().first;
					int y = q.front().second;
					q.pop();
					for (int k = 0; k < 4; k++) {
						int nx = x + dx[k];
						int ny = y + dy[k];
						if (nx < 0 || ny < 0 || nx == 5 || ny == 5 || visited[nx][ny] || board[nx][ny] != board[i][j]) continue;
						visited[nx][ny] = true;
						cnt++;
						q.push({ nx, ny });
					}
				}
				cntMax = max(cntMax, cnt);
			}
		}
	}
	if (cntMax < 3) return 0;
	return cntMax;
}

void turn(int x, int y) {
	// x, y를 중심으로 3*3배열 시계 방향으로 90도 회전
	int tmp_board[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp_board[j][2-i] = board[x+i-1][y+j-1];
		}
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[x + i - 1][y + j - 1] = tmp_board[i][j];
		}
	}
}

void turnBoard() {
	// 회전 안하는 경우는 없음. 시계방향 90 180 270 중 하나
	// 1. 유물 획득 최대화 2. 회전 각도가 가장 작음 3. 열이 작음, 행이 작음
	int r, c, degree = -1, maxRelic = 0;
	
	// 유물을 최대로 획득 가능한 경우 탐색
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			int tmp_board[3][3];
			// 회전 배열 저장
			for (int x = 0; x <= 2 ; x++) {
				for(int y = 0; y <= 2; y++) {
					tmp_board[x][y] = board[i + x - 1][j + y - 1];
				}
			}
			// 90도 회전을 세번. 한 번 회전할 때마다 최대 유물 수 확인.
			for (int deg = 0; deg < 3; deg++) {
				turn(i, j); // 90도 회전
				int tmpCnt = cntRelic();
				if (tmpCnt == 0) continue;
				if (tmpCnt > maxRelic) {
					r = i;
					c = j;
					degree = deg;
					maxRelic = tmpCnt;
				}
				else if (tmpCnt == maxRelic) {
					if (degree > deg) {
						r = i;
						c = j;
						degree = deg;
					}
				}
			}
			// 회전 복원
			for (int x = 0; x <= 2; x++) {
				for (int y = 0; y <= 2; y++) {
					board[i+x-1][j+y-1] = tmp_board[x][y];
				}
			}

			int asdf = 1;
		}
	}

	int de = 1;
	for (int i = 0; i <= degree; i++) {
		turn(r, c);
	}
}

int getRelic() {
	int cntGet = 0;
	
	bool visited[5][5] = { false };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (visited[i][j]) continue;
			visited[i][j] = true;
			int relicNum = board[i][j];
			int tmp_cnt = 1;
			queue<pair<int, int>> q;
			queue<pair<int, int>> dList;
			q.push({ i,j });
			dList.push({ i, j });
			while (!q.empty()) {
				int x = q.front().first;
				int y = q.front().second;
				q.pop();
				for (int k = 0; k < 4; k++) {
					int nx = x + dx[k];
					int ny = y + dy[k];
					if (nx < 0 || ny < 0 || nx == 5 || ny == 5 || visited[nx][ny] || board[nx][ny] != relicNum) continue;
					visited[nx][ny] = true;
					tmp_cnt++;
					q.push({ nx, ny });
					dList.push({ nx, ny });
				}
			}
			if (tmp_cnt >= 3) { 
				cntGet += tmp_cnt;
				int size = dList.size();
				while (size--) {
					board[dList.front().first][dList.front().second] = 0;
					dList.pop();
				}
			}
		}
	}
	
	return cntGet;
}

void insertRelic() {
	for (int j = 0; j < 5; j++) {
		for (int i = 4; i >= 0; i--) {
			if (board[i][j] == 0) {
				board[i][j] = relicQ.front();
				relicQ.pop();
			}
		}
	}

	int de = 1;
}

void solve() {
	for (int i = 0; i < K; i++) {
		int sum = 0;
		turnBoard();
		int num = getRelic();
		if (num == 0) return; // 수확할 수 있는 방법 없음
		while (num != 0) { // 연쇄 획득 끝날 때까지
			sum += num;
			insertRelic();
			num = getRelic();
		}
		cout << sum << ' ';
	}
}

int main() {
	input();
	solve();
}