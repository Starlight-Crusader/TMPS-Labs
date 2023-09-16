#include <random>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <ctime>

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

		static std::vector<std::string> tokenize_input(std::string str, char separator) {
			std::istringstream iss(str);
			std::vector<std::string> tokens;

			std::string token;
			while (std::getline(iss, token, separator)) {
				tokens.push_back(token);
			}

			return tokens;
		}

		static int convert_time_delta(std::string sdelta) {
			size_t separator_pos = sdelta.find_first_of("-");
			
			int value = std::stoi(sdelta.substr(0, separator_pos));
			std::string unit = sdelta.substr(separator_pos+1);

			int tdelta = 0;
			if (unit == "s") {
				tdelta = value;
			} else if (unit == "m") {
				tdelta = value * 60;
			} else if (unit == "h") {
				tdelta = value * 3600;
			} else if (unit == "d") {
				tdelta = value * 86400;
			} else if (unit == "w") {
				tdelta = value * 7 * 86400;
			}

			return tdelta;
		}

        static std::string get_current_time_str() {
            const char* format = "%Y-%m-%d %H:%M:%S";
            char buffer[80];

            time_t current_time = time(0);
            std::strftime(buffer, sizeof(buffer), format, std::localtime(&current_time));
            std::string current_time_str(buffer);

            return current_time_str;
        }
};
