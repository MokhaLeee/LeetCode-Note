#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * 两阶段：
 * 1) 先给所有串建 hash（按签名入桶）
 * 2) 再扫每个桶，把签名相同的串收成一组
 */
#define M3_HASH_SIZE 1024

struct hash_node {
	const char *str;
	char *sig;
	int key;
	int idx;
	int used;
	struct hash_node *next;
};

static int cmp_char(const void *a, const void *b)
{
	return *(const unsigned char *)a - *(const unsigned char *)b;
}

static char *make_sig(const char *s)
{
	size_t len = strlen(s);
	char *sig = malloc(len + 1);

	memcpy(sig, s, len + 1);
	qsort(sig, len, 1, cmp_char);
	return sig;
}

static int m3_hash_key(const char *sig)
{
	unsigned val = 0;
	int i;

	for (i = 0; sig[i] != '\0'; i++)
		val = val * 31 + (uint8_t)sig[i];

	return (int)(val % M3_HASH_SIZE);
}

static struct hash_node **generate_hash_table(char **strs, int str_size)
{
	int i;
	struct hash_node **table = calloc(M3_HASH_SIZE, sizeof(*table));

	for (i = 0; i < str_size; i++) {
		struct hash_node *node = malloc(sizeof(*node));
		char *sig = make_sig(strs[i]);
		int key = m3_hash_key(sig);

		node->str = strs[i];
		node->sig = sig;
		node->idx = i;
		node->key = key;
		node->used = 0;

		/* 头插：挂到 table[key] */
		node->next = table[key];
		table[key] = node;
	}
	return table;
}

static void free_hash_table(struct hash_node **table)
{
	int i;

	for (i = 0; i < M3_HASH_SIZE; i++) {
		struct hash_node *node = table[i];

		while (node) {
			struct hash_node *tmp = node;

			node = node->next;
			free(tmp->sig);
			free(tmp);
		}
	}
	free(table);
}

char ***group_anagrams1(char **strs, int str_size, int *ret_size,
		       int **return_col_size)
{
	int b;
	struct hash_node **hash;
	char ***groups = NULL;
	int *col_size = NULL;
	int n_groups = 0;
	int groups_cap = 0;

	/* ---- 阶段1: 先建完整 hash ---- */
	hash = generate_hash_table(strs, str_size);

	/* ---- 阶段2: 扫每个桶，按签名聚合成组 ---- */
	for (b = 0; b < M3_HASH_SIZE; b++) {
		struct hash_node *anchor;

		for (anchor = hash[b]; anchor; anchor = anchor->next) {
			struct hash_node *cur;
			int count;
			char **row;
			int gidx;

			if (anchor->used)
				continue;

			/* 先数这一组有多少个（同桶且签名相同） */
			count = 0;
			for (cur = anchor; cur; cur = cur->next) {
				if (!cur->used && strcmp(cur->sig, anchor->sig) == 0)
					count++;
			}

			row = malloc(count * sizeof(char *));
			count = 0;
			for (cur = anchor; cur; cur = cur->next) {
				if (!cur->used && strcmp(cur->sig, anchor->sig) == 0) {
					row[count++] = (char *)cur->str;
					cur->used = 1;
				}
			}

			if (n_groups >= groups_cap) {
				groups_cap = groups_cap ? groups_cap * 2 : 8;
				groups = realloc(groups, groups_cap * sizeof(*groups));
				col_size = realloc(col_size, groups_cap * sizeof(*col_size));
			}
			gidx = n_groups++;
			groups[gidx] = row;
			col_size[gidx] = count;
		}
	}

	free_hash_table(hash);

	*ret_size = n_groups;
	*return_col_size = col_size;
	return groups;
}
