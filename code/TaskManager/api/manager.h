#include <string>
#include <iostream>

#include "../model/taskRepository.h"
#include "../model/taskRepository.h"

#pragma once

class Manager {
    private:
        Manager() {}

        static Manager m_instance;

	public:
        static Manager& get() {
            return m_instance;
        }

		void createTask(TaskRepository* repository, Task* new_task) {
            repository->saveRecord(new_task);
        }

		void deleteTask(TaskRepository* repository, const int& id) {
            repository->removeRecord(id);
        }

        void editTaskDesc(TaskRepository* repository, const int& id, const std::string& new_desc) {
            repository->editDesc(id, new_desc);
        }

        void editTaskExDt(TaskRepository* repository, const int&id, const std::time_t& new_dt) {
            repository->editExDt(id, new_dt);
        }

        void editTaskInt(TaskRepository* repository, const int&id, const int& new_int) {
            repository->editInt(id, new_int);
        }

		void maintainTasks(TaskRepository*);
};

Manager Manager::m_instance;

void Manager::maintainTasks(TaskRepository* repository) {
    std::cout << "MAINTENANCE = " << Utility::getCurrentTimeStr() << ':' << '\n';

	for (Task* task : repository->getAll()) {
        char flag = task->maintenanceCheck();

        std::cout << flag << ' ' << task->getId() << '|' << task->getDesc() << '\n';

        switch (flag) {
            case 's':
                break;
            case 'd':
                this->deleteTask(repository, task->getId()); 
                break;
            case 'e':
                this->editTaskExDt(repository, task->getId(), time(0)+task->getInt());
                break;
        }
	}
};