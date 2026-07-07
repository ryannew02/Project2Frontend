#include <iostream>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>
#include "SplayTree.h"
#include "Location.h"
#include "Trie.h"

constexpr double PI = 3.14159265358979323846;

using namespace std;
SplayTree<Location, string> loadSplayCSV(const string& filepath, const string& zipFilepath);
Trie<Location> loadTrieCSV(const string& filepath, const string& zipFilepath);
string getStringValue(const string& json, const string& key);
vector<string> getStringArray(const string& json, const string& key);
double haversine(double lat1, double lon1, double lat2, double lon2);
void runIO(SplayTree<Location, string>& splayTree, Trie<Location>& trieTree);

int main() { 
    string addressFilepath = "../data/addresses.csv";
    string zipCityFilepath = "../data/zip_city.csv";
	SplayTree<Location, string> splayTree = loadSplayCSV(addressFilepath, zipCityFilepath);
	Trie<Location> trieTree = loadTrieCSV(addressFilepath, zipCityFilepath);

	runIO(splayTree, trieTree);
	return 0;
}

//csv parser...

SplayTree<Location, string> loadSplayCSV(const string& filepath,
const string& zipFilepath) {
	SplayTree<Location, string> splayTree;
	map<string, string> zipCityMap;
	ifstream file(filepath);
	if (!file.is_open()) {
    cerr << "FAILED TO OPEN: " << filepath << endl;
	}
	ifstream zipFile(zipFilepath);
	if (!zipFile.is_open()) {
    cerr << "FAILED TO OPEN: " << zipFilepath << endl;
	}

	string line;
	string zipLine;
	
	// Skip header
	getline(zipFile, zipLine);

	while (getline(zipFile, zipLine)) {
		if (zipLine.empty()) continue;

		if (!zipLine.empty() && zipLine.back() == '\r') zipLine.pop_back();

		// Split CSV columns: zip, city
		stringstream ss(zipLine);
		string zipLookup, cityLookup;
		//example format 33801, Lakeland
		getline(ss, zipLookup, ',');
		getline(ss, cityLookup);
		zipCityMap[zipLookup] = cityLookup;
	}

	// Skip header
	getline(file, line);

	while (getline(file, line)) {
		if (line.empty()) continue;

		if (!line.empty() && line.back() == '\r') line.pop_back();

		// Split CSV columns: lat, lon, addressNumber, addressName, unit, zip
		stringstream ss(line);
		string latStr, lonStr, addressNumber, addressName, unit, zip;
		//example format 28.56656102,-81.36566349,1650,N Mills Ave,Apartment 313,32828
		getline(ss, latStr, ',');
		getline(ss, lonStr, ',');
		getline(ss, addressNumber, ',');
		getline(ss, addressName, ',');
		getline(ss, unit, ',');
		getline(ss, zip);

		// Store CSV row values in an Address object
		Location* a = new Location();
		a->latitude = stod(latStr);
		a->longitude = stod(lonStr);
		a->addressNumber = addressNumber;
		a->addressName = addressName;
		a->zip = zip;

		if (zipCityMap.count(zip)) {
			a->city = zipCityMap.at(zip);
		}

		if (!unit.empty()) {

			a->fullAddress = addressNumber + " " + addressName + " " + unit;

		} else {

			a->fullAddress = addressNumber + " " + addressName;

		}

		// Store address using the display address as the lookup key
		string key = a->fullAddress;
		splayTree.insert(a , key);
	}

	return splayTree;
}

Trie<Location> loadTrieCSV(const string& filepath,
const string& zipFilepath) {
	Trie<Location> trieTree;
	map<string, string> zipCityMap;
	ifstream file(filepath);
	ifstream zipFile(zipFilepath);
	string line;
	string zipLine;

	// Skip header
	getline(zipFile, zipLine);

	while (getline(zipFile, zipLine)) {
		if (zipLine.empty()) continue;

		if (!zipLine.empty() && zipLine.back() == '\r') zipLine.pop_back();

		// Split CSV columns: zip, city
		stringstream ss(zipLine);
		string zipLookup, cityLookup;
		//example format 33801, Lakeland
		getline(ss, zipLookup, ',');
		getline(ss, cityLookup);
		zipCityMap[zipLookup] = cityLookup;
	}

	// Skip header
	getline(file, line);

	while (getline(file, line)) {
		if (line.empty()) continue;

		if (!line.empty() && line.back() == '\r') line.pop_back();

		// Split CSV columns: lat, lon, addressNumber, addressName, unit, zip
		stringstream ss(line);
		string latStr, lonStr, addressNumber, addressName, unit, zip;
		//example format 28.56656102,-81.36566349,1650,N Mills Ave,Apartment 313,32828
		getline(ss, latStr, ',');
		getline(ss, lonStr, ',');
		getline(ss, addressNumber, ',');
		getline(ss, addressName, ',');
		getline(ss, unit, ',');
		getline(ss, zip);

		// Store CSV row values in an Address object
		Location* a = new Location();
		a->latitude = stod(latStr);
		a->longitude = stod(lonStr);
		a->addressNumber = addressNumber;
		a->addressName = addressName;
		a->zip = zip;

		if (zipCityMap.count(zip)) {
			a->city = zipCityMap.at(zip);
		}

		if (!unit.empty()) {

			a->fullAddress = addressNumber + " " + addressName + " " + unit;

		} else {

			a->fullAddress = addressNumber + " " + addressName;

		}

		// Store address using the display address as the lookup key
		string key = a->fullAddress;
		trieTree.insert(a , key);
	}

	return trieTree;
}

// _______________________________ I/O handling ______________________________

// _____________ I/O helpers ____________________
string getStringValue(const string& json, const string& key) {
    string search = "\"" + key + "\":";
    int pos = json.find(search);
    if (pos == string::npos) return "";
    pos += search.length();
    while (pos < json.size() && json[pos] == ' ') pos++;
    if (json[pos] == '"') {
        pos++;
        int end = json.find('"', pos);
        return json.substr(pos, end - pos);
    }
    int end = json.find_first_of(",}", pos);
    return json.substr(pos, end - pos);
}

vector<string> getStringArray(const string& json, const string& key) {
    vector<string> results;
    string search = "\"" + key + "\":[";
    int pos = json.find(search);
    if (pos == string::npos) return results;
    pos += search.length();
    int end = json.find(']', pos);
    string arrayContent = json.substr(pos, end - pos);
    int i = 0;
    while (i < arrayContent.size()) {
        if (arrayContent[i] == '"') {
            i++;
            int close = arrayContent.find('"', i);
            results.push_back(arrayContent.substr(i, close - i));
            i = close + 1;
        } else {
            i++;
        }
    }
    return results;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double EarthR = 3958.8;
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) *
               sin(dLon/2) * sin(dLon/2);
    return EarthR * 2 * atan2(sqrt(a), sqrt(1-a));
}

void runIO(SplayTree<Location, string>& splayTree, Trie<Location>& trieTree){
string input;
    // Read incoming JSON from Node
    while(getline(cin, input))
    {    
        if (input.empty()) continue;

        vector<string> requestedAddresses = getStringArray(input, "addresses");
        string passengers = getStringValue(input, "passengers");
		string algorithm = getStringValue(input, "algorithm");

        // Look up each address and build response
        string foundJson = "[";
        double totalDistance = 0.0;
        Location prev;
        bool hasPrev = false;

        for (unsigned int i = 0; i < requestedAddresses.size(); i++) {
            string req = requestedAddresses[i];
            
			if(algorithm == "splay")
			{
				Location* a = splayTree.search(req);
				if(a == nullptr){
                cerr << "NOT FOUND: " << req << endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            	if (i < requestedAddresses.size() - 1) foundJson += ",";
				continue;
				}
				else if (hasPrev) {
					totalDistance += haversine(prev.latitude, prev.longitude, a->latitude, a->longitude);
					}
					prev = *a;
					hasPrev = true;

					foundJson += "{\"address\":\"" + a->fullAddress + "\","
							+ "\"city\":\"" + a->city + "\","
							+ "\"zip\":\"" + a->zip + "\","
							+ "\"lat\":" + to_string(a->latitude) + ","
							+ "\"lon\":" + to_string(a->longitude) + "}";

				if (i < requestedAddresses.size() - 1) foundJson += ",";
			}

			else if(algorithm == "trie")
			{
                Location* a = trieTree.search(req);
				if(a == nullptr){
                cerr << "NOT FOUND: " << req << endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            	if (i < requestedAddresses.size() - 1) foundJson += ",";
				continue;
            	}
                else if (hasPrev) {
                    totalDistance += haversine(prev.latitude, prev.longitude, a->latitude, a->longitude);
                }
                prev = *a;
                hasPrev = true;

                foundJson += "{\"address\":\"" + a->fullAddress + "\","
                        + "\"city\":\"" + a->city + "\","
                        + "\"zip\":\"" + a->zip + "\","
                        + "\"lat\":" + to_string(a->latitude) + ","
                        + "\"lon\":" + to_string(a->longitude) + "}";

            	if (i < requestedAddresses.size() - 1) foundJson += ",";
			}
			else {
                cerr << "NOT FOUND: " << req << endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            	if (i < requestedAddresses.size() - 1) foundJson += ",";
            }
			
        }
        foundJson += "]";

        // Build response
        string response = "{";
        response += "\"status\":\"ok\",";
        response += "\"passengers\":" + passengers + ",";
        response += "\"total_distance_miles\":" + to_string(totalDistance) + ",";
        response += "\"route\":" + foundJson;
        response += "}";

        //Pipe output and clean up buffer for reuse
        cout << response << "\n";
        cout.flush();
    }
}