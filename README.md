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
| 7 | [Reverse Integer](https://leetcode.cn/problems/reverse-integer/) | `7.c` | 逐位弹出,乘加前用 `INT_MAX/10` 预判溢出 | O(log x) | 2026-10-26 |
| 8 | [String to Integer (atoi)](https://leetcode.cn/problems/string-to-integer-atoi/) | `8.c` | 跳过空格 → 取符号 → 逐位累加,累加前判溢出,越界截断 | O(n) | 2026-10-29 |
| 9 | [Palindrome Number](https://leetcode.cn/problems/palindrome-number/) | `9.c` | 取各位到数组,首尾指针比较(不翻转整数,不会溢出) | O(log x) | 2026-11-01 |
| 10 | [Regular Expression Matching](https://leetcode.cn/problems/regular-expression-matching/) | `10.c` | 记忆化 DFS(自顶向下;`*` 分「匹配 0 次」与「吃掉一个字符」两支) | O(mn) | 2026-11-01 |
| 11 | [Container With Most Water](https://leetcode.cn/problems/container-with-most-water/) | `11.c` | 双指针,每步移动较矮的一侧 | O(n) | 2026-11-02 |
| 12 | [Integer to Roman](https://leetcode.cn/problems/integer-to-roman/) | `12.c` | 13 组「值-符号」对(含 900/400/90/40/9/4)逐档贪心拼接 | O(1) | 2026-11-04 |
| 13 | [Roman to Integer](https://leetcode.cn/problems/roman-to-integer/) | `13.c` | 按降序表的下标比较,小值在大值前则减 | O(n) | 2026-11-08 |
| 14 | [Longest Common Prefix](https://leetcode.cn/problems/longest-common-prefix/) | `14.c` | 纵向扫描:以最短串为界,逐列比较所有字符串 | O(S),S 为所有字符串长度之和 | 2026-11-08 |
| 15 | [3Sum](https://leetcode.cn/problems/3sum/) | `15.c` | 排序 + 双指针:固定首数后左右夹逼,三处跳过重复值(`15_fixed.c` 为动态扩容的参考实现) | O(n²) | 2026-11-11 |
| 16 | [3Sum Closest](https://leetcode.cn/problems/3sum-closest/) | `16.c` | 排序 + 双指针:维护与 `target` 的最小差值(`16_faster.c` 另加区间端点剪枝) | O(n²) | 2026-11-12 |
| 17 | [Letter Combinations of a Phone Number](https://leetcode.cn/problems/letter-combinations-of-a-phone-number/) | `17.c` | 回溯(DFS):逐位选字母,叶子处复制路径入结果;空串返回空列表 | O(n·4ⁿ) | 2026-11-12 |

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

第 8 题附有行为检查脚本 `8_check.c`(覆盖前后空格、正负号、非法起始字符、`INT_MAX`/`INT_MIN` 截断等边界),与 `8.c` 一起编译:

```bash
gcc -std=c11 -Wall -Wextra 8_check.c 8.c -o 8_check
./8_check
```

第 11 题用到了 `fmin`/`fmax`,单独编译时需链接数学库(`-lm`)。

第 15 题另附三个文件:`15_fixed.c` 是参考实现(动态扩容 + 三处去重 + 内存不足时的兜底),`15_check.c` 拿暴力解当标尺做随机对拍,`15_faster.c` 对比「标准双指针 / 加剪枝 / 只枚举不同值」几种写法的耗时:

```bash
gcc -Wall -Wextra -O2 15_fixed.c 15_check.c -o 15_check.exe
./15_check
gcc -Wall -Wextra -O2 15_faster.c -o 15_faster
./15_faster
```

第 16 题附有 `16_fixed.c`(修正 `16.c` 里 `res` 被逐轮覆盖的问题)、`16_test.c`(与暴力三重循环对拍)、`16_faster.c`(剪枝加速版)和 `16_bench.c`(基准测试;用到 `<windows.h>` 的 QPC 计时,Windows 专用):

```bash
gcc -std=c11 -Wall -Wextra 16_test.c 16.c -o 16_test
./16_test
gcc -O2 -std=c11 -Wall -Wextra 16_bench.c 16_fixed.c 16_faster.c -o 16_bench
./16_bench
```

第 17 题附有自检脚本 `17_check.c`(覆盖 1~4 位输入、空串和 `4ⁿ` 上界,并逐条检查字符串结尾的 `'\0'`、字母是否来自对应按键、答案是否重复):

```bash
gcc -std=c11 -Wall -Wextra 17_check.c 17.c -o 17_check
./17_check
```

## 说明

- 只提交源码,编译产物(`.exe`、`.o` 等)由 `.gitignore` 排除
- 每题追求「先能过,再优化」:暴力解法先跑通,再考虑哈希表等优化
