#include <stdio.h>
#include <stdlib.h>

/**
 * 思路3: hash
 */
#define M3_HASH_SIZE 1024

struct m3_hash_node {
	int val;
	int idx;
	struct m3_hash_node *next;
};

static int m3_hash_key(int val)
{
	if (val < 0)
		val = -val;

	return val % M3_HASH_SIZE;
}

static struct m3_hash_node **generate_hash_table(const int *nums, int size)
{
	int i;
	struct m3_hash_node **table;

	table = calloc(M3_HASH_SIZE, sizeof(struct m3_hash_node *));

	for (i = 0; i < size; i++) {
		struct m3_hash_node *node = malloc(sizeof(struct m3_hash_node));

		int val = nums[i];
		int key = m3_hash_key(val);

		node->idx = i;
		node->val = val;

		node->next = table[key];
		table[key] = node;
	}
	return table;
}

static void free_hash_table(struct m3_hash_node **table)
{
	int i;

	for (i = 0; i < M3_HASH_SIZE; i++) {
		struct m3_hash_node *node = table[i];

		while (node != NULL) {
			struct m3_hash_node *tmp;

			tmp = node;
			node = node->next;
			free(tmp);
		}
	}
	free(table);
}

static struct m3_hash_node *find_hash(struct m3_hash_node **table, int val, int skip_idx)
{
	struct m3_hash_node *node;
	int key = m3_hash_key(val);

	for (node = table[key]; node != NULL; node = node->next) {
		if (node->idx == skip_idx)
			continue;

		if (node->val == val)
			return node;
	}
	return NULL;
}

static int *method3(const int *nums, int size, int target, int *return_size)
{
	int i;
	struct m3_hash_node **hash;

	/**
	 * step1: generate the hash
	 */
	hash = generate_hash_table(nums, size);

	/**
	 * step2: for each unit, find hash for (target - unit), skip himself.
	 */
	for (i = 0; i < size; i++) {
		int partner = target - nums[i];
		struct m3_hash_node *node = find_hash(hash, partner, i);

		if (node != NULL) {
			int *result = calloc(2, sizeof(int));

			result[0] = i;
			result[1] = node->idx;

			free_hash_table(hash);
			*return_size = 2;
			return result;
		}
	}

	free_hash_table(hash);
	*return_size = 0;
	return NULL;
}

static int *two_sum(const int *nums, int size, int target, int *return_size)
{
	return method3(nums, size, target, return_size);
}

int main(void)
{
	int return_size = 0;
	int nums[] = { 15, 19, 3, 2, 7, 11, 15 };
	int *ret = two_sum(nums, sizeof(nums) / sizeof(*nums), 9, &return_size);

	if (return_size != 0)
		printf("two-sum ret: %d %d\n", ret[0], ret[1]);
	else
		printf("two-sum error\n");

	if (ret)
		free(ret);
	return 0;
}
