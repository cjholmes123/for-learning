/* ============================================================
 * 三数之和：几种写法的速度对比
 * 编译: gcc -Wall -Wextra -O2 15_faster.c -o 15_faster
 *
 * A      = 双指针（标准写法）
 * B      = 双指针 + 两个剪枝
 * D      = 只枚举"不同的值"：复杂度 O(m^2 log m)，m = 不同值个数
 * A_alloc= 标准写法 + 真正的 malloc（复现 LeetCode 提交版本的开销）
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REP 20          /* 每项重复次数, 用来抵消 clock() 的 1ms 精度 */

static int cmp_int(const void *p, const void *q) {
    int x = *(const int *)p, y = *(const int *)q;
    return (x > y) - (x < y);
}

/* ---------------- A：标准双指针 ---------------- */
static long threeSum_A(int *a, int n) {
    qsort(a, n, sizeof(int), cmp_int);
    long res = 0;
    for (int i = 0; i < n - 2; i++) {
        if (a[i] > 0) break;
        if (i > 0 && a[i] == a[i - 1]) continue;
        int l = i + 1, r = n - 1;
        while (l < r) {
            int s = a[i] + a[l] + a[r];
            if (s < 0) {
                l++;
            } else if (s > 0) {
                r--;
            } else {
                res++;
                l++;
                r--;
                while (l < r && a[l] == a[l - 1]) l++;
                while (l < r && a[r] == a[r + 1]) r--;
            }
        }
    }
    return res;
}

/* ---------------- B：加两个剪枝 ---------------- */
static long threeSum_B(int *a, int n) {
    qsort(a, n, sizeof(int), cmp_int);
    long res = 0;
    for (int i = 0; i < n - 2; i++) {
        if (a[i] > 0) break;
        if (i > 0 && a[i] == a[i - 1]) continue;
        if (a[i] + a[i + 1] + a[i + 2] > 0) break;      /* 最小三个都 > 0 */
        if (a[i] + a[n - 2] + a[n - 1] < 0) continue;   /* 最大两个都不够 */

        int l = i + 1, r = n - 1;
        while (l < r) {
            int s = a[i] + a[l] + a[r];
            if (s < 0) {
                l++;
            } else if (s > 0) {
                r--;
            } else {
                res++;
                l++;
                r--;
                while (l < r && a[l] == a[l - 1]) l++;
                while (l < r && a[r] == a[r + 1]) r--;
            }
        }
    }
    return res;
}

/* ---------------- D：只枚举"不同的值" ---------------- */
static long threeSum_D(int *src, int n) {
    int *a = (int *)malloc(sizeof(int) * (size_t)n);
    int *v = (int *)malloc(sizeof(int) * (size_t)n);
    int *c = (int *)malloc(sizeof(int) * (size_t)n);
    if (a == NULL || v == NULL || c == NULL) { free(a); free(v); free(c); return -1; }
    memcpy(a, src, sizeof(int) * (size_t)n);
    qsort(a, n, sizeof(int), cmp_int);

    int m = 0;
    for (int i = 0; i < n; i++) {
        if (m > 0 && v[m - 1] == a[i]) c[m - 1]++;
        else { v[m] = a[i]; c[m] = 1; m++; }
    }

    long res = 0;
    for (int x = 0; x < m; x++) {
        for (int y = x; y < m; y++) {
            long long need = -(long long)v[x] - v[y];
            if (need < v[y] || need > v[m - 1]) continue;

            int lo = y, hi = m - 1, id = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (v[mid] == (int)need) { id = mid; break; }
                else if (v[mid] < (int)need) lo = mid + 1;
                else hi = mid - 1;
            }
            if (id < 0) continue;

            int ok;
            if (x == y && y == id) ok = (c[x] >= 3);
            else if (x == y)       ok = (c[x] >= 2);
            else if (y == id)      ok = (c[y] >= 2);
            else                   ok = 1;
            if (ok) res++;
        }
    }
    free(a); free(v); free(c);
    return res;
}

/* ---------------- A_alloc：连 malloc 一起算 ---------------- */
static long threeSum_A_alloc(int *a, int n) {
    qsort(a, n, sizeof(int), cmp_int);
    int **res = (int **)malloc(sizeof(int *) * (size_t)(n * n));
    if (res == NULL) return -1;
    long l = 0;
    for (int i = 0; i < n - 2; i++) {
        if (a[i] > 0) break;
        if (i > 0 && a[i] == a[i - 1]) continue;
        int x = i + 1, r = n - 1;
        while (x < r) {
            int s = a[i] + a[x] + a[r];
            if (s < 0) {
                x++;
            } else if (s > 0) {
                r--;
            } else {
                int *t = (int *)malloc(3 * sizeof(int));   /* 每组一次 malloc */
                t[0] = a[i]; t[1] = a[x]; t[2] = a[r];
                res[l++] = t;
                x++;
                r--;
                while (x < r && a[x] == a[x - 1]) x++;
                while (x < r && a[r] == a[r + 1]) r--;
            }
        }
    }
    for (long i = 0; i < l; i++) free(res[i]);
    free(res);
    return l;
}

/* ---------------- 测试数据 ---------------- */
static void fill_worst(int *a, int n) { for (int i = 0; i < n; i++) a[i] = i - n / 2; }
static void fill_dup(int *a, int n)   { srand(7);  for (int i = 0; i < n; i++) a[i] = rand() % 11 - 5; }
static void fill_rand(int *a, int n)  { srand(11); for (int i = 0; i < n; i++) a[i] = rand() % 200001 - 100000; }

/* 跑 REP 轮取平均, 顺便把答案数量带回来 */
static double run(long (*f)(int *, int), const int *base, int n, long *cnt) {
    int *w = (int *)malloc(sizeof(int) * (size_t)n);
    long acc = 0;
    clock_t t0 = clock();
    for (int r = 0; r < REP; r++) {
        memcpy(w, base, sizeof(int) * (size_t)n);
        acc += f(w, n);
    }
    double ms = 1000.0 * (double)(clock() - t0) / CLOCKS_PER_SEC / REP;
    *cnt = acc / REP;
    free(w);
    return ms;
}

int main(void) {
    const int n = 3000;
    int *base = (int *)malloc(sizeof(int) * (size_t)n);
    long cA, cB, cD;

    struct { const char *name; void (*fill)(int *, int); } cases[] = {
        {"最坏情况: 值全不重复, 答案极多", fill_worst},
        {"大量重复: 只有 11 个不同值",     fill_dup},
        {"一般随机: 值域 [-1e5, 1e5]",    fill_rand},
    };

    for (int k = 0; k < 3; k++) {
        cases[k].fill(base, n);
        printf("%s (n=%d, 重复 %d 轮取平均)\n", cases[k].name, n, REP);
        double tA = run(threeSum_A, base, n, &cA);
        double tB = run(threeSum_B, base, n, &cB);
        double tD = run(threeSum_D, base, n, &cD);
        printf("   A 双指针            %7.2f ms   答案 %ld 组\n", tA, cA);
        printf("   B 双指针+剪枝       %7.2f ms   答案 %ld 组  %s\n", tB, cB, cB == cA ? "[一致]" : "[不一致!]");
        printf("   D 只枚举不同值      %7.2f ms   答案 %ld 组  %s\n", tD, cD, cD == cA ? "[一致]" : "[不一致!]");

        if (k == 0) {
            long cX;
            double tX = run(threeSum_A_alloc, base, n, &cX);
            printf("   A 双指针+真 malloc  %7.2f ms   答案 %ld 组  <- 提交版本的开销\n", tX, cX);
        }
        printf("\n");
    }
    free(base);
    return 0;
}
