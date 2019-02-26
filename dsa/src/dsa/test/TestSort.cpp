#include "Sort.H"
#include <vector>
#include <iostream>

using namespace std;

int main () {
  // declare an array:
  vector<int> a {35, 90, 80, 50, 10, 10, 0, 100, 1000, 5, 70000, 100, 50, 50, 100, 30, 34, 49, 500};
  for (int i = 1; i < 50000; i += 200) {
    a.push_back( (i % 137));
  }
  //vector<int> a {};
  //insertionSort(a);
  //shellSort(a);
  //mergeSort(a);
  //heapsort(a);
  //insertionSort(a, 0, a.size());
  quicksort(a);
  for (auto & item : a) {
    cout << item << endl;
  }
}
