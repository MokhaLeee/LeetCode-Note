#include <stdio.h>
#include <stdlib.h>

__attribute_maybe_unused__
static int *method1(const int *nums, int size, int target, int *return_size)
{
	int i, j;
	int *result = (int *)malloc(2 * sizeof(int));

	*return_size = 2;

	for (i = 0; i < size; i++) {
		for (j = i + 1; j < size; j++) {
			if ((nums[i] + nums[j]) == target) {
				result[0] = i;
				result[1] = j;
				return result;
			}
		}
	}

	*return_size = 0;
	return result;
}

static int *two_sum(const int *nums, int size, int target, int *return_size)
{
	return method1(nums, size, target, return_size);
}

int main(void)
{
	int return_size;
	int nums[] = { 2, 7, 11, 15 };
	int *ret = two_sum(nums, 4, 9, &return_size);

	if (return_size != 0)
		printf("two-sum ret: %d %d\n", ret[0], ret[1]);
	else
		printf("two-sum error\n");

	if (ret)
		free(ret);
	return 0;
}
