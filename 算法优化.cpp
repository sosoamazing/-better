#include<iostream>
#include<stdlib.h>
#include<ctime>

constexpr auto N = 0xffffffff;

using namespace std;

//**************************************************************	获取新数组

int* get_array(int n)
{
	int* a = new int[n];
	srand(clock() % N);
	for (int i = 0; i < n; i++)a[i] = rand();
	return a;
}

//************************************************************	优化算法1

int search(int* a, int r,int point)
{
	if (a[r] <= point)return r + 1;
	int l = 0;
	while (l < r)
	{
		int mid = (l + r) / 2;
		if (a[mid] <= point)l = mid + 1;
		else r = mid;
	}
	return l;
}

void insert_sort_better1(int* a, int n)
{
	for (int i = 1; i < n; i++) {
		int p = search(a, i - 1, a[i]);
		int x = a[i];
		for (int j = i; j > p; j--)a[j] = a[j - 1];
		a[p] = x;
	}
}

//**************************************************************	优化算法2

struct Node {
	Node* next;
	int val;
	Node() :next(nullptr), val(0) {}
	Node(int value) :next(nullptr), val(value) {}
	Node(Node* node) :next(node), val(0) {}
	Node(int value, Node* node) :val(value), next(node) {}
};

void insert_sort_better2(int* a, int n)
{
	Node* head = new Node(new Node(a[0]));
	for (int i = 1; i < n; i++)
	{
		Node* node = head->next;
		Node* last = head;
		while (node && node->val < a[i])
		{
			node = node->next;
			last = last->next;
		}
		if (node == nullptr)
		{
			last->next = new Node(a[i]);
		}
		else {
			last->next = new Node(a[i], node);
		}
	}
	for (int i = -1; i < n; i++)
	{
		Node* node = head;
		if (i >= 0)a[i] = head->val;
		head = head->next;
		delete node;
	}
}

//*************************************************************	做实验

const char* op_name[2] = { "插入算法优化1","插入算法优化2" };
void(*op[2])(int*, int) = { insert_sort_better1,insert_sort_better2 };

void experiment()
{
	FILE* file;
	freopen_s(&file, "1out.txt", "w", stdout);
	for (int i = 0; i < 2; i++)
	{
		cout << op_name[i] << endl;
		for (int j = 10000; j <= 50000; j += 10000)
		{
			int sum = 0;
			for (int time = 0; time < 20; time++)
			{
				int* a = get_array(j);
				clock_t now = clock();
				op[i](a, j);
				sum += clock() - now;
				delete[]a;
			}
			cout << "n == " << j << ": " << sum / 20 << endl;
		}
	}
	if (file)fclose(file);
}

//**************************************************************	测试算法是否正确

bool is_sorted(int* a,int n)
{
	for (int i = 1; i < n; i++)
	{
		if (a[i] < a[i - 1])return false;
	}
	return true;
}

void test()
{
	for (int i = 0; i < 2; i++)
	{
		cout << op_name[i] << endl;
		for (int n = 100; n <= 1000; n += 100)
		{
			int* a = get_array(n);
			op[i](a, n);
			if (is_sorted(a, n))cout << "YES" << " ";
			else cout << "NO" << " ";
			delete[]a;
		}
		cout << endl;
	}
}





int main()
{
	test();
	experiment();
	return 0;
}