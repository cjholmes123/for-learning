/* 临时检查文件：验证 8.c 的行为（检查完可删除） */
#include <stdio.h>
#include <limits.h>

int myAtoi(char *s);

static void t(const char *name, char *s, int expect) {
    int got = myAtoi(s);
    printf("%-28s expect=%-12d got=%-12d %s\n", name, expect, got,
           got == expect ? "OK" : "<== WRONG");
}

int main(void) {
    char a[] = "42";
    char b[] = "   -42";
    char c[] = "4193 with words";
    char d[] = "words and 987";
    char e[] = "-91283472332";              /* 应截断为 INT_MIN */
    char f[] = "2147483648";                /* 应截断为 INT_MAX */
    char g[] = "  +00123";
    char h[] = "   ";                       /* 全空格 */
    char i[] = "";
    char j[] = "+-12";

    t("[42]", a, 42);
    t("[   -42]", b, -42);
    t("[4193 with words]", c, 4193);
    t("[words and 987]", d, 0);
    t("[-91283472332] clamp", e, INT_MIN);
    t("[2147483648] clamp", f, INT_MAX);
    t("[  +00123]", g, 123);
    t("[   ] spaces only", h, 0);
    t("[] empty", i, 0);
    t("[+-12]", j, 0);
    return 0;
}
