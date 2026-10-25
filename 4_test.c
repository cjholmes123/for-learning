/* 4_test.c —— 对拍 + 三种实现的对照(不是提交内容)
 *
 * 编译运行:
 *   gcc -std=c11 -Wall -Wextra 4_test.c -o 4_test -lm
 *   ./4_test
 *
 * 里面有三个"求中位数"的实现:
 *   1. findMedianSortedArrays  来自 4.c(你自己写的第 1 层做法)
 *   2. refMedian               归并 O(m+n), 只当标准答案
 *   3. medianByPartition       第 2 层做法: 在短数组上二分切分点, O(log(min(m,n)))
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* 把 4.c 整个包含进来, 但它里面的 main 改名, 免得冲突 */
#define main lc_unused_main
#include "4.c"
#undef main

/* ============================ 公用小工具 ============================ */

typedef double (*MedianFn)(int*, int, int*, int);

static void printArr(const char* name, const int* a, int n) {
    printf("%s={", name);
    for (int i = 0; i < n; i++) {
        printf("%d%s", a[i], i + 1 < n ? "," : "");
    }
    printf("}");
}

/* 参照答案: 老老实实归并, 再取中间(题目保证至少一个元素) */
static double refMedian(const int* a, int na, const int* b, int nb) {
    const int total = na + nb;
    int* c = (int*)malloc(sizeof(int) * (size_t)total);
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) c[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < na) c[k++] = a[i++];
    while (j < nb) c[k++] = b[j++];

    double r = (total & 1) ? (double)c[total / 2]
                           : ((double)c[total / 2 - 1] + (double)c[total / 2]) / 2.0;
    free(c);
    return r;
}

static int same(double x, double y) {
    return fabs(x - y) <= 1e-9 * (1.0 + fabs(y));
}

static int cmpInt(const void* x, const void* y) {
    int u = *(const int*)x, v = *(const int*)y;
    return (u > v) - (u < v);
}

/* MinGW 的 RAND_MAX 只有 32767, 拼一下才能覆盖较大值域 */
static int rnd(int maxv) {
    return (int)(((long long)rand() * 32768 + rand()) % (maxv + 1));
}

static void fillSorted(int* arr, int n, int maxv) {
    for (int i = 0; i < n; i++) arr[i] = rnd(maxv);
    qsort(arr, (size_t)n, sizeof(int), cmpInt);
}

/* ================= 第 2 层: 在短数组上二分切分点 ================= */
/*
 * 把两个数组各切一刀, 分出"左半边"和"右半边", 要求:
 *   1. 左半边总个数 = (na+nb+1)/2   <- 多出来的那个给奇数长度用
 *   2. 交叉处不逆序: a[i-1] <= b[j] 且 b[j-1] <= a[i]
 * 满足这两条, 中位数就等于 左半边最大值 (奇数) 或 左右各取一个的平均 (偶数)。
 *
 * 关键: 只枚举"短数组"上的切分位置 i, 长数组那边的 j = half - i 就唯一确定了。
 *       i 变大时 a[i-1] 变大、b[j] 变小 => 交叉条件随 i 单调 => 可以二分。
 * 只在短数组上二分 => 复杂度 O(log(min(na,nb))), 比 O(log(na+nb)) 更省。
 *
 * 边界技巧: 用 -inf / +inf 代替越界下标, i==0、i==na、j==0、j==nb 都不用特判。
 */
static int g_steps;   /* 循环跑了多少轮, 只为观察 log 的量级 */

static double medianByPartition(int* a, int na, int* b, int nb) {
    if (na > nb) {                     /* 保证在较短的那个数组上二分 */
        int* tp = a; a = b; b = tp;
        int tn = na; na = nb; nb = tn;
    }
    const int total = na + nb;
    const int half = (total + 1) / 2;  /* 左半边应该有多少个元素 */
    int lo = 0, hi = na;               /* 切分位置 i 的搜索区间 [0, na] */
    g_steps = 0;

    for (;;) {
        g_steps++;
        const int i = lo + (hi - lo) / 2;      /* a 的左半边取 i 个 */
        const int j = half - i;                /* b 的左半边取 j 个 */
        const long long aL = (i > 0)  ? a[i - 1] : LLONG_MIN;
        const long long aR = (i < na) ? a[i]     : LLONG_MAX;
        const long long bL = (j > 0)  ? b[j - 1] : LLONG_MIN;
        const long long bR = (j < nb) ? b[j]     : LLONG_MAX;

        if (aL > bR) {                 /* a 切多了: a 的左半边塞进了不该要的数 */
            hi = i - 1;
        } else if (bL > aR) {          /* a 切少了: b 的左半边塞进了不该要的数 */
            lo = i + 1;
        } else {                       /* 切分正确, 直接取答案 */
            const long long leftMax  = (aL > bL) ? aL : bL;
            const long long rightMin = (aR < bR) ? aR : bR;
            if (total & 1) return (double)leftMax;
            return ((double)leftMax + (double)rightMin) / 2.0;
        }
    }
}

/* ============================ 用例与检查 ============================ */

typedef struct {
    int a[8], b[8];
    int na, nb;
} Case;

static Case g_cases[] = {
    {{0}, {1}, 0, 1},
    {{1}, {0}, 1, 0},
    {{1, 3}, {2}, 2, 1},
    {{1, 2}, {3, 4}, 2, 2},
    {{1}, {2, 3}, 1, 2},
    {{1, 3, 5, 7}, {2, 4, 6, 8}, 4, 4},
    {{1, 2, 3, 4, 5}, {8, 9, 10}, 5, 3},   /* 中间两个数都在同一个数组 */
    {{1, 2}, {3, 100}, 2, 2},
    {{2, 2, 4}, {8}, 3, 1},                /* 中间两个数都在同一个数组 */
    {{1, 2, 3, 4}, {5, 6}, 4, 2},          /* 同上 */
    {{2}, {1, 3, 4}, 1, 3},
    {{1, 1, 1}, {1, 1, 1}, 3, 3},          /* 全是重复值 */
    {{0, 0}, {0, 0}, 2, 2},
    {{-5, -3}, {-4, -1}, 2, 2},            /* 负数 */
    {{1, 2, 3}, {4, 5, 6}, 3, 3},
};
#define NCASE ((int)(sizeof(g_cases) / sizeof(g_cases[0])))

static int checkFixed(MedianFn fn, const char* title) {
    printf("\n=== %s : 固定用例 ===\n", title);
    int fail = 0;
    for (int t = 0; t < NCASE; t++) {
        printArr("nums1", g_cases[t].a, g_cases[t].na);
        printf(" ");
        printArr("nums2", g_cases[t].b, g_cases[t].nb);

        double want = refMedian(g_cases[t].a, g_cases[t].na, g_cases[t].b, g_cases[t].nb);
        double got = fn(g_cases[t].a, g_cases[t].na, g_cases[t].b, g_cases[t].nb);
        int ok = same(got, want);
        if (!ok) fail++;
        printf("  got=%.6f 期望=%.6f %s\n", got, want, ok ? "OK" : "FAIL");
    }
    printf("固定用例失败 %d 个\n", fail);
    return fail;
}

static int checkRandom(MedianFn fn, const char* title, int rounds, int maxsize, int maxv) {
    printf("\n=== %s : 随机对拍 %d 组 (长度 0~%d, 值域 0~%d) ===\n",
           title, rounds, maxsize, maxv);
    srand(20261103);
    int bad = 0;
    int a[2001], b[2001];
    for (int t = 0; t < rounds; t++) {
        int na = rand() % (maxsize + 1);
        int nb = rand() % (maxsize + 1);
        if (na + nb == 0) nb = 1;      /* 题目保证至少一个元素 */
        fillSorted(a, na, maxv);
        fillSorted(b, nb, maxv);

        double want = refMedian(a, na, b, nb);
        double got = fn(a, na, b, nb);
        if (!same(got, want)) {
            bad++;
            if (bad <= 5) {
                printArr("nums1", a, na);
                printf(" ");
                printArr("nums2", b, nb);
                printf("  got=%.6f 期望=%.6f\n", got, want);
            }
        }
    }
    printf("不一致 %d 组\n", bad);
    return bad;
}

/* 只看切分版的轮数: 两个数组都取同一值域的随机数, 必然深度交错, 切分点只有一个 */
static void stepDemo(void) {
    printf("\n=== O(log) 量级观察: 随机交错数据下, 切分版跑几轮 ===\n");
    const int sizes[] = {10, 100, 1000, 10000, 100000, 1000000};
    for (int s = 0; s < (int)(sizeof(sizes) / sizeof(sizes[0])); s++) {
        const int N = sizes[s];
        int* a = (int*)malloc(sizeof(int) * (size_t)N);
        int* b = (int*)malloc(sizeof(int) * (size_t)N);
        fillSorted(a, N, 10 * N);
        fillSorted(b, N, 10 * N);

        double got = medianByPartition(a, N, b, N);
        double want = refMedian(a, N, b, N);
        printf("  n=m=%-8d 轮数=%-3d got=%.1f 期望=%.1f %s\n",
               N, g_steps, got, want, same(got, want) ? "OK" : "FAIL");
        free(a);
        free(b);
    }
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    /* ---- 你的 4.c(第 1 层做法) ---- */
    checkFixed(findMedianSortedArrays, "4.c: findMedianSortedArrays");
    checkRandom(findMedianSortedArrays, "4.c: findMedianSortedArrays", 5000, 5, 9);

    /* ---- 第 2 层做法 ---- */
    checkFixed(medianByPartition, "第2层: medianByPartition");
    checkRandom(medianByPartition, "第2层: medianByPartition", 5000, 5, 9);
    checkRandom(medianByPartition, "第2层: medianByPartition(大规模)", 300, 2000, 1000000);
    stepDemo();

    getchar();
    return 0;
}
