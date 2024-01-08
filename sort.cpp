#include <cassert>
#include <algorithm>
#include <ctime>


// ð������
void BubbleSort(int* a, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (a[j] > a[j + 1]) {
				std::swap(a[j], a[j + 1]);
			}
		}
	}
}

// ѡ������
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

// ��������
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

// ϣ������
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

// �鲢����
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

// ��������
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

// ������
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
	// ��������
	for (int i = n / 2 - 1; i >= 0; i--) {
		AdjustDown(a, i, n);
	}

	// ��ȡ���Ѷ�Ԫ�أ����е���
	for (int i = n - 1; i > 0; i--) {
		std::swap(a[0], a[i]);
		AdjustDown(a, 0, i);
	}
}

// ��������
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
	const int N = 100000;//�������ݸ���
	int* a1 = (int*)malloc(sizeof(int) * N);
	int* a2 = (int*)malloc(sizeof(int) * N);
	int* a3 = (int*)malloc(sizeof(int) * N);
	int* a4 = (int*)malloc(sizeof(int) * N);
	int* a5 = (int*)malloc(sizeof(int) * N);
	int* a6 = (int*)malloc(sizeof(int) * N);
	int* a7 = (int*)malloc(sizeof(int) * N);
	int* a8 = (int*)malloc(sizeof(int) * N);
	//���������鸳ֵ
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
	//�������򲢼���ʱ�仨��
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

	//չʾ����
	printf("InsertSort:%d\n", end1 - begin1);
	printf("ShellSort:%d\n", end2 - begin2);
	printf("SelectSort:%d\n", end3 - begin3);
	printf("HeapSort:%d\n", end4 - begin4);
	printf("QuickSort:%d\n", end5 - begin5);
	printf("MergeSort:%d\n", end6 - begin6);
	printf("BubbleSort:%d\n", end7 - begin7);
	printf("CountSort:%d\n", end8 - begin8);

	//�ͷ�����
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

//ð������Bubble Sort����
//�ص㣺����Ԫ�������Ƚϣ�ÿ�ν������С��Ԫ��ð�ݵ�ĩβ����ͷ����
//�ŵ㣺ʵ�ּ򵥣�����������⡣
//ȱ�㣺Ч�ʽϵͣ�ʱ�临�ӶȽϸߡ�
//ʱ�临�Ӷȣ�ƽ�������������ΪO(n ^ 2)����������ΪO(n)��
//�ռ临�Ӷȣ�O(1)��
//���ó�������������������С�������
//�ȶ��ԣ��ȶ������㷨��
//
//ѡ������Selection Sort����
//�ص㣺ÿ�δ�δ���򲿷�ѡ����С�����Ԫ�طŵ������򲿷ֵ�ĩβ����ͷ����
//�ŵ㣺ʵ�ּ򵥣���ռ�ö�����ڴ�ռ䡣
//ȱ�㣺Ч�ʽϵͣ�ʱ�临�ӶȽϸߡ�
//ʱ�临�Ӷȣ�ƽ�������������ΪO(n ^ 2)����������ΪO(n ^ 2)��
//�ռ临�Ӷȣ�O(1)��
//���ó�������������������С�������
//�ȶ��ԣ����ȶ������㷨��
//
//��������Insertion Sort����
//�ص㣺��δ���򲿷ֵ�Ԫ��������뵽�����򲿷ֵĺ���λ�á�
//�ŵ㣺�ڲ�������������Ч�ʽϸߣ�����С��ģ���ݻ�������������Ч���á�
//ȱ�㣺���������������������Ч�ʽϵ͡�
//ʱ�临�Ӷȣ�ƽ�������������ΪO(n ^ 2)����������ΪO(n)��
//�ռ临�Ӷȣ�O(1)��
//���ó�������������������С���������������
//�ȶ��ԣ��ȶ������㷨��
//
//ϣ������Shell Sort����
//�ص㣺�����������а�һ��������飬������в�����������������У�����𽥼�С��ֱ�����Ϊ1��������һ�β�������
//�ŵ㣺����ڼ򵥲�������ϣ�������Ч�ʽϸߡ�
//ȱ�㣺�㷨��ʱ�临�Ӷ����Ծ�ȷ������
//ʱ�临�Ӷȣ�ƽ�������������ΪO(n ^ 1.3)����������ΪO(n)��
//�ռ临�Ӷȣ�O(1)��
//���ó������������еȹ�ģ���ݵ�����
//�ȶ��ԣ����ȶ������㷨��
//
//�鲢����Merge Sort����
//�ص㣺ʹ�÷��ε�˼�룬�����������в��ϻ���Ϊ��С�������У�Ȼ�������кϲ�����������С�
//�ŵ㣺�ȶ��������㷨�������ڴ��ģ���ݵ�����
//ȱ�㣺��Ҫ����Ŀռ����洢�鲢�����е���ʱ���ݡ�
//ʱ�临�Ӷȣ�ƽ��������������������¾�ΪO(nlogn)��
//�ռ临�Ӷȣ�O(n)��
//���ó����������ڴ��ģ����
//�ȶ��ԣ��ȶ������㷨��
// 
//��������Quick Sort����
//�ص㣺ʹ�÷��ε�˼�룬ѡ��һ����׼Ԫ�أ������з�Ϊ�����֣���ߵ�Ԫ��С�ڵ��ڻ�׼���ұߵ�Ԫ�ش��ڻ�׼��Ȼ��������ֵݹ�ؽ��п�������
//�ŵ㣺ƽ����������ܽϺã�ʵ��Ӧ�ù㷺��
//ȱ�㣺�������¿��ܻ��˻�ΪO(n ^ 2)��ʱ�临�Ӷȡ�
//ʱ�临�Ӷȣ�ƽ�������ΪO(nlogn)��������ΪO(n ^ 2)��
//�ռ临�Ӷȣ�ƽ�������ΪO(logn)��������ΪO(n)��
//���ó����������ڴ��ģ���ݵ�����
//�ȶ��ԣ����ȶ������㷨��
//
//������Heap Sort����
//�ص㣺ͨ���������ѣ��������򣩻���С�ѣ��������򣩵ķ�ʽ��������
//�ŵ㣺���нϺõ�ƽ��ʱ�临�Ӷȣ������������ݵĳ�ʼ״̬Ӱ�졣
//ȱ�㣺��Ҫ����Ŀռ����洢�ѡ�
//ʱ�临�Ӷȣ�ƽ��������������������¾�ΪO(nlogn)��
//�ռ临�Ӷȣ�O(1)��
//���ó����������ڴ��ģ���ݵ�����
//�ȶ��ԣ����ȶ������㷨��
//
//��������Counting Sort����
//�ص㣺ͳ�ƴ�����������ÿ��Ԫ�صĳ��ִ���������ͳ�ƽ����������
//�ŵ㣺������Ԫ�ط�Χ��С�ҷֲ����ȵ���������нϺõ�ʱ�临�Ӷȡ�
//ȱ�㣺����Ԫ�ط�Χ�ϴ����������ܻ�ռ�ýϴ���ڴ�ռ䡣
//ʱ�临�Ӷȣ�ƽ��������������������¾�ΪO(n + k)������kΪԪ�صķ�Χ��
//�ռ临�Ӷȣ�O(n + k)��
//���ó�����������Ԫ�ط�Χ��С�ҷֲ����ȵ������
//�ȶ��ԣ��ȶ������㷨��

//InsertSort: Time = 3926
//ShellSort : Time = 18
//SelectSort : Time = 5787
//HeapSort : Time = 15
//QuickSort : Time = 8
		//MergeSort : Time = 15
//BubbleSort : Time = 73135
		//CountSort : Time = 1