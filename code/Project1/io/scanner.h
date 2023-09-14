#include <iostream>
#include <string>
#include <vector>

#include "../model/tasks.h"
#include "../misc/utility.h"

class Scanner {
	public:
		virtual std::vector<Task*> read_data() = 0;
};

class ConsoleScanner : public Scanner {
	public:
		std::vector<Task*> read_data() override {
			std::vector<Task*> new_tasks;

            std::cout << "Insert tasks one by one is such a form - reg/rec|interval|description ..." << '\n';
            std::cout << "OR leave the line blank to break:" << '\n';

			while (1) {
                std::string new_entry;
				std::getline(std::cin, new_entry);
				if (new_entry.empty()) { break; }

				std::vector<std::string> parameters = Utility::tokenize_input(new_entry, '|');

				int tdelta = Utility::convert_time_delta(parameters[1]);

                time_t current_time;
                time(&current_time);

                RegularTask* newTask = new RegularTask(parameters[2], current_time + tdelta);

				if (parameters[0] == "reg") {
					new_tasks.push_back(new RegularTask(parameters[2], current_time + tdelta));
				} else if (parameters[0] == "rec") {
					new_tasks.push_back(new RecurringTask(parameters[2], tdelta));
				}
			}

			return new_tasks;
		}
};

class FileScanner : public Scanner {
	// File input functionality implemented if necessary ...
};
