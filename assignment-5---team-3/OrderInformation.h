#include <string>

using namespace std;

class OrderInformation{

public:
	OrderInformation();
	void setCustomerName(string pname = "Guest");
	void setDrinkDescription(string pdesc = "No Drink", string pdesc2 = "", string pdesc3 = "");
	void setGiftCardNumber(int pgetNum = 0);
	void setPreferredStoreNumber(int pnum);

	string getCustomerName();
	string getDrinkDescription();
	int getGiftCardNumber();
	int getPreferredStoreNumber();

	OrderInformation operator+(const OrderInformation& pOrder);
	
private:
	string customerName;
	string drinkDescription;
	int giftCardNumber;
	int preferredStoreNumber;
};
