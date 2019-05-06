#include "pch.h"
#include "CppUnitTest.h"
#include "../NumExpand/NumExpand.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NumExpand;

namespace UnitTest
{
	TEST_CLASS(Integer_test)
	{
	public:
		
		TEST_METHOD(Inverse_Code)
		{
			Int a(5);
			Assert::IsTrue(a == (~(~a)));
		}
	};
}
