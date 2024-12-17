#include <iostream>
using namespace std;
#define MAX 100
int arr[MAX];
int tree[MAX * 4]; 
int lazy[MAX * 4]; // lazy 배열 추가

// lazy 값을 적용하는 함수
void update_lazy(int start, int end, int node) {
    if (lazy[node] != 0) {
        tree[node] += (end - start + 1) * lazy[node]; // 구간의 크기만큼 lazy 값을 곱해서 더함
        if (start != end) { // 리프 노드가 아니라면 자식에게 lazy 전파
            lazy[node * 2] += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = 0; // lazy 값 초기화
    }
}

int query(int start, int end, int node, int left, int right) {
    update_lazy(start, end, node); // lazy 값 업데이트
    if (left > end || right < start) return 0;
    if (left <= start && end <= right) return tree[node];
    int mid = (start + end) / 2;
    return query(start, mid, node * 2, left, right) + query(mid + 1, end, node * 2 + 1, left, right);
}

void update_range(int start, int end, int node, int left, int right, int diff) {
    update_lazy(start, end, node); // lazy 값 업데이트
    if (left > end || right < start) return;
    if (left <= start && end <= right) {
        tree[node] += (end - start + 1) * diff;
        if (start != end) { // 리프 노드가 아니라면 lazy 값 설정
            lazy[node * 2] += diff;
            lazy[node * 2 + 1] += diff;
        }
        return;
    }
    int mid = (start + end) / 2;
    update_range(start, mid, node * 2, left, right, diff);
    update_range(mid + 1, end, node * 2 + 1, left, right, diff);
    tree[node] = tree[node * 2] + tree[node * 2 + 1]; // 자식 노드의 합을 부모 노드에 반영
}

void init_tree(int start, int end, int node) {
    if (start == end) {
        tree[node] = arr[start];
        return;
    }
    int mid = (start + end) / 2;
    init_tree(start, mid, node * 2);
    init_tree(mid + 1, end, node * 2 + 1);
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

int main() {
    for (int i = 0; i <= 9; i++) {
        arr[i] = i;
    }
    init_tree(0, 9, 1); // 세그먼트 트리 초기화
    
    cout << query(0, 9, 1, 2, 5) << endl; // 2번째에서 5번째까지 출력하기 값: 14 (2+3+4+5)
    update_range(0, 9, 1, 2, 5, 1); // 2번째부터 5번째까지 각각 1씩 증가
    cout << query(0, 9, 1, 2, 5) << endl; // 결과: 18 (3+4+5+6)
    
    return 0;
}