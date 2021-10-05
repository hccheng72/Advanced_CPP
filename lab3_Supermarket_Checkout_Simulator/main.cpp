#include "Supermarket.h"
using namespace std;

int main() {
	Supermarket afile("ProductPrice.xml", "Carts.csv");

	vector<shared_ptr<LaneThreaded<Cart>>> lanes(5, shared_ptr<LaneThreaded<Cart>>(new LaneThreaded<Cart>));
	thread addcart[5];
	thread check[5];
	for (int i = 0; i < 5; i++) {
		addcart[i] = thread(&Supermarket::addCartToLane, ref(afile), ref(lanes[i]));
	}
	for (int i = 0; i < 5; i++) {
		check[i] = thread(&Supermarket::checkout, ref(afile), ref(lanes[i]));
	}

	for (auto& t : addcart)
		t.join();
	for (auto& t : check)
		t.join();

	return 0;
}