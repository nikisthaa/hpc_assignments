# hpc_assignments

## Assignment 1: Parallelizing Prime Number Counting

### Objective
The objective of this assignment is to parallelize the program for finding and counting all prime numbers smaller than a given integer n using OpenMP or a similar parallel programming framework.


### Requirements
You will enhance the existing C/C++ program that finds and counts prime numbers smaller thannby parallelizing it using OpenMP approach or a similar parallel programming framework. Implement a function is_prime that takes an integer as input and returns a boolean value indicating whether the input is a prime number. 

Ensure that this function is thread-safe and does not produce race conditions. Use parallel constructs such as #pragma omp parallel for or their equivalent in other parallel programming frameworks to distribute the prime number checking tasks across multiple threads. 

Handle user input for the value of n. Ensure that n is a positive integer greater than 1. Count theprime numbers found in parallel and print both the list of prime numbers and the count. 

Sample Output:
Enter a positive integer n: 100000
Prime numbers smaller than 100000: 2 3 5 7 11 13 ... (list of prime numbers)
Count of prime numbers: 9592

### Additional Challenges
Implement load balancing techniques to distribute prime number checking tasks evenly among threads, considering varying execution times for different numbers.
Extend the program to use a more efficient prime number generation algorithm, such as the Sieve of Eratosthenes, and parallelize it as well for improved performance.