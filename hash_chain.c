/* ============================================================
 * 哈希表第 2 版：链地址法（每个桶挂一条链表）
 * 编译:  gcc -Wall -Wextra -O2 hash_chain.c -o hash_chain
 * 运行:  ./hash_chain
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

#define N 7           /* 桶的个数 */

typedef struct Node {
    int key;
    int val;
    struct Node *next;
} Node;

Node *bucket[N];      /* 每个桶是一条链表；全局变量默认全为 NULL */

static int H(int k) {
    int h = k % N;
    return h < 0 ? h + N : h;
}

/* ---- 插入 / 更新：找到就改，找不到就头插 ---- */
void put(int k, int v) {
    for (Node *p = bucket[H(k)]; p; p = p->next) {
        if (p->key == k) { p->val = v; return; }     /* 已经有了 */
    }

    Node *p = (Node *)malloc(sizeof(Node));          /* 没有，新建一个 */
    if (!p) return;
    p->key = k;
    p->val = v;
    p->next = bucket[H(k)];                          /* 头插，最简单的接法 */
    bucket[H(k)] = p;
}

/* ---- 查找 ---- */
int get(int k) {
    for (Node *p = bucket[H(k)]; p; p = p->next) {
        if (p->key == k) return p->val;
    }
    return -1;
}

void dump(const char *title) {
    printf("===== %s =====\n", title);
    for (int i = 0; i < N; i++) {
        if (!bucket[i]) continue;
        printf("  桶[%d]:", i);
        for (Node *p = bucket[i]; p; p = p->next) {
            printf(" (%d -> %d)", p->key, p->val);
        }
        printf("\n");
    }
}

int main(void) {
    put(1, 10);  put(8, 80);  put(15, 150);  put(3, 30);
    dump("插入 1, 8, 15, 3 之后");

    printf("\nget(8)  = %d\n", get(8));
    printf("get(99) = %d         <- 链表走完也没找到\n", get(99));

    put(8, 888);
    printf("\nput(8, 888) 之后 get(8) = %d   <- 原地覆盖\n", get(8));

    for (int i = 0; i < N; i++) {        /* 收尾：把 malloc 的还回去 */
        Node *p = bucket[i];
        while (p) { Node *nx = p->next; free(p); p = nx; }
    }
    return 0;
}
