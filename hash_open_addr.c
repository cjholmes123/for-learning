/* ============================================================
 * 哈希表第 1 版：开放寻址法（数组 + 往后找空位）
 * 编译:  gcc -Wall -Wextra -O2 hash_open_addr.c -o hash_open_addr
 * 运行:  ./hash_open_addr
 * ============================================================ */
#include <stdio.h>

#define N 13          /* 桶的个数。习惯取质数，撞车少一点 */

int key[N];           /* 每个桶里放的 key      */
int val[N];           /* 每个桶里放的 value    */
int used[N];          /* 每个桶的状态: 0=空, 1=有数据 */

/* ---- 哈希函数：任意整数 -> 0..N-1 的下标 ---- */
static int H(int k) {
    int h = k % N;
    if (h < 0) h += N;     /* C 里 -7 % 13 == -7，是负数，修正成 6 */
    return h;
}

/* ---- 插入 / 更新 ---- */
void put(int k, int v) {
    int h = H(k);

    while (used[h]) {              /* 这个位置被占了 */
        if (key[h] == k) {         /* 而且正好是同一个 key -> 更新 */
            val[h] = v;
            return;
        }
        h = (h + 1) % N;           /* 不是它，看下一个；到底了就绕回 0 */
    }

    /* 走到这里说明遇到了空桶 */
    used[h] = 1;
    key[h] = k;
    val[h] = v;
}

/* ---- 查找：找到返回值，找不到返回 -1 ---- */
int get(int k) {
    int h = H(k);

    while (used[h]) {              /* 一遇到空桶就停下：说明不存在 */
        if (key[h] == k) return val[h];
        h = (h + 1) % N;
    }
    return -1;
}

/* ---- 把整张表打印出来，肉眼看看它长什么样 ---- */
void dump(const char *title) {
    printf("===== %s =====\n", title);
    for (int i = 0; i < N; i++) {
        if (used[i]) printf("  桶[%2d]  key=%3d   val=%3d\n", i, key[i], val[i]);
    }
}

int main(void) {
    put(8, 100);  put(21, 200);  put(7, 300);  put(34, 400);
    dump("插入 8, 21, 7, 34 之后");

    printf("\nget(21) = %d        <- 第 1 次就命中\n", get(21));
    printf("get(34) = %d        <- 探测了 2 个桶才找到\n", get(34));
    printf("get(9)  = %d         <- 找不到\n", get(9));

    put(21, 999);
    printf("\nput(21, 999) 之后 get(21) = %d   <- 覆盖而不是新增\n", get(21));

    puts("\n(说明: 桶越满, 探测越长; 满了还插会死循环, 所以表要比数据多不少)");
    return 0;
}
