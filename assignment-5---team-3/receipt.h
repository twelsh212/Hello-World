#include <string>

using namespace std;

class receipt {
public:
	receipt();

	void setConfirmationNumber(int pNo);
	void setPickupTime(string pDate);
	void setGCBalance(double pBal = 0);

	int getConfirmationNumber();
	string getPickupTime();
	double getGCBalance();

private:
	int confirmationNumber;
	string pickupTime;
	double gcBalance;
};
