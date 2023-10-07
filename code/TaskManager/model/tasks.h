#include <string>
#include <ctime>

#include "../misc/utility.h"

#pragma once

class Task {
	public:
        int id;
	    std::time_t creation_dt;
	    std::string task_description;

		Task(const std::string& task_description) : id(Utility::generateRandomId()), creation_dt(time(0)), task_description(task_description) {}

        int getId() { return id; }
        std::time_t getCrDt() { return creation_dt; }
        std::string getDesc() { return task_description; }

        virtual void setDesc(std::string new_desc) { task_description = new_desc; }

        virtual std::time_t getExDt() = 0;
        virtual void setExDt(std::time_t) = 0;
        virtual int getInt() = 0;
        virtual void setInt(int) = 0;
 
        virtual std::string getDataString() = 0;
		virtual char maintenanceCheck() = 0;
};

class RegularTask : public Task {
    private:
        std::time_t expiration_dt;

	public:
		RegularTask(const std::string& task_description, const std::time_t& interval) : Task(task_description), expiration_dt(time(0) + interval) {}

        std::time_t getExDt() override { return expiration_dt; }
        void setExDt(std::time_t new_dt) { expiration_dt = new_dt; }

		std::string getDataString() override {
            const char* format = "%Y-%m-%d %H:%M:%S";
            char buffer[80];

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&creation_dt));
            std::string creation_dt_str(buffer);

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&expiration_dt));
            std::string expiration_dt_str(buffer);

			std::string data_string = std::to_string(id) + "|" + task_description + "|" + creation_dt_str + "|" + expiration_dt_str;
			return data_string;
		}

		char maintenanceCheck() override {
			if (time(0) > expiration_dt) {
				return 'd';
			} else {
                return 's';
            }
		}

        int getInt() override { return 0; }
        void setInt(int new_interval) {}
};

class RecurringTask : public Task {
    private:
        std::time_t extension_dt;
		int interval;

	public:
		RecurringTask(const std::string& task_description, const int& interval) : Task(task_description), extension_dt(time(0) + interval), interval(interval) {}

        std::time_t getExDt() override { return extension_dt; }
        void setExDt(std::time_t new_dt) { extension_dt = new_dt; }
        
        int getInt() override { return interval; }
        void setInt(int new_interval) { interval = new_interval; }

		std::string getDataString() override {
            const char* format = "%Y-%m-%d %H:%M:%S";
            char buffer[80];

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&creation_dt));
            std::string creation_dt_str(buffer);

            std::strftime(buffer, sizeof(buffer), format, std::localtime(&extension_dt));
            std::string extension_dt_str(buffer);

			std::string data_string = std::to_string(id) + "|" + task_description + "|" + creation_dt_str + "|" + extension_dt_str + "|" + std::to_string(interval) + 's';
			return data_string;
		}

		char maintenanceCheck() override {
			if (time(0) - extension_dt > interval) {
			    return 'e';
			} else {
                return 's';
            }
		}
};