#include <string>
#include "giftCard.h"
#include <iostream>

using namespace std;

GiftCard::GiftCard(){}


void GiftCard::setActivationDate(string adate){
	if(adate == ""){
	actDate = "00/00/0000";
	cout << "Card is not activated." << endl;
}
	else{
		actDate = adate;
	}
}

void GiftCard::setExpirationDate(string edate){
	if(edate == ""){
	expDate = "00/00/0000";
	cout << "Card is expired." << endl;
}
	else{
		expDate = edate;
	}
}

void GiftCard::setBalance(double tbal){
if (tbal >= 0){
	bal = tbal;
}
else
	bal = 0;
}
void GiftCard::setCode(int tcode){
	if(tcode >= 100 && tcode <= 999){
	code = tcode;
}
	else{
	cout << "Invalid Security Code. Code set to 111." << endl;
	code = 111;
	}

}

string GiftCard::getActivationDate(){
	return actDate;
}

string GiftCard::getExpirationDate(){
	return expDate;
}

double GiftCard::getBalance(){
	return bal;
}


int GiftCard::getCode(){
	return code;
}

