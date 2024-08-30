#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;

int n, m, p, santapower, roopower;

int rooi, rooj;

bool santaongame[30];

int arr[50][50];

int santadx[4] = { 0,1,0,-1 };
int santady[4] = { -1,0,1,0 };

int roodx[8] = { 0,1,0,-1,1,1,-1,-1 };
int roody[8] = { -1,0,1,0,1,-1,1,-1 };

int santascore[30] = { 0, };
int santaloci[30];
int santalocj[30];
int santastan[30];

int caldist(int i, int j, int ii, int jj) {
	return (i - ii)*(i - ii) + (j - jj)*(j - jj);
}

void santadfs(int i, int j, int dir, int befindex) {
	if (i >= n || j >= n || i < 0 || j < 0) {
		santaongame[befindex] = false;
		return;
	}
	if (arr[i][j] != -1) {
		int nx = j + roodx[dir];
		int ny = i + roody[dir];
		santaloci[befindex] = ny;
		santalocj[befindex] = nx;
		if (nx >= n || ny >= n || nx < 0 || ny < 0) {
			santaongame[arr[i][j]] = false;
			arr[i][j] = befindex;
			return;
		}
		santadfs(ny, nx, dir, arr[i][j]);
		arr[i][j] = befindex;
	}
	else {
		santaloci[befindex] = i;
		santalocj[befindex] = j;
		arr[i][j] = befindex;
		return;
	}
}

int main() {
	cin >> n >> m >> p >> roopower >> santapower;

	cin >> rooi >> rooj;
	rooi--;
	rooj--;
	for (int i = 0; i < n; i++) {
		fill(arr[i], arr[i] + n, -1);
	}
	for (int temp = 0; temp < p; temp++) {
		int i, j, num;
		cin >> num >> i >> j;
		santaloci[num - 1] = i - 1;
		santalocj[num - 1] = j - 1;
		santascore[num - 1] = 0;
		arr[i - 1][j - 1] = num - 1;
		santaongame[num - 1] = true;
	}
	for (int round = 1; round <= m; round++) {

		int mindist = 1000000000;
		int santaindex = -1;
		//루돌프가 달려갈 산타 선택
		for (int i = n - 1; i >= 0; --i) {
			for (int j = n - 1; j >= 0; --j) {
				if (arr[i][j] != -1) {
					int temp = caldist(i, j, rooi, rooj);
					if (mindist > temp) {
						mindist = temp;
						santaindex = arr[i][j];
					}
				}
			}
		}

		// 선택된 산타로 가는 방향 선택
		mindist = 1000000000;
		int dirindex = -1;
		for (int dir = 0; dir < 8; dir++) {
			int nx = rooj + roodx[dir];
			int ny = rooi + roody[dir];
			if (nx >= n || ny >= n || nx < 0 || ny < 0) continue;
			int temp = caldist(ny, nx, santaloci[santaindex], santalocj[santaindex]);
			if (temp < mindist) {
				mindist = temp;
				dirindex = dir;
			}
		}

		//루돌프 이동
		rooj = rooj + roodx[dirindex];
		rooi = rooi + roody[dirindex];
		//루돌프가 이동한 곳에 산타가 있다면?
		if (arr[rooi][rooj] != -1) {
			santascore[arr[rooi][rooj]] += roopower;
			santalocj[arr[rooi][rooj]] += roopower * roodx[dirindex];
			santaloci[arr[rooi][rooj]] += roopower * roody[dirindex];
			santastan[arr[rooi][rooj]] = round + 1;
			//튕겨나간 산타 탈락
			if (santalocj[arr[rooi][rooj]] >= n || santaloci[arr[rooi][rooj]] >= n || santaloci[arr[rooi][rooj]] < 0 || santalocj[arr[rooi][rooj]] < 0) {
				santaongame[arr[rooi][rooj]] = false;
			}
			else {
				//연쇄 반응
				santadfs(santaloci[arr[rooi][rooj]], santalocj[arr[rooi][rooj]], dirindex, arr[rooi][rooj]);
			}
			arr[rooi][rooj] = -1;
		}
		
		////산타이동
		mindist = 1000000000;
		for (int temp = 0; temp < p; temp++) {
			if (santastan[temp] >= round || santaongame[temp] == false) continue; // 기절 or 탈락
			dirindex = -1;
			mindist = caldist(santaloci[temp], santalocj[temp], rooi, rooj); // 현재 거리
			for (int dir = 0; dir < 4; dir++) {
				int ny = santaloci[temp] + santady[dir];
				int nx = santalocj[temp] + santadx[dir];
				if (nx >= n || ny >= n || nx < 0 || ny < 0) continue; //배열 넘어감
				if (arr[ny][nx] != -1) continue; //해당 칸에 산타가 있음
				int tempdist = caldist(ny, nx, rooi, rooj);
				if (tempdist < mindist) {
					mindist = tempdist;
					dirindex = dir;
				}
			}

			if (dirindex == -1) continue; // 움직일 수 없음
			
			int ny = santaloci[temp] + santady[dirindex];
			int nx = santalocj[temp] + santadx[dirindex];

			if (ny == rooi && nx == rooj) { // 산타가 루돌프 있는 칸으로 감
				// 방향 전환
				int de = 1;
				if (dirindex < 2) {
					dirindex += 2;
				}
				else {
					dirindex -= 2;
				}
				
				// 튕겨나갈 칸
				ny += santapower * santady[dirindex];
				nx += santapower * santadx[dirindex];

				santascore[temp] += santapower;
				arr[santaloci[temp]][santalocj[temp]] = -1;
				santastan[temp] = round + 1;
				santadfs(ny, nx, dirindex, temp);
				santaloci[temp] = ny;
				santalocj[temp] = nx;
			}
			else { // 빈 칸
				arr[ny][nx] = temp;
				arr[santaloci[temp]][santalocj[temp]] = -1;
				santaloci[temp] = ny;
				santalocj[temp] = nx;
			}
			
		}

		for (int temp = 0; temp < p; temp++) {
			if (santaongame[temp] == true) santascore[temp]++;
		}
		
	}
	for (int temp = 0; temp < p; temp++) {
		cout << santascore[temp] << " ";
	}
}