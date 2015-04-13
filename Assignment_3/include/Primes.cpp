#include "Primes.h"


Primes::Primes(){
	currentPrime = 0;
}
		
Primes::Primes(int startingNum){
	currentPrime = startingNum;
}

bool Primes::isPrime(int x){
    
	if (x < 2){ return false;}
    
	for(int i=2; i<x; i++){
        if (x % i == 0){ return false; }
	}
    return true;
}

double Primes::nextPrime(){
	currentPrime++;
	for (; !isPrime(currentPrime); ++currentPrime){}
    
	return currentPrime;
}

double Primes::getNthPrime(int N){
	int numPrimes = 0;
	int curPrime;

	for(int i=2;; i++){

		if(isPrime(i)){
			numPrimes++;
			curPrime = i;
		}

		if(numPrimes >= N){break;}
	}
	return curPrime;
}