#include "pch.h"
#include "CppUnitTest.h"
#include "../NumExpand/NumExpand.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NumExpand;

Int a(5), b(true, { 134564,456456,45644654,468454132 }), c(true, { 146548678,485486,4,4686684,4654145,54,4456 });
Int d = ~b, e = ~c;

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


}
