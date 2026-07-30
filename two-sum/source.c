#include <stdio.h>
#include <stdlib.h>

/**
 * 方案1: 双重循环:
 * 枚举每一对下标 (i, j)（j > i，避免用同一个元素两次），和等于 target 就返回。
 */
__attribute_maybe_unused__
static int *method1(const int *nums, int size, int target, int *return_size)
{
	int i, j;
	int *result;

	result = (int *)malloc(2 * sizeof(int));
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
	free(result);
	return NULL;
}

/**
 * 方案2: 双指针+排序:
 * 先把数组排序，再用左右两个指针从两端往中间夹
 */
struct m2_pair {
    int val;
    int idx;
};

static void method2_sort(struct m2_pair *arr, int num_size)
{
	int i, j;

	for (i = 0; i < num_size; i++) {
		for (j = i + 1; j < num_size; j++) {
			if (arr[i].val >= arr[j].val) {
				struct m2_pair tmp = arr[i];

				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

__attribute_maybe_unused__
static int *method2(const int *nums, int size, int target, int *return_size)
{
	int i, l, r;
	struct m2_pair *arr;
	int *result;

	/**
	 * step1: sort the array
	 */
	arr = malloc(sizeof(struct m2_pair) * size);

	for (i = 0; i < size; i++) {
		arr[i].val = nums[i];
		arr[i].idx = i;
	}
	method2_sort(arr, size);

	/**
	 * step2: check the sorted array
	 */
	result = (int *)malloc(2 * sizeof(int));

	l = 0, r = size - 1;
	while (l < r) {
		long sum = arr[l].val + arr[r].val;

		if (sum == target) {
			/* match! */
			result[0] = arr[l].idx;
			result[1] = arr[r].idx;
			free(arr);

			*return_size = 2;
			return result;
		}

		if (sum < target)	
			l++;
		else
			r--;
	}

	free(arr);
	*return_size = 0;
	free(result);
	return NULL;
}

static int *two_sum(const int *nums, int size, int target, int *return_size)
{
	// return method1(nums, size, target, return_size);
	return method2(nums, size, target, return_size);
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
