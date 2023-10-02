#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../io/scanner.h"
#include "../io/printer.h"
#include "../model/tasks.h"
#include "../model/taskRepository.h"
#include "../api/manager.h"

#pragma once

class Application {
	public:
        Scanner* scanner;
		Printer* printer;
        TaskRepository* repository;

        void switchWorkingRepo(TaskRepository* new_repository) { repository = new_repository; }
        void switchScanner(Scanner* new_scanner) { scanner = new_scanner; }
        void switchPrinter(Printer* new_printer) { printer = new_printer; }

		void consoleInputTest() {
			std::vector<Task*> input = scanner->readData();
            
            for (Task* input_item : input) { 
                Manager::get().createTask(repository, input_item);
            }
		}

        void consoleOutputTest() {
            printer->listRecords(repository);
        }

        void removeRecordsTest() {
            std::cout << "Input id-s of records to remove OR leave the line blank to break:" << '\n';

            while(1) {
                std::string id;
                std::getline(std::cin, id);
				if (id.empty()) { break; }

                Manager::get().deleteTask(repository, std::stoi(id));
            }
        }

        void maintenanceTest(int delay) {
            #ifdef _WIN32
            Sleep(delay * 1000);
            #else
            sleep(delay);
            #endif

            Manager::get().maintainTasks(repository);
        }

        void editTest() {
            std::cout << "Input the id and a new description for the record to edit - id|field_name|new_value ..." << '\n';
            std::cout << "OR leave the line blank to break:" << '\n';

            while(1) {
                std::string input_line;
                std::getline(std::cin, input_line);
				if (input_line.empty()) { break; }

                std::vector<std::string> parameters = Utility::tokenizeInput(input_line, '|');

                if (parameters[1] == "desc") {
                    Manager::get().editTaskDesc(repository, stoi(parameters[0]), parameters[2]);
                } else if (parameters[1] == "int") {
                    Manager::get().editTaskInt(repository, stoi(parameters[0]), Utility::convertTimeDelta(parameters[2]));
                } else {
                    std::cout << "ERROR: Task::" << parameters[1] << " - not found!" << '\n';
                }
            }
        }

        void getOneTest() {
            std::cout << "Input the id of the record to print or leave the line blank to cancel: " << '\n';
            std::string input_line;
            std::getline(std::cin, input_line);
			if (input_line.empty()) { return; }

            printer->printRecord(repository, std::stoi(input_line));
        }

        void basicFunctionalityTest() {
            std::cout << "! During this functionality test you will have 60 s before maintenance !" << '\n';

            this->consoleInputTest();
            this->consoleOutputTest();

            this->editTest();
            this->getOneTest();
    
            this->removeRecordsTest();
            this->consoleOutputTest();

            this->maintenanceTest(10);
            this->consoleOutputTest();
        }

        void filePrinterTest() {
            printer = new FilePrinter();
            printer->setPath("file_printer.out");

            std::cout << "=== Input task data to be inserted to the file ===" << '\n';

            this->consoleInputTest();
            printer->listRecords(repository);
        }
};