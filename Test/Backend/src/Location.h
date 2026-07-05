#pragma once

#include <string>

using namespace std;

struct Location {
	double longitude = 0;
	double latitude = 0;
	string address;

	Location() {};
	Location(string address, double longitude, double latitude) : address(address), longitude(longitude), latitude(latitude) {};
	
};
