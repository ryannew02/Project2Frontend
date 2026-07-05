#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

// ─── Data structure ────────────────────────────────────────────────────────
struct Address {
    double lat;
    double lon;
    std::string fullAddress;
    std::string addressNumber;
    std::string addressName;
    std::string zip;
    std::string city;
};

// ─── CSV Parser ────────────────────────────────────────────────────────────
std::map<std::string, std::string> loadZipCity(const std::string& filepath) {
    std::map<std::string, std::string> zipCityMap;
    std::ifstream file(filepath);
    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string zip;
        std::string city;

        std::getline(ss, zip, ',');
        std::getline(ss, city, ',');

        zipCityMap[zip] = city;
    }

    return zipCityMap;
}

// ─── CSV Parser ────────────────────────────────────────────────────────────
std::map<std::string, Address> loadCSV(const std::string& filepath,
                                       const std::map<std::string, std::string>& zipCityMap) {
    std::map<std::string, Address> addressMap;
    std::ifstream file(filepath);
    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Split CSV columns: lat, lon, addressNumber, addressName, unit, zip
        std::stringstream ss(line);
        std::string latStr, lonStr, addressNumber, addressName, unit, zip;

        std::getline(ss, latStr, ',');
        std::getline(ss, lonStr, ',');
        std::getline(ss, addressNumber, ',');
        std::getline(ss, addressName, ',');
        std::getline(ss, unit, ',');
        std::getline(ss, zip, ',');

        // Store CSV row values in an Address object
        Address a;
        a.lat = std::stod(latStr);
        a.lon = std::stod(lonStr);
        a.addressNumber = addressNumber;
        a.addressName = addressName;
        a.zip = zip;

        if (zipCityMap.count(zip)) {
            a.city = zipCityMap.at(zip);
        }

        if (!unit.empty()) {

            a.fullAddress = addressNumber + " " + addressName + " " + unit;

        } else {

            a.fullAddress = addressNumber + " " + addressName;

        }

        // Store address using the display address as the lookup key
        std::string key = a.fullAddress;
        addressMap[key] = a;
    }

    return addressMap;
}

// ─── JSON helpers ──────────────────────────────────────────────────────────
std::string getStringValue(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    while (pos < json.size() && json[pos] == ' ') pos++;
    if (json[pos] == '"') {
        pos++;
        size_t end = json.find('"', pos);
        return json.substr(pos, end - pos);
    }
    size_t end = json.find_first_of(",}", pos);
    return json.substr(pos, end - pos);
}

std::vector<std::string> getStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> results;
    std::string search = "\"" + key + "\":[";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return results;
    pos += search.length();
    size_t end = json.find(']', pos);
    std::string arrayContent = json.substr(pos, end - pos);
    size_t i = 0;
    while (i < arrayContent.size()) {
        if (arrayContent[i] == '"') {
            i++;
            size_t close = arrayContent.find('"', i);
            results.push_back(arrayContent.substr(i, close - i));
            i = close + 1;
        } else {
            i++;
        }
    }
    return results;
}

// ─── Distance (Haversine) ─────────────────────────────────────────────────
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 3958.8; // Earth radius in miles
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon/2) * sin(dLon/2);
    return R * 2 * atan2(sqrt(a), sqrt(1-a));
}

// ─── Main ─────────────────────────────────────────────────────────────────
int main() {
    // Load CSV — path is relative to where app.js runs (Test/)
    std::map<std::string, std::string> zipCityMap = loadZipCity("Backend/data/zip_city.csv");
    std::map<std::string, Address> addressMap = loadCSV("Backend/data/addresses.csv", zipCityMap);
    std::cerr << "Loaded " << addressMap.size() << " addresses from CSV" << std::endl;

    std::string input;
    // Read incoming JSON from Node
    while(std::getline(std::cin, input))
    {    
        if (input.empty()) continue;

        std::vector<std::string> requestedAddresses = getStringArray(input, "addresses");
        std::string passengers = getStringValue(input, "passengers");

        // Look up each address and build response
        std::string foundJson = "[";
        double totalDistance = 0.0;
        Address prev;
        bool hasPrev = false;

        for (size_t i = 0; i < requestedAddresses.size(); i++) {
            const std::string& req = requestedAddresses[i];

            if (addressMap.count(req)) {
                Address& a = addressMap[req];

                if (hasPrev) {
                    totalDistance += haversine(prev.lat, prev.lon, a.lat, a.lon);
                }
                prev = a;
                hasPrev = true;

                foundJson += "{\"address\":\"" + a.fullAddress + "\","
                        + "\"city\":\"" + a.city + "\","
                        + "\"zip\":\"" + a.zip + "\","
                        + "\"lat\":" + std::to_string(a.lat) + ","
                        + "\"lon\":" + std::to_string(a.lon) + "}";
            } else {
                std::cerr << "NOT FOUND: " << req << std::endl;
                foundJson += "{\"address\":\"" + req + "\",\"lat\":0,\"lon\":0,\"error\":\"not found\"}";
            }

            if (i < requestedAddresses.size() - 1) foundJson += ",";
        }
        foundJson += "]";

        // Build response
        std::string response = "{";
        response += "\"status\":\"ok\",";
        response += "\"passengers\":" + passengers + ",";
        response += "\"total_distance_miles\":" + std::to_string(totalDistance) + ",";
        response += "\"route\":" + foundJson;
        response += "}";

        //Pipe output and clean up buffer for reuse
        std::cout << response << "\n";
        std::cout.flush();
    }
    return 0;
}