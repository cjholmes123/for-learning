#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int lengthOfLongestSubstring(char* s);

/* ================= 参考实现: 暴力枚举(只用于对拍, 不提交) ================= */

/* 判断 s[start .. start+len-1] 里是否有重复字符 */
static int hasRepeat(const char* s, int start, int len) {
    for (int p = start; p < start + len - 1; p++) {
        for (int q = p + 1; q < start + len; q++) {
            if (s[p] == s[q]) {
                return 1;
            }
        }
    }
    return 0;
}

/* O(n^4): 长度从长到短枚举, 第一个无重复的长度就是答案 */
static int bruteForce(char* s) {
    int n = (int)strlen(s);
    for (int len = n; len >= 1; len--) {
        for (int i = 0; i + len <= n; i++) {
            if (!hasRepeat(s, i, len)) {
                return len;
            }
        }
    }
    return 0;
}

/* ================= 提交版本: 滑动窗口 O(n) ================= */
/*
 * 思路: 用窗口 [left, right] 表示"当前正在看的子串", 窗口内始终无重复字符。
 *  - right 每次右移一格, 把 s[right] 加进窗口;
 *  - 如果 s[right] 在窗口里出现过(上次位置 last[c] >= left), 说明冲突,
 *    就把 left 直接跳到 last[c] + 1(跳过所有仍然包含这个重复的起点);
 *  - 每一步用窗口长度 right-left+1 更新答案。
 * left 只增不减 => 每个下标最多被 left、right 各扫过一次 => O(n)。
 */
int lengthOfLongestSubstring(char* s) {
    int last[256];                     /* last[c] = 字符 c 上一次出现的位置 */
    memset(last, -1, sizeof(last));    /* 全置 -1, 表示还没出现过 */

    int left = 0;                      /* 窗口左端(含) */
    int best = 0;                      /* 目前找到的最长长度 */

    for (int right = 0; s[right] != '\0'; right++) {
        unsigned char c = (unsigned char)s[right];   /* 用无符号下标, 避免负索引 */
        if (last[c] >= left) {
            left = last[c] + 1;        /* 窗口里已有 c, 左端跳到它上次位置之后 */
        }
        last[c] = right;

        int len = right - left + 1;    /* 当前窗口长度 */
        if (len > best) {
            best = len;
        }
    }
    return best;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0); // stdout 不缓冲,printf 立刻显示

    /* --- 1. 固定用例 --- */
    char* tests[] = {"abcabac", "pwwkew", "bbbbb", "", "abcde", "abba", "dvdf", "tmmzuxt"};
    int expect[] = {3, 3, 1, 0, 5, 2, 3, 5};
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int t = 0; t < n; t++) {
        int got = lengthOfLongestSubstring(tests[t]);
        printf("\"%-8s\" -> %d (期望 %d) %s\n", tests[t], got, expect[t],
               got == expect[t] ? "OK" : "FAIL");
    }

    /* --- 2. 随机对拍: 小数据上暴力版和滑动窗口必须完全一致 --- */
    srand(20261024);
    char buf[64];
    int bad = 0;
    for (int t = 0; t < 20000; t++) {
        int len = rand() % 30;
        for (int k = 0; k < len; k++) {
            buf[k] = (char)('a' + rand() % 5);
        }
        buf[len] = '\0';
        int a = bruteForce(buf);
        int b = lengthOfLongestSubstring(buf);
        if (a != b) {
            printf("MISMATCH \"%s\": brute=%d window=%d\n", buf, a, b);
            bad = 1;
            break;
        }
    }
    printf("随机对拍 20000 组: %s\n", bad ? "FAIL" : "全部一致");

    /* --- 3. 测速: 同样类型的随机数据, 暴力 vs 滑动窗口 --- */
    printf("\n随机小写字母串的耗时 (n 每翻一倍, 暴力版耗时约 ×4):\n");
    for (int N = 1000; N <= 4000; N *= 2) {
        char* big = malloc(N + 1);
        if (big == NULL) {
            return 1;
        }
        for (int k = 0; k < N; k++) {
            big[k] = (char)('a' + rand() % 26);
        }
        big[N] = '\0';
        clock_t t0 = clock();
        int r = bruteForce(big);
        double ms = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
        printf("  暴力版     n=%5d  答案=%3d  %8.1f ms\n", N, r, ms);
        free(big);
    }

    /* n=2000000(比力扣上限 5e4 还大 40 倍), 暴力版会跑到天荒地老, 只测滑动窗口 */
    int N = 2000000;
    char* big = malloc(N + 1);
    if (big == NULL) {
        return 1;
    }
    for (int k = 0; k < N; k++) {
        big[k] = (char)('a' + rand() % 26);
    }
    big[N] = '\0';

    clock_t t0 = clock();
    int r3 = lengthOfLongestSubstring(big);
    double ms3 = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("  滑动窗口   n=%d  答案=%3d  %8.1f ms\n", N, r3, ms3);

    free(big);
    getchar();
    return 0;
}
