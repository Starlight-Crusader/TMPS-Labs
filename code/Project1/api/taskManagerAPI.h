#include <string>
#include <iostream>

#include "../model/taskRepository.h"
#include "../model/taskRepository.h"

class TaskManagerAPI {
	public:
		void create_task(TaskRepository*, Task*);
		void delete_task(TaskRepository*, int);

		void maintain_tasks(TaskRepository*);

		void edit_task(TaskRepository*, int, std::string);
};

void TaskManagerAPI::create_task(TaskRepository* repository, Task* new_task) {
	repository->save_record(new_task);
};

void TaskManagerAPI::delete_task(TaskRepository* repository, int id) {
	repository->remove_record(id);
};

void TaskManagerAPI::maintain_tasks(TaskRepository* repository) {
    std::cout << "MAINTENANCE = " << Utility::get_current_time_str() << ':' << '\n';

	for (Task* task : repository->get_all()) {
        int code = task->maintenance();

        if (code == -1) {
            std::cout << "-" << task->get_data(&Task::id) << "|" << task->get_data(&Task::id) << " - removed " << '\n';
            this->delete_task(repository, task->get_data(&Task::id));
        } else if (code == 1) {
            std::cout << "+" << task->get_data(&Task::id) << "|" << task->get_data(&Task::id) << " - extended " << '\n';
        } else {
            std::cout << "~" << task->get_data(&Task::id) << "|" << task->get_data(&Task::id) << " - checked " << '\n';
        }
	}
};

void TaskManagerAPI::edit_task(TaskRepository* repository, int id, std::string new_description) {
	repository->edit_one(id, new_description);
};
