#include <iostream>
#include <random>
#include <tuple>
#include "../NumExpand/NumExpand.h"

using namespace std;
using namespace NumExpand;

int main() {
	while (true) {
		Int p, q;
		q = Int::rand(12);
		p = Int::rand(12);
		system("cls");
		//cout << "q: " << q.toString_binary() << endl;
		//cout << "p: " << p.toString_binary() << endl;
		//cout << "q + p: " << (q + p).toString_binary() << endl;
		//cout << "q * p: " << (q * p).toString_binary() << endl;
		//cout << "q / p: " << (q / p).toString_binary() << endl;
		//cout << "q % p: " << (q % p).toString_binary() << endl;
		//cout << "q - q / p * p: " << (q - (q / p) * p).toString_binary() << endl;
		//cout << "q / p * p + q % p: " << ((q / p) * p + (q % p)).toString_binary() << endl;
		//cout << endl;
		//cout << endl;
#define TEST(x) cout<<#x<<": "<<(x).toString()<<endl;
		TEST(q);
		TEST(-q);
		TEST(p);
		TEST(-p);
		TEST(q + p);
		TEST(q - p);
		cout << endl;
		TEST(q * p);
		TEST(p * q);
		TEST((-q) * p);
		TEST(p * (-q));
		TEST(q * (-p));
		TEST((-p) * q);
		TEST((-q) * (-p));
		TEST((-p) * (-q));
		cout << endl;
		TEST(q / p);
		TEST(q % p);
		TEST((q / p) * p + (q % p));
		TEST(q);
		TEST((-q) / p);
		TEST((-q) % p);
		TEST(((-q) / p) * p + ((-q) % p));
		TEST(-q);
		TEST(q / (-p));
		TEST(q % (-p));
		TEST((q / (-p)) * (-p) + (q % (-p)));
		TEST(q);
		TEST((-q) / (-p));
		TEST((-q) % (-p));
		TEST(((-q) / (-p)) * (-p) + ((-q) % (-p)));
		TEST(-q);

		//cout << "q: " << q.toString() << endl;
		//cout << "p: " << p.toString() << endl;
		//cout << "-q: " << (-q).toString() << endl;
		//cout << "-p: " << p.toString() << endl;
		//cout << "q + p: " << (q + p).toString() << endl;
		//cout << "q - p: " << (q - p).toString() << endl;
		//cout << "q * p: " << (q * p).toString() << endl;
		//cout << "q / p: " << (q / p).toString() << endl;
		//cout << "q % p: " << (q % p).toString() << endl;
		//cout << endl;
		//cout << "q: " << q.toString() << endl;
		//cout << "q / p * p + q % p: " << ((q / p) * p + (q % p)).toString_binary() << endl;

		//cout << (((q / p) * p + (q % p)) == q) << endl;
		system("pause");
	}
}