#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 数字 2~9 对应的字母串，下标 = digits[i] - '2' */
static const char* re[] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
/* 每个数字对应的字母个数：算结果总数、也当本层 for 循环次数 */
static const int a[] = {3, 3, 3, 3, 3, 4, 3, 4};

/*
 * 回溯：depth 是"当前填到第几位"，同时也等于已拼好的前缀长度。
 * buf 是唯一一份路径（长度 n+1），所有递归层共用；
 * 第 depth 位永远写在 buf[depth]，下一轮直接覆盖，所以不需要显式撤销。
 * 走满 n 位就复制一份收进结果数组，结果下标由共享的 cnt 统一分配。
 */
static void dfs(const char* digits, int n, int depth, char* buf, char** res, int* cnt) {
    if (depth == n) {                                    /* 前缀凑满 n 位：得到一个答案 */
        char* s = (char*)malloc(sizeof(char) * (n + 1));
        memcpy(s, buf, n + 1);                           /* 必须复制，连结尾的 '\0' 一起 */
        res[(*cnt)++] = s;
        return;
    }
    int d = digits[depth] - '2';
    for (int i = 0; i < a[d]; i++) {                     /* 本层把该数字的字母都试一遍 */
        buf[depth] = re[d][i];                           /* 写在"第 depth 位"，不是往后追加 */
        dfs(digits, n, depth + 1, buf, res, cnt);
    }
}

char** letterCombinations(char* digits, int* returnSize) {
    int l = (int)strlen(digits);
    if (l == 0) {                                        /* 空串返回 []，不是 [""] */
        *returnSize = 0;
        return NULL;
    }

    int pi = 1;                                          /* 结果总数 = 每位字母数的乘积 */
    for (int i = 0; i < l; i++) {
        pi *= a[digits[i] - '2'];
    }

    char** res = (char**)malloc(sizeof(char*) * pi);
    char* buf = (char*)malloc(sizeof(char) * (l + 1));
    if (res == NULL || buf == NULL) {
        *returnSize = 0;
        return NULL;
    }
    buf[l] = '\0';                                       /* 终止符只在这里设一次 */

    int cnt = 0;
    dfs(digits, l, 0, buf, res, &cnt);
    free(buf);                                           /* buf 只是临时脚手架 */

    *returnSize = cnt;                                   /* 出参：必须写回调用方的变量 */
    return res;
}
