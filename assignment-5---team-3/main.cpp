#include <iostream>
#include "testClass.h"

using namespace std;

int main(){

	testClass testOrder;

	//tests for order information
	testOrder.testOrderInformation();
	testOrder.testForDefaultValues();
	testOrder.testForInvalidIntegerValues();
	testOrder.testForMultipleDrinks();
	testOrder.testCombiningOrders();

	//test for gift card
	testOrder.testGiftCard();
	testOrder.testGiftCardBal();
	testOrder.testGiftCardExpDate();
	testOrder.testGiftCardActDate();
	testOrder.testGiftCardCode();

	//tests for receipt
	testOrder.testStdReceipt();
	testOrder.testNegGCBal();
	testOrder.testPickUpTime();
	testOrder.testConfirmationNumber();
	testOrder.testPickUpTimeLength();

	return 0;
}

