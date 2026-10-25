# for-learning

LeetCode 刷题记录,用 C 语言实现。

## 题目列表

| # | 题目 | 文件 | 解法 | 时间复杂度 | 完成日期 |
|---|---|---|---|---|---|
| 1 | [Two Sum](https://leetcode.cn/problems/two-sum/) | `1.c` | 暴力枚举 | O(n²) | 2026-10-23 |
| 2 | [Add Two Numbers](https://leetcode.cn/problems/add-two-numbers/) | `2.c` | 链表模拟竖式加法(带进位) | O(max(m,n)) | 2026-10-24 |
| 3 | [Longest Substring Without Repeating Characters](https://leetcode.cn/problems/longest-substring-without-repeating-characters/) | `3.c` | 滑动窗口 + 数组记录上次出现位置 | O(n) | 2026-10-24 |
| 4 | [Median of Two Sorted Arrays](https://leetcode.cn/problems/median-of-two-sorted-arrays/) | `4.c` | 二分找第 k 小(`findMedianSortedArrays1` 为归并对照版) | O(log(m+n)) | 2026-10-24 |
| 5 | [Longest Palindromic Substring](https://leetcode.cn/problems/longest-palindromic-substring/) | `5.c` | 中心扩展(奇数/偶数两种回文) | O(n²) | 2026-10-25 |
| 6 | [Zigzag Conversion](https://leetcode.cn/problems/zigzag-conversion/) | `6.c` | 按行号收集(方向变量控制上下折返) | O(n·numRows) | 2026-10-25 |

## 编译运行

单题编译:

```bash
gcc 1.c -o 1
./1
```

开启全部警告(推荐,能提前发现未初始化变量等问题):

```bash
gcc -Wall -Wextra -std=c11 1.c -o 1
```

第 4 题附有对拍脚本 `4_test.c`(内含归并、二分切分和 `4.c` 的三方对照),需要链接数学库:

```bash
gcc -std=c11 -Wall -Wextra 4_test.c -o 4_test -lm
./4_test
```

## 说明

- 只提交源码,编译产物(`.exe`、`.o` 等)由 `.gitignore` 排除
- 每题追求「先能过,再优化」:暴力解法先跑通,再考虑哈希表等优化
