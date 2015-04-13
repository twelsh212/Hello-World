
#ifndef PRIMES_H_
#define PRIMES_H_


class Primes{
	
	public : 	
		Primes();
		Primes(int startingPrime);
		double nextPrime();
		double getNthPrime(int N);

	private:
		bool isPrime(int x);
		int  currentPrime;
};

#endif