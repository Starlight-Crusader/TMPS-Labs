#include <iostream>
#include <string>
#include <vector>

#include "../model/tasks.h"
#include "../misc/utility.h"
#include "../creationals/factory.h"

#pragma once

class Scanner {
	public:
		virtual std::vector<Task*> readData() = 0;
};

class ConsoleScanner : public Scanner {
	public:
		std::vector<Task*> readData() override {
			std::vector<Task*> new_tasks;

            std::cout << "Insert tasks one by one is such a form - reg/rec|interval|description ..." << '\n';
            std::cout << "OR leave the line blank to break:" << '\n';

			while (1) {
                std::string new_entry;
				std::getline(std::cin, new_entry);
				if (new_entry.empty()) { break; }

				std::vector<std::string> parameters = Utility::tokenizeInput(new_entry, '|');

				int tdelta = Utility::convertTimeDelta(parameters[1]);

				new_tasks.push_back(TaskFactory::create_task(parameters[0], parameters[2], tdelta));
			}

			return new_tasks;
		}
};

class FileScanner : public Scanner {
	public:
        std::vector<Task*> readData();
};
