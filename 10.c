#include <string.h>//这题真不会，下面我也看不懂，希望以后能会
#include <stdbool.h>
#define MAXN 32
static int memo[MAXN][MAXN];   /* -1 未算, 0 假, 1 真 */

static bool dfs_memo(const char *s, const char *p, int i, int j) {
    if (memo[i][j] != -1)
        return memo[i][j] == 1;

    bool ans;
    if (p[j] == '\0') {
        ans = (s[i] == '\0');
    } else {
        bool first = (s[i] != '\0') && (p[j] == s[i] || p[j] == '.');
        if (p[j + 1] == '*')
            ans = dfs_memo(s, p, i, j + 2) || (first && dfs_memo(s, p, i + 1, j));
        else
            ans = first && dfs_memo(s, p, i + 1, j + 1);
    }

    memo[i][j] = ans ? 1 : 0;
    return ans;
}

bool isMatch(char *s, char *p) {
    memset(memo, -1, sizeof memo);   /* int 数组按字节填 0xFF,正好是 -1 */
    return dfs_memo(s, p, 0, 0);
}