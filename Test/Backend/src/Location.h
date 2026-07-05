#pragma once

#include <string>

using namespace std;

struct Location {
	double longitude = 0;
	double latitude = 0;
    string fullAddress;
    string addressNumber;
    string addressName;
    string zip;
    string city;
	Location() {};
	Location(string address, double longitude, double latitude) : fullAddress(address), longitude(longitude), latitude(latitude) {};
	
};
