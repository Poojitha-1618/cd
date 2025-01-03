#include <stdio.h>
#include <omp.h>
int main() {
 int n = 16; // Number of elements in the array
 int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
 int sum = 0; // Variable to store the global sum
 omp_set_num_threads(4); // Set the number of threads to 4
 int num_threads;
 int local_sums[4] = {0}; // Array to hold partial sums for each thread
 // Parallel region with thread-local partial sums
 #pragma omp parallel
 {
 int tid = omp_get_thread_num(); // Thread ID
 int chunk_size = n / omp_get_num_threads();
 int start = tid * chunk_size; // Start index for this thread
 int end = start + chunk_size; // End index for this thread
 // Compute the partial sum for this thread
 for (int i = start; i < end; i++) {
 local_sums[tid] += arr[i];
 }
 // Synchronize threads before the final reduction
 #pragma omp barrier
// Combine partial sums in a tree-like fashion
 #pragma omp single
 {
 num_threads = omp_get_num_threads();
 for (int step = 1; step < num_threads; step *= 2) {
 for (int i = 0; i + step < num_threads; i += 2 * step) {
 local_sums[i] += local_sums[i + step];
 }
 }
 }
 }
 // The final sum is stored in local_sums[0]
 sum = local_sums[0];
 printf("The sum of the array elements is: %d\n", sum);
 return 0;
}
