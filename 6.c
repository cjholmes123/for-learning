#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* convert(char* s, int numRows);

int main(void) {
    char s1[] = "PAYPALISHIRING";
    char s2[] = "PAYPALISHIRING";
    char s3[] = "PAYPALISHIRING";
    char s4[] = "A";

    char* r1 = convert(s1, 3);
    char* r2 = convert(s2, 4);
    char* r3 = convert(s3, 1);
    char* r4 = convert(s4, 1);

    printf("numRows=3: %s  (期望 PAHNAPLSIIGYIR)\n", r1);
    printf("numRows=4: %s  (期望 PINALSIGYAHRPI)\n", r2);
    printf("numRows=1: %s  (期望 PAYPALISHIRING)\n", r3);
    printf("单字符   : %s  (期望 A)\n", r4);

    free(r1);
    free(r2);
    free(r3);
    free(r4);
    return 0;
}

char* convert(char* s, int numRows) {
    int n = (int)strlen(s);
    char* res = (char*)malloc((size_t)n + 1);
    if (res == NULL) return NULL;

    /* numRows 为 1 时就是原串;numRows >= n 时每个字符各占一行,读出来仍是原串。
       顺带处理 n == 0 的情况。 */
    if (numRows <= 1 || numRows >= n) {
        memcpy(res, s, (size_t)n + 1);
        return res;
    }

    int* record = (int*)malloc(sizeof(int) * (size_t)n);
    if (record == NULL) {
        free(res);
        return NULL;
    }

    /* record[i] = s[i] 所在的行号(1 ~ numRows)。
       用一个 direction 变量代替取模判断,上边界和下边界各翻转一次。 */
    int row = 1, direction = 1;
    for (int i = 0; i < n; i++) {
        record[i] = row;
        if (row == numRows) direction = -1;   /* 到底,改向上 */
        else if (row == 1) direction = 1;     /* 到顶,改向下 */
        row += direction;
    }

    int k = 0;
    for (int j = 1; j <= numRows; j++) {
        for (int i = 0; i < n; i++) {
            if (record[i] == j) {
                res[k++] = s[i];
            }
        }
    }
    res[k] = '\0';

    free(record);
    return res;
}
