#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <unordered_map>
#include <vector>
using namespace std;

#define ArraySize 100000
#define ArrayDepth 3
#define RandomSize 100

// misc funcs
void display(vector<int> vec, int size);
void display(int array[], int size);
void StartProg();
// insertion sort
void inssort(vector<vector<int>> A, int n, int k, int sortlevel);
// bubble sort
void bubblesort(vector<vector<int>> A, int n, int k, int sortlevel);
// selection sort
void selectionsort(vector<vector<int>> A, int n, int k, int sortlevel);
// shell sort
void shellsort(vector<vector<int>> A, int n, int k, int sortlevel);
// Modified Insertion Sort for varying increments
void inssort2(vector<vector<int>> A, int start, int incr, int n, int k,
              int sortlevel);
// Merge Sort
// from programmiz
void merge(vector<vector<int>> arr, int p, int q, int r, int g, int sortlayer,
           int sortlevel);
void mergeSort(vector<vector<int>> arr, int l, int r, int k, int sortlevel);
// quick sort
void quicksort(vector<vector<int>> A, int i, int j, int k, int sortlevel);
int findpivot(int i, int j);
int partition(vector<vector<int>> A, int left, int right, int pivot, int k,
              int sortlevel);
// bucket sort
// void bucket_sort(std::vector<int> &src);
// heap sort
// from programmiz
// wants full array size not array size-1
void heapify(vector<vector<int>> arr, int n, int i, int k, int sortlevel);
void heapSort(vector<vector<int>> arr, int n, int k, int sortlevel);
// radix sort
void radixsort(vector<vector<int>> array, int size, int k, int sortlevel);
int getMax(vector<vector<int>> array, int n, int k);
void countingSort(vector<vector<int>> array, int size, int place, int k,
                  int sortlevel);

int main() {
  StartProg();
}

// interface functions

void StartProg() {
  // a holder string for error testing
  string holder;
  int num_of_algos;
  // introduce the program and initial options
  cout << "This program is designed to compartively analyze the efficiency of "
          "various sorting algorithms.\n\n";
  cout << "There are 8 sorting algorithms included for testing:\n"
       << "1. Insertion Sort\n"
       << "2. Bubble Sort\n"
       << "3. Selection Sort\n"
       << "4. Shell Sort\n"
       << "5. Merge Sort\n"
       << "6. Quick Sort\n"
       << "7. Heap Sort\n"
       << "8. Radix Sort\n\n";
  while (true) {
    try {
      // initial option
      cout << "How many algorithms will you be testing? (1-8): ";
      cin >> holder;
      num_of_algos = stoi(holder);
      if (num_of_algos < 1 || num_of_algos > 8) {
        throw out_of_range("is not between 1 and 8");
      }
      // if input is valid go through
      break;
    } catch (std::invalid_argument &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: Invalid argument" << std::endl;
    } catch (std::out_of_range &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Input not between 1 and 9" << std::endl;
    } catch (std::exception &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: " << err.what() << std::endl;
    }
  }
  cout << "You will be testing " << num_of_algos << " algorithms.\n\n";
  int choices = 0;
  vector<int> choiceVec;
  while (choices < num_of_algos) {
    try {
      cout << "Choose which algorithms you want to test by entering the number "
              "(1-8) ("
           << choices << " so far): ";
      cin >> holder;
      if (find(choiceVec.begin(), choiceVec.end(), stoi(holder)) !=
          choiceVec.end()) {
        throw underflow_error("Entry is already in the list");
      }
      if (stoi(holder) < 1 || stoi(holder) > 8)
        throw out_of_range("is not between 1 and 8");
      choiceVec.push_back(stoi(holder));
      choices++;
    } catch (std::invalid_argument &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: Invalid argument" << std::endl;
    } catch (std::out_of_range &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Input not between 1 and 9" << std::endl;
    } catch (std::exception &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: " << err.what() << std::endl;
    }
  }
  int sortlevel = 0;
  while (true) {
    try {
      // initial option
      cout << "\nWill you be testing the algorithms on Primary, Secondary or "
              "Tertiary level sorting? (1-3): ";
      cin >> holder;
      if (stoi(holder) < 1 || stoi(holder) > 3) {
        throw out_of_range("is not between 1 and 9");
      }
      sortlevel = stoi(holder);
      // if input is valid go through
      break;
    } catch (std::invalid_argument &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: Invalid argument" << std::endl;
    } catch (std::out_of_range &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Input not between 1 and 9" << std::endl;
    } catch (std::exception &err) {
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Ignore the rest of the line
      std::cout << "Conversion failure: " << err.what() << std::endl;
    }
  }
  // now we have the algorithms used, the number of algos, and the level of
  // sorting part 2 of the program creating the arrays used for sorting create
  // the arrays
  cout << "Generating arrays...\n";
  random_device rd;
  // Initialize a Mersenne Twister pseudo-random number generator
  mt19937 gen(rd());

  // initialize it to 3 regardless, just dont fill

  vector<vector<vector<int>>> vec;
  int arr[3][ArraySize];

  // Define the range of numbers to generate
  uniform_int_distribution<> distr(1, RandomSize);
  if (sortlevel != 1) {
    for (int i = 0; i < sortlevel; i++) {
      for (int j = 0; j < ArraySize; j++) {
        arr[i][j] = distr(gen);
      }
    }
  } else {
    for (int i = 0; i < ArraySize; i++) {
      arr[0][i] = distr(gen);
    }
  }
  vec.resize(choiceVec.size());
  for (int i = 0; i < choiceVec.size(); i++) {
   vec[i].resize(sortlevel);
  }
  for (int i = 0; i < choiceVec.size(); i++) {
    // push back the array once for each algorithm tested
    for (int j = 0; j < sortlevel; j++) 
      for (int k = 0; k < ArraySize; k++) 
        vec[i][j].push_back(arr[j][k]);
  }
  // now that we have th arrays we start sorting
  cout << "sorting arrays...\n";
  // map to store the time it takes to sort each array
  unordered_map<string, chrono::duration<double>> timesPrimary, timesSecondary,
      timesTertiary;
  for (int i = 0; i < choiceVec.size(); i++) {
    switch (choiceVec[i]) {
    case 1: {
      // insertion sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      inssort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Insertion Sort"] = end - start;
      break;
    }
    case 2: {
      // bubble sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      bubblesort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Bubble Sort"] = end - start;
      break;
    }
    case 3: {
      // selection sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      selectionsort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Selection Sort"] = end - start;
      break;
    }
    case 4: {
      // Shell sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      shellsort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Shell Sort"] = end - start;
      break;
    }
    case 5: {
      // Merge sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      mergeSort(vec[i], 0, ArraySize - 1, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Merge Sort"] = end - start;
      break;
    }
    case 6: {
      // Quick sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      quicksort(vec[i], 0, ArraySize - 1, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Quick Sort"] = end - start;
      break;
    }
    case 7: {
      // Heap sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      heapSort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Heap Sort"] = end - start;
      break;
    }
    case 8: {
      // Radix sort
      //  Start the timer
      auto start = std::chrono::high_resolution_clock::now();
      // sort
      radixsort(vec[i], ArraySize, 0, sortlevel);
      auto end = std::chrono::high_resolution_clock::now();
      timesPrimary["Radix Sort"] = end - start;
      break;
    }
    }
    // now for secondary/tertiary sorting
    for (int j = 2; j <= sortlevel; j++) {
      // cheat by finding total num of elements to be sorted then run each algo
      // on that number of elements rate of conflicts for a primary sort R=
      // N/ArraySize where N is the max of the random number generator secondary
      // conflicts R^2 tertiary = R^3
      float conflictRate = RandomSize / (float)ArraySize;
      int Conflicts = (int)(pow(conflictRate, j) * ArraySize);
      if (j == 2) {
        switch (choiceVec[i]) {
        case 1: {
          // insertion sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          inssort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Insertion Sort"] = end - start;
          break;
        }
        case 2: {
          // bubble sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          bubblesort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Bubble Sort"] = end - start;
          break;
        }
        case 3: {
          // selection sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          selectionsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Selection Sort"] = end - start;
          break;
        }
        case 4: {
          // Shell sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          shellsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Shell Sort"] = end - start;
          break;
        }
        case 5: {
          // Merge sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          mergeSort(vec[i], 0, Conflicts - 1, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Merge Sort"] = end - start;
          break;
        }
        case 6: {
          // Quick sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          quicksort(vec[i], 0, Conflicts - 1, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Quick Sort"] = end - start;
          break;
        }
        case 7: {
          // Heap sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          heapSort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Heap Sort"] = end - start;
          break;
        }
        case 8: {
          // Radix sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          radixsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesSecondary["Radix Sort"] = end - start;
          break;
        }
        }
      } else {
        switch (choiceVec[i]) {
        case 1: {
          // insertion sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          inssort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Insertion Sort"] = end - start;
          break;
        }
        case 2: {
          // bubble sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          bubblesort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Bubble Sort"] = end - start;
          break;
        }
        case 3: {
          // selection sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          selectionsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Selection Sort"] = end - start;
          break;
        }
        case 4: {
          // Shell sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          shellsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Shell Sort"] = end - start;
          break;
        }
        case 5: {
          // Merge sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          mergeSort(vec[i], 0, Conflicts - 1, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Merge Sort"] = end - start;
          break;
        }
        case 6: {
          // Quick sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          quicksort(vec[i], 0, Conflicts - 1, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Quick Sort"] = end - start;
          break;
        }
        case 7: {
          // Heap sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          heapSort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Heap Sort"] = end - start;
          break;
        }
        case 8: {
          // Radix sort
          //  Start the timer
          auto start = std::chrono::high_resolution_clock::now();
          // sort
          radixsort(vec[i], Conflicts, j-1, sortlevel);
          auto end = std::chrono::high_resolution_clock::now();
          timesTertiary["Radix Sort"] = end - start;
          break;
        }
        }
      }
    }
  }
  // now we display the times
  cout << "sorting done...\n\n";
  for (int i = 0; i < choiceVec.size(); i++) {
    chrono::duration<double> totalTime;
    switch (choiceVec[i]) {
    case 1: {
      // insertion sort
      cout << "Insertion Sort (in order of level): "
           << timesPrimary["Insertion Sort"].count();
      totalTime = timesPrimary["Insertion Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Insertion Sort"];
        cout << " + " << timesSecondary["Insertion Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Insertion Sort"];
        cout << " + " << timesTertiary["Insertion Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 2: {
      // bubble sort
      cout << "Bubble Sort (in order of level): "
           << timesPrimary["Bubble Sort"].count();
      totalTime = timesPrimary["Bubble Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Bubble Sort"];
        cout << " + " << timesSecondary["Bubble Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Bubble Sort"];
        cout << " + " << timesTertiary["Bubble Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 3: {
      // selection sort
      cout << "Selection Sort (in order of level): "
           << timesPrimary["Selection Sort"].count();
      totalTime = timesPrimary["Selection Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Selection Sort"];
        cout << " + " << timesSecondary["Selection Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Selection Sort"];
        cout << " + " << timesTertiary["Selection Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 4: {
      // Shell sort
      cout << "Shell Sort (in order of level): "
           << timesPrimary["Shell Sort"].count();
      totalTime = timesPrimary["Shell Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Shell Sort"];
        cout << " + " << timesSecondary["Shell Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Shell Sort"];
        cout << " + " << timesTertiary["Shell Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 5: {
      // Merge sort
      cout << "Merge Sort (in order of level): "
           << timesPrimary["Merge Sort"].count();
      totalTime = timesPrimary["Merge Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Merge Sort"];
        cout << " + " << timesSecondary["Merge Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Merge Sort"];
        cout << " + " << timesTertiary["Merge Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 6: {
      // Quick sort
      cout << "Quick Sort (in order of level): "
           << timesPrimary["Quick Sort"].count();
      totalTime = timesPrimary["Quick Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Quick Sort"];
        cout << " + " << timesSecondary["Quick Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Quick Sort"];
        cout << " + " << timesTertiary["Quick Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 7: {
      // Heap sort
      cout << "Heap Sort (in order of level): "
           << timesPrimary["Heap Sort"].count();
      totalTime = timesPrimary["Heap Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Heap Sort"];
        cout << " + " << timesSecondary["Heap Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Heap Sort"];
        cout << " + " << timesTertiary["Heap Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    case 8: {
      // Radix sort
      cout << "Radix Sort (in order of level): "
           << timesPrimary["Radix Sort"].count();
      totalTime = timesPrimary["Radix Sort"];
      if (sortlevel >= 2) {
        totalTime += timesSecondary["Radix Sort"];
        cout << " + " << timesSecondary["Radix Sort"].count();
      }
      if (sortlevel == 3) {
        totalTime += timesTertiary["Radix Sort"];
        cout << " + " << timesTertiary["Radix Sort"].count();
      }
      cout << " = " << totalTime.count() << " total seconds\n";
      break;
    }
    }
    
  }
  cout << "press any button to close";
  string temp;
  cin >> temp;
}

/*
// Driver program to test above function
int main() {
    float arr[] = {0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434};
    int n = sizeof(arr) / sizeof(arr[0]);
    bucketSort(arr, n);

    cout << "Sorted array is \n";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    return 0;
}
*/

// sorting algos

// Bucket sort
/*
void bucket_sort(std::vector<vector<int>> &src, int k, int sortlevel) {
  if (src.size() <= 1)
    return;

  int min = *std::min_element(src[k].begin(), src[k].end());
  int max = *std::max_element(src[k].begin(), src[k].end());
  if (min == max)
    return;

  std::vector<std::vector<int>> vec;
  vec.resize(src[k].size());

  for (int i = 0; i < src[k].size(); i++) {
    // edit: double precision required
    int v = int(double(src[k][i] - min) * src[k].size() / double(max - min +
1)); vec[v].push_back(src[i]);
  }

  for (size_t i = 0; i < vec.size(); i++)
    // std::sort(vec[i].begin(), vec[i].end());
    bucket_sort(vec[i]);

  int index = 0;
  for (size_t i = 0; i < vec.size(); i++)
    for (size_t j = 0; j < vec[i].size(); j++)
      src[index++] = vec[i][j];
}
*/
// counting sort

void display(int array[], int size) {
  for (int i = 0; i < size; i++)
    cout << array[i] << " ";
  cout << endl;
}
void display(vector<int> vec, int size) {
  for (int i = 0; i < size; i++)
    cout << vec[i] << " ";
  cout << endl;
}

int getMax(vector<vector<int>> array, int n, int k) {
  int max = array[k][0];
  for (int i = 1; i < n; i++)
    if (array[k][i] > max)
      max = array[k][i];
  return max;
}

void countingSort(vector<vector<int>> array, int size, int place, int k,
                  int sortlevel) {
  const int max = 10;
  int output[sortlevel][size];
  int count[max];

  for (int i = 0; i < max; ++i)
    count[i] = 0;

  // Calculate count of elements
  for (int i = 0; i < size; i++)
    count[(array[k][i] / place) % 10]++;

  // Calculate cumulative count
  for (int i = 1; i < max; i++)
    count[i] += count[i - 1];

  // Place the elements in sorted order
  for (int i = size - 1; i >= 0; i--) {
    for (int x = 0; x < sortlevel; x++)
      output[x][count[(array[k][i] / place) % 10] - 1] = array[x][i];
    count[(array[k][i] / place) % 10]--;
  }

  for (int i = 0; i < size; i++)
    for (int x = 0; x < sortlevel; x++)
      array[x][i] = output[x][i];
}

// insertion sort
void inssort(vector<vector<int>> A, int n, int k, int sortlevel) { // Insertion Sort
  for (int i = 1; i < n; i++) // Insert i'th record
    for (int j = i; (j > 0) && (A[k][j] < A[k][j - 1]); j--) {
      for (int x = 0; x < sortlevel; x++)
        swap(A[x][j], A[x][j - 1]);
    }
}

// bubble sort

void bubblesort(vector<vector<int>> A, int n, int k, int sortlevel) {
  for (int i = 0; i < n; i++) // Insert i'th record
    for (int j = 0; j < n - i; j++)
      if (A[k][j] > A[k][j + 1]) {
        for (int x = 0; x < sortlevel; x++)
          swap(A[x][j], A[x][j + 1]);
      }
}

// selection sort

void selectionsort(vector<vector<int>> A, int n, int k, int sortlevel) {
  for (int i = 0; i < n - 1; i++) { // Select i'th biggest record
    int bigindex = 0;               // Current biggest index
    for (int j = 1; j < n - i; j++) // Find the max value
      if (A[k][j] > A[k][bigindex]) // Found something bigger
        bigindex = j;               // Remember bigger index
    for (int x = 0; x < sortlevel; x++)
      swap(A[x][bigindex], A[x][n - i - 1]); // Put it into place
  }
}

// shell sort

void shellsort(vector<vector<int>> A, int n, int k, int sortlevel) {
  for (int i = n / 2; i > 2; i /= 2) // For each increment
    for (int j = 0; j < i; j++)      // Sort each sublist
      inssort2(A, j, i, n, k, sortlevel);
  inssort2(A, 0, 1, n, k, sortlevel);
}

// modified insertionsort

// Modified Insertion Sort for varying increments
void inssort2(vector<vector<int>> A, int start, int incr, int n, int k, int sortlevel) {
  for (int i = start + incr; i < n; i += incr)
    for (int j = i; ((j >= incr) && (A[k][j] < A[k][j - incr])); j -= incr) {
      for (int x = 0; x < sortlevel; x++) {
        swap(A[x][j], A[x][j - incr]);
      }
    }
}

// mergesort

// Merge two subarrays L and M into arr
void merge(vector<vector<int>> arr, int p, int q, int r, int sortlayer,
           int sortlevel) {

  // Create L ← A[p..q] and M ← A[q+1..r]
  int n1 = q - p + 1;
  int n2 = r - q;

  int L[sortlevel][n1], M[sortlevel][n2];

  for (int i = 0; i < n1; i++)
    for (int j = 0; j < sortlevel; j++)
      L[j][i] = arr[j][p + i];
  for (int j = 0; j < n2; j++)
    for (int o = 0; o < sortlevel; o++)
      M[o][j] = arr[o][q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either L or M, pick larger among
  // elements L and M and place them in the correct position at A[p..r]
  while (i < n1 && j < n2) {
    if (L[sortlayer][i] <= M[sortlayer][j]) {
      for (int x = 0; x < sortlevel; x++)
        arr[x][k] = L[x][i];
      i++;
    } else {
      for (int x = 0; x < sortlevel; x++)
        arr[x][k] = M[x][j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either L or M,
  // pick up the remaining elements and put in A[p..r]
  while (i < n1) {
    for (int x = 0; x < sortlevel; x++)
      arr[x][k] = L[x][i];
    i++;
    k++;
  }

  while (j < n2) {
    for (int x = 0; x < sortlevel; x++)
      arr[x][k] = M[x][j];
    j++;
    k++;
  }
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(vector<vector<int>> arr, int l, int r, int k, int sortlevel) {
  if (l < r) {
    // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(arr, l, m, k, sortlevel);
    mergeSort(arr, m + 1, r, k, sortlevel);

    // Merge the sorted subarrays
    merge(arr, l, m, r, k, sortlevel);
  }
}

// quick sort

void quicksort(vector<vector<int>> A, int i, int j, int sortlayer, int sortlevel) {
  int pivotindex = findpivot(i, j);
  for (int x = 0; x < sortlevel; x++)
    swap(A[x][pivotindex], A[x][j]); // Stick pivot at end
  // k will be the first position in the right subarray
  int k = partition(A, i, j - 1, A[sortlayer][j], sortlayer, sortlevel);
  for (int x = 0; x < sortlevel; x++)
  swap(A[x][k], A[x][j]); // Put pivot in place
  if ((k - i) > 1)
    quicksort(A, i, k - 1, sortlayer, sortlevel); // Sort left partition
  if ((j - k) > 1)
    quicksort(A, k + 1, j, sortlayer, sortlevel); // Sort right partition
}
int findpivot(int i, int j) { return (i + j) / 2; }

int partition(vector<vector<int>> A, int left, int right, int pivot, int k,
              int sortlevel) {
  while (left <= right) { // Move bounds inward until they meet
    while (A[k][left] < pivot)
      left++;
    while ((right >= left) && (A[k][right] >= pivot))
      right--;
    if (right > left)
      for (int x = 0; x < sortlevel; x++)
        swap(A[x][left], A[x][right]); // Swap out-of-place values
  }
  return left; // Return first position in right partition
}

// heap sort

void heapify(vector<vector<int>> arr, int n, int i, int k, int sortlevel) {
  // Find largest among root, left child and right child
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[k][left] > arr[k][largest])
    largest = left;

  if (right < n && arr[k][right] > arr[k][largest])
    largest = right;

  // Swap and continue heapifying if root is not largest
  if (largest != i) {
    for (int x = 0; x < sortlevel; x++)
      swap(arr[x][i], arr[x][largest]);
    heapify(arr, n, largest, k, sortlevel);
  }
}

// main function to do heap sort
void heapSort(vector<vector<int>> arr, int n, int k, int sortlevel) {
  // Build max heap
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i, k, sortlevel);

  // Heap sort
  for (int i = n - 1; i >= 0; i--) {

    for (int x = 0; x < sortlevel; x++)
      swap(arr[x][0], arr[x][i]);

    // Heapify root element to get highest element at root again
    heapify(arr, i, 0, k, sortlevel);
  }
}

// radix sort
void radixsort(vector<vector<int>> array, int size, int k, int sortlevel) {
  // Get maximum element
  int max = getMax(array, size, k);

  // Apply counting sort to sort elements based on place value.
  for (int place = 1; max / place > 0; place *= 10)
    countingSort(array, size, place, k, sortlevel);
}
