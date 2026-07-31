#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 例题: 找到字符串中所有字母异位词
 *
 * 给定两个字符串 s 和 p，找到 s 中所有 p 的异位词的子串，
 * 返回这些子串的起始索引。不考虑答案输出的顺序。
 *
 * 示例 1:
 *   输入: s = "cbaebabacd", p = "abc"
 *   输出: [0,6]
 *   解释:
 *     起始索引等于 0 的子串是 "cba"，它是 "abc" 的异位词。
 *     起始索引等于 6 的子串是 "bac"，它是 "abc" 的异位词。
 *
 * 示例 2:
 *   输入: s = "abab", p = "ab"
 *   输出: [0,1,2]
 *   解释:
 *     起始索引等于 0 的子串是 "ab"，它是 "ab" 的异位词。
 *     起始索引等于 1 的子串是 "ba"，它是 "ab" 的异位词。
 *     起始索引等于 2 的子串是 "ab"，它是 "ab" 的异位词。
 *
 * 提示:
 *   - 1 <= s.length, p.length <= 3 × 10⁴
 *   - s 和 p 仅包含小写字母
 *
 * 返回数组需由调用方 free；*return_size 为结果个数。
 */
static int *find_anagrams(const char *s, const char *p, int *return_size)
{
	(void)s;
	(void)p;
	*return_size = 0;
	return NULL;
}

static void print_indices(const int *arr, int n)
{
	int i;

	printf("[");
	for (i = 0; i < n; i++) {
		printf("%d", arr[i]);
		if (i + 1 < n)
			printf(",");
	}
	printf("]");
}

static int same_multiset(const int *a, int na, const int *b, int nb)
{
	int cnt[100] = { 0 }; /* 下标范围小，示例够用 */
	int i;

	if (na != nb)
		return 0;

	for (i = 0; i < na; i++) {
		cnt[a[i]]++;
		cnt[b[i]]--;
	}
	for (i = 0; i < 100; i++) {
		if (cnt[i] != 0)
			return 0;
	}
	return 1;
}

int main(void)
{
	struct {
		const char *s;
		const char *p;
		int expect[8];
		int expect_n;
	} cases[] = {
		{ "cbaebabacd", "abc", { 0, 6 }, 2 },
		{ "abab", "ab", { 0, 1, 2 }, 3 },
	};
	int i;

	for (i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); i++) {
		int ret_size = 0;
		int *ret = find_anagrams(cases[i].s, cases[i].p, &ret_size);
		int ok = same_multiset(ret, ret_size, cases[i].expect, cases[i].expect_n);

		printf("s = \"%s\", p = \"%s\" -> ", cases[i].s, cases[i].p);
		print_indices(ret, ret_size);
		printf(" (expect ");
		print_indices(cases[i].expect, cases[i].expect_n);
		printf(")%s\n", ok ? " OK" : " FAIL");

		free(ret);
	}

	return 0;
}
