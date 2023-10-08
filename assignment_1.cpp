#include <map>
#include <math.h>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdexcept>


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
map<int, double> basic_prime_number_generator(int num, int num_threads[], int thread_size){
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
    return time_per_thread;
}


// A function that prints the prime number less than the input number, and distributes the work in parallel among multiple threads.
// Used dynamic scheduling with chunk size of 1 for load balancing
map<int, double> advanced_prime_number_generator(int num, int num_threads[], int thread_size){
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
        #pragma omp parallel for schedule(dynamic, 1)
        for (int p=2; p<= int(sqrt(num)); p++){
            if(is_prime[p]){
                for (int j = p*p; j < num; j += p){
                    is_prime[j] = false;
                }
            }
        }

        std::vector<int> primes;
        #pragma omp parallel for schedule(dynamic, 1)
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

    return time_per_thread;  
}



// Function to calculate speedup and efficiency, and then save in csv.
void save_speedup_and_efficiency_to_csv(map<int, double> &time_per_thread){
    // Assuming the sequential time is precalculated ( using no. of thread as 1)
    double T_1 = time_per_thread[1];

    std::vector<double> speedups;
    std::vector<double> efficiencies;
    for (const auto& [thread_count, T_P] : time_per_thread){
        double speedup = T_1/T_P;
        double efficiency = speedup/thread_count;

        speedups.push_back(speedup);
        efficiencies.push_back(efficiency);
    }

    // Open a file in write mode.
    std::ofstream outfile("result.csv");

    // Error check for file open operation
    if (!outfile.is_open()){
        throw std::runtime_error("Failed to open the result.csv file.");
    }

    // Write the header
    outfile << "Threads,Speadup,Efficiency\n";

    int i=0;
    // Writing data to file
    for (const auto& [thread_count, T_P] : time_per_thread){
        outfile << thread_count << "," << speedups[i] << "," << efficiencies[i] << "\n";
        i++;
    }

    // Close the file
    outfile.close();
    
    if (!outfile.good()) {
        throw std::runtime_error("Error occurred while writing to the result.csv file.");
    }

    std::cout << "Data written to result.csv" << std::endl;
}


int main(){
    int num;
    int option;
    int num_threads[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    map<int, double> execution_time;

    while(true){
        cout << "Choose one of the following options:\n";
        cout << "--------------------------------------------------------\n";
        cout << "1. Prime number generator with basic requirements. Type 1\n";
        cout << "2. Prime number generator with advanced requirements. Type 2\n";
        cout << "3. Exit. Type 3\n";
        cin >> option;
        switch (option)
        {
            case 1:
                // 1. Prime number generator with basic requirements
                cout << "\nPrime number generator with basic requirements\n";
                cout << "--------------------------------------------------------\n\n";
                cout << "Type a number: ";
                cin >> num;
                execution_time = basic_prime_number_generator(num, num_threads, size(num_threads));
                save_speedup_and_efficiency_to_csv(execution_time);
                break;
            
            case 2:
                // 2. Prime number generator with advance requirements ( load balancing + advance algorithm to check prime numbers)
                cout << "\nPrime number generator with basic requirements" << endl;
                cout << "--------------------------------------------------------\n\n";
                cout << "Type a number: ";
                cin >> num;
                execution_time = advanced_prime_number_generator(num, num_threads, size(num_threads));
                save_speedup_and_efficiency_to_csv(execution_time);
                break;
            case 3:
                return 0;
            default:
                cout << "Please select correct options\n";
                break;
        }

    }

    return 0;
}
