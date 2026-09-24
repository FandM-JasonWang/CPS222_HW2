#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "TimeCode.h"

using namespace std;

// read the notes blah blah blah blah

vector<string> split(const string &str, char delim)
{ // Split a string by a delimiter
    vector<string> tokens;
    string temp = "";
    // Loop through each character in the string and build tokens based on the delimiter and push them into the vector
    for (size_t i = 0; i < str.length(); ++i)
    { //
        if (str[i] == delim)
        {
            tokens.push_back(temp);
            temp = "";
        }
        else
        {
            temp += str[i];
        }
    }
    tokens.push_back(temp);
    return tokens;
}

bool parse_line(const string &line, TimeCode &tc)
{
    // Find the position of "UTC" in the line
    size_t utc_pos = line.find("UTC");
    if (utc_pos == string::npos)
    {
        return false;
    }

    // Find the last non-space character before "UTC"
    int end = (int)utc_pos - 1;
    while (end >= 0 && (line[end] == ' ' || line[end] == '\t'))
    { // Skip whitespace
        end--;
    }
    if (end < 0)
        return false;

    int start = end;
    while (start >= 0 && line[start] != ' ' && line[start] != '"' && line[start] != ',')
    { // Find the start of the time string
        start--;
    }

    string time_str = line.substr(start + 1, end - start); // Extract the time string

    // Check if the time string contains a colon
    if (time_str.find(':') == string::npos)
    {
        return false;
    }

    vector<string> parts = split(time_str, ':');
    if (parts.size() < 2)
    {
        return false;
    }

    try
    {
        unsigned int hours = stoi(parts[0]);
        unsigned int minutes = stoi(parts[1]);
        unsigned int seconds = 0;
        if (parts.size() >= 3)
        {
            seconds = stoi(parts[2]);
        }
        tc = TimeCode(hours, minutes, seconds);
        return true;
    }
    catch (...)
    { // Catch any exceptions
        return false;
    }
}

int main()
{
    ifstream file("Space_Corrected.csv");
    if (!file.is_open())
    {
        cout << "Could not open file Space_Corrected.csv" << endl;
        return 1;
    }

    vector<TimeCode> launch_times;
    string line;
    while (getline(file, line))
    {
        TimeCode tc;
        if (parse_line(line, tc))
        {
            launch_times.push_back(tc);
        }
    }
    file.close();

    if (launch_times.empty())
    {
        cout << "Oops, 0 data points." << endl;
        return 0;
    }

    TimeCode sum;
    for (size_t i = 0; i < launch_times.size(); ++i)
    {
        sum = sum + launch_times[i];
    }

    TimeCode avg = sum / launch_times.size();

    cout << launch_times.size() << " data points." << endl;
    cout << "AVERAGE: " << avg.ToString() << endl;

    return 0;
}