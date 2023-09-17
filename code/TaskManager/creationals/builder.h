#include "../io/scanner.h"
#include "../io/printer.h"
#include "../model/taskRepository.h"
#include "../manager/taskManager.h"

#pragma once

class Builder {
    public:
        virtual void reset() = 0;
        virtual Application* get_product() = 0;

        virtual void set_scanner() = 0;
        virtual void set_printer() = 0;
        virtual void set_repository() = 0;
        virtual void set_manager() = 0;
};

class ConsoleBuilder : public Builder {
    private:
        Application* product;

    public:
        void reset() { product = new Application(); }
        Application* get_product() { return product; }

        void set_scanner() { product->scanner = new ConsoleScanner(); }
        void set_printer() { product->printer = new ConsolePrinter(); }
        void set_repository() { product->repository = new MemoryRepository(); }
        void set_manager() { product->manager = new TaskManager(); }
};

class FileBuilder : public Builder {
    private:
        Application* product;

    public:
        void reset() { product = new Application(); }
        Application* get_product() { return product; }

        void set_scanner() { product->scanner = new FileScanner(); }
        void set_printer() { product->printer = new FilePrinter(""); }
        void set_repository() { product->repository = new MemoryRepository(); }
        void set_manager() { product->manager = new TaskManager(); }
};

class Director {
    private:
        Builder* builder;

        public:
            void set_builder(Builder* new_builder) { builder = new_builder; }

            Application* build_product() {
                builder->set_scanner();
                builder->set_printer();
                builder->set_repository();
                builder->set_manager();

                return builder->get_product();
            }
};