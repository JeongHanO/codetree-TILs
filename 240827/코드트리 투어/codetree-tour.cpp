#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

struct NODE {
	int revenue;
	int destination;
	int distance;
};

struct cmp {
	bool operator()(pair<int, int> a, pair<int, int> b) {
		if (a.first < b.first) return true;
		if (a.first == b.first) {
			if (a.second > b.second) return true;
		}
		return false;
	}
};

int Q, n, m, s = 0;
int land[2010];
vector<pair<int, int>> graph[2010]; // 노드, 거리
unordered_map<int, NODE> um; // id, NODE(매출, 도착지, 거리)
priority_queue < pair<int, int>, vector<pair<int, int>>, cmp> pq; // 수익, id

void dij(int s) {
	for (int i = 0; i < n; i++) {
		land[i] = 1e8;
	}
	land[s] = 0;
	priority_queue<pair<int, int>> q;
	q.push({ 0, s }); // 거리, 노드
	while (!q.empty()) {
		int dist = -q.top().first;
		int now = q.top().second;
		q.pop();
		if (land[now] < dist) continue;
		for (int i = 0; i < graph[now].size(); i++) {
			int nextDist = graph[now][i].second + land[now];
			int next = graph[now][i].first;
			if (nextDist < land[next]) {
				land[next] = nextDist;
				q.push({ -nextDist, next });
			}
		}
	}
}

int main() {
	cin >> Q;
	int order;
	cin >> order >> n >> m;
	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		graph[a].push_back({ b, c });
		graph[b].push_back({ a, c });
	}

	dij(s);
	Q--;

	int id, rev, dest; 
	NODE node;
	bool flag;
	while (Q--) {
		cin >> order;
		switch (order) {
			case 200:
				cin >> id >> rev >> dest;
				node = { rev, dest, land[dest] }; // 비용, 도착지, 거리
				um.insert({ id, node });
				if (rev >= land[dest]) {
					pq.push({ rev - land[dest], id });
				}
				break;
			case 300:
				cin >> id;
				um.erase(id);
				break;
			case 400:
				if (um.empty()) {
					cout << -1 << '\n';
					continue;
				}
				flag = false;
				while (!pq.empty()) {
					int tmp_id = pq.top().second;
					if (um.find(tmp_id) == um.end()) { // 삭제됨
						pq.pop();
						continue;
					}
					cout << tmp_id << '\n';
					um.erase(tmp_id);
					pq.pop();
					flag = true;
					break;
				}
				if(!flag) cout << -1 << '\n';
				break;
			case 500:
				cin >> s;
				dij(s);
				
				while (!pq.empty()) pq.pop();
				for (auto it : um) {
					it.second.distance = land[it.second.destination];
					if (it.second.revenue >= land[it.second.destination]) {
						pq.push({ it.second.revenue - land[it.second.destination], it.first });
					}
				}
				break;
		}
	}
}