#include <iostream>
#include <fstream>
#include <string>

#include "../model/taskRepository.h"

class Printer {
	public:
		virtual void print_record(TaskRepository*, int) = 0;
		virtual void list_records(TaskRepository*) = 0;
};

class ConsolePrinter : public Printer {
	public:
		void print_record(TaskRepository* repository, int id) override {
			Task* record = repository->get_one(id);
			std::cout << record->get_data_string() << '\n' << '\n';
		}

		void list_records(TaskRepository* repository) {
			for (Task* record : repository->get_all()) {
				std::cout << record->get_data_string() << '\n';
			}

            std::cout << '\n';
		}
};

class FilePrinter : public Printer {
	private:
        std::string path;

    public:
        FilePrinter(std::string path) : path(path) {}

        void set_path(std::string new_path) { path = new_path; }

        void print_record(TaskRepository* repository, int id) override {
            std::ofstream output_file;
            output_file.open(path, std::ios::app);

            if (!output_file.is_open()) {
                output_file.open(path, std::ios::out);
            }

            Task* record_to_out = repository->get_one(id);
            output_file << record_to_out->get_data_string() << '\n';

            output_file.close();
        }

        void list_records(TaskRepository* repository) override {
            std::ofstream output_file;
            output_file.open(path, std::ios::out);

            for (Task* record_to_out : repository->get_all()) {
                output_file << record_to_out->get_data_string() << '\n';
            }

            output_file.close();
        }
};
