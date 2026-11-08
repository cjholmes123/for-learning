#include <limits.h>
#include <string.h>

int myAtoi(char *s) {
    if (s == NULL) return 0;
    int i = 0;
    while (s[i] == ' ') i++;            /* 1. 跳过前导空格 */
    int sign = 1;
    if (s[i] == '+' || s[i] == '-') {   /* 2. 处理可选符号 */
        if (s[i] == '-') sign = -1;
        i++;
    }
    int result = 0;
    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        int digit = s[i] - '0';
        /* 3. 先判断再加，防止 result * 10 + digit 溢出 */
        if (result > (INT_MAX - digit) / 10) {
            return sign == 1 ? INT_MAX : INT_MIN;
        }
        result = result * 10 + digit;
    }
    return sign * result;
}












