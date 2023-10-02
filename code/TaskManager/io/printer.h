#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "../model/taskRepository.h"

#pragma once

class Printer {
	public:
		virtual void printRecord(TaskRepository*, const int&) = 0;
		virtual void listRecords(TaskRepository*) = 0;
        virtual void setPath(std::string) = 0;
        virtual std::string getPath() = 0;
};

class ConsolePrinter : public Printer {
	public:
		void printRecord(TaskRepository* repository, const int& id) override {
			Task* record = repository->getOne(id);
			std::cout << record->getDataString() << '\n' << '\n';
		}

		void listRecords(TaskRepository* repository) {
			for (Task* record : repository->getAll()) {
				std::cout << record->getDataString() << '\n';
			}

            std::cout << '\n';
		}

        void setPath(std::string new_path) override {}
        std::string getPath() override {return nullptr; }
};

class FilePrinter : public Printer {
	private:
        std::string path;

    public:
        void setPath(std::string new_path) override { path = new_path; }
        std::string getPath() override { return path; }

        void printRecord(TaskRepository* repository, const int& id) override {
            std::ofstream output_file;
            output_file.open(path, std::ios::app);

            if (!output_file.is_open()) {
                output_file.open(path, std::ios::out);
            }

            Task* record_to_out = repository->getOne(id);
            output_file << record_to_out->getDataString() << '\n';

            output_file.close();
        }

        void listRecords(TaskRepository* repository) override {
            std::ofstream output_file;
            output_file.open(path, std::ios::out);

            if (output_file.is_open()) {
                for (Task* record_to_out : repository->getAll()) {
                    output_file << record_to_out->getDataString() << '\n';
                }

                output_file.close();
                std::cout << "LOG: Records written to: " << path << '\n';
            } else {
                std::cout << "ERROR: Unable to open: " << path << '\n';
            }
        }
};
