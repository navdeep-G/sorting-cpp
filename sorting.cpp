#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <climits>

using namespace std;

//Util fxns
template <typename T>
void print(vector<T> &v) {
	
	for(auto i : v) {
		cout << i << " ";
	}		

	cout << endl;
}

struct Timer {
	size_t start;
	Timer() {
		reset();
	}
	void reset() {
		start = clock();
	}
	double elapsed() {
		return ((double)clock() - start) / CLOCKS_PER_SEC;
	}
	void printElapsed(const char * label) {
		cout << label << ": " << elapsed() << "s\n";
	}
};

void check(vector<int> &control, vector<int> &data) {
	if(equal(control.begin(), control.end(), data.begin())) {
		cout << "Correct.\n";
	}
	else {
		cout << "Incorrect.\n";
	}
}

/* Sorting Functions */

//Insertion sort
void insertion_sort(vector<int> &data) {
	for(int i = 1; i < data.size(); i++) {
		int current = data[i];
		int j = i - 1;	
		int cmp;
		while(j >= 0) {
			cmp = data[j];
			//Swap if next element down is greater
			if(cmp > current) {
				data[j+1] = data[j];
				data[j] = current;
			}
			else {
				break;
			}
			j--;
		}
	}
}

//Selection sort
void selection_sort(vector<int> &data) {
	for(int i = 0; i < data.size(); i++) {
		//Find smallest
		int min = data[i];
		int index_min = i;
		for(int j = i+1; j < data.size(); j++) {
			if(data[j] < min) {
				min = data[j];
				index_min = j;
			}
		}

		//Swap it with i
		data[index_min] = data[i];
		data[i] = min;
	}
}


inline int left_child(int i) {
	return 2*i + 1;
}

inline int right_child(int i) {
	return 2*i + 2;
}

inline int parent(int i) {
	return (i-1)/2;
}

void max_heapify(vector<int> &data, int i, int end) {
	int left = left_child(i);
	int right = right_child(i);
	int largest = i;
	
	if(left < end) {
		if(data[left] > data[largest]) {
			largest = left;
		}	
	}

	if(right < end) {
		if(data[right] > data[largest]) {
			largest = right;
		}	
	}

	if(largest != i) {
		int tmp = data[i];
		data[i] = data[largest];
		data[largest] = tmp;
		max_heapify(data, largest, end);
	}	
}

//Heap sort
void heap_sort(vector<int> &data) {

	int n = data.size();

	//Build heap
	for(int i = parent(n-1); i >= 0; i--) {
		max_heapify(data, i, data.size());
	}

	//Sort
	int end = n - 1;
	while(end > 0) {
		int tmp = data[end];
		data[end] = data[0];
		data[0] = tmp;
		max_heapify(data, 0, end);
		end--;
	}

}

//LSB Radix Sort
//Works only on signed 32 bit integers
void lsb_radix_sort(vector<int> &data) {

	vector<int> buffer = data;	
	//Convert to unsigned
	for(int i = 0; i < data.size(); i++) {
		data[i] ^= INT_MIN;
	}

	//Number of bits to increment
	int nbits = 8;
	int mask = (1 << nbits) - 1;

	int nbuckets = (1 << nbits) + 1;
	int count[nbuckets];

	for(int bit=0; bit < 32; bit += nbits) {
		//Copy data into buffer
		for(int i = 0; i < data.size(); i++) {
			//data[i] ^= INT_MIN;
			buffer[i] = data[i];
		}
		//Init count to 0
		for(int i = 0; i < nbuckets; i++) {
			count[i] = 0;
		}	
		//Take count
		for(int i=0; i < data.size(); i++) {
			int sort_bits = (data[i] >> bit) & mask;	
			count[sort_bits + 1]++;
		}
		//Scan count
		for(int i=0; i < nbuckets; i++) {
			count[i+1] += count[i];
		}
		//Sort
		for(int i=0; i < data.size(); i++) {
			int sort_bits = (buffer[i] >> bit) & mask;	
			int new_index = count[sort_bits];
			data[new_index] = buffer[i];
			count[sort_bits]++;
		}
		
	}
	//Convert back to signed
	for(int i = 0; i < data.size(); i++) {
		data[i] ^= INT_MIN;
	}

}

//Merge sort
void merge(vector<int> &data, vector<int> &buffer, int begin, int mid, int end) {
	
	int n = end - begin;
	int run0_start = begin;
	int run1_start = mid;

	for(int i = begin; i < end; i++) { 
		//Left run is smaller
		if(run0_start < mid && ( data[run0_start] <= data[run1_start] || run1_start >= end)) {
			buffer[i] = data[run0_start];
			run0_start++;
		}
		//Right run is smaller
		else {
			buffer[i] = data[run1_start];
			run1_start++;
		}
	}

	//Copy result into data
	for(int i = begin; i < end; i++)
		data[i] = buffer[i];

}

void split(vector<int> &data, vector<int> &buffer, int begin, int end) {
	//We are down to size 1 - go back up
	if(end-begin==1) return;

	int mid = begin + ((end-begin)/2);

	split(data, buffer, begin, mid);
	split(data, buffer, mid, end);
	merge(data, buffer, begin, mid, end);
}

void mergesort(vector<int> &data) {
	//Work array
	vector<int> buffer(data.size());
	split(data, buffer, 0, data.size());	
}


int main() {

	size_t n = 10000000;
	//Alphabet size
	int alphabet = 65000;

	cout << "Sorting " << n << " items over integer alphabet |X| =  " << alphabet << endl;

	//Create a vector of random data
	vector<int> data(n);

	generate(data.begin(), data.end(), [alphabet]() { return (rand() % alphabet) - alphabet/2; });

	//Create a sorted "control" vector to check our results against	
	vector<int> control_sort = data;
	Timer t;
	sort(control_sort.begin(), control_sort.end());
	t.printElapsed("std::sort");

/*
	//Insertion
	vector<int> insertion_sorted = data;
	t.reset();
	insertion_sort(insertion_sorted);
	t.printElapsed("insertion sort");
	check(control_sort, insertion_sorted);

	//Selection
	vector<int> selection_sorted = data;
	t.reset();
	selection_sort(selection_sorted);	
	t.printElapsed("selection sort");
	check(control_sort, selection_sorted);
*/


	//Heap
	vector<int> heap_sorted = data;
	t.reset();
	heap_sort(heap_sorted);
	t.printElapsed("heap sort");
	check(control_sort, heap_sorted);

	//Mergesort
	vector<int> merge_sorted = data;
	t.reset();
	mergesort(merge_sorted);
	t.printElapsed("mergesort");
	check(control_sort, merge_sorted);

	//LSB Radix sort
	vector<int> lsb_radix_sorted = data;
	t.reset();
	lsb_radix_sort(lsb_radix_sorted);
	t.printElapsed("lsb_radix sort");
	check(control_sort, lsb_radix_sorted);
	
	return 0;
}
