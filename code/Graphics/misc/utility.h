#include <random>
#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#pragma once

class Utility {
	public:
		static int generateRandomId() {
			std::random_device rd;
			std::mt19937 gen(rd());
			const std::string digits = "0123456789";

			std::uniform_int_distribution<int> distribution(0, digits.size() - 1);

			std::string newId;
			for (int i = 0; i < 8; ++i) {
				int randomIndex = distribution(gen);
				newId += digits[randomIndex];
			}

			return std::stoi(newId);
		}

        static std::vector<std::string> tokenizeLine(std::string line) {
            std::vector<std::string> tokens;
            std::istringstream iss(line);
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            return tokens;
        }
};