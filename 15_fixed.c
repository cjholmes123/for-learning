/* ============================================================
 * LeetCode 15. 三数之和 —— C 语言参考实现
 *
 * 提交到 LeetCode 时只复制 cmp_int 和 threeSum 两个函数
 * 本地验证: gcc -Wall -Wextra -O2 15_fixed.c 15_check.c -o 15_check.exe
 * ============================================================ */
#include <stdlib.h>

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);        /* 不能写 x - y: INT_MIN - 1 会溢出 */
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    int **res = NULL;
    int cnt = 0;                     /* 已经收集了几组 = 最终要返回的数量 */
    int cap = 0;                     /* res 现在最多能装几组 */

    /* ① 两个输出参数先置空。这样任何一条提前返回的路径都不会留下垃圾值 */
    *returnSize = 0;
    *returnColumnSizes = NULL;
    if (nums == NULL || numsSize < 3) return NULL;

    /* ② 排序。这一步同时让"去重"和"双指针的单调性"成立 */
    qsort(nums, (size_t)numsSize, sizeof(int), cmp_int);

    /* ③ 固定第一个数, 后面两个用左右指针往中间夹 */
    for (int i = 0; i < numsSize - 2; i++) {
        if (nums[i] > 0) break;                         /* 剪枝: 最小的都为正, 和不可能为 0 */
        if (i > 0 && nums[i] == nums[i - 1]) continue;  /* 去重①: 首数重复就跳过 */

        int need = -nums[i];                            /* 后两个数需要的和 */
        int l = i + 1;
        int r = numsSize - 1;

        while (l < r) {
            int s = nums[l] + nums[r];

            if (s < need) {                             /* 和太小 -> 左端换大一点的 */
                l++;
                continue;
            }
            if (s > need) {                             /* 和太大 -> 右端换小一点的 */
                r--;
                continue;
            }

            /* ---------------- 命中一组 ---------------- */
            if (cnt == cap) {                           /* 容量不够, 先翻倍再写 */
                int newcap = (cap == 0) ? 16 : cap * 2;
                int **tmp = (int **)realloc(res, sizeof(int *) * (size_t)newcap);
                if (tmp == NULL) goto done;             /* 内存不足: 交出现有的结果, 不崩 */
                res = tmp;
                cap = newcap;
            }
            int *t = (int *)malloc(3 * sizeof(int));
            if (t == NULL) goto done;
            t[0] = nums[i];
            t[1] = nums[l];
            t[2] = nums[r];
            res[cnt++] = t;

            l++;                                            /* 先各走一步, 离开刚用过的值 */
            r--;
            while (l < r && nums[l] == nums[l - 1]) l++;    /* 去重②: 中间数 */
            while (l < r && nums[r] == nums[r + 1]) r--;    /* 去重③: 尾数   */
        }
    }

done:
    /* ④ 收尾: 数量和每组长度, 两个输出参数都必须写 */
    *returnSize = cnt;
    if (cnt > 0) {
        *returnColumnSizes = (int *)malloc(sizeof(int) * (size_t)cnt);
        if (*returnColumnSizes == NULL) {               /* 兜底: 宁可返回空也不给错的 */
            *returnSize = 0;
            return NULL;
        }
        for (int m = 0; m < cnt; m++) (*returnColumnSizes)[m] = 3;
    }
    return res;
}
