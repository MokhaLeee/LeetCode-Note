# 例题:两数之和

给定一个整数数组 `nums` 和一个整数目标值 `target`, 请你在该数组中找出和为目标值 `target` 的那两个整数，并返回它们的数组下标。<br>
你可以假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。<br>
你可以按任意顺序返回答案。

示例 1:
> 输入：nums = [2,7,11,15], target = 9<br>
> 输出：[0,1]<br>
> 解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

示例 2:
> 输入：nums = [3,2,4], target = 6<br>
> 输出：[1,2]

示例 3：
> 输入：nums = [3,3], target = 6<br>
> 输出：[0,1]

提示：
- 2 <= nums.length <= 1e4
- -1e9 <= nums[i] <= 1e9
- -1e9 <= target <= 1e9
- 只会存在一个有效答案

# 思路1: 双重循环

```c
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
```

**思路**: 枚举每一对下标 (i, j)（j > i，避免用同一个元素两次），和等于 target 就返回。

**复杂度**: 时间 O(n²)，空间 O(1). 本题 n ≤ 10⁴，能过.

# 思路2: 双指针+排序

先把数组排序，再用左右两个指针从两端往中间夹，找和为 target 的一对。

例如:

`nums = [3, 2, 4], target = 6`: 先排序成 [2, 3, 4], 随后:
```
left → 2          4 ← right
sum = 6 == target  → 找到
```

规则：
- sum < target → left++（需要更大）
- sum > target → right--（需要更小）
- sum == target → 找到
