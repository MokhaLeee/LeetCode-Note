#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * 建表时就分组：
 * 每个 hash 节点 = 一个异位词组（存签名 + 该组的串列表）
 * 插入时：算签名 → 找桶 → 链上 strcmp → 命中则追加，否则新建节点
 */
#define M3_HASH_SIZE 1024

struct hash_node {
	char *sig;
	char **strs;
	int count;
	int cap;
	struct hash_node *next;
};

static int m3_hash_key(const char *sig)
{
	unsigned val = 0;
	int i;

	for (i = 0; sig[i] != '\0'; i++)
		val = val * 31 + (uint8_t)sig[i];

	return (int)(val % M3_HASH_SIZE);
}

static char *make_sig(const char *s)
{
	size_t len = strlen(s);
	char *sig = malloc(len + 1);
	size_t i, j;

	memcpy(sig, s, len + 1);

	/* 简单排序，保证同异位词签名一致 */
	for (i = 0; i < len; i++) {
		for (j = i + 1; j < len; j++) {
			if ((uint8_t)sig[i] > (uint8_t)sig[j]) {
				char tmp = sig[i];

				sig[i] = sig[j];
				sig[j] = tmp;
			}
		}
	}
	return sig;
}

static void group_append(struct hash_node *node, char *str)
{
	if (node->count >= node->cap) {
		node->cap = node->cap ? node->cap * 2 : 4;
		node->strs = realloc(node->strs, node->cap * sizeof(char *));
	}
	node->strs[node->count++] = str;
}

static struct hash_node *hash_find_or_insert(struct hash_node **table, char *sig)
{
	int key = m3_hash_key(sig);
	struct hash_node *node;

	for (node = table[key]; node; node = node->next) {
		if (strcmp(node->sig, sig) == 0) {
			free(sig); /* 已有同签名组，签名可丢 */
			return node;
		}
	}

	/* 新组：签名挂到节点上 */
	node = malloc(sizeof(*node));
	node->sig = sig;
	node->strs = NULL;
	node->count = 0;
	node->cap = 0;
	node->next = table[key];
	table[key] = node;
	return node;
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
			/* strs 已交给返回结果，这里不再 free */
			free(tmp);
		}
	}
	free(table);
}

char ***group_anagrams2(char **strs, int str_size, int *ret_size,
		       int **return_col_size)
{
	int i, b;
	struct hash_node **table;
	char ***groups;
	int *col_size;
	int n_groups = 0;

	table = calloc(M3_HASH_SIZE, sizeof(*table));

	/* 一遍：边建表边分组 */
	for (i = 0; i < str_size; i++) {
		char *sig = make_sig(strs[i]);
		struct hash_node *node = hash_find_or_insert(table, sig);

		group_append(node, strs[i]);
	}

	/* 统计组数，导出结果 */
	for (b = 0; b < M3_HASH_SIZE; b++) {
		struct hash_node *node;

		for (node = table[b]; node; node = node->next)
			n_groups++;
	}

	groups = malloc(n_groups * sizeof(*groups));
	col_size = malloc(n_groups * sizeof(*col_size));
	n_groups = 0;

	for (b = 0; b < M3_HASH_SIZE; b++) {
		struct hash_node *node;

		for (node = table[b]; node; node = node->next) {
			groups[n_groups] = node->strs;
			col_size[n_groups] = node->count;
			n_groups++;
		}
	}

	free_hash_table(table);

	*ret_size = n_groups;
	*return_col_size = col_size;
	return groups;
}
