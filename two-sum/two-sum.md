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

```c
static int *method2(const int *nums, int size, int target, int *return_size)
{
	int i, l, r;
	struct m2_pair *arr;
	int *result;

	arr = malloc(sizeof(struct m2_pair) * size);

	for (i = 0; i < size; i++) {
		arr[i].val = nums[i];
		arr[i].idx = i;
	}

	method2_sort(arr, size);

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
```

# 思路3: hash

先回看双重循环的策略: 其本质是在对每个外层 i 的循环中, 查找剩下的数组中有没有哪个元素满足: `arr[j] == target - arr[i]`.

既然如此, 那么就将每个遍历到的元素都建立一个 `target-unit` 的 hash 好了.但是 hash 的构建是有说法的: 计算 key 的方法可以非常简单, 例如此处可以利用 `key = val % HASH_SIZE` 的方法获得 key，但是需要考虑碰撞问题, 那么问题也很简单, 构建的 hash table 并非一个数组, 而是一个链表头数组. 例如:

```
# HASH_SIZE=1000

hash table:
[key=0] -> [val=1000] -> [val=2000] -> NULL
[key=100] -> [val=1100] -> [val=2100] -> NULL
[key=101: NULL]
```

其对应 hash table 的构建策略就是:

```c
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
```

从而利用这个 hash 可以实现 O(n) 级别的查表:

```c
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
```
