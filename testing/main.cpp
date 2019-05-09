#include <iostream>
#include <random>
#include "../NumExpand/NumExpand.h"

using namespace std;
using namespace NumExpand;

int main() {
	for(int i =0;i<10;++i)
	cout << Int::rand(1024).toString_binary() << endl;
}