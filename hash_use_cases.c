/* ============================================================
 * 同一个哈希表，存 4 种不同的东西 -> 4 种完全不同的解法
 * 编译: gcc -Wall -Wextra -O2 hash_use_cases.c -o hash_use_cases
 * ============================================================ */
#include <stdio.h>
#include <string.h>

/* ================== 通用小哈希表（开放寻址，抄 hash_open_addr.c） ================== */
#define CAP 100003          /* 质数, 要比可能的数据量大不少 */

typedef struct {
    int key;
    int val;
    int used;
} Slot;

static Slot T[CAP];

static int H(int k) {
    return (int)(((unsigned)k * 2654435761u) % CAP);   /* 乘个大奇数再取模, 分布更散 */
}

static void mclear(void) {
    memset(T, 0, sizeof T);
}

static void mput(int k, int v) {
    int h = H(k);
    while (T[h].used && T[h].key != k) h = (h + 1) % CAP;
    T[h].used = 1;
    T[h].key = k;
    T[h].val = v;
}

static int mget(int k) {          /* 返回 value; -1 表示 key 不存在 */
    int h = H(k);
    while (T[h].used) {
        if (T[h].key == k) return T[h].val;
        h = (h + 1) % CAP;
    }
    return -1;
}

static int mhas(int k) {          /* 只问"在不在", 不关心 value */
    int h = H(k);
    while (T[h].used) {
        if (T[h].key == k) return 1;
        h = (h + 1) % CAP;
    }
    return 0;
}

/* ==========================================================
 * 场景 1（LeetCode 1 两数之和）：存 "值 -> 下标"
 * 内层循环的 O(n) 查找被换成 O(1) 查表
 * ========================================================== */
static void two_sum(int *a, int n, int target) {
    mclear();
    for (int i = 0; i < n; i++) {
        int j = mget(target - a[i]);       /* 之前见过 target - a[i] 吗? 下标是几? */
        if (j >= 0) {
            printf("  a[%d]+a[%d] = %d+%d = %d\n", j, i, a[j], a[i], target);
            return;
        }
        mput(a[i], i);                     /* 先查再存: 否则自己会配到自己 */
    }
    printf("  无解\n");
}

/* ==========================================================
 * 场景 2（LeetCode 128 最长连续序列）：存 "值 -> 在不在"
 * 注意: 只存了一个 1, 但省掉了一整个排序
 * ========================================================== */
static int longest_consecutive(int *a, int n) {
    mclear();
    for (int i = 0; i < n; i++) mput(a[i], 1);

    int best = 0;
    for (int i = 0; i < n; i++) {
        if (mhas(a[i] - 1)) continue;      /* x-1 在 => x 不是起点, 跳过 */
        int len = 1, x = a[i];
        while (mhas(x + 1)) { x++; len++; }
        if (len > best) best = len;
    }
    return best;
}

/* ==========================================================
 * 场景 3（LeetCode 560 和为 K 的子数组）：存 "前缀和 -> 出现次数"
 * 这是最不容易想到、也最值钱的一种用法
 * ========================================================== */
static int subarray_sum(int *a, int n, int k) {
    mclear();
    mput(0, 1);                    /* 空前缀, 少了这一句会漏掉从头开始的答案 */
    int pre = 0, cnt = 0;
    for (int i = 0; i < n; i++) {
        pre += a[i];
        if (mhas(pre - k)) cnt += mget(pre - k);   /* 有多少个前缀和等于 pre-k */
        int c = mget(pre);
        mput(pre, c < 0 ? 1 : c + 1);              /* 注意是"次数+1", 不是覆盖 */
    }
    return cnt;
}

/* ==========================================================
 * 场景 4（LeetCode 242 有效的字母异位词）：这里根本不需要哈希表
 * key 值域只有 26 个连续整数, 直接用数组当表, 比任何哈希表都快
 * ========================================================== */
static int is_anagram(const char *s, const char *t) {
    int cnt[26] = {0};             /* 值域小 -> 数组打表 */
    if (strlen(s) != strlen(t)) return 0;
    for (int i = 0; s[i]; i++) { cnt[s[i] - 'a']++; cnt[t[i] - 'a']--; }
    for (int i = 0; i < 26; i++) if (cnt[i] != 0) return 0;
    return 1;
}

int main(void) {
    int a1[] = {2, 7, 11, 15};
    puts("场景1 两数之和 (存 值->下标):");
    two_sum(a1, 4, 9);
    int a2[] = {3, 3};
    two_sum(a2, 2, 6);

    int a3[] = {100, 4, 200, 1, 3, 2};
    printf("\n场景2 最长连续序列 (存 值->在不在): %d\n", longest_consecutive(a3, 6));

    int a4[] = {1, 1, 1};
    printf("\n场景3 和为K的子数组 (存 前缀和->次数): [1,1,1], k=2 -> %d\n", subarray_sum(a4, 3, 2));
    int a5[] = {1, -1, 0};
    printf("                                     [1,-1,0], k=0 -> %d\n", subarray_sum(a5, 3, 0));
    puts("  (有负数, 所以窗口法失效, 必须用哈希表)");

    printf("\n场景4 字母异位词 (值域小, 用数组就够了): anagram/nagaram = %d\n",
           is_anagram("anagram", "nagaram"));
    return 0;
}
