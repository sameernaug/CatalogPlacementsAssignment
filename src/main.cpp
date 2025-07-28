#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Simplified approach using long long for most operations
// This should handle the numbers in the test cases
using cpp_int = long long;

cpp_int base_to_decimal(const std::string& value, int base) {
    cpp_int result = 0;
    cpp_int power = 1;
    
    for (int i = static_cast<int>(value.size()) - 1; i >= 0; i--) {
        char c = value.at(i);
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            digit = 10 + (c - 'A');
        } else {
            throw std::runtime_error("Invalid character in number string");
        }
        if (digit >= base) {
            throw std::runtime_error("Digit exceeds base");
        }
        
        result += digit * power;
        power *= base;
    }
    return result;
}

cpp_int compute_secret(const std::vector<std::pair<cpp_int, cpp_int>>& points, int k) {
    cpp_int secret = 0;
    for (int i = 0; i < k; ++i) {
        cpp_int numerator = 1;
        cpp_int denominator = 1;
        
        for (int j = 0; j < k; ++j) {
            if (i == j) continue;
            numerator *= -points[j].first;
            denominator *= (points[i].first - points[j].first);
        }
        
        cpp_int term = points[i].second * numerator;
        term /= denominator;
        secret += term;
    }
    return secret;
}

int main() {
    std::vector<std::string> filenames = {"../testcase1.json", "../testcase2.json"};
    for (const auto& filename : filenames) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening " << filename << std::endl;
            continue;
        }

        json data;
        file >> data;

        int n = data["keys"]["n"];
        int k = data["keys"]["k"];
        std::vector<std::pair<cpp_int, cpp_int>> points;

        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it.key() == "keys") continue;
            cpp_int x = std::stoll(it.key());
            json obj = it.value();
            int base = std::stoi(obj["base"].get<std::string>());
            std::string value_str = obj["value"].get<std::string>();
            cpp_int y = base_to_decimal(value_str, base);
            points.push_back({x, y});
        }

        if (points.size() != static_cast<size_t>(n)) {
            std::cerr << "Mismatch in number of points for " << filename << std::endl;
            continue;
        }

        std::sort(points.begin(), points.end(), 
            [](const auto& a, const auto& b) { 
                return a.first < b.first; 
            });

        cpp_int secret = compute_secret(points, k);
        std::cout << "Secret for " << filename << ": " << secret << std::endl;
    }
    return 0;
}