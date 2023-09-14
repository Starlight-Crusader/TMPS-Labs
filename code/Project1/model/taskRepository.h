#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "tasks.h"
#include "../misc/utility.h"

#pragma once

class TaskRepository {
	public:
        int id;

		TaskRepository() : id(Utility::generateRandomId()) {}

		virtual void save_record(Task*) = 0;
		virtual void remove_record(int) = 0;

		virtual Task* get_one(int) = 0;
		virtual std::vector<Task*> get_all() = 0;

		virtual void edit_one(int, std::string) = 0;
};

class MemoryRepository : public TaskRepository {
	public:
        std::vector<Task*> records;

		MemoryRepository() : TaskRepository() {}

		void save_record(Task* new_record) override {
			records.push_back(new_record);
		}

		void remove_record(int id) override {
			for (Task* record : records) {
				if (record->get_data(&Task::id) == id) {
                    auto it = std::remove(records.begin(), records.end(), record);
                    records.erase(it, records.end());
				}
			}
		}

		Task* get_one(int id) override {
			for (Task* record : records) {
				if (record->get_data(&Task::id) == id) {
					return record;
				}
			}
		}

		std::vector<Task*> get_all() override {
			return records;
		}

		void edit_one(int id, std::string new_description) override {
			for (Task* record : records) {
				if (record->get_data(&Task::id) == id) {
					record->set_data(&Task::task_description, new_description);
					break;
				}
			}
		}
};

class FileRepository : public TaskRepository {
	// File repository functionality implemented if necessary ...
};
