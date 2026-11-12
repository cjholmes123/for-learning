/* 第 16 题加速版:排序 + 双指针,叠三个剪枝
 *  1. 每个值最多保留 3 份(三元组不可能用到第 4 个相同值)
 *  2. 固定 i 时先看区间端点 smin/smax:target 落在区间外就 O(1) 出结果
 *     target <= smin 时后面的 i 只会更远,可以直接 break
 *  3. 命中 target 立刻返回(差值 0 已是最优)
 * 最坏复杂度仍是 O(n^2),但多数输入的实际比较次数大幅下降。
 *
 * 编译: gcc -O2 -std=c11 -Wall -Wextra 16_faster.c -o 16_faster
 */
#include <stdlib.h>

static int cmp_fast(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

/* 把 i 开头的那段双指针扫完(调用前已保证 target 落在 [smin, smax] 内) */
static int scan(int* nums, int n, int i, int target, int best) {
    int j = i + 1, k = n - 1;
    while (j < k) {
        int sum = nums[i] + nums[j] + nums[k];
        int d = sum - target;
        if (d == 0) return target;
        if (abs(d) < abs(best - target)) best = sum;
        if (d < 0) j++; else k--;
    }
    return best;
}

int threeSumClosestFast(int* nums, int numsSize, int target) {
    qsort(nums, numsSize, sizeof(int), cmp_fast);

    /* 压缩重复值:每个值留 3 份就够(原地覆盖,已排序所以相同值相邻) */
    int n = 0, run = 0;
    for (int i = 0; i < numsSize; i++) {
        run = (i > 0 && nums[i] == nums[i - 1]) ? run + 1 : 1;
        if (run <= 3) nums[n++] = nums[i];
    }

    int best = nums[0] + nums[1] + nums[2];
    if (best == target) return target;

    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;   /* i 去重 */

        int smin = nums[i] + nums[i + 1] + nums[i + 2];
        int smax = nums[i] + nums[n - 2] + nums[n - 1];

        if (target <= smin) {
            /* 该 i 及之后所有候选和都 >= target,全局最近的就是 smin */
            if (smin - target < abs(best - target)) best = smin;
            break;
        }
        if (target >= smax) {
            /* 该 i 的最优是 smax;但更大的 i 可能更近,只能 continue */
            if (target - smax < abs(best - target)) best = smax;
            continue;
        }

        int b = scan(nums, n, i, target, best);
        if (b == target) return target;                  /* scan 内命中 */
        best = b;
    }
    return best;
}

/* ---- 再进一步:n ≤ 500 时 qsort 才是大头,值域有界就用计数排序 ---- */

/* 计数排序 + 顺手把每个值的个数截到 3,返回压缩后的长度 */
static int count_sort_cap3(int* a, int n) {
    int lo = a[0], hi = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < lo) lo = a[i];
        if (a[i] > hi) hi = a[i];
    }
    long range = (long)hi - lo + 1;
    if (range > 4L * n + 64) {          /* 值域太散(比如 ±1e9),计数排序不划算 */
        qsort(a, n, sizeof(int), cmp_fast);
        return n;
    }
    int* cnt = calloc((size_t)range, sizeof(int));
    if (cnt == NULL) {
        qsort(a, n, sizeof(int), cmp_fast);
        return n;
    }
    for (int i = 0; i < n; i++)
        if (cnt[a[i] - lo] < 3) cnt[a[i] - lo]++;

    int m = 0;
    for (long v = 0; v < range; v++)
        for (int c = 0; c < cnt[v]; c++) a[m++] = (int)(v + lo);
    free(cnt);
    return m;
}

int threeSumClosestCount(int* nums, int numsSize, int target) {
    int n = count_sort_cap3(nums, numsSize);

    int best = nums[0] + nums[1] + nums[2];
    if (best == target) return target;

    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int smin = nums[i] + nums[i + 1] + nums[i + 2];
        int smax = nums[i] + nums[n - 2] + nums[n - 1];

        if (target <= smin) {
            if (smin - target < abs(best - target)) best = smin;
            break;
        }
        if (target >= smax) {
            if (target - smax < abs(best - target)) best = smax;
            continue;
        }

        int b = scan(nums, n, i, target, best);
        if (b == target) return target;
        best = b;
    }
    return best;
}
