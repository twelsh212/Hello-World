#include <iostream>

using namespace std;

class testClass{
public:
	//tests for receipt class
	testClass();
	void testStdReceipt();
	void testNegGCBal();
	void testPickUpTime();
	void testConfirmationNumber();
	void testPickUpTimeLength();

	//tests for gift card class
	void testGiftCard();
	void testGiftCardBal();
	void testGiftCardExpDate();
	void testGiftCardActDate();
	void testGiftCardCode();

	//tests for order information
	void testOrderInformation();
	void testForDefaultValues();
	void testForInvalidIntegerValues();
	void testForMultipleDrinks();
	void testCombiningOrders();
};
