# for-learning

LeetCode 刷题记录,用 C 语言实现。

## 题目列表

| # | 题目 | 文件 | 解法 | 时间复杂度 | 完成日期 |
|---|---|---|---|---|---|
| 1 | [Two Sum](https://leetcode.cn/problems/two-sum/) | `1.c` | 暴力枚举 | O(n²) | 2026-10-23 |

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

## 说明

- 只提交源码,编译产物(`.exe`、`.o` 等)由 `.gitignore` 排除
- 每题追求「先能过,再优化」:暴力解法先跑通,再考虑哈希表等优化
