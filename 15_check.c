/* ============================================================
 * 15 题对拍程序：拿暴力解当标尺，随机数据比对
 * 编译: gcc -Wall -Wextra -O2 15_fixed.c 15_check.c -o 15_check.exe
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** threeSum(int *nums, int numsSize, int *returnSize, int **returnColumnSizes);

typedef struct { int v[3]; } Tri;

static int tri_cmp(const void *a, const void *b) {
    const Tri *p = (const Tri *)a, *q = (const Tri *)b;
    for (int t = 0; t < 3; t++)
        if (p->v[t] != q->v[t]) return p->v[t] > q->v[t] ? 1 : -1;
    return 0;
}

static void sort3(int *x) {
    if (x[0] > x[1]) { int t = x[0]; x[0] = x[1]; x[1] = t; }
    if (x[1] > x[2]) { int t = x[1]; x[1] = x[2]; x[2] = t; }
    if (x[0] > x[1]) { int t = x[0]; x[0] = x[1]; x[1] = t; }
}

/* 暴力: 枚举所有三元组, 排序后去重 */
static int brute(const int *a, int n, Tri *out) {
    int c = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++)
                if (a[i] + a[j] + a[k] == 0) {
                    out[c].v[0] = a[i]; out[c].v[1] = a[j]; out[c].v[2] = a[k];
                    sort3(out[c].v);
                    c++;
                }
    qsort(out, (size_t)c, sizeof(Tri), tri_cmp);
    int w = 0;
    for (int i = 0; i < c; i++)
        if (w == 0 || tri_cmp(&out[w - 1], &out[i]) != 0) out[w++] = out[i];
    return w;
}

static int check(const int *a, int n) {
    int *copy = (int *)malloc(sizeof(int) * (size_t)(n ? n : 1));
    if (n) memcpy(copy, a, sizeof(int) * (size_t)n);

    int rs = 0;
    int *cols = NULL;
    int **res = threeSum(copy, n, &rs, &cols);

    if (rs > 0 && cols == NULL) { puts("  returnColumnSizes 没分配"); return 0; }
    for (int i = 0; i < rs; i++)
        if (cols[i] != 3) { puts("  某一组的长度不是 3"); return 0; }

    Tri got[256];
    for (int i = 0; i < rs; i++) {
        got[i].v[0] = res[i][0]; got[i].v[1] = res[i][1]; got[i].v[2] = res[i][2];
        if (res[i][0] + res[i][1] + res[i][2] != 0) { puts("  某一组和不为 0"); return 0; }
        sort3(got[i].v);
    }
    qsort(got, (size_t)rs, sizeof(Tri), tri_cmp);

    Tri exp[256];
    int e = brute(a, n, exp);

    if (rs != e) { printf("  数量不符: 你的 %d, 期望 %d\n", rs, e); return 0; }
    for (int i = 0; i < e; i++)
        if (tri_cmp(&got[i], &exp[i]) != 0) {
            printf("  第 %d 组不符: 你 [%d,%d,%d], 期望 [%d,%d,%d]\n", i,
                   got[i].v[0], got[i].v[1], got[i].v[2],
                   exp[i].v[0], exp[i].v[1], exp[i].v[2]);
            return 0;
        }
    return 1;
}

int main(void) {
    struct { int n; int a[8]; const char *name; } cases[] = {
        {6, {-1, 0, 1, 2, -1, -4}, "题目示例"},
        {3, {0, 0, 0},             "三个 0"},
        {4, {0, 0, 0, 0},          "四个 0 (去重考验)"},
        {4, {-1, -1, 0, 1},        "i 去重考验"},
        {3, {1, 2, 3},             "全正数 (应无解)"},
        {3, {-1, -2, -3},          "全负数 (应无解)"},
        {0, {0},                   "空数组"},
        {2, {0, 0},                "不足三个"},
    };
    int total = (int)(sizeof(cases) / sizeof(cases[0]));
    int pass = 0;

    puts("=== 固定用例 ===");
    for (int i = 0; i < total; i++) {
        int ok = check(cases[i].a, cases[i].n);
        printf("%s %-16s\n", ok ? "[通过]" : "[失败]", cases[i].name);
        pass += ok;
    }

    puts("\n=== 随机对拍 (值域 -5..5, n=3..10, 5000 组) ===");
    srand(20261111);
    int bad = 0;
    for (int t = 0; t < 5000; t++) {
        int n = 3 + rand() % 8;
        int a[10];
        for (int i = 0; i < n; i++) a[i] = rand() % 11 - 5;
        if (!check(a, n)) {
            printf("  抓到反例: n=%d [", n);
            for (int i = 0; i < n; i++) printf("%d%s", a[i], i + 1 < n ? "," : "");
            puts("]");
            bad++;
            if (bad >= 3) break;
        }
    }
    printf("随机对拍: %s\n", bad == 0 ? "5000 组全部通过" : "有失败");

    puts("\n=== 题目示例的输出 ===");
    int demo[] = {-1, 0, 1, 2, -1, -4};
    int rs = 0; int *cols = NULL;
    int **res = threeSum(demo, 6, &rs, &cols);
    printf("returnSize = %d\n", rs);
    for (int i = 0; i < rs; i++)
        printf("  [%d,%d,%d]\n", res[i][0], res[i][1], res[i][2]);

    printf("\n固定用例 %d/%d 通过\n", pass, total);
    return (pass == total && bad == 0) ? 0 : 1;
}
