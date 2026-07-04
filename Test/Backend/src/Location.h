#pragma once

#include <string>

using namespace std;

struct Location {
	float longitude = 0;
	float latitude = 0;
	string address;

	Location() {};
	Location(string address, float longitude, float latitude) : address(address), longitude(longitude), latitude(latitude) {};
	
};
