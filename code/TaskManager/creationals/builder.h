#include "../io/scanner.h"
#include "../io/printer.h"
#include "../model/taskRepository.h"
#include "../manager/taskManager.h"
#include "../application.h"
#include "./abstractFactory.h"

#pragma once

class Builder {
    public:
        virtual void reset() = 0;
        virtual Application* get_product() = 0;

        virtual void set_io() = 0;
        virtual void set_repository() = 0;
};

class ConsoleBuilder : public Builder {
    private:
        Application* product;
        IOFactory* factory;

    public:
        void reset() { product = new Application(); factory = new ConsoleIOFactory(); }
        Application* get_product() { return product; }

        void set_io() { 
            product->scanner = factory->getScanner();
            product->printer = factory->getPrinter();
        }
        
        void set_repository() { product->repository = new MemoryRepository(); }
};

class FileBuilder : public Builder {
    private:
        Application* product;
        IOFactory* factory;

    public:
        void reset() { product = new Application(); factory = new FileIOFactory(); }
        Application* get_product() { return product; }

        void set_io() { 
            product->scanner = factory->getScanner();
            product->printer = factory->getPrinter();
        }

        void set_repository() { product->repository = new MemoryRepository(); }
};

class Director {
    private:
        Builder* builder;

        public:
            void set_builder(Builder* new_builder) { builder = new_builder;}

            Application* build_app() {
                builder->reset();
                builder->set_io();
                builder->set_repository();

                return builder->get_product();
            }
};