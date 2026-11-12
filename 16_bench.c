/* 第 16 题基准测试:16_fixed.c(标准双指针) vs 16_faster.c(剪枝版)
 * 用 QPC 高精度计时(Windows clock() 只有 15.6ms 一格,不适合微秒级测量)。
 * 输入全部取偶数 + target 取奇数 => 永远不可能差值 0,避免「提前返回」掩盖真实开销。
 *
 * 编译: gcc -O2 -std=c11 -Wall -Wextra 16_bench.c 16_fixed.c 16_faster.c -o 16_bench
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int threeSumClosest(int* nums, int numsSize, int target);
int threeSumClosestFast(int* nums, int numsSize, int target);
int threeSumClosestCount(int* nums, int numsSize, int target);

static double freq;

static double now(void) {
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / freq;
}

/* 跑到至少 0.15 秒,返回单次平均微秒数 */
static double time_one(int (*fn)(int*, int, int), const int* base, int n, int target, int* out) {
    int* buf = malloc(sizeof(int) * n);
    int rounds = 0;
    int ans = 0;
    double t0 = now(), t1;
    do {
        memcpy(buf, base, sizeof(int) * n);
        ans = fn(buf, n, target);
        rounds++;
        t1 = now();
    } while (t1 - t0 < 0.15 && rounds < 1000000);
    free(buf);
    *out = ans;
    return (t1 - t0) * 1e6 / rounds;
}

static void bench(const char* name, const int* base, int n, int target) {
    int r1, r2, r3;
    double us1 = time_one(threeSumClosest, base, n, target, &r1);
    double us2 = time_one(threeSumClosestFast, base, n, target, &r2);
    double us3 = time_one(threeSumClosestCount, base, n, target, &r3);
    int ok = (abs(r1 - target) == abs(r2 - target)) && (abs(r1 - target) == abs(r3 - target));
    printf("%-30s n=%4d t=%6d | 标准 %8.2f | 剪枝 %8.2f (%5.2fx) | 计数排序 %8.2f (%5.2fx)  (us)%s\n",
           name, n, target, us1, us2, us2 > 0 ? us1 / us2 : 0.0,
           us3, us3 > 0 ? us1 / us3 : 0.0, ok ? "" : "  <-- 结果不一致!");
}

static int cmpq(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

/* 单独量一下 qsort 的固定开销 */
static void bench_sort(const int* base, int n) {
    int* buf = malloc(sizeof(int) * n);
    int rounds = 0;
    double t0 = now(), t1;
    do {
        memcpy(buf, base, sizeof(int) * n);
        qsort(buf, n, sizeof(int), cmpq);
        rounds++;
        t1 = now();
    } while (t1 - t0 < 0.15);
    printf("qsort(%d 个 int) 单次 %.2f us —— n 较小时这是主要固定开销\n", n, (t1 - t0) * 1e6 / rounds);
    free(buf);
}

static void fill_random_even(int* a, int n) {
    for (int i = 0; i < n; i++) a[i] = (rand() % 1001 - 500) * 2;   /* 偶数,值域 ±1000 */
}

int main(void) {
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    freq = (double)f.QuadPart;
    srand(2024);

    int n500[500], n2000[2000], dup[500], hit[500];
    fill_random_even(n500, 500);
    fill_random_even(n2000, 2000);
    for (int i = 0; i < 500; i++) dup[i] = (rand() % 10) * 2;        /* 10 种偶数,大量重复 */
    fill_random_even(hit, 500);
    hit[0] = 100; hit[1] = 200; hit[2] = -300;                      /* 100+200-300 = 0 */

    puts("== 最坏情形:不可能命中,双指针全跑(n 取 LeetCode 上限 500)==");
    bench("随机偶数 / target=1(奇数)", n500, 500, 1);
    bench("随机偶数 / target=1", n2000, 2000, 1);
    puts("\n== 剪枝生效的输入 ==");
    bench("大量重复(10 种值)", dup, 500, 1);
    bench("target=+3000(远大于任何和)", n500, 500, 3000);
    bench("target=-3000(远小于任何和)", n500, 500, -3000);
    bench("精确命中(可提前返回)", hit, 500, 0);
    puts("");
    bench_sort(n500, 500);
    return 0;
}
