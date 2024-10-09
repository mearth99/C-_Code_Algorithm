# LCA

## 개요

최소 공통 조상이라는 이 알고리즘은, 두 노드의 동일한 조상을 찾는데, 그 조상 중 가장 가까운 조상을 의미한다.
보통 조상을 찾기 위해서는 N번의 시간 복잡도가 걸리게 되는데, 여러번 탐색해야하는 경우, 많은 시간이 소모된다.

**특정한 상황에** 사용할 수 있는 LCA는 탐색에 Log N이 걸리게 되어 매우 빠른 속도로 탐색이 가능해진다.
 
특정한 상황은 아래와 같다.

1. 트리 구조에서 사용할 수 있다. => 사이클이 없으며, 모든 노드가 연결되어 있어야 한다.
2. 노드 수가 N이라면, **간선의 수가 N-1**이어야 한다.

이러한 상황에서 사용할 수 있으며, 시간복잡도는
1. 전처리: NlogN
2. 쿼리: logN이 걸린다.

## 방법
**전처리**
1. DFS를 통해 모든 노드를 탐색하되, 모든 노드를 탐색하면서 각 노드의 부모 및 깊이를 기록한다.
2. 전처리 과정을 통해 등록한 값을 이용하여 노드간 테이블을 제작한다.
3. 노드간 테이블은 다음의 수식으로 이루어 진다. parent[i][j] = parent[parent[i][j-1]][j-1];
4. 수식을 해석하면, i의 2^j 부모는 =  i의 2^(j-1) 노드의 2^(j-1)이 된다. ==> 2^(j-1) + 2^(j-1) 은 2^j가 되기 때문이다.
5. 이를 통해 테이블을 채워나가면, 해당하는 부모 테이블을 완성할 수 있다.

**LCA 쿼리**
1. LCA 쿼리를 받으면, A와 B에 해당하는 두 노드를 받을 탠대, 그 두 노드에 해당하는 최소조상노드를 찾아야한다.
	```cpp
	for (int i = 0; i < MAX_LOG; i++) {
			if ((diff >> i) & 1) {
				u = parent[i][u];
			}
		}
	if (u == v) return u; //최소 조상노드를 찾음
	```
	이 과정을 통해, 서로의 깊이를 동일하게 맞춰준다.
	깊이를 동일하게 맞춘 상태에서, u와 v가 다르다면 최소 조상 노드를 찾아가야한다.

2. 이제 여기서 최소 조상 노드를 찾아가야하는데, 하나씩 부모를 찾아가면 O(N)이 소요된다.

	우리는 테이블을 통해 조상을 등록해줬으므로 여기서도 더 빠르게 탐색할 수 있다.
	```cpp
		for (int i = MAX_LOG - 1; i >= 0; i--) {
		if (parent[u][i] != parent[v][i]) {
			u = parent[i][u];
			v = parent[i][v];
		}
	}
	``` 
	parent[u][i] != parent[v][i]인 경우,  lca는 최소한 2^i 이상 떨어져 있다는 것을 알 수 있다.
	그러니, 큰 값부터 내려가면서, 찾은 경우 동시에 2^i만큼 올려준다.
	즉 우리는 2^i에 해당하는 i를 순회하면서, 한번에 2^i만큼 올려주기 때문에,
	lca 쿼리 탐색에도 O(log N)이 소요된다.



## 요약

LCA는 최소공통조상을 찾는 것으로, 최소 공통 조상을 찾기 위해 log(N)의 시간이 걸린다.
핵심 점화식인 ``` parent[i][j] = parent[parent[i][j-1]][j-1] ``` 을 통해 빠르게 부모를 탐색하며

여러번의 쿼리에도 log N의 시간이 소요된다.



## 예제 코드


```cpp
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

vector<int> graph[100001];
vector<int> depth;
vector<vector<int>> parents;
int N, M, par, chi;
int maxnode = 0;
int LCA_Upgrade(int a, int b) {
    if (depth[a] < depth[b]) {
        swap(a, b);
    }
    int diff = depth[a] - depth[b];
    for (int i = maxnode; i >= 0; i--) {
        if ((diff & (1 << i)) != 0) {
            a = parents[a][i];
        }
    }
    if (a == b) return a;

    for (int i = maxnode; i >= 0; i--) {
        if (parents[a][i] != parents[b][i]) {
            a = parents[a][i];
            b = parents[b][i];
        }
    }
    return parents[a][0];
}

void DFS(int cur, int par) {
    parents[cur][0] = par;
    depth[cur] = depth[par] + 1;
    for (auto it : graph[cur]) {
        if (it != par) {
            DFS(it, cur);
        }
    }
}

void solution() {
    cin >> N;
    maxnode = ceil(log2(N));
    depth.resize(N + 1, 0);
    parents.resize(N + 1, vector<int>(maxnode + 1, -1));
    for (int i = 0; i < N - 1; i++) {
        cin >> par >> chi;
        graph[chi].push_back(par);
        graph[par].push_back(chi);
    }
    DFS(1, 0);
    for (int j = 1; j <= maxnode; j++) {
        for (int i = 1; i <= N; i++) {
            if (parents[i][j - 1] != -1) {
                parents[i][j] = parents[parents[i][j - 1]][j - 1];
            }
        }
    }
    cin >> M;
    for (int i = 0; i < M; i++) {
        int from, to;
        cin >> from >> to;
        cout << LCA_Upgrade(from, to) << '\n';
    }

    for (int i = 0; i <= N; i++) graph[i].clear();
    depth.clear();
    parents.clear();
}

int main() {
    cout.tie(0);
    cin.tie(NULL); ios_base::sync_with_stdio(false);
    solution();
    return 0;
}
```