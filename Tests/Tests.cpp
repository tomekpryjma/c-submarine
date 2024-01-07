#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
	#include "../c-submarine/maths/maths.h"
	#include "../c-submarine/game/compass.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestCompassReturnsCorrectBearing)
		{
			// otherDirection is 0 degrees from cartesian coord system origin but should
			// be 90 degrees away from north. Same goes for the 270 degrees below.
			int expected = 90;
			Vector otherDirection = {1, 0};
			Assert::AreEqual(expected, Compass_GetBearing(otherDirection));
			expected = 270;
			otherDirection = { -1, 0 };
			Assert::AreEqual(expected, Compass_GetBearing(otherDirection));
		}
	};
}
