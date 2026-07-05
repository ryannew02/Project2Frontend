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
	Location& operator=(const Location& other) 
	{
		if (this != &other) {
			longitude = other.longitude;
			latitude = other.latitude;
			fullAddress = other.fullAddress;
			addressNumber = other.addressNumber;
			addressName = other.addressName;
			zip = other.zip;
			city = other.city;
		}
		return *this;
	}	
};

