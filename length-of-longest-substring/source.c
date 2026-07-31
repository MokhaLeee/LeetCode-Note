#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * 例题: 无重复字符的最长子串
 *
 * 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
 *
 * 示例 1:
 *   输入: s = "abcabcbb"
 *   输出: 3
 *   解释: 无重复字符的最长子串是 "abc"，长度为 3。
 *         "bca" 和 "cab" 也是正确答案。
 *
 * 示例 2:
 *   输入: s = "bbbbb"
 *   输出: 1
 *   解释: 无重复字符的最长子串是 "b"，长度为 1。
 *
 * 示例 3:
 *   输入: s = "pwwkew"
 *   输出: 3
 *   解释: 无重复字符的最长子串是 "wke"，长度为 3。
 *         注意答案必须是子串的长度，"pwke" 是子序列，不是子串。
 *
 * 提示:
 *   - 0 <= s.length <= 5 × 10⁴
 *   - s 由英文字母、数字、符号和空格组成
 */
__attribute_maybe_unused__
static int method1(const char *s)
{
	bool seen[128] = { false };
	int l, r;
	int best, n = strlen(s);

	l = 0;
	best = 0;

	for (r = 0; r < n; r++) {
		int cur_len;
		uint8_t ch_r = s[r];

		while (seen[ch_r] == true) {
			uint8_t ch_l = s[l];

			seen[ch_l] = false;
			l++;
		}

		seen[ch_r] = true;

		cur_len = r - l + 1;
		printf("current: [%d-%d]=%d, best=%d\n", l, r, cur_len, best);
		if (cur_len > best)
			best = cur_len;
	}
	return best;
}

static int length_of_longest_substring(const char *s)
{
	return method1(s);
}

int main(void)
{
	const char *cases[] = { "abcabcbb", "bbbbb", "pwwkew" };
	const int expect[] = { 3, 1, 3 };
	int i;

	for (i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); i++) {
		int ret = length_of_longest_substring(cases[i]);

		printf("s = \"%s\" -> %d (expect %d)%s\n",
		       cases[i], ret, expect[i],
		       ret == expect[i] ? " OK" : " FAIL");
	}

	return 0;
}
