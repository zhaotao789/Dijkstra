#include <cassert>
#include <algorithm>
#include <ctime>


// 冒泡排序
void BubbleSort(int* a, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (a[j] > a[j + 1]) {
				std::swap(a[j], a[j + 1]);
			}
		}
	}
}

// 选择排序
void SelectionSort(int* a, int n) {
	for (int i = 0; i < n - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < n; j++) {
			if (a[j] < a[minIndex]) {
				minIndex = j;
			}
		}
		std::swap(a[i], a[minIndex]);
	}
}

// 插入排序
void InsertionSort(int* a, int n) {
	for (int i = 1; i < n; i++) {
		int key = a[i];
		int j = i - 1;
		while (j >= 0 && a[j] > key) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
}

// 希尔排序
void ShellSort(int* a, int n) {
	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; i++) {
			int key = a[i];
			int j = i;
			while (j >= gap && a[j - gap] > key) {
				a[j] = a[j - gap];
				j -= gap;
			}
			a[j] = key;
		}
	}
}

// 归并排序
void Merge(int* a, int left, int mid, int right, int* temp) {
	int i = left;
	int j = mid + 1;
	int k = 0;

	while (i <= mid && j <= right) {
		if (a[i] <= a[j]) {
			temp[k++] = a[i++];
		}
		else {
			temp[k++] = a[j++];
		}
	}

	while (i <= mid) {
		temp[k++] = a[i++];
	}

	while (j <= right) {
		temp[k++] = a[j++];
	}

	for (i = 0; i < k; i++) {
		a[left + i] = temp[i];
	}
}

void MergeSort(int* a, int left, int right, int* temp) {
	if (left >= right) {
		return;
	}

	int mid = left + (right - left) / 2;
	MergeSort(a, left, mid, temp);
	MergeSort(a, mid + 1, right, temp);
	Merge(a, left, mid, right, temp);
}

void MergeSort(int* a, int n) {
	int* temp = new int[n];
	MergeSort(a, 0, n - 1, temp);
	delete[] temp;
}

// 快速排序
int Partition(int* a, int low, int high) {
	int pivot = a[low];
	while (low < high) {
		while (low < high && a[high] >= pivot) {
			high--;
		}
		a[low] = a[high];
		while (low < high && a[low] <= pivot) {
			low++;
		}
		a[high] = a[low];
	}
	a[low] = pivot;
	return low;
}

void QuickSort(int* a, int low, int high) {
	if (low < high) {
		int pivotPos = Partition(a, low, high);
		QuickSort(a, low, pivotPos - 1);
		QuickSort(a, pivotPos + 1, high);
	}
}

void QuickSort(int* a, int n) {
	QuickSort(a, 0, n - 1);
}

// 堆排序
void AdjustDown(int* a, int k, int n) {
	int temp = a[k];
	int child = 2 * k + 1;

	while (child < n) {
		if (child + 1 < n && a[child] < a[child + 1]) {
			child++;
		}
		if (temp >= a[child]) {
			break;
		}
		a[k] = a[child];
		k = child;
		child = 2 * k + 1;
	}

	a[k] = temp;
}

void HeapSort(int* a, int n) {
	// 构建最大堆
	for (int i = n / 2 - 1; i >= 0; i--) {
		AdjustDown(a, i, n);
	}

	// 逐步取出堆顶元素，进行调整
	for (int i = n - 1; i > 0; i--) {
		std::swap(a[0], a[i]);
		AdjustDown(a, 0, i);
	}
}

// 计数排序
void CountingSort(int* a, int n) {
	int maxVal = *std::max_element(a, a + n);

	int* count = new int[maxVal + 1]();
	int* result = new int[n];

	for (int i = 0; i < n; i++) {
		count[a[i]]++;
	}

	for (int i = 1; i <= maxVal; i++) {
		count[i] += count[i - 1];
	}

	for (int i = n - 1; i >= 0; i--) {
		result[--count[a[i]]] = a[i];
	}

	for (int i = 0; i < n; i++) {
		a[i] = result[i];
	}

	delete[] count;
	delete[] result;
}

void TestOP()
{
	srand(time(0));
	const int N = 100000;//测试数据个数
	int* a1 = (int*)malloc(sizeof(int) * N);
	int* a2 = (int*)malloc(sizeof(int) * N);
	int* a3 = (int*)malloc(sizeof(int) * N);
	int* a4 = (int*)malloc(sizeof(int) * N);
	int* a5 = (int*)malloc(sizeof(int) * N);
	int* a6 = (int*)malloc(sizeof(int) * N);
	int* a7 = (int*)malloc(sizeof(int) * N);
	int* a8 = (int*)malloc(sizeof(int) * N);
	//给开辟数组赋值
	for (int i = 0; i < N; ++i)
	{
		a1[i] = rand();
		a2[i] = a1[i];
		a3[i] = a1[i];
		a4[i] = a1[i];
		a5[i] = a1[i];
		a6[i] = a1[i];
		a7[i] = a1[i];
		a8[i] = a1[i];
	}
	//数组排序并计算时间花费
	int begin1 = clock();
	InsertionSort(a1, N);
	int end1 = clock();

	int begin2 = clock();
	ShellSort(a2, N);
	int end2 = clock();

	int begin3 = clock();
	SelectionSort(a3, N);
	int end3 = clock();

	int begin4 = clock();
	HeapSort(a4, N);
	int end4 = clock();

	int begin5 = clock();
	QuickSort(a5, 0, N - 1);
	int end5 = clock();

	int begin6 = clock();
	MergeSort(a6, N);
	int end6 = clock();

	int begin7 = clock();
	BubbleSort(a7, N);
	int end7 = clock();

	int begin8 = clock();
	CountingSort(a8, N);
	int end8 = clock();

	//展示数据
	printf("InsertSort:%d\n", end1 - begin1);
	printf("ShellSort:%d\n", end2 - begin2);
	printf("SelectSort:%d\n", end3 - begin3);
	printf("HeapSort:%d\n", end4 - begin4);
	printf("QuickSort:%d\n", end5 - begin5);
	printf("MergeSort:%d\n", end6 - begin6);
	printf("BubbleSort:%d\n", end7 - begin7);
	printf("CountSort:%d\n", end8 - begin8);

	//释放数组
	free(a1);
	free(a2);
	free(a3);
	free(a4);
	free(a5);
	free(a6);
	free(a7);
	free(a8);
}

int main()
{
	TestOP();
	return 0;
}

//冒泡排序（Bubble Sort）：
//特点：相邻元素两两比较，每次将最大（最小）元素冒泡到末尾（开头）。
//优点：实现简单，代码易于理解。
//缺点：效率较低，时间复杂度较高。
//时间复杂度：平均情况和最坏情况下为O(n ^ 2)，最好情况下为O(n)。
//空间复杂度：O(1)。
//适用场景：适用于数据量较小的情况。
//稳定性：稳定排序算法。
//
//选择排序（Selection Sort）：
//特点：每次从未排序部分选择最小（最大）元素放到已排序部分的末尾（开头）。
//优点：实现简单，不占用额外的内存空间。
//缺点：效率较低，时间复杂度较高。
//时间复杂度：平均情况和最坏情况下为O(n ^ 2)，最好情况下为O(n ^ 2)。
//空间复杂度：O(1)。
//适用场景：适用于数据量较小的情况。
//稳定性：不稳定排序算法。
//
//插入排序（Insertion Sort）：
//特点：将未排序部分的元素逐个插入到已排序部分的合适位置。
//优点：在部分有序的情况下效率较高，对于小规模数据或基本有序的数据效果好。
//缺点：对于逆序或基本逆序的数据效率较低。
//时间复杂度：平均情况和最坏情况下为O(n ^ 2)，最好情况下为O(n)。
//空间复杂度：O(1)。
//适用场景：适用于数据量较小或基本有序的情况。
//稳定性：稳定排序算法。
//
//希尔排序（Shell Sort）：
//特点：将待排序序列按一定间隔分组，分组进行插入排序，随着排序进行，间隔逐渐减小，直到间隔为1，完成最后一次插入排序。
//优点：相对于简单插入排序，希尔排序的效率较高。
//缺点：算法的时间复杂度难以精确分析。
//时间复杂度：平均情况和最坏情况下为O(n ^ 1.3)，最好情况下为O(n)。
//空间复杂度：O(1)。
//适用场景：适用于中等规模数据的排序。
//稳定性：不稳定排序算法。
//
//归并排序（Merge Sort）：
//特点：使用分治的思想，将待排序序列不断划分为较小的子序列，然后将子序列合并成有序的序列。
//优点：稳定的排序算法，适用于大规模数据的排序。
//缺点：需要额外的空间来存储归并过程中的临时数据。
//时间复杂度：平均情况、最坏情况和最好情况下均为O(nlogn)。
//空间复杂度：O(n)。
//适用场景：适用于大规模数据
//稳定性：稳定排序算法。
// 
//快速排序（Quick Sort）：
//特点：使用分治的思想，选择一个基准元素，将序列分为两部分，左边的元素小于等于基准，右边的元素大于基准，然后对两部分递归地进行快速排序。
//优点：平均情况下性能较好，实际应用广泛。
//缺点：在最坏情况下可能会退化为O(n ^ 2)的时间复杂度。
//时间复杂度：平均情况下为O(nlogn)，最坏情况下为O(n ^ 2)。
//空间复杂度：平均情况下为O(logn)，最坏情况下为O(n)。
//适用场景：适用于大规模数据的排序。
//稳定性：不稳定排序算法。
//
//堆排序（Heap Sort）：
//特点：通过构建最大堆（升序排序）或最小堆（降序排序）的方式进行排序。
//优点：具有较好的平均时间复杂度，不受输入数据的初始状态影响。
//缺点：需要额外的空间来存储堆。
//时间复杂度：平均情况、最坏情况和最好情况下均为O(nlogn)。
//空间复杂度：O(1)。
//适用场景：适用于大规模数据的排序。
//稳定性：不稳定排序算法。
//
//计数排序（Counting Sort）：
//特点：统计待排序序列中每个元素的出现次数，根据统计结果进行排序。
//优点：适用于元素范围较小且分布均匀的情况，具有较好的时间复杂度。
//缺点：对于元素范围较大的情况，可能会占用较大的内存空间。
//时间复杂度：平均情况、最坏情况和最好情况下均为O(n + k)，其中k为元素的范围。
//空间复杂度：O(n + k)。
//适用场景：适用于元素范围较小且分布均匀的情况。
//稳定性：稳定排序算法。

//InsertSort: Time = 3926
//ShellSort : Time = 18
//SelectSort : Time = 5787
//HeapSort : Time = 15
//QuickSort : Time = 8
		//MergeSort : Time = 15
//BubbleSort : Time = 73135
		//CountSort : Time = 1