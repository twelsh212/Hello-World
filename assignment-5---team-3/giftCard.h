#include <string>

using namespace std;

class GiftCard{

public:
	GiftCard();

	void setActivationDate(string adate);
	void setExpirationDate(string edate);
	void setBalance(double tbal);
	void setCode(int tcode);
	string getActivationDate();
	string getExpirationDate();
	double getBalance();
	int getCode();


private:
	string actDate;
	string expDate;
	double bal;


	int code;

};
