#include <iostream>
#include "../NumExpand/NumExpand.h"

using namespace std;
using namespace NumExpand;

int main() {
	Int a(5), b(true, {141556,2464546}), c(true, { 14545456,15416542,1313154658 });
	cout << b.toString_binary() << endl;
	cout << (~b).toString_binary() << endl;
	cout << endl;
	cout << c.toString_binary() << endl;
	cout << (~c).toString_binary() << endl;
	cout << endl;
	cout << b.toString_binary() << endl;
	cout << c.toString_binary() << endl;
	cout << (b|c).toString_binary() << endl;
	cout << (b&c).toString_binary() << endl;
	cout << (b^c).toString_binary() << endl;
	cout << ((b & (~c)) | ((~b) & c)).toString_binary() << endl;
	cout << (((~b) & c)).toString_binary() << endl;
	cout << ((b & (~c))).toString_binary() << endl;
	cout << ((b & (~c)) | ((~b) & c)).toString_binary() << endl;
}