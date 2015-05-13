#include <string>
#include "OrderInformation.h"

using namespace std;


OrderInformation::OrderInformation(){}

void OrderInformation::setCustomerName(string pname){
	customerName = pname;
}

void OrderInformation::setDrinkDescription(string pdesc, string pdesc2, string pdesc3){
	if(pdesc2 != "" && pdesc3 != ""){
		drinkDescription = pdesc + ", " + pdesc2 + ", " + pdesc3;
	}
	else if(pdesc2 != ""){
		drinkDescription = pdesc + ", " + pdesc2;
	}
	else{
		drinkDescription = pdesc;
	}
}

void OrderInformation::setGiftCardNumber(int pgetNum){
	if (pgetNum < 0){
		giftCardNumber = 0;
	}
	else{
		giftCardNumber = pgetNum;
	}
}

void OrderInformation::setPreferredStoreNumber(int pnum){
	if(pnum <= 0 || pnum > 35){
		preferredStoreNumber = 1;
	}
	else{
		preferredStoreNumber = pnum;
	}
}

string OrderInformation::getCustomerName(){
	return customerName;
}

string OrderInformation::getDrinkDescription(){
	return drinkDescription;
}

int OrderInformation::getGiftCardNumber(){
	return giftCardNumber;
}	

int OrderInformation::getPreferredStoreNumber(){
	return preferredStoreNumber;
}

OrderInformation OrderInformation::operator+(const OrderInformation& pOrder){
	OrderInformation order;
	order.customerName = this->customerName;
	order.drinkDescription = this->drinkDescription + ", " + pOrder.drinkDescription;
	order.giftCardNumber = this->giftCardNumber;
	order.preferredStoreNumber = this->preferredStoreNumber;
	return order;
}
