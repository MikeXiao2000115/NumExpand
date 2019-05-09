#include "pch.h"
#include "CppUnitTest.h"
#include <random>
#include "../NumExpand/NumExpand.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NumExpand;

Int a(5), b(true, { 134564,456456,45644654,468454132 }), c(true, { 146548678,485486,4,4686684,4654145,54,4456 });
Int d = ~b, e = ~c;

Int rand(unsigned a, unsigned b) {
	static std::random_device r;
	static std::default_random_engine e1(r());
	static std::uniform_int_distribution<unsigned> uniform_dist(a, b);
	static std::uniform_int_distribution<Int::base_type> uniform_dist2(0, static_cast<unsigned long>(-1));
	unsigned times = uniform_dist(e1);
	Int out(0);
	for (unsigned i = 0; i < times; ++i)
		out += uniform_dist2(e1);
	return out;
}

namespace UnitTest
{
	TEST_CLASS(Inverse_Code)
	{
	public:
		TEST_METHOD(test_a)
		{
			Assert::IsTrue(a == (~(~a)));
		}
		TEST_METHOD(test_b)
		{
			Assert::IsTrue(b == (~(~b)));
		}
		TEST_METHOD(test_c)
		{
			Assert::IsTrue(c == (~(~c)));
		}
	};

	TEST_CLASS(OR_AND_XOR)
	{
	public:
		TEST_METHOD(b_c)
		{
			Assert::IsTrue((b ^ c) == ((b & (~c)) | ((~b) & c)));
		}
		TEST_METHOD(d_c)
		{
			Assert::IsTrue((d ^ c) == ((d & (~c)) | ((~d) & c)));
		}
		TEST_METHOD(b_e)
		{
			Assert::IsTrue((b ^ e) == ((b & (~e)) | ((~b) & e)));
		}

	};
	TEST_CLASS(ADD_NEGTIVE)
	{
	public:
		TEST_METHOD(random)
		{
			for (unsigned k = 0; k < 1024; ++k) {
				Int a = rand(), b = rand();
				Int c = -a, d = -b;
#define test(x,y) Assert::IsTrue((x) + (y) + (-(y)) == (x))
				test(a, a);
				test(a, b);
				test(a, c);
				test(a, d);
				test(b, a);
				test(b, b);
				test(b, c);
				test(b, d);
				test(c, a);
				test(c, b);
				test(c, c);
				test(c, d);
				test(d, a);
				test(d, b);
				test(d, c);
				test(d, d);
			}
		}
		TEST_METHOD(b_c)
		{
			Assert::IsTrue(b + c + (-c) == b);
		}
		TEST_METHOD(d_c)
		{
			Assert::IsTrue(d + c + (-c) == d);
		}
		TEST_METHOD(b_e)
		{
			Assert::IsTrue(b + e + (-e) == b);
		}
		TEST_METHOD(perform)
		{
			std::random_device r;
			std::default_random_engine e1(r());
			std::uniform_int_distribution<unsigned> uniform_dist(0, static_cast<unsigned long>(-1));
			std::uniform_int_distribution<unsigned> uniform_dist2(0, 1);
			Int a(0);
			for (unsigned j = 0; j < 1000; ++j)
				for (unsigned k = 0; k < 1000; ++k)
					if (uniform_dist2(e1))
						a += uniform_dist2(e1);
					else
						a += -Int(uniform_dist2(e1));
		}
	};

}
