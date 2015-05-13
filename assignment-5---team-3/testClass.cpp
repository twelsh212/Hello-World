#include <iostream>
#include <string>
#include "testClass.h"
#include "OrderInformation.h"
#include "giftCard.h"
#include "receipt.h"

using namespace std;

testClass::testClass(){}


//Implementation for testing order information class class
void testClass::testOrderInformation(){
	cout << endl << "Testing... " << endl;
	OrderInformation orderInfo;
	orderInfo.setCustomerName("Terry");
	orderInfo.setDrinkDescription("Large Coffee");
	orderInfo.setGiftCardNumber(111222333);
	orderInfo.setPreferredStoreNumber(12);
	string custName = orderInfo.getCustomerName();
	string drinkDesc = orderInfo.getDrinkDescription();
	int giftCard = orderInfo.getGiftCardNumber();
	int prefStore = orderInfo.getPreferredStoreNumber();
	cout << custName << "   " << drinkDesc << "   " << giftCard << "   " << prefStore << endl;
	cout << "Test for basic order has passed" << endl << endl;
}

void testClass::testForDefaultValues(){
	cout << endl << "Testing... " << endl;
	OrderInformation orderInfo;
	orderInfo.setCustomerName();
	orderInfo.setDrinkDescription("Large Coffee");
	orderInfo.setGiftCardNumber();
	orderInfo.setPreferredStoreNumber(12);
	string custName = orderInfo.getCustomerName();
	string drinkDesc = orderInfo.getDrinkDescription();
	int giftCard = orderInfo.getGiftCardNumber();
	int prefStore = orderInfo.getPreferredStoreNumber();
	cout << custName << "   " << drinkDesc << "   " << giftCard << "   " << prefStore << endl;
	cout << "Test for overrided default values has passed" << endl << endl;
}

void testClass::testForInvalidIntegerValues(){
	cout << endl << "Testing... " << endl;
	OrderInformation orderInfo;
	orderInfo.setCustomerName("Terry");
	orderInfo.setDrinkDescription("Large Coffee");
	orderInfo.setGiftCardNumber(-1234945);
	orderInfo.setPreferredStoreNumber(123164);
	string custName = orderInfo.getCustomerName();
	string drinkDesc = orderInfo.getDrinkDescription();
	int giftCard = orderInfo.getGiftCardNumber();
	int prefStore = orderInfo.getPreferredStoreNumber();
	if (giftCard < 0 || prefStore <= 0 || prefStore > 35){
		cout << "Test case 3 has failed with invalid integers" << endl << endl;
	}
	else{
		cout << custName << "   " << drinkDesc << "   " << giftCard << "   " << prefStore << endl;
		cout << "Test for invalid integer values has passed" << endl << endl;
	}
}

void testClass::testForMultipleDrinks(){
	cout << endl << "Testing... " << endl;
	OrderInformation orderInfo;
	orderInfo.setCustomerName("Terry");
	orderInfo.setDrinkDescription("Large Coffee", "Small Coffee", "Medium Coffee");
	orderInfo.setGiftCardNumber(111222333);
	orderInfo.setPreferredStoreNumber(12);
	string custName = orderInfo.getCustomerName();
	string drinkDesc = orderInfo.getDrinkDescription();
	int giftCard = orderInfo.getGiftCardNumber();
	int prefStore = orderInfo.getPreferredStoreNumber();
	cout << custName << "   " << drinkDesc << "   " << giftCard << "   " << prefStore << endl;
	cout << "Test for multiple drink orders has passed" << endl << endl;
}

void testClass::testCombiningOrders(){
	cout << endl << "Testing... " << endl;
	OrderInformation orderInfo;
	OrderInformation orderInfo2;
	OrderInformation orderInfo3;
	orderInfo.setCustomerName("Terry");
	orderInfo.setDrinkDescription("Large Coffee");
	orderInfo.setGiftCardNumber(111222333);
	orderInfo.setPreferredStoreNumber(12);
	string custName = orderInfo.getCustomerName();
	string drinkDesc = orderInfo.getDrinkDescription();
	int giftCard = orderInfo.getGiftCardNumber();
	int prefStore = orderInfo.getPreferredStoreNumber();
	orderInfo2 = orderInfo;

	orderInfo3 = orderInfo + orderInfo2;

	cout << "Order 1:" << custName << "   " << drinkDesc << "   " << giftCard << "   " << prefStore << endl;
	cout << "Order 2:" << orderInfo2.getCustomerName() << "   " << orderInfo2.getDrinkDescription() << "   " 
		<< orderInfo2.getGiftCardNumber() << "   " << orderInfo2.getPreferredStoreNumber() << endl;
	cout << "Order 3:" << orderInfo3.getCustomerName() << "   " << orderInfo3.getDrinkDescription() << "   " 
		<< orderInfo3.getGiftCardNumber() << "   " << orderInfo3.getPreferredStoreNumber() << endl;
	cout << "Test for combining orders has passed" << endl << endl;

}


//Implementation for testing giftcard class
void testClass::testGiftCard(){
	GiftCard giftCard;

	giftCard.setActivationDate("01/01/2016");
	giftCard.setExpirationDate("01/01/2019");
	giftCard.setBalance(500.00);
	string actDate = giftCard.getActivationDate();
	string expDate = giftCard.getExpirationDate();
	double bal = giftCard.getBalance();
	cout << "GREEN" << endl;
	cout << actDate << " " << expDate << " " << bal << endl;
}

void testClass::testGiftCardBal(){
	GiftCard giftCard;
	giftCard.setActivationDate("01/01/2016");
	giftCard.setExpirationDate("01/01/2019");
	giftCard.setBalance(-500.00);
	string actDate = giftCard.getActivationDate();
	string expDate = giftCard.getExpirationDate();
	double bal = giftCard.getBalance();
	if(bal >= 0){
		cout << "GREEN" << endl;
		cout << actDate << " " << expDate << " " << bal << endl;
	}
	else
		cout << "Fail!" << endl;
}

void testClass::testGiftCardExpDate(){
	GiftCard giftCard;
	giftCard.setActivationDate("01/01/2016");
	giftCard.setExpirationDate("");
	giftCard.setBalance(500.00);
	string actDate = giftCard.getActivationDate();
	string expDate = giftCard.getExpirationDate();
	double bal = giftCard.getBalance();
	if(expDate == ""){
		cout << "Fail!" << endl;
	}
	else{
		cout << "Green!" << endl;
		cout << actDate << " " << expDate << " " << bal << endl;
	}
}

void testClass::testGiftCardActDate(){
	GiftCard giftCard;
	giftCard.setActivationDate("");
	giftCard.setExpirationDate("01/01/2019");
	giftCard.setBalance(500.00);
	string actDate = giftCard.getActivationDate();
	string expDate = giftCard.getExpirationDate();
	double bal = giftCard.getBalance();
	if(actDate == ""){
		cout << "Fail!" << endl;
	}
	else{
		cout << "Green!" << endl;
		cout << actDate << " " << expDate << " " << bal << endl;
	}
}

void testClass::testGiftCardCode(){
	GiftCard giftCard;
	giftCard.setActivationDate("01/01/2016");
	giftCard.setExpirationDate("01/01/2019");
	giftCard.setBalance(500.00);
	giftCard.setCode(0);
	string actDate = giftCard.getActivationDate();
	string expDate = giftCard.getExpirationDate();
	double bal = giftCard.getBalance();
	int code = giftCard.getCode();
	if(code == 0){
		cout << "Fail!" << endl;
	}
	else{
		cout << "Green!" << endl;
		cout << actDate << " " << expDate << " " << bal << " " << code << endl;
	}
}


//class implemations for testing receipt class
void testClass::testStdReceipt(){
	receipt tstReceipt;
	int confNumb;
	double gcBal;
	string pickUpTime;
	tstReceipt.setConfirmationNumber(1);
	tstReceipt.setPickupTime("01/01/2020");
	tstReceipt.setGCBalance(50.00);
	confNumb = tstReceipt.getConfirmationNumber();
	pickUpTime = tstReceipt.getPickupTime();
	gcBal = tstReceipt.getGCBalance();
	cout << endl << confNumb << "   " << pickUpTime << "   " << gcBal << endl;
	cout << "Test for standard receipt has passed" << endl << endl;
}

void testClass::testNegGCBal(){
	receipt tstReceipt;
	int confNumb;
	double gcBal;
	string pickUpTime;
	tstReceipt.setConfirmationNumber(1);
	tstReceipt.setPickupTime("01/01/2020");
	tstReceipt.setGCBalance(50.00);
	confNumb = tstReceipt.getConfirmationNumber();
	pickUpTime = tstReceipt.getPickupTime();
	gcBal = tstReceipt.getGCBalance();
	if (gcBal < 0){
		cout << "Negative balance test has failed" << endl;
	}
	else{
		cout << endl << confNumb << "   " << pickUpTime << "   " << gcBal << endl;
	cout << "Test for negative GC Balance has passed" << endl << endl;
	}
	
}

void testClass::testPickUpTime(){
	receipt tstReceipt;
	int confNumb;
	double gcBal;
	string pickUpTime;
	tstReceipt.setConfirmationNumber(1);
	tstReceipt.setPickupTime("");
	tstReceipt.setGCBalance(50.00);
	confNumb = tstReceipt.getConfirmationNumber();
	pickUpTime = tstReceipt.getPickupTime();
	gcBal = tstReceipt.getGCBalance();
	if (pickUpTime == "" ){
		cout << "Date test has failed" << endl;
	}
	else{
		cout << endl << confNumb << "   " << pickUpTime << "   " << gcBal << endl;
	cout << "Test for non-compatable date has passed" << endl << endl;
	}
	
}

void testClass::testConfirmationNumber(){
	receipt tstReceipt;
	int confNumb;
	double gcBal;
	string pickUpTime;
	tstReceipt.setConfirmationNumber(-1);
	tstReceipt.setPickupTime("01/01/2020");
	tstReceipt.setGCBalance(50.00);
	confNumb = tstReceipt.getConfirmationNumber();
	pickUpTime = tstReceipt.getPickupTime();
	gcBal = tstReceipt.getGCBalance();
	if (confNumb < 0 ){
		cout << "Negative Confirmation Number test has failed" << endl;
	}
	else{
		cout << endl << confNumb << "   " << pickUpTime << "   " << gcBal << endl;
	cout << "Test for Negative Confirmation Number has passed" << endl << endl;
	}
}

void testClass::testPickUpTimeLength(){
	receipt tstReceipt;
	int confNumb;
	double gcBal;
	string pickUpTime;
	tstReceipt.setConfirmationNumber(-1);
	tstReceipt.setPickupTime("01/01/2020/97");
	tstReceipt.setGCBalance(50.00);
	confNumb = tstReceipt.getConfirmationNumber();
	pickUpTime = tstReceipt.getPickupTime();
	gcBal = tstReceipt.getGCBalance();
	if (pickUpTime.length() != 10 ){
		cout << "Pick Up Date form test has failed" << endl;
	}
	else{
		cout << endl << confNumb << "   " << pickUpTime << "   " << gcBal << endl;
	cout << "Test for Pick Up Date form has passed" << endl << endl;
	}
	
}
