#include <iostream>
#include "../NumExpand/NumExpand.h"

using namespace std;
using namespace NumExpand;

int main() {
	Int a(5), b(true, {1,2});
	cout << a.toString_binary() << endl;
	cout << b.toString_binary() << endl;
}