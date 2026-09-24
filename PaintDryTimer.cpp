#include <ctime>	  // for time(0)
#include <iostream>	  // for cin and cout
#include <cmath>	  // for M_PI and others
#include <string>	  // for to_string
#include <vector>	  // for vectors (duh)
#include <cstdlib>	  // for random
#include <cassert>	  // for assert in the tests() function
#include "TimeCode.h" // for timecode's (duh again)

using namespace std;

// read the notes blah blah blah blah

struct DryingSnapShot
{
	string name;
	time_t startTime;
	TimeCode *timeToDry;
};

long long int get_time_remaining(DryingSnapShot dss)
{
	time_t now = time(0);
	long long int elapsed = now - dss.startTime;
	long long int total_seconds = dss.timeToDry->GetTimeCodeAsSeconds();
	return total_seconds - elapsed;
}

string drying_snap_shot_to_string(DryingSnapShot dss)
{
	long long int rem = get_time_remaining(dss);
	string prefix = dss.name + " (takes " + dss.timeToDry->ToString() + " to dry) ";
	if (rem > 0)
	{
		unsigned int h = rem / 3600;
		unsigned int m = (rem % 3600) / 60;
		unsigned int s = rem % 60;
		TimeCode rem_tc(h, m, s);
		return prefix + "time remaining: " + rem_tc.ToString();
	}
	else
	{
		return prefix + "DONE!";
	}
}

double get_sphere_sa(double rad)
{
	return 4.0 * acos(-1.0) * rad * rad;
}

TimeCode *compute_time_code(double surfaceArea)
{
	unsigned int total_seconds = (unsigned int)surfaceArea;
	unsigned int h = total_seconds / 3600;
	unsigned int m = (total_seconds % 3600) / 60;
	unsigned int s = total_seconds % 60;
	return new TimeCode(h, m, s);
}

void tests()
{
	// get_time_remaining
	DryingSnapShot dss;
	dss.startTime = time(0);
	TimeCode tc = TimeCode(0, 0, 7);
	dss.timeToDry = &tc;
	long long int ans = get_time_remaining(dss);
	assert(ans > 6 && ans < 8);

	// get_time_remaining expired
	DryingSnapShot dss_expired;
	dss_expired.startTime = time(0) - 10;
	dss_expired.timeToDry = &tc;
	assert(get_time_remaining(dss_expired) <= 0);

	// get_sphere_sa
	double sa = get_sphere_sa(2.0);
	assert(50.2654 < sa && sa < 50.2655);

	// get_sphere_sa edge cases
	assert(get_sphere_sa(0.0) == 0.0);
	double sa1 = get_sphere_sa(1.0);
	assert(sa1 > 12.56 && sa1 < 12.57);

	// compute_time_code
	TimeCode *tc2 = compute_time_code(1.0);
	assert(tc2->GetTimeCodeAsSeconds() == 1);
	delete tc2;

	// get_time_remaining with compute_time_code
	TimeCode *tc3 = compute_time_code(3665.0);
	assert(tc3->GetHours() == 1 && tc3->GetMinutes() == 1 && tc3->GetSeconds() == 5);
	delete tc3;

	cout << "ALL TESTS PASSED!" << endl;
}

int main()
{
	tests();

	srand(time(0));
	vector<DryingSnapShot> batches;

	while (true)
	{
		cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
		char choice;
		if (!(cin >> choice))
		{
			break;
		}

		if (choice == 'A' || choice == 'a')
		{
			cout << "radius: ";
			double r;
			cin >> r;

			double sa = get_sphere_sa(r);
			TimeCode *tc = compute_time_code(sa);

			DryingSnapShot dss;
			dss.name = "Batch-" + to_string(rand());
			dss.startTime = time(0);
			dss.timeToDry = tc;

			batches.push_back(dss);
			cout << drying_snap_shot_to_string(dss) << endl;
		}
		else if (choice == 'V' || choice == 'v')
		{
			// loop through the batches and print out the drying_snap_shot_to_string for each one
			for (size_t i = 0; i < batches.size(); ++i)
			{
				cout << drying_snap_shot_to_string(batches[i]) << endl;
			}
			cout << batches.size() << " batches being tracked." << endl;

			// loop through the batches and remove any that are done drying (time remaining <= 0)
			vector<DryingSnapShot> active_batches;
			for (size_t i = 0; i < batches.size(); ++i)
			{
				if (get_time_remaining(batches[i]) <= 0)
				{
					delete batches[i].timeToDry; //
				}
				else
				{
					active_batches.push_back(batches[i]);
				}
			}
			batches = active_batches;
		}
		else if (choice == 'Q' || choice == 'q')
		{
			// loop through the batches and delete the timeToDry pointers, then clear the vector
			for (size_t i = 0; i < batches.size(); ++i)
			{
				delete batches[i].timeToDry;
			}
			batches.clear();
			break;
		}
	}

	return 0;
	// yay end of main
}