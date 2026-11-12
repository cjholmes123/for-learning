/* 第 16 题 Three Sum Closest 的修正版(原 16.c 的 res 被每次循环覆盖)
 * 编译: gcc -std=c11 -Wall -Wextra 16_fixed.c -o 16_fixed
 */
#include <stdlib.h>

static int cmp(const void* a, const void* b) {
    int num1 = *(const int*)a;
    int num2 = *(const int*)b;
    return (num1 > num2) - (num1 < num2);
}

int threeSumClosest(int* nums, int numsSize, int target) {
    qsort(nums, numsSize, sizeof(int), cmp);

    /* 用真实三元组初始化,比“先算再比”更清楚 */
    int best = nums[0] + nums[1] + nums[2];

    for (int i = 0; i < numsSize - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;   /* 去重 */
        int j = i + 1;
        int k = numsSize - 1;
        while (j < k) {
            int sum = nums[i] + nums[j] + nums[k];
            if (abs(sum - target) < abs(best - target)) best = sum;
            if (sum == target) return target;            /* 已经最优,提前返回 */
            if (sum < target) j++;
            else k--;
        }
    }
    return best;
}
