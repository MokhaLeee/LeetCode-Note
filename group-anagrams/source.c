#include <stdio.h>
#include <stdlib.h>

extern char ***group_anagrams1(char **, int, int *, int **);
extern char ***group_anagrams2(char **, int, int *, int **);

static void print_groups(char ***groups, int ret_size, int *col_size)
{
	int i, j;

	printf("[\n");
	for (i = 0; i < ret_size; i++) {
		printf("  [");
		for (j = 0; j < col_size[i]; j++) {
			printf("\"%s\"", groups[i][j]);
			if (j + 1 < col_size[i])
				printf(", ");
		}
		printf("]");
		if (i + 1 < ret_size)
			printf(",");
		printf("\n");
	}
	printf("]\n");
}

static void free_groups(char ***groups, int ret_size, int *col_size)
{
	int i;

	if (groups) {
		for (i = 0; i < ret_size; i++)
			free(groups[i]);
		free(groups);
	}
	free(col_size);
}

int main(void)
{
	char *strs[] = { "eat", "tea", "tan", "ate", "nat", "bat" };
	int str_size = sizeof(strs) / sizeof(strs[0]);
	int ret_size = 0;
	int *col_size = NULL;
	char ***groups;

	groups = group_anagrams1(strs, str_size, &ret_size, &col_size);
	print_groups(groups, ret_size, col_size);
	free_groups(groups, ret_size, col_size);

	return 0;
}
