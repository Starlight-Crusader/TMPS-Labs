#include <string>

#include "tasks.h"

class TaskFactory {
	public:
		static Task* create_task(std::string type, std::string desc, std::time_t interval) {
			if (type == "reg") {
				return new RegularTask(desc, interval);
			} else if (type == "rec") {
				return new RecurringTask(desc, interval);
			}
		}
};
