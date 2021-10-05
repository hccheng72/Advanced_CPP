#include "Files.h"

void Files::readFile(vector<string>& bs, vector<double>& pc) {
	ifstream file(filename);
	regex barcode("<Barcode>(\\d*)</Barcode>");
	regex price("<Price>(\\d*.\\d{2})</Price>");

	string buffer = "";
	if (file) {
		while (!file.eof()) {
			getline(file, buffer);
			if (buffer.length() > 0) {
				smatch match;
				if (regex_search(buffer, match, barcode)) {
					bs.push_back(move(match[1]));
				}
				else if (regex_search(buffer, match, price)) {
					pc.push_back(move(stod(match[1])));
				}
				buffer = "";
			}
		}
	}
	else {
		cout << "Unable to open file:  " << filename << endl;
	}
	file.close();
}

void Files::readFile(queue<Cart>& cart) {
	ifstream file(filename);
	int count = 1;
	Cart aCart;
	string buffer = "";
	if (file) {
		while (!file.eof()) {
			getline(file, buffer);
			if (buffer.length() > 0) {
				if (count % 2 == 1) {
					aCart.cartNum = buffer;
				}
				else if (count % 2 == 0) {
					vector<string> products;
					sregex_token_iterator end;  // default value is npos
					regex pattern("[0-9A-F]{12}");
					int count = 0;
					for (sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern); pos != end; ++pos)
					{
						if ((*pos).length() > 0)
						{
							if ((static_cast<string>(*pos))[0] != 0x20)  //0x20=whitespace
							{
								products.push_back(*pos);
							}
						}
					}
					aCart.products = products;
					cart.push(move(aCart));
				}

				count++;
				buffer = "";

			}
		}
	}
	else {
		cout << "Unable to open file:  " << filename << endl;
	}
	file.close();
}
