
#include <string>
#include <vector>
#include <map>


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





// ─── Distance (Haversine) ─────────────────────────────────────────────────


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