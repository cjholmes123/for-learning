/* ============================================================
 * 链表最基础的写法（C 语言）
 * 编译: gcc -Wall -Wextra -g linked_list_basics.c -o linked_list_basics
 * 运行: ./linked_list_basics
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

/* ---------- 1. 节点 ---------- */
typedef struct Node {
    int val;
    struct Node *next;      /* 结构体内部还不能用 Node 这个名字, 只能写 struct Node */
} Node;

/* ---------- 看清链表长什么样（调试用，强烈建议每步都打） ---------- */
void show(Node *head) {
    for (Node *p = head; p != NULL; p = p->next) printf("%d -> ", p->val);
    printf("NULL");
}
void print_list(const char *title, Node *head) {
    printf("%-18s", title);
    show(head);
    printf("\n");
}

/* ---------- 2. 头插 O(1)：新节点放到最前面 ---------- */
Node *push_front(Node *head, int v) {
    Node *p = (Node *)malloc(sizeof(Node));
    if (p == NULL) return head;          /* 内存不够就直接放弃 */
    p->val = v;
    p->next = head;      /* 先让新节点记住原来的头 */
    return p;            /* 再让 head 指向新节点（靠返回值交回去） */
}

/* ---------- 3. 尾插 O(n)：接到最后一个后面 ---------- */
Node *push_back(Node *head, int v) {
    Node *p = (Node *)malloc(sizeof(Node));
    if (p == NULL) return head;
    p->val = v;
    p->next = NULL;                      /* 尾巴的 next 一定是 NULL */

    if (head == NULL) return p;          /* 空表: 新节点就是头 */
    Node *tail = head;
    while (tail->next != NULL) tail = tail->next;   /* 走到最后一个节点 */
    tail->next = p;
    return head;
}

/* ---------- 4. 查找 O(n) ---------- */
Node *find(Node *head, int v) {
    for (Node *p = head; p != NULL; p = p->next)
        if (p->val == v) return p;       /* 返回节点本身, 不只返回值 */
    return NULL;
}

/* ---------- 5. 删除第一个值为 v 的节点 ---------- */
Node *remove_one(Node *head, int v) {
    if (head == NULL) return NULL;

    if (head->val == v) {                /* 特殊情况: 删的就是头 */
        Node *next = head->next;
        free(head);
        return next;
    }
    Node *prev = head;                   /* prev 永远落在 cur 前面一格 */
    while (prev->next != NULL && prev->next->val != v)
        prev = prev->next;
    if (prev->next != NULL) {            /* 找到了 */
        Node *dead = prev->next;
        prev->next = dead->next;         /* 让前一个直接跳过它 */
        free(dead);
    }
    return head;
}

/* ---------- 6. 哨兵节点：省掉所有"删头/空表"的特殊判断 ---------- */
Node *remove_all(Node *head, int v) {
    Node dummy;                  /* 假节点, 不放数据, 站在真 head 前面 */
    dummy.next = head;
    Node *prev = &dummy;

    while (prev->next != NULL) {
        if (prev->next->val == v) {
            Node *dead = prev->next;
            prev->next = dead->next;     /* 所有情况都变成同一个动作 */
            free(dead);
        } else {
            prev = prev->next;
        }
    }
    return dummy.next;           /* dummy 自己在栈上, 只把它的 next 交出去 */
}

/* ---------- 7. 反转链表（三指针），带每轮打印 ---------- */
Node *reverse_trace(Node *head) {
    Node *prev = NULL, *cur = head;
    int step = 0;
    printf("\n反转链表（LeetCode 206 的经典写法）:\n");
    printf("  %-8s 已反转部分: ", "初始");
    show(prev);
    printf("   |  待处理: ");
    show(cur);
    printf("\n");

    while (cur != NULL) {
        Node *next = cur->next;   /* ① 先记下后面, 因为等会儿要改 cur->next */
        cur->next = prev;         /* ② 掉头 */
        prev = cur;               /* ③ 两个指针一起前进 */
        cur = next;
        step++;
        printf("  第%d轮后   已反转部分: ", step);
        show(prev);
        printf("   |  待处理: ");
        show(cur);
        printf("\n");
    }
    return prev;                  /* cur 变成 NULL 时, prev 正好是新的头 */
}

/* ---------- 8. 释放整条链表 ---------- */
void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next;      /* 先存 next, 再 free */
        free(head);
        head = next;
    }
}

/* ---------- 9. 用数组模拟链表（很多 C 题解这么写，不用 malloc） ---------- */
void array_list_demo(void) {
    puts("\n=== 用数组模拟链表: next[i] 表示 i 的下一个节点是几号 ===");
    int val[]  = {10, 20, 30};
    int next[] = {2, -1,  1};      /* 0->2->1->-1，即 10->30->20->结束 */
    int head = 0;

    printf("  从头走一遍: ");
    for (int i = head; i != -1; i = next[i]) printf("%d -> ", val[i]);
    puts("结束");
    puts("  (换成指针, 就是把 next[i] 换成结构体里的指针, 逻辑一模一样)");
}

int main(void) {
    puts("=== 1. 头插: 1,2,3 依次插到最前面 ===");
    Node *head = NULL;
    for (int i = 1; i <= 3; i++) {
        head = push_front(head, i);
        print_list("  插完之后:", head);
    }

    puts("\n=== 2. 尾插: 再接 4,5 ===");
    for (int i = 4; i <= 5; i++) {
        head = push_back(head, i);
        print_list("  插完之后:", head);
    }

    puts("\n=== 3. 查找 ===");
    Node *p = find(head, 1);
    printf("  find(1)  -> %s\n", p ? "找到" : "没找到");
    printf("  find(99) -> %s\n", find(head, 99) ? "找到" : "没找到");

    puts("\n=== 4. 删除一个 2 ===");
    head = remove_one(head, 2);
    print_list("  删完之后:", head);

    puts("\n=== 5. 反转 ===");
    head = reverse_trace(head);
    print_list("\n  反转结果:", head);

    puts("\n=== 6. 哨兵节点版: 删掉所有 3 ===");
    head = remove_all(head, 3);
    print_list("  删完之后:", head);

    array_list_demo();

    free_list(head);
    puts("\n已释放全部节点");
    return 0;
}
