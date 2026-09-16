#include <iostream>
#include <assert.h>
#include <stdexcept>
using namespace std;

#include "TimeCode.h"

void TestComponentsToSeconds()
{
	cout << "Testing ComponentsToSeconds" << endl;

	// Random but "safe" inputs
	long long unsigned int t = TimeCode::ComponentsToSeconds(3, 17, 42);
	assert(t == 11862);

	// huge cursed input to verify raw seconds calculation
	long long unsigned int t_beast = TimeCode::ComponentsToSeconds(0, 0, 114514);
	assert(t_beast == 114514);

	long long unsigned int t_huge = TimeCode::ComponentsToSeconds(10, 20, 676767676767ULL);
	assert(t_huge == 676767713967ULL);

	cout << "PASSED!" << endl
		 << endl;
}

void TestDefaultConstructor()
{
	cout << "Testing Default Constructor" << endl;
	TimeCode tc;

	assert(tc.ToString() == "0:0:0");
	assert(tc.GetTimeCodeAsSeconds() == 0);

	cout << "PASSED!" << endl
		 << endl;
}

void TestComponentConstructor()
{
	cout << "Testing Component Constructor" << endl;
	TimeCode tc = TimeCode(0, 0, 0);
	assert(tc.ToString() == "0:0:0");

	// Roll-over inputs
	TimeCode tc3 = TimeCode(3, 71, 3801);
	assert(tc3.ToString() == "5:14:21");

	// absurd seconds rollover into hours and mins
	TimeCode tc_homo = TimeCode(0, 0, 114514);
	assert(tc_homo.ToString() == "31:48:34");

	// gigantic roll-over should not cause unsigned overflow
	TimeCode tc_senpai = TimeCode(0, 0, 1919810);
	assert(tc_senpai.ToString() == "533:16:50");

	TimeCode tc_nice = TimeCode(0, 0, 6969696969ULL);
	assert(tc_nice.ToString() == "1936026:56:9");

	TimeCode tc_titan = TimeCode(0, 0, 676767676767ULL);
	assert(tc_titan.ToString() == "187991021:19:27");

	cout << "PASSED!" << endl
		 << endl;
}

void TestCopyConstructor()
{
	cout << "Testing Copy Constructor" << endl;

	// copy must be an exact clone or life is meaningless
	TimeCode tc1(0, 0, 114514);
	TimeCode tc2(tc1);
	assert(tc2.ToString() == "31:48:34");
	assert(tc2.GetTimeCodeAsSeconds() == tc1.GetTimeCodeAsSeconds());

	cout << "PASSED!" << endl
		 << endl;
}

void TestGetComponents()
{
	cout << "Testing GetComponents" << endl;

	unsigned int h;
	unsigned int m;
	unsigned int s;

	// Regular values
	TimeCode tc = TimeCode(5, 2, 18);
	tc.GetComponents(h, m, s);
	assert(h == 5 && m == 2 && s == 18);

	// testing deconstruction of cursed timestamps
	TimeCode tc_senpai(0, 0, 1919810);
	tc_senpai.GetComponents(h, m, s);
	assert(h == 533 && m == 16 && s == 50);

	cout << "PASSED!" << endl
		 << endl;
}

void TestGetters()
{
	cout << "Testing Getters" << endl;

	TimeCode tc(0, 0, 114514);
	assert(tc.GetHours() == 31);
	assert(tc.GetMinutes() == 48);
	assert(tc.GetSeconds() == 34);

	cout << "PASSED!" << endl
		 << endl;
}

void TestSettersAndReset()
{
	cout << "Testing Setters and Reset" << endl;

	TimeCode tc(1, 20, 30);

	tc.SetHours(114514);
	assert(tc.GetHours() == 114514);
	assert(tc.GetMinutes() == 20 && tc.GetSeconds() == 30);

	tc.SetMinutes(59);
	assert(tc.GetMinutes() == 59);

	try
	{
		tc.SetMinutes(60);
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	try
	{
		tc.SetMinutes(114514);
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	tc.SetSeconds(59);
	assert(tc.GetSeconds() == 59);

	try
	{
		tc.SetSeconds(60);
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	try
	{
		tc.SetSeconds(1919810);
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	// wiping time back to the void
	tc.reset();
	assert(tc.ToString() == "0:0:0");
	assert(tc.GetTimeCodeAsSeconds() == 0);

	cout << "PASSED!" << endl
		 << endl;
}

void TestSetMinutes()
{
	cout << "Testing SetMinutes" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetMinutes(15); // test valid change
	assert(tc.ToString() == "8:15:9");

	try
	{
		tc.SetMinutes(80); // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:15:9");

	cout << "PASSED!" << endl
		 << endl;
}

void TestAdd()
{
	cout << "Testing Add" << endl;

	// adding massive numbers without breaking rollover
	TimeCode tc1(0, 0, 114514);
	TimeCode tc2(0, 0, 1919810);
	TimeCode res = tc1 + tc2;
	assert(res.GetTimeCodeAsSeconds() == 2034324);
	assert(res.ToString() == "565:5:24");

	cout << "PASSED!" << endl
		 << endl;
}

void TestSubtract()
{
	cout << "Testing Subtract" << endl;
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(0, 50, 0);
	TimeCode tc3 = tc1 - tc2;
	assert(tc3.ToString() == "0:10:0");

	TimeCode tc4 = TimeCode(1, 15, 45);
	try
	{
		TimeCode tc5 = tc1 - tc4;
		cout << "tc5: " << tc5.ToString() << endl;
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// just leave this empty
		// and keep doing more tests
	}

	// subtracting larger time must fail immediately
	TimeCode beast(0, 0, 114514);
	TimeCode boss(0, 0, 1919810);
	TimeCode sub = boss - beast;
	assert(sub.ToString() == "501:28:16");

	try
	{
		TimeCode doomed = beast - boss;
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	cout << "PASSED!" << endl
		 << endl;
}

void TestMultiply()
{
	cout << "Testing Multiply" << endl;

	TimeCode tc(0, 0, 114514);
	TimeCode double_tc = tc * 2.0;
	assert(double_tc.GetTimeCodeAsSeconds() == 229028);

	// scaling by negative double must suffer and throw
	try
	{
		TimeCode cursed = tc * -1.0;
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	cout << "PASSED!" << endl
		 << endl;
}

void TestDivide()
{
	cout << "Testing Divide" << endl;

	TimeCode tc(0, 0, 1919810);
	TimeCode half = tc / 2.0;
	assert(half.GetTimeCodeAsSeconds() == 959905);

	// division by zero is forbidden by the universe
	try
	{
		TimeCode void_tc = tc / 0.0;
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	// dividing by negative is equally absurd
	try
	{
		TimeCode neg_tc = tc / -1919810.0;
		assert(false);
	}
	catch (const invalid_argument &)
	{
	}

	cout << "PASSED!" << endl
		 << endl;
}

void TestComparisons()
{
	cout << "Testing Comparisons" << endl;

	// comparing cursed numbers against each other
	TimeCode t1(0, 0, 114514);
	TimeCode t2(0, 0, 1919810);
	TimeCode t3(0, 0, 6969696969ULL);
	TimeCode t4(0, 0, 676767676767ULL);
	TimeCode t1_clone(0, 0, 114514);

	assert(t1 == t1_clone);
	assert(t1 != t2);
	assert(t1 < t2);
	assert(t2 < t3);
	assert(t3 < t4);
	assert(t4 > t3);
	assert(t1 <= t1_clone);
	assert(t4 >= t1);

	cout << "PASSED!" << endl
		 << endl;
}

int main()
{
	TestComponentsToSeconds();
	TestDefaultConstructor();
	TestComponentConstructor();
	TestCopyConstructor();
	TestGetComponents();
	TestGetters();
	TestSettersAndReset();
	TestSetMinutes();
	TestAdd();
	TestSubtract();
	TestMultiply();
	TestDivide();
	TestComparisons();

	cout << "PASSED ALL TESTS!!!" << endl;
	return 0;

	// Summary of added tests:
	// Added absurd number cases to verify massive seconds rollover behavior.
	// Added tests for operator* negative scaling and operator/ division by zero.
	// Added strict bounds checking tests for SetMinutes and SetSeconds over 59.
	// Added subtraction underflow tests to ensure negative times throw invalid_argument.
	// Added complete coverage for all six comparison operators and copy constructor.
}
