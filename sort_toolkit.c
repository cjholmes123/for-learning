/* ============================================================
 * C 语言排序工具包：刷题到底该用哪个
 * 编译: gcc -Wall -Wextra -O2 sort_toolkit.c -o sort_toolkit
 * ============================================================ */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void show_int(const char *title, int *a, int n) {
    printf("%-16s", title);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

/* ==========================================================
 * ① 给 int 数组排序 —— 90% 的题用这一个就够了
 * ========================================================== */
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
    /* 注意: 不要写 return x - y;
       当 x = INT_MIN, y = 1 时 x - y 溢出, 顺序会排错 */
}

/* ==========================================================
 * ② 带着"原始下标"一起排序（很多题需要知道排序后对应原来哪个位置）
 *    用第二关键字把 qsort 变成"稳定"的
 * ========================================================== */
typedef struct {
    int val;
    int idx;      /* 原数组里的位置 */
} Item;

static int cmp_item(const void *a, const void *b) {
    const Item *p = (const Item *)a;
    const Item *q = (const Item *)b;
    if (p->val != q->val) return (p->val > q->val) - (p->val < q->val);
    return (p->idx > q->idx) - (p->idx < q->idx);   /* 值相同就按原顺序, 等价于稳定 */
}

/* ==========================================================
 * ③ 字符串数组排序（49 字母异位词分组这类题要用）
 * ========================================================== */
static int cmp_str(const void *a, const void *b) {
    /* 数组元素本身是 char*, qsort 传进来的是 char** */
    return strcmp(*(char *const *)a, *(char *const *)b);
}

/* ==========================================================
 * ④ 计数排序 O(n + 值域)：key 是小范围整数时，比任何比较排序都快
 * ========================================================== */
static void counting_sort(int *a, int n, int lo, int hi) {
    int range = hi - lo + 1;
    int *cnt = (int *)calloc((size_t)range, sizeof(int));
    if (!cnt) return;
    for (int i = 0; i < n; i++) cnt[a[i] - lo]++;
    int k = 0;
    for (int v = 0; v < range; v++)
        while (cnt[v]-- > 0) a[k++] = v + lo;
    free(cnt);
}

/* ==========================================================
 * ⑤ 归并排序：唯一稳的 O(n log n)，还能顺手数逆序对（493 题）
 * ========================================================== */
static void msort(int *a, int *tmp, int l, int r, long long *inv) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;                 /* 别写 (l+r)/2, 会溢出 */
    msort(a, tmp, l, m, inv);
    msort(a, tmp, m, r, inv);

    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) {                  /* <= 保证稳定 */
            tmp[k++] = a[i++];
        } else {
            if (inv) *inv += m - i;          /* a[j] 比左边剩下的 m-i 个都小 */
            tmp[k++] = a[j++];
        }
    }
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    memcpy(a + l, tmp + l, (size_t)(r - l) * sizeof(int));
}

static void merge_sort(int *a, int n, long long *inv) {
    int *tmp = (int *)malloc((size_t)n * sizeof(int));
    if (!tmp) return;
    msort(a, tmp, 0, n, inv);
    free(tmp);
}

/* ==========================================================
 * ⑥ 快速选择：只要"第 k 大"，不需要整个数组有序（215 题）
 *    平均 O(n)，比先排序再取快一个 log
 * ========================================================== */
static int quickselect(int *a, int l, int r, int k) {   /* 返回升序后下标 k 的值 */
    while (l < r) {
        int p = a[l + (r - l) / 2];      /* 取中间当基准, 比取首元素安全 */
        int i = l, j = r;
        while (i <= j) {
            while (a[i] < p) i++;
            while (a[j] > p) j--;
            if (i <= j) {
                int t = a[i]; a[i] = a[j]; a[j] = t;
                i++; j--;
            }
        }
        if (k <= j)      r = j;          /* 目标在左半边 */
        else if (k >= i) l = i;          /* 目标在右半边 */
        else             return a[k];    /* 正好被夹在 j 和 i 之间 */
    }
    return a[k];
}

/* ==========================================================
 * ⑦ 插入排序：n 很小时反而比 qsort 快（qsort 的函数调用开销占大头）
 * ========================================================== */
static void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int x = a[i], j = i - 1;
        while (j >= 0 && a[j] > x) { a[j + 1] = a[j]; j--; }
        a[j + 1] = x;
    }
}

int main(void) {
    /* ① 最常用 */
    int a1[] = {5, -2, 9, 0, -2, 7};
    qsort(a1, 6, sizeof(int), cmp_int);
    show_int("① qsort 升序:", a1, 6);
    puts("   (15 三数之和就只要这一句)");

    /* ② 带下标 */
    int a2[] = {40, 10, 30, 10};
    Item it[4];
    for (int i = 0; i < 4; i++) { it[i].val = a2[i]; it[i].idx = i; }
    qsort(it, 4, sizeof(Item), cmp_item);
    printf("② 带下标排序:   ");
    for (int i = 0; i < 4; i++) printf("(%d@%d) ", it[i].val, it[i].idx);
    printf("\n   (两个 10 保持了原来的先后顺序 -> 稳定)\n");

    /* ③ 字符串 */
    char *ss[] = {"banana", "apple", "cherry", "Apple"};
    qsort(ss, 4, sizeof(char *), cmp_str);
    printf("③ 字符串排序:   ");
    for (int i = 0; i < 4; i++) printf("%s ", ss[i]);
    printf("\n   (按 ASCII: 大写 A 在小写前面)\n");

    /* ④ 计数排序 */
    int a4[] = {4, 1, 3, 4, 2, 1, 4};
    counting_sort(a4, 7, 1, 4);
    show_int("④ 计数排序:", a4, 7);
    puts("   (值域只有 1..4, O(n+4), 比 qsort 快)");

    /* ⑤ 归并 + 数逆序对 */
    int a5[] = {5, 4, 3, 2, 1};
    long long inv = 0;
    merge_sort(a5, 5, &inv);
    show_int("⑤ 归并排序:", a5, 5);
    printf("   顺带数出逆序对: %lld 对 (5..1 全逆序 = 5*4/2 = 10)\n", inv);

    /* ⑥ 第 k 大 */
    int a6[] = {3, 2, 1, 5, 6, 4};
    int k = 2, n6 = 6;
    int kmax = quickselect(a6, 0, n6 - 1, n6 - k);
    printf("⑥ 第 %d 大的数（先排序取下标会多花 log n）: %d\n", k, kmax);

    /* ⑦ 小数组 */
    int a7[] = {9, 2, 8, 1};
    insertion_sort(a7, 4);
    show_int("⑦ 插入排序:", a7, 4);
    puts("   (n < 20 左右时, 插入排序常常比 qsort 还快)");
    return 0;
}
