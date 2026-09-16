#include "TimeCode.h"
#include <stdexcept> // for invalid stuff
#include <string>

using namespace std;

// Got help from:
// cplusplus.com for throwing invalid_argument syntax
// StackOverflow for how to convert long long unsigned int to string
// many other issues from google search

TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec)
{
    // just convert all to total secs
    t = ComponentsToSeconds(hr, min, sec);
}

TimeCode::TimeCode(const TimeCode &tc)
{
    t = tc.t;
}

long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec)
{
    // pure manual math, math.h forbidden
    return (hr * 3600ULL) + (min * 60ULL) + sec;
}

void TimeCode::GetComponents(unsigned int &hr, unsigned int &min, unsigned int &sec) const
{
    // extract parts via basic div and mod
    hr = t / 3600;
    unsigned int remaining = t % 3600;
    min = remaining / 60;
    sec = remaining % 60;
}

void TimeCode::SetHours(unsigned int hours)
{
    unsigned int h, m, s;
    GetComponents(h, m, s);
    t = ComponentsToSeconds(hours, m, s);
}

void TimeCode::SetMinutes(unsigned int minutes)
{
    // no roll-over allowed here, othewise a baby squirrel dies.
    // (PETA approved, no animals were harmed in the making of this code)
    if (minutes > 59)
    {
        throw invalid_argument("Minutes cannot exceed 59");
    }
    unsigned int h, m, s;
    GetComponents(h, m, s);
    t = ComponentsToSeconds(h, minutes, s);
}

void TimeCode::SetSeconds(unsigned int seconds)
{
    // same, no roll-over permitted
    if (seconds > 59)
    {
        throw invalid_argument("Seconds cannot exceed 59");
    }
    unsigned int h, m, s;
    GetComponents(h, m, s);
    t = ComponentsToSeconds(h, m, seconds);
}

void TimeCode::reset()
{
    t = 0;
}

unsigned int TimeCode::GetHours() const
{
    unsigned int h, m, s;
    GetComponents(h, m, s);
    return h;
}

unsigned int TimeCode::GetMinutes() const
{
    unsigned int h, m, s;
    GetComponents(h, m, s);
    return m;
}

unsigned int TimeCode::GetSeconds() const
{
    unsigned int h, m, s;
    GetComponents(h, m, s);
    return s;
}

string TimeCode::ToString() const
{
    unsigned int h, m, s;
    GetComponents(h, m, s);
    return to_string(h) + ":" + to_string(m) + ":" + to_string(s);
}

TimeCode TimeCode::operator+(const TimeCode &other) const
{
    return TimeCode(0, 0, this->t + other.t);
}

TimeCode TimeCode::operator-(const TimeCode &other) const
{
    // prevent unsigned underflow nightmare ugh
    if (this->t < other.t)
    {
        throw invalid_argument("Resulting time cannot be negative");
    }
    return TimeCode(0, 0, this->t - other.t);
}

TimeCode TimeCode::operator*(double a) const
{
    // negative time scale makes no sense like "whatttt"
    if (a < 0.0)
    {
        throw invalid_argument("Cannot multiply by a negative number");
    }
    return TimeCode(0, 0, static_cast<long long unsigned int>(this->t * a));
}

TimeCode TimeCode::operator/(double a) const
{
    // catch negative double and zero division, both are forbidden by the universe
    if (a < 0.0)
    {
        throw invalid_argument("Cannot divide by a negative number");
    }
    if (a == 0.0)
    {
        throw invalid_argument("Cannot divide by zero");
    }
    return TimeCode(0, 0, static_cast<long long unsigned int>(this->t / a));
}

// Comparison operators
bool TimeCode::operator==(const TimeCode &other) const { return this->t == other.t; } // Equality
bool TimeCode::operator!=(const TimeCode &other) const { return this->t != other.t; } // Inequality
bool TimeCode::operator<(const TimeCode &other) const { return this->t < other.t; }   // Less than
bool TimeCode::operator<=(const TimeCode &other) const { return this->t <= other.t; } // Less than or equal to
bool TimeCode::operator>(const TimeCode &other) const { return this->t > other.t; }   // Greater than
bool TimeCode::operator>=(const TimeCode &other) const { return this->t >= other.t; } // Greater than or equal to