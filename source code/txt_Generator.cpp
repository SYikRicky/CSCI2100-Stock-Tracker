#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>

int main() {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Generate 10,000 unique IDs from 1 to 1,000,000
    std::vector<int> ids;
    ids.reserve(1000000);
    for (int i = 1; i <= 1000000; ++i) {
        ids.push_back(i);
    }
    std::shuffle(ids.begin(), ids.end(), gen);
    ids.resize(10000); // Take first 10,000 unique IDs
    
    // Open output file
    std::ofstream file("test_operations.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open test_operations.txt for writing\n";
        return 1;
    }
    
    // Set precision for floating-point prices
    file << std::fixed << std::setprecision(2);
    
    // Distributions for random prices and volumes
    std::uniform_real_distribution<float> price_dist(1.0f, 100.0f);
    std::uniform_int_distribution<int> vol_dist(1, 100);
    
    // 1. Generate 10,000 insert-new-stock operations
    file << "# 10,000 insert-new-stock operations\n";
    for (int id : ids) {
        float price = price_dist(gen);
        file << "insert " << id << " " << price << "\n";
    }
    
    // 2. Generate 10,000 update-price operations with price-range after every 1,000
    file << "\n# 10,000 update-price operations with price-range after every 1,000\n";
    float last_price = 0.0f;
    for (size_t i = 0; i < ids.size(); ++i) {
        int id = ids[i];
        float price = price_dist(gen);
        file << "update " << id << " " << price << "\n";
        last_price = price;
        if ((i + 1) % 1000 == 0) {
            file << "range " << last_price << " " << (last_price + 2.0f) << "\n";
        }
    }
    
    // 3. Generate 10,000 increase-volume operations with max-vol after every 100
    file << "\n# 10,000 increase-volume operations with max-vol after every 100\n";
    for (size_t i = 0; i < ids.size(); ++i) {
        int id = ids[i];
        int vol_inc = vol_dist(gen);
        file << "increase " << id << " " << vol_inc << "\n";
        if ((i + 1) % 100 == 0) {
            file << "maxvol\n";
        }
    }
    
    // 4. Generate 10,000 lookup-by-id operations
    file << "\n# 10,000 lookup-by-id operations\n";
    for (int id : ids) {
        file << "lookup " << id << "\n";
    }
    
    // Close file
    file.close();
    
    std::cout << "Generated test_operations.txt with 40,110 operations\n";
    return 0;
}
