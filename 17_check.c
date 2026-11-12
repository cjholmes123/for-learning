/* 临时检查文件：验证 17.c 的行为（检查完可删除） */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **letterCombinations(char *digits, int *returnSize);

static const char *MAP[8] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

static int fail = 0;

static void report(const char *name, int ok, int expect, int got) {
    printf("%-12s expect=%-4d got=%-4d %s\n", name, expect, got, ok ? "OK" : "<== WRONG");
    if (!ok) fail++;
}

static void release(char **r, int size) {
    if (r == NULL) return;
    for (int i = 0; i < size; i++) free(r[i]);
    free(r);
}

/* 通用检查：个数对、每个串长度对、合法范围内有 '\0'、字母来自对应按键、无重复 */
static void check(const char *digits, int expectCount) {
    int n = (int)strlen(digits);
    char d[16];
    strcpy(d, digits);                     /* 接口参数是非 const 的 char* */
    int size = -1;
    char **r = letterCombinations(d, &size);

    int ok = (size == expectCount);
    if (ok && size == 0 && r != NULL) ok = 0;          /* 空串应返回 NULL */
    if (ok && size > 0 && r == NULL) ok = 0;

    for (int i = 0; ok && i < size; i++) {
        if (memchr(r[i], '\0', (size_t)n + 1) == NULL) { ok = 0; break; }  /* 缺终止符 */
        if ((int)strlen(r[i]) != n) { ok = 0; break; }
        for (int j = 0; j < n; j++) {
            if (strchr(MAP[digits[j] - '2'], r[i][j]) == NULL) { ok = 0; break; }
        }
        for (int j = 0; j < i && ok; j++) {
            if (strcmp(r[i], r[j]) == 0) { ok = 0; break; }               /* 重复答案 */
        }
    }

    report(digits[0] ? digits : "(空串)", ok, expectCount, size);
    if (!ok) {
        for (int i = 0; i < size; i++) printf("    [%d] \"%s\"\n", i, r[i]);
    }
    release(r, size);
}

/* 单独核对 "23" 的 9 个答案和顺序（回溯是深度优先，顺序是确定的） */
static void check_order_23(void) {
    static const char *exp[9] = {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"};
    char d[] = "23";
    int size = -1;
    char **r = letterCombinations(d, &size);

    int ok = (size == 9);
    for (int i = 0; ok && i < 9; i++) ok = (strcmp(r[i], exp[i]) == 0);
    report("\"23\" 顺序", ok, 9, size);
    if (!ok) {
        for (int i = 0; i < size; i++) printf("    [%d] \"%s\"\n", i, r[i]);
    }
    release(r, size);
}

int main(void) {
    check("2", 3);
    check("7", 4);          /* 7 和 9 是 4 个字母 */
    check("9", 4);
    check("22", 9);
    check("23", 9);
    check("79", 16);        /* 4 * 4 */
    check("234", 27);       /* 3 * 3 * 3 */
    check("789", 48);       /* 4 * 3 * 4 */
    check("9999", 256);     /* 上界：4^4 */
    check("", 0);
    check_order_23();

    if (fail == 0) printf("\n全部通过\n");
    else printf("\n有 %d 项失败\n", fail);
    return fail != 0;
}
