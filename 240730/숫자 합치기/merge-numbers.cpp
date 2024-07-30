#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
	priority_queue<int, vector<int>, greater<int>> pq;
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int a; cin >> a;
		pq.push(a);
	}
	int sum = 0;
	while (pq.size() > 1) {
		int a = pq.top();
		pq.pop();
		int b = pq.top();
		pq.pop();
		sum += (a + b);
		pq.push(a + b);
	}
	cout << sum << endl;
	return 0;
}