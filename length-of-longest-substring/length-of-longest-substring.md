# 例题:无重复字符的最长子串

给定一个字符串 `s`，请你找出其中不含有重复字符的最长子串的长度。

示例 1:
> 输入: s = "abcabcbb"<br>
> 输出: 3<br>
> 解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。注意 "bca" 和 "cab" 也是正确答案。

示例 2:
> 输入: s = "bbbbb"<br>
> 输出: 1<br>
> 解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。

示例 3:
> 输入: s = "pwwkew"<br>
> 输出: 3<br>
> 解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。<br>
> 请注意，你的答案必须是子串的长度，"pwke" 是一个子序列，不是子串。

提示：
- 0 <= s.length <= 5 × 10⁴
- `s` 由英文字母、数字、符号和空格组成

---

核心观察：用窗口 `[left, right]` 表示当前无重复子串。`right` 右扩纳入新字符；若新字符已在窗口内，则 `left` 右收缩到把旧的那个踢掉。窗口始终合法，最长长度在扩缩过程中更新。

# 思路1: 滑动窗口 + 集合

对应 `source.c`。

**变量含义**：
- `seen[128]`：集合，标记当前窗口里有哪些字符（ASCII）
- `left` / `right`：窗口左右端
- `best`：历史最长长度

**不变量**：每次更新完后，`[left, right]` 内字符互不重复，且 `seen[c] == true` 当且仅当 `c` 在窗口里。

**步骤**：对每个新右端字符 `c = s[right]`：

1. 若 `seen[c]` 已是 true → `c` 已在窗口里，从左边一个个踢：`seen[s[left]] = false`，`left++`，直到旧的那个 `c` 被踢掉
2. `seen[c] = true` → 把新字符纳入窗口
3. 用 `right - left + 1` 更新 `best`

注意：`while` 踢的不只是重复的那个字符，中间被越过的字符也会清掉标记——它们本来就不在新窗口里了。

例如 `s = "pwwkew"`：

```
right→  p   w   w         k   e   w
窗口:  [p] [pw] [w]      [wk][wke][kew]
              ↑ 遇到第2个w，left从0收到2
                              ↑ 遇到第2个w，left从2收到3
best:   1   2   1→2→3→3
```

`right=2` 时 `c='w'`，`seen['w']` 已 true：
- 先踢 `s[0]='p'` → left=1，窗口剩 `[w]`，`seen['w']` 仍 true
- 再踢 `s[1]='w'` → left=2，`seen['w']=false`，while 结束
- 再标记当前 `w`，窗口变成 `[w]`（下标 2）

```c
static int length_of_longest_substring(const char *s)
{
	bool seen[128] = { false };
	int left = 0, right;
	int best = 0;
	int n = (int)strlen(s);

	for (right = 0; right < n; right++) {
		unsigned char c = (unsigned char)s[right];

		/* 窗口内已有 c：不断收缩左端，直到把旧的 c 踢出去 */
		while (seen[c]) {
			seen[(unsigned char)s[left]] = false;
			left++;
		}

		seen[c] = true;

		if (right - left + 1 > best)
			best = right - left + 1;
	}

	return best;
}
```

**细节**：
- `unsigned char`：`char` 可能有符号，当索引用可能越界
- `seen[128]`：题目字符在 ASCII 内，布尔数组即可当集合；字符集更大时再换哈希表

**复杂度**：`left` / `right` 都只向右走，每个下标最多进/出一次，时间 O(n)；空间 O(字符集)（此处 O(1)）。

# 思路2: 滑动窗口 + 上次出现位置

集合只回答「有没有重复」，不知道在哪，所以冲突时只能 `while` 从左边一个个踢。若再记下每个字符**上次出现的下标**，左端就能一步跳过去。

**变量含义**：
- `last[128]`：字符 → 上次出现下标（初值 `-1` 表示从未出现）。ASCII 下数组即完美哈希；字符集更大时换成真正的 hash map
- `left` / `right` / `best`：同思路1

**步骤**：对每个 `c = s[right]`：

1. 若 `last[c] >= left`（上次出现仍在窗口内）→ `left = last[c] + 1`，一步跳到旧 `c` 右边
2. `last[c] = right`
3. 用 `right - left + 1` 更新 `best`

必须用 `last[c] >= left`（或 `left = max(left, last[c] + 1)`）：`last[c]` 可能已在当前窗口左边（早被挤出去），不能把 `left` 往回拉。

例如 `s = "pwwkew"`（`last` 初值全 `-1`）：

```
right  c   last[c]  left 更新后     窗口        best
0      p   -1       0              [p]         1
1      w   -1       0              [pw]        2
2      w    1       max(0,1+1)=2   [w]         2
3      k   -1       2              [wk]        2
4      e   -1       2              [wke]       3
5      w    2       max(2,2+1)=3   [kew]       3
```

`right=2` 时第二次遇到 `w`：上次在下标 1，直接 `left = 2`，省掉思路1里的 while。

```c
static int length_of_longest_substring(const char *s)
{
	int last[128];
	int left = 0, right, best = 0;
	int n = (int)strlen(s);
	int i;

	for (i = 0; i < 128; i++)
		last[i] = -1;

	for (right = 0; right < n; right++) {
		unsigned char c = (unsigned char)s[right];

		if (last[c] >= left)          /* 重复且仍在窗口内 */
			left = last[c] + 1;

		last[c] = right;

		if (right - left + 1 > best)
			best = right - left + 1;
	}

	return best;
}
```

**为什么更快**：
- vs 暴力：窗口 + 查表把重复检测从反复扫描变成 O(1) 查表，整体 O(n)
- vs 思路1：总复杂度同为 O(n)；差别是集合只知「有没有」，hash 知「在哪里」，冲突时单步 O(1) 跳转，少了内层 while，常数更好

**复杂度**：时间 O(n)，每个下标只处理一次；空间 O(字符集)。

# 对比

| | 思路1 集合 | 思路2 上次出现位置 |
|--|--|--|
| 记什么 | 窗口里有没有 | 上次在哪 |
| 遇重复 | `while` 左移逐个清 | `left = last[c] + 1` 直接跳 |
| 单步 | 冲突时可能多次 while | 固定 O(1) |
| 总时间 | O(n) | O(n) |
