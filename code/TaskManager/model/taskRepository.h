#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "tasks.h"
#include "../misc/utility.h"

#pragma once

class TaskRepository {
    private:
        int id;

	public:
		TaskRepository() : id(Utility::generateRandomId()) {}

		virtual void saveRecord(Task*) = 0;
		virtual void removeRecord(const int&) = 0;

		virtual Task* getOne(const int&) = 0;
		virtual std::vector<Task*> getAll() = 0;

        virtual void editDesc(const int&, const std::string&) = 0;
        virtual void editExDt(const int&, const time_t&) = 0;
        virtual void editInt(const int&, const int&) = 0; 
};

class MemoryRepository : public TaskRepository {
    private:
        std::vector<Task*> records;

	public:
		MemoryRepository() : TaskRepository() {}

		void saveRecord(Task* new_record) override {
			records.push_back(new_record);
		}

		void removeRecord(const int& id) override {
			for (Task* record : records) {
				if (record->getId() == id) {
                    auto it = std::remove(records.begin(), records.end(), record);
                    records.erase(it, records.end());
				}
			}
		}

		Task* getOne(const int& id) override {
			for (Task* record : records) {
				if (record->getId() == id) {
					return record;
				}
			}
		}

		std::vector<Task*> getAll() override {
			return records;
		}

        void editDesc(const int& id, const std::string& new_desc) override {
            for (Task* record : records) {
				if (record->getId() == id) {
					record->setDesc(new_desc);
				}
			}
        }

        void editExDt(const int& id, const time_t& new_dt) override {
            for (Task* record : records) {
				if (record->getId() == id) {
					record->setExDt(new_dt);
				}
			}
        }

        void editInt(const int& id, const int& new_int) override {
            for (Task* record : records) {
				if (record->getId() == id) {
					record->setInt(new_int);
				}
			}
        }
};

class FileRepository : public TaskRepository {
	// File repository functionality implemented if necessary ...
};