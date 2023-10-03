#include <map>
#include <math.h>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <iostream>

using namespace std;

// Function to check if input number is prime or not.
int is_prime(int num){
    if (num <= 1)
        return false;
    
    for (int i=2; i<=num/2; i++){
        if (num%i == 0){
            return false;
        }
    }
    return true;
}


// A function that prints the prime number less than the input number, and distributes the work in parallel among multiple threads.
void basic_prime_number_generator(int num, int num_threads[], int thread_size){
    map<int, double> time_per_thread;
    if (num<2){
        cout << "Invalid number. Type number greater than 1" <<endl;
        exit;
    }

    for (int i=0; i<thread_size; i++){
        // Set the number of threads for OpenMP parallel regions
        omp_set_num_threads(num_threads[i]);

        double start_time = omp_get_wtime();

        int prime_count = 0;

        // use parallel directive with reduction to get count of prime numbers
        #pragma omp parallel for reduction(+:prime_count)
        for (int j = 0; j<num; j++){
            if(is_prime(j)){
                prime_count++;
            }
        }

        int* prime_nums = new int[prime_count];
        int idx = 0;

        // use parallel for directive with critical to make it threadsafe
        #pragma omp parallel for
        for (int j = 2; j < num; j++) {
            if (is_prime(j)) {
                #pragma omp critical
                prime_nums[idx++] = j;
            }
        }

        // Print the primes
        cout << "Using " << num_threads[i] << " threads: " << endl;
        cout << "Prime numbers smaller than " << num << " are : "; 
        for (int j = 0; j < prime_count; j++) {
            cout << prime_nums[j] << " ";
        }

        cout << "\nCount of prime numbers: " << prime_count <<endl;

        double end_time = omp_get_wtime(); 
        time_per_thread[num_threads[i]] = end_time - start_time;

        cout << "Time taken: " << time_per_thread[num_threads[i]] << " seconds." << endl;
        cout << "-----------------------------------------" << endl;

        delete[] prime_nums;
    }
    
}


// A function that prints the prime number less than the input number, and distributes the work in parallel among multiple threads.
// Used dynamic scheduling with chunk size of 1 for load balancing
void advanced_prime_number_generator(int num, int num_threads[], int thread_size){
    map<int, double> time_per_thread;
    if (num<2){
        cout << "Invalid number. Type number greater than 1" <<endl;
        exit(0);
    }

    for (int k=0; k<thread_size; k++){
        // Set the number of threads for OpenMP parallel regions
        omp_set_num_threads(num_threads[k]);

        double start_time = omp_get_wtime();

        // create a boolean vector and initialize all its element to true.
        std::vector<bool> is_prime(num, true);
        is_prime[0] = false;
        is_prime[1] = false;

        // use parallel directive with for. Used dynamic scheduling with chunk size of 1 for load balancing
        #pragma omp parallel for
        for (int p=2; p*p<= num; p++){
            if(is_prime[p]){
                for (int j = p*p; j < num; j += p){
                    is_prime[j] = false;
                }
            }
        }

        std::vector<int> primes;
        #pragma omp parallel for
        for (int i = 2; i < num; i++) {
            if (is_prime[i]) {
                #pragma omp critical
                primes.push_back(i);
            }
        }

        // Print the primes
        cout << "Using " << num_threads[k] << " threads: " << endl;
        cout << "Prime numbers smaller than " << num << " are : "; 
        for (int j = 0; j < size(primes); j++) {
            cout << primes[j] << " ";
        }

        cout << "\nCount of prime numbers: " << size(primes) <<endl;

        double end_time = omp_get_wtime(); 
        time_per_thread[num_threads[k]] = end_time - start_time;

        cout << "Time taken: " << time_per_thread[num_threads[k]] << " seconds." << endl;
        cout << "-----------------------------------------" << endl;
    }    
}


int main(){
    int num;
    int num_threads[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

    cout << "Type a number: ";
    cin >> num;

    // 1. Prime number generator with basic requirements
    // basic_prime_number_generator(num, num_threads, size(num_threads));

    // 2. Prime number generator with advance requirements ( load balancing + advance algorithm to check prime numbers)
    advanced_prime_number_generator(num, num_threads, size(num_threads));

    return 0;
}