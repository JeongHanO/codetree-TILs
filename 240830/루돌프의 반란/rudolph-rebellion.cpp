#include <iostream>
using namespace std;

int N, M, P, C, D, survivedSanta;
pair<int, int> Rpos; // 루돌프 위치
pair<int, int> Spos[30]; // 산타 위치
int status[30]; // -1: 탈락, 1~m: n턴에 기절
int score[30]; // 산타 별 획득 점수
int board[51][51]; // 0: 빈칸, 1~P: 산타, -1: 루돌프

int rdx[] = { 1, 1, 1, 0, 0, -1, -1, -1 };
int rdy[] = { 1, 0, -1, 1, -1, 1, 0, -1 };

// 상우하좌
int sdx[] = { -1, 0, 1, 0 };
int sdy[] = { 0, 1, 0, -1 };

int calcDist(pair<int, int> santa, pair<int, int> ru) { // 산타 - 루돌프 거리
	return ((ru.first - santa.first) * (ru.first - santa.first) +
		(ru.second - santa.second) * (ru.second - santa.second));
}

void input() {
	cin >> N >> M >> P >> C >> D;
	
	survivedSanta = P;
	int x, y;
	cin >> x >> y;
	Rpos = { x, y };
	board[x][y] = -1;

	int n;
	for (int i = 1; i <= P; i++) {
		cin >> n >> x >> y;
		Spos[n] = { x, y };
		board[x][y] = n;
		status[i] = -10000;
	}
}

// 범위 밖이면 false, 범위 안이면 true.
bool checkRange(pair<int,int> pos) {
	if (pos.first < 1 || pos.second < 1 || pos.first > N || pos.second > N) return false;
	return true;
}

// pos에서 dir 방향으로 연쇄 반응
void chained(pair<int, int> pos, pair<int, int> dir, int santaNum) {
	if (board[pos.first][pos.second] > 0) { // 이동한 자리에 산타 있음 -> 연쇄 작용
		// 1. 새롭게 밀릴 산타(기존에 있던 산타) 정보 저장
		int nSantaNum = board[pos.first][pos.second]; // 연쇄로 밀릴 산타 번호
		pair<int, int> npos = { pos.first + dir.first, pos.second + dir.second }; // 연쇄로 밀릴 산타가 갈 곳
		
		// 2. 밀려온 산타 정보 갱신
		board[pos.first][pos.second] = santaNum;
		Spos[santaNum] = pos;

		if (!checkRange(npos)) { // 새롭게 밀릴 산타가 밖으로 나감 -> 연쇄작용 끝
			status[nSantaNum] = -1; // 탈락
			survivedSanta--;
			return;
		}
		
		// 3. 새롭게 밀릴 산타 정보로 재귀
		chained(npos, dir, nSantaNum);
	}
	else { // 빈 칸임 -> 연쇄작용 끝
		board[pos.first][pos.second] = santaNum;
		Spos[santaNum] = pos;
		return;
	}
}

// santaNum에 해당하는 산타가 dir방향으로 C or D만큼 날라감
void crash(int santaNum, pair<int, int> dir, bool isRu) {
	pair<int, int> fliedPos;
	if (isRu) { // 루돌프 -> 산타
		// C만큼 dir로 날라가기
		fliedPos = { Spos[santaNum].first + (dir.first*C), Spos[santaNum].second + (dir.second*C) };
		}
	else { // 산타 -> 루돌프
		// D만큼 dir로 날라가기
		fliedPos = { Rpos.first + (dir.first*D), Rpos.second + (dir.second*D) };
	}

	// 날아간 곳이 게임판 밖
	if (fliedPos.first < 1 || fliedPos.second < 1 || fliedPos.first > N || fliedPos.second > N) {
		status[santaNum] = -1; // 탈락
		survivedSanta--;
		return;
	}

	// "날아간 칸"에서 산타 연쇄 반응 확인
	chained(fliedPos, dir, santaNum);
}

// 루돌프 이동
void moveR(int t) {
	pair<int, int> target = { -1, -1 };
	int dist = 1e8;

	// 목표 산타 설정
	for (int i = 1; i <= P; i++) {
		if (status[i] == -1) continue; // 탈락한 산타는 표적 X
		int tmp_dist = calcDist(Spos[i], Rpos);
		if (tmp_dist < dist) { // 가장 가까운 산타 우선
			dist = tmp_dist;
			target = Spos[i];
		}
		else if (tmp_dist == dist) { // 거리가 동일한 산타
			if (target.first < Spos[i].first) { // r이 큰 산타
				target = Spos[i];
			}
			else if (target.first == Spos[i].first) {
				if (target.second < Spos[i].second) { // c가 큰 산타
					target = Spos[i];
				}
			}
		}
	}

	// 이동(산타와 가까워지는 방향 -> 8방향에 대해 가까워 지는 방향 탐색)
	dist = 1e8;
	int dir = 0;
	pair<int, int> closestPos;
	for (int i = 0; i < 8; i++) {
		pair<int, int> tmpPos = { Rpos.first + rdx[i], Rpos.second + rdy[i] };
		int tmp_dist = calcDist(target, tmpPos);
		if (dist > tmp_dist) {
			dist = tmp_dist;
			closestPos = tmpPos;
			dir = i;
		}
	}

	// 충돌 확인 & 충돌(점수 획득). 위치 이동 board에 기록할 것
	if (board[closestPos.first][closestPos.second] > 0) { // 이동한 자리에 산타가 있음(충돌)
		int santaNum = board[closestPos.first][closestPos.second];
		score[santaNum] += C; // 점수 계산
		status[santaNum] = t; // 산타 기절 기록
		crash(santaNum, {rdx[dir], rdy[dir]}, true); // 충돌 & 연쇄 반응
	}

	// 루돌프 이동
	board[Rpos.first][Rpos.second] = 0;
	board[closestPos.first][closestPos.second] = -1;
	Rpos = closestPos;
}

// 산타 이동
void moveS(int t) {
	for (int sanNum = 1; sanNum <= P; sanNum++) {
		if (status[sanNum] == -1) continue; // 탈락
		if (status[sanNum]+1 >= t) continue; // 기절 (기절한 턴+1이 현재 턴보다 크거나 같음)
		
		// 루돌프와 가까워 지는 방향 있는지 확인 & 이동(board랑 sPos 갱신)
		pair<int, int> nPos = { -1, -1 };
		int dir = 0;
		int nowDist = calcDist(Spos[sanNum], Rpos); // 현재 위치에서 루돌프와의 거리
		for (int i = 0; i < 4; i++) {
			pair<int, int> tmpPos = { Spos[sanNum].first + sdx[i], Spos[sanNum].second + sdy[i] };
			if (!checkRange(tmpPos)) continue; // 범위 밖으로 이동 X
			if (board[tmpPos.first][tmpPos.second] > 0) continue; // 이미 산타가 있는 칸이면 이동 X
			int tmpDist = calcDist(tmpPos, Rpos);
			if (nowDist > tmpDist) { // 탐색한 칸이 가까운 칸
				nowDist = tmpDist;
				nPos = tmpPos;
				dir = i;
			}
		}

		if (nPos.first != -1 && nPos.second != -1) { // 가까워지는 자리로 이동 가능
			board[Spos[sanNum].first][Spos[sanNum].second] = 0;
			// 충돌 확인 & 충돌(점수 획득)
			if (board[nPos.first][nPos.second] == -1) { // 이동한 칸에 루돌프 있음
				score[sanNum] += D;
				status[sanNum] = t; // 산타 기절 기록
				crash(sanNum, {-sdx[dir], -sdy[dir]}, false);
			}
			else {
				board[nPos.first][nPos.second] = sanNum;
				Spos[sanNum] = nPos;
			}
		}
	}
}

void increaseScore() {
	for (int i = 1; i <= P; i++) {
		if (status[i] != -1) { // 탈락한 산타 빼고(생존한 산타)
			score[i]++; // 1점 증가
		}
	}
}

void print() {
	for (int i = 1; i <= P; i++) {
		cout << score[i] << ' ';
	}
}

void printB() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cout << board[i][j] << ' ';
		}
		cout << endl;
	}
}

void solve() {
	for(int i = 1; i <= M; i++) {
		if (survivedSanta == 0) break; // 남은 산타가 없으면 종료
		moveR(i);
		moveS(i);
		increaseScore();
	}
}

int main() {
	input();
	solve();
	print();
}