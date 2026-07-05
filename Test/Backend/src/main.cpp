#include <iostream>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SplayTree.h"
#include "Location.h"
#include "Trie.h"

using namespace std;

void print(SplayTree<Location, string> tree);

int main() {
	SplayTree<Location, string> splayTree;

	Location loc1("1313 N Collier Blvd", 5.8f, 5.2f);
	Location loc2("4365 Kauila St", 2.4f, 8.1f);
	Location loc3("2090 Hanalima St", 1.8f, 9.4f);

	splayTree.insert(&loc1, loc1.fullAddress);
	print(splayTree);
	splayTree.insert(&loc2, loc2.fullAddress);
	print(splayTree);
	splayTree.insert(&loc3, loc3.fullAddress);
	print(splayTree);

	return 0;
}

void print(SplayTree<Location, string> tree) {
	auto dataPtrs = tree.getPreOrder();
	for (unsigned int i = 0; i < dataPtrs.size(); i++)
		cout << dataPtrs[i]->fullAddress << endl;
	cout << endl << endl;
}

//_____________________CSV Parser needs to be updated to accomadate new object_______________
//_____________________CSV Parser needs to be updated to accomadate new object_______________
//_____________________CSV Parser needs to be updated to accomadate new object_______________

			// // ─── Start of updated CSV Parser ────────────────────────────────────────────────────────────
			// std::map<std::string, std::string> loadZipCity(const std::string& filepath) {
			//     std::map<std::string, std::string> zipCityMap;
			//     std::ifstream file(filepath);
			//     std::string line;

			//     // Skip header
			//     std::getline(file, line);

			//     while (std::getline(file, line)) {
			//         if (line.empty()) continue;

			//         if (!line.empty() && line.back() == '\r') line.pop_back();

			//         std::stringstream ss(line);
			//         std::string zip;
			//         std::string city;

			//         std::getline(ss, zip, ',');
			//         std::getline(ss, city, ',');

			//         zipCityMap[zip] = city;
			//     }

			//     return zipCityMap;
			// }

			// // ─── Original CSV Parser ────────────────────────────────────────────────────────────
			// std::map<std::string, Address> loadCSV(const std::string& filepath,
			//                                        const std::map<std::string, std::string>& zipCityMap) {
			//     std::map<std::string, Address> addressMap;
			//     std::ifstream file(filepath);
			//     std::string line;

			//     // Skip header
			//     std::getline(file, line);

			//     while (std::getline(file, line)) {
			//         if (line.empty()) continue;

			//         // Remove carriage return if present
			//         if (!line.empty() && line.back() == '\r') line.pop_back();

			//         // Split CSV columns: lat, lon, addressNumber, addressName, unit, zip
			//         std::stringstream ss(line);
			//         std::string latStr, lonStr, addressNumber, addressName, unit, zip;

			//         std::getline(ss, latStr, ',');
			//         std::getline(ss, lonStr, ',');
			//         std::getline(ss, addressNumber, ',');
			//         std::getline(ss, addressName, ',');
			//         std::getline(ss, unit, ',');
			//         std::getline(ss, zip, ',');

			//         // Store CSV row values in an Address object
			//         Address a;
			//         a.lat = std::stod(latStr);
			//         a.lon = std::stod(lonStr);
			//         a.addressNumber = addressNumber;
			//         a.addressName = addressName;
			//         a.zip = zip;

			//         if (zipCityMap.count(zip)) {
			//             a.city = zipCityMap.at(zip);
			//         }

			//         if (!unit.empty()) {

			//             a.fullAddress = addressNumber + " " + addressName + " " + unit;

			//         } else {

			//             a.fullAddress = addressNumber + " " + addressName;

			//         }

			//         // Store address using the display address as the lookup key
			//         std::string key = a.fullAddress;
			//         addressMap[key] = a;
			//     }

			//     return addressMap;
			// }

//_____________________CSV Parser needs to be updated to accomadate new object_______________
//_____________________CSV Parser needs to be updated to accomadate new object_______________
//_____________________CSV Parser needs to be updated to accomadate new object_______________



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
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
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
            const string& req = requestedAddresses[i];
//need to pass an option on which tree to search like a boolean...
//need to find a replacement for count function to verify address is in the listing or implement a try catch response in the tree search algo
//need to identify how to pass the details into the tree search algo
            
			if(algorithm == "splay")
			{

			if (addressMap.count(req)) {
                Location& a = addressMap[req];

                if (hasPrev) {
                    totalDistance += haversine(prev.latitude, prev.longitude, a.latitude, a.longitude);
                }
                prev = a;
                hasPrev = true;

                foundJson += "{\"address\":\"" + a.fullAddress + "\","
                        + "\"city\":\"" + a.city + "\","
                        + "\"zip\":\"" + a.zip + "\","
                        + "\"lat\":" + to_string(a.latitude) + ","
                        + "\"lon\":" + to_string(a.longitude) + "}";
            } else {
                cerr << "NOT FOUND: " << req << endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            }

            if (i < requestedAddresses.size() - 1) foundJson += ",";
				
			}

			if(algorithm == "trie")
			{


			if (addressMap.count(req)) {
                Location& a = addressMap[req];

                if (hasPrev) {
                    totalDistance += haversine(prev.latitude, prev.longitude, a.latitude, a.longitude);
                }
                prev = a;
                hasPrev = true;

                foundJson += "{\"address\":\"" + a.fullAddress + "\","
                        + "\"city\":\"" + a.city + "\","
                        + "\"zip\":\"" + a.zip + "\","
                        + "\"lat\":" + to_string(a.latitude) + ","
                        + "\"lon\":" + to_string(a.longitude) + "}";
            } else {
                cerr << "NOT FOUND: " << req << endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            }

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