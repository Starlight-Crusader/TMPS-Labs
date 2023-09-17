#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "./io/scanner.h"
#include "./io/printer.h"
#include "./model/tasks.h"
#include "./model/taskRepository.h"
#include "./manager/taskManager.h"

#pragma once

class Application {
	public:
        Scanner* scanner;
		Printer* printer;
        TaskRepository* repository;

        void switch_working_repo(TaskRepository* new_repository) { repository = new_repository; }
        void switch_scanner(Scanner* new_scanner) { scanner = new_scanner; }
        void switch_printer(Printer* new_printer) { printer = new_printer; }

		void console_input_test() {
			std::vector<Task*> input = scanner->read_data();
            
            for (Task* input_item : input) { 
                TaskManager::get().create_task(repository, input_item);
            }
		}

        void console_output_test() {
            printer->list_records(repository);
        }

        void remove_record_test() {
            std::cout << "Input id-s of records to remove OR leave the line blank to break:" << '\n';

            while(1) {
                std::string id;
                std::getline(std::cin, id);
				if (id.empty()) { break; }

                TaskManager::get().delete_task(repository, std::stoi(id));
            }
        }

        void maintenance_test(int delay) {
            #ifdef _WIN32
            Sleep(delay * 1000);
            #else
            sleep(delay);
            #endif

            TaskManager::get().maintain_tasks(repository);
        }

        void edit_test() {
            std::cout << "Input the id and a new description for the record to edit - id|desc ..." << '\n';
            std::cout << "OR leave the line blank to break:" << '\n';

            while(1) {
                std::string input_line;
                std::getline(std::cin, input_line);
				if (input_line.empty()) { break; }

                std::vector<std::string> parameters = Utility::tokenize_input(input_line, '|');

                TaskManager::get().edit_task(repository, std::stoi(parameters[0]), parameters[1]);
            }
        }

        void get_one_test() {
            std::cout << "Input the id of the record to print or leave the line blank to cancel: " << '\n';
            std::string input_line;
            std::getline(std::cin, input_line);
			if (input_line.empty()) { return; }

            printer->print_record(repository, std::stoi(input_line));
        }

        void basic_functionality_test() {
            std::cout << "! During this functionality test you will have 60 s before maintenance !" << '\n';

            this->console_input_test();
            this->console_output_test();

            this->edit_test();
            this->get_one_test();
    
            this->remove_record_test();
            this->console_output_test();

            this->maintenance_test(60);
            this->console_output_test();
        }

        void file_printer_test() {
            printer = new FilePrinter("file_printer.out");

            std::cout << "=== Input task data to be inserted to the file ===" << '\n';

            this->console_input_test();
            printer->list_records(repository);

            std::cout << "=== Input task data to be added to the file ===" << '\n';

            this->console_input_test();
            printer = new ConsolePrinter();

            this->console_output_test();
            printer = new FilePrinter("file_printer.out");

            std::cout << "Input the id of the record to print or leave the line blank to cancel: " << '\n';
            std::string input_line;
            std::getline(std::cin, input_line);

            printer->print_record(repository, std::stoi(input_line));
        }
};