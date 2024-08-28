#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
using namespace std;
int k, m;

int arr[5][5];
int subcheck[5][5];
int copy_arr[5][5];
vector<int> photo;

int dx[4] = { 0,0,1,-1 };
int dy[4] = { 1,-1,0,0 };

int photoindex = 0;

int remove(int i, int j, int target) {
	queue<pair<int, int>> q;
	q.push({ i,j });
	int cnt = 1;
	while (!q.empty()) {
		auto temp = q.front();
		int y = temp.first;
		int x = temp.second;
		q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			if (nx >= 5 || ny >= 5 || ny < 0 || nx < 0) continue;
			if (copy_arr[ny][nx] == target && subcheck[ny][nx] == 0) {
				subcheck[ny][nx] = 1;
				q.push({ ny,nx });
				cnt++;
			}
		}
	}

	if (cnt >= 3) {
		return cnt;
	}
	else {
		return 0;
	}

}

void realremove(int i, int j, int target) {

	queue<pair<int, int>> q;
	q.push({ i,j });
	subcheck[i][j] = 1;
	int cnt = 1;
	while (!q.empty()) {
		auto temp = q.front();
		int y = temp.first;
		int x = temp.second;
		q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			if (nx >= 5 || ny >= 5 || ny < 0 || nx < 0) continue;
			if (copy_arr[ny][nx] == target && subcheck[ny][nx] == 0) {
				subcheck[ny][nx] = 1;
				cnt++;
				q.push({ ny,nx });
			}
		}
	}
	if (cnt >= 3) {
		for (int ii = 0; ii < 5; ii++) {
			for (int jj = 0; jj < 5; jj++) {
				if (subcheck[ii][jj] == 1) {
					copy_arr[ii][jj] = 0;
				}
			}
		}
	}
}

void arr2copy() {
	for (int ii = 0; ii < 5; ii++) {
		for (int jj = 0; jj < 5; jj++) {
			copy_arr[ii][jj] = arr[ii][jj];
		}
	}
}

void copy2arr() {
	for (int ii = 0; ii < 5; ii++) {
		for (int jj = 0; jj < 5; jj++) {
			arr[ii][jj] = copy_arr[ii][jj];
		}
	}
}

void rotation(int i, int j, int k) {

	//90도 일때,
	if (k == 0) { // 90도
		for (int ii = i - 1; ii < i + 2; ii++) {
			for (int jj = j - 1; jj < j + 2; jj++) {
				if (ii >= 0 && ii < 5 && jj >= 0 && jj < 5) {
					int new_i = i + (jj - j);
					int new_j = j - (ii - i);
					if (new_i >= 0 && new_i < 5 && new_j >= 0 && new_j < 5) {
						copy_arr[new_i][new_j] = arr[ii][jj];
					}
				}
			}
		}
	}
	//180도 일때,
	else if (k == 1) { // 180도
		for (int jj = j - 1; jj < j + 2; jj++) {
			for (int ii = i - 1; ii < i + 2; ii++) {
				if (ii >= 0 && ii < 5 && jj >= 0 && jj < 5) {
					int new_i = i + (i - ii);
					int new_j = j + (j - jj);
					if (new_i >= 0 && new_i < 5 && new_j >= 0 && new_j < 5) {
						copy_arr[new_i][new_j] = arr[ii][jj];
					}
				}
			}
		}
	}
	//270도 일 때,
	else { // 270도
		for (int ii = i - 1; ii < i + 2; ii++) {
			for (int jj = j - 1; jj < j + 2; jj++) {
				if (ii >= 0 && ii < 5 && jj >= 0 && jj < 5) {
					int new_i = i - (jj - j);
					int new_j = j + (ii - i);
					if (new_i >= 0 && new_i < 5 && new_j >= 0 && new_j < 5) {
						copy_arr[new_i][new_j] = arr[ii][jj];
					}
				}
			}
		}
	}
}

void printarr() {

	//cout << "############배열 출력###############\n";
	//for(int i = 0; i < 5; i++) {
	//    for (int j = 0; j < 5; j++) {
	//        cout << copy_arr[i][j] << " ";
	//    }
	//    cout << "\n";
	//}
}

int main() {
	cin >> k >> m;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> arr[i][j];
		}
	}
	for (int i = 0; i < m; i++) {
		int temp;
		cin >> temp;
		photo.push_back(temp);
	}

	for (int test = 0; test < k; test++) {
		int indexi = -1;
		int indexj = -1;
		int indexk = -1;
		int max_sum = 0;
		for (int kk = 0; kk < 3; kk++) {
			for (int j = 1; j < 4; j++) {
				for (int i = 1; i < 4; i++) {
					arr2copy();
					rotation(i, j, kk);
					memset(subcheck, 0, sizeof(subcheck)); // subcheck 초기화
					int tempsum = 0;
					for (int ii = 0; ii < 5; ii++) {
						for (int jj = 0; jj < 5; jj++) {
							if (subcheck[ii][jj] == 0) {
								subcheck[ii][jj] = 1;
								tempsum += remove(ii, jj, copy_arr[ii][jj]);
							}
						}
					}
					if (tempsum > max_sum) {
						indexi = i;
						indexj = j;
						indexk = kk;
						max_sum = tempsum;
					}
				}
			}
		}

		if (max_sum < 3) return 0;

		arr2copy();
		rotation(indexi, indexj, indexk);
		for (int ii = 0; ii < 5; ii++) {
			for (int jj = 0; jj < 5; jj++) {
				memset(subcheck, 0, sizeof(subcheck));
				realremove(ii, jj, copy_arr[ii][jj]);
			}
		}
		printarr();
		for (int jj = 0; jj < 5; jj++) {
			for (int ii = 4; ii >= 0; ii--) {
				if (copy_arr[ii][jj] == 0) {
					copy_arr[ii][jj] = photo[photoindex]; // photoindex로 photo 배열 참조
					photoindex++;
				}
			}
		}
		printarr();
		while (1) {
			int flag = 0;
			for (int ii = 0; ii < 5; ii++) {
				for (int jj = 0; jj < 5; jj++) {
					memset(subcheck, 0, sizeof(subcheck));
					realremove(ii, jj, copy_arr[ii][jj]);
				}
			}
			printarr();
			for (int jj = 0; jj < 5; jj++) {
				for (int ii = 4; ii >= 0; ii--) {
					if (copy_arr[ii][jj] == 0) {
						flag = 1;
						copy_arr[ii][jj] = photo[photoindex];
						photoindex++;
						max_sum++;
					}
				}
			}
			if (flag == 0) break;
			printarr();
		}
		printarr();
		copy2arr();
		cout << max_sum << " ";
	}
}