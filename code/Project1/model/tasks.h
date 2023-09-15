#include <string>
#include <ctime>

#include "../misc/utility.h"

#pragma once

class Task {
	public:
        int id;
		std::time_t creation_dt;
		std::string task_description;

		Task(std::string task_description) : id(Utility::generateRandomId()), creation_dt(time(0)), task_description(task_description) {}

		template <typename T>
		T get_data(T Task::*member) { return this->*member; }

		template <typename T>
		void set_data(T Task::*member, T value) { this->*member = value; }

		virtual std::string get_data_string() = 0;
		virtual int maintenance() = 0;
};

class RegularTask : public Task {
	public:
        std::time_t expiration_dt;

		RegularTask(std::string task_description, std::time_t interval) : Task(task_description), expiration_dt(time(0) + interval) {}

		std::string get_data_string() override {
            const char* format = "%Y-%m-%d %H:%M:%S";
            char buffer[80];

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&creation_dt));
            std::string creation_dt_str(buffer);

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&expiration_dt));
            std::string expiration_dt_str(buffer);

			std::string data_string = std::to_string(id) + "|" + task_description + "|" + creation_dt_str + "|" + expiration_dt_str;
			return data_string;
		}

		int maintenance() override {
			if (time(0) > expiration_dt) {
				return -1;
			} else {
                return 0;
            }
		}
};

class RecurringTask : public Task {
	public:
        std::time_t extension_dt;
		int interval;

		RecurringTask(std::string task_description, int interval) : Task(task_description), extension_dt(time(0) + interval), interval(interval) {}

		std::string get_data_string() override {
            const char* format = "%Y-%m-%d %H:%M:%S";
            char buffer[80];

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&creation_dt));
            std::string creation_dt_str(buffer);

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&extension_dt));
            std::string extension_dt_str(buffer);

			std::string data_string = std::to_string(id) + "|" + task_description + "|" + creation_dt_str + "|" + extension_dt_str + "|" + std::to_string(interval) + 's';
			return data_string;
		}

		int maintenance() override {
			if (time(0) - extension_dt > interval) {
				extension_dt = time(0) + interval;
				return 1;
			} else {
                return 0;
            }
		}
};
