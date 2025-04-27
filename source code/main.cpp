#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "Market.h"

int main(int argc, char* argv[]) {
    // Default file paths
    std::string input_file_path = "/Users/yik/Library/CloudStorage/OneDrive-TheChineseUniversityofHongKong/University/Year 2/Sem 2/CSCI2100/CSCI2100 Project Stock Data Tracker/test_operations.txt";
    std::string output_file_path = "output.txt";

    // Check command-line arguments
    if (argc > 1) {
        input_file_path = argv[1];
    }
    if (argc > 2) {
        output_file_path = argv[2];
    }

    // Open the input file
    std::ifstream input_file(input_file_path);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open input file '" << input_file_path << "'\n";
        return 1;
    }

    // Open the output file
    std::ofstream output_file(output_file_path);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open output file '" << output_file_path << "'\n";
        input_file.close();
        return 1;
    }

    // Store original cout and cerr buffers
    std::streambuf* cout_buf = std::cout.rdbuf();
    std::streambuf* cerr_buf = std::cerr.rdbuf();

    // Redirect cout and cerr to the output file
    std::cout.rdbuf(output_file.rdbuf());
    std::cerr.rdbuf(output_file.rdbuf());

    // Process input file
    Market market;
    std::string line;
    while (std::getline(input_file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "insert") {
            int id;
            float price;
            if (iss >> id >> price) {
                market.insertNewStock(id, price);
            } else {
                std::cerr << "Error: Invalid insert command: " << line << "\n";
            }
        } else if (command == "update") {
            int id;
            float price;
            if (iss >> id >> price) {
                market.updatePrice(id, price);
            } else {
                std::cerr << "Error: Invalid update command: " << line << "\n";
            }
        } else if (command == "increase") {
            int id, vol_inc;
            if (iss >> id >> vol_inc) {
                market.increaseVolume(id, vol_inc);
            } else {
                std::cerr << "Error: Invalid increase command: " << line << "\n";
            }
        } else if (command == "range") {
            float lower, upper;
            if (iss >> lower >> upper) {
                market.priceRange(lower, upper);
            } else {
                std::cerr << "Error: Invalid range command: " << line << "\n";
            }
        } else if (command == "maxvol") {
            market.maxVol();
        } else if (command == "lookup") {
            int id;
            if (iss >> id) {
                market.lookUpByID(id);
            } else {
                std::cerr << "Error: Invalid lookup command: " << line << "\n";
            }
        } else {
            std::cerr << "Error: Unknown command: " << command << "\n";
        }
    }

    // Restore original cout and cerr buffers
    std::cout.rdbuf(cout_buf);
    std::cerr.rdbuf(cerr_buf);

    // Close files
    input_file.close();
    output_file.close();

    // Print confirmation to console
    std::cout << "Completed processing '" << input_file_path << "'. Output written to '" << output_file_path << "'\n";

    return 0;
}