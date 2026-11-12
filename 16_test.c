/* 第 16 题对拍:16.c 的 threeSumClosest vs 暴力三重循环
 * 编译: gcc -std=c11 -Wall -Wextra 16_test.c 16.c -o 16_test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int threeSumClosest(int* nums, int numsSize, int target);

static int brute(int* nums, int n, int target) {
    int best = nums[0] + nums[1] + nums[2];   /* 不能用 INT_MAX:best-target 会溢出 */
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++) {
                int s = nums[i] + nums[j] + nums[k];
                if (abs(s - target) < abs(best - target)) best = s;
            }
    return best;
}

static int fails = 0;

static void check(int* nums, int n, int target) {
    int* a = malloc(sizeof(int) * n);
    int* b = malloc(sizeof(int) * n);
    memcpy(a, nums, sizeof(int) * n);
    memcpy(b, nums, sizeof(int) * n);
    int got = threeSumClosest(a, n, target);
    int exp = brute(b, n, target);
    /* 只要求「差值」与期望一致,允许等距时返回别的解 */
    if (abs(got - target) != abs(exp - target)) {
        fails++;
        if (fails <= 10) {
            printf("FAIL nums=[");
            for (int i = 0; i < n; i++) printf("%d%s", nums[i], i + 1 < n ? "," : "");
            printf("] target=%d  got=%d(diff %d)  expected=%d(diff %d)\n",
                   target, got, abs(got - target), exp, abs(exp - target));
        }
    }
    free(a); free(b);
}

int main(void) {
    printf("== 手写用例 ==\n");
    int c1[] = {-1, 2, 1, -4};               check(c1, 4, 1);      /* 期望 2 */
    int c2[] = {0, 0, 0};                    check(c2, 3, 1);      /* 期望 0 */
    int c3[] = {1, 1, 1, 0};                 check(c3, 4, 100);    /* 期望 3 */
    int c4[] = {0, 1, 1, 1};                 check(c4, 4, -100);   /* 期望 2 */
    int c5[] = {4, 0, 5, -5, 3, 3, 0, -4, -5}; check(c5, 9, -2);  /* 期望 -2 */
    printf("== 随机用例 ==\n");

    srand(12345);
    int n, t;
    for (int iter = 0; iter < 20000; iter++) {
        n = 3 + rand() % 6;
        t = (rand() % 41) - 20;
        int* nums = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) nums[i] = (rand() % 21) - 10;
        check(nums, n, t);
        free(nums);
    }

    printf(fails ? "\n共 %d 个用例失败\n" : "\n全部通过\n", fails);
    return fails != 0;
}
