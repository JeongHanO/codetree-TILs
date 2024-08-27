#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
using namespace std;

#define creat_land 100
#define creat_product 200
#define cancel_product 300
#define opti_sales 400
#define change_start 500

struct Edge {
    int num;
    int cost;
    bool operator < (Edge right) const {
        return cost > right.cost;
    }
};

struct product {
    int revenue;
    int* cost;
};

unordered_map<int, product> sale_product;
vector<Edge> alis[10001];
int n, m;
int dis[2001];
int pre_start_point = 0;

void dijkstra(int start) {
    for (int i = 0; i < n; i++) {
        dis[i] = 21e8;
    }
    priority_queue<Edge> pq;
    pq.push({ start, 0 });
    dis[start] = 0;

    while (!pq.empty()) {
        auto now = pq.top(); pq.pop();
        if (dis[now.num] < now.cost) continue;

        for (const Edge& next : alis[now.num]) {
            int nextcost = dis[now.num] + next.cost;
            if (nextcost < dis[next.num]) {
                dis[next.num] = nextcost;
                pq.push({ next.num, nextcost });
            }
        }
    }
}

int main() {

    int t;
    cin >> t;

    for (int test = 0; test < t; test++) {
        int index;
        cin >> index;

        // 코드트리 랜드 건설
        if (creat_land == index) {
            cin >> n >> m;
            for (int i = 0; i < m; i++) {
                int from, to, cost;
                cin >> from >> to >> cost;
                if (from == to) continue;
                alis[from].push_back({ to, cost });
                alis[to].push_back({ from, cost });
            }
            dijkstra(pre_start_point);
        }
        // 여행 상품 생성
        else if (creat_product == index) {
            int tempid, revenue, dest;
            cin >> tempid >> revenue >> dest;
            sale_product[tempid] = { revenue, &dis[dest] };
        }
        // 여행 상품 취소
        else if (cancel_product == index) {
            int tempid;
            cin >> tempid;
            sale_product.erase(tempid);
        }
        // 최적의 여행 상품 판매
        else if (opti_sales == index) {
            if (!sale_product.empty()) {
                int max_profit = 0;
                int best_product_id = -1;

                for (const auto& p : sale_product) {
                    int profit = p.second.revenue - *(p.second.cost);
                    if (profit >= max_profit) {
                        max_profit = profit;
                        best_product_id = p.first;
                    }
                }

                if (best_product_id != -1) {
                    cout << best_product_id << "\n";
                    sale_product.erase(best_product_id);
                }
                else {
                    cout << -1 << "\n";
                }
            }
            else {
                cout << -1 << "\n";
            }
        }
        // 여행 상품의 출발지 변경
        else if (change_start == index) {
            int change;
            cin >> change;
            pre_start_point = change;
            dijkstra(pre_start_point);
        }
    }
    return 0;
}