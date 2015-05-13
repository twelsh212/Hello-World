#include <iostream>
#include <string>
#include "receipt.h"

using namespace std;

receipt::receipt(){}

void receipt::setConfirmationNumber(int pNo){
	if (pNo > 0){

		confirmationNumber = pNo;
	}
	else
		confirmationNumber = 1;
		

}

void receipt::setPickupTime(string pDate){
	if (pDate != "" && pDate.length() == 10){

		pickupTime = pDate;
	}
	else
		pickupTime = "01/01/2020";
		
}

void receipt::setGCBalance(double pBal){
	gcBalance = pBal;
}

int receipt::getConfirmationNumber(){
	return confirmationNumber;
}

string receipt::getPickupTime(){
	return pickupTime;
}

double receipt::getGCBalance(){
	return gcBalance;
}
