#include "../io/scanner.h"
#include "../io/printer.h"
#include "../model/taskRepository.h"
#include "../api/manager.h"
#include "../controller/application.h"
#include "./abstractFactory.h"

#pragma once

class Builder {
    public:
        virtual void reset() = 0;
        virtual Application* getProduct() = 0;

        virtual void setIO() = 0;
        virtual void setRepository() = 0;
};

class ConsoleBuilder : public Builder {
    private:
        Application* product;
        IOFactory* factory;

    public:
        void reset() override { product = new Application(); factory = new ConsoleIOFactory(); }
        Application* getProduct() override { return product; }

        void setIO() override { 
            product->scanner = factory->getScanner();
            product->printer = factory->getPrinter();
        }
        
        void setRepository() override { product->repository = new MemoryRepository(); }
};

class FileBuilder : public Builder {
    private:
        Application* product;
        IOFactory* factory;

    public:
        void reset() override { product = new Application(); factory = new FileIOFactory(); }
        Application* getProduct() override { return product; }

        void setIO() override { 
            product->scanner = factory->getScanner();
            product->printer = factory->getPrinter();
        }

        void setRepository() override { product->repository = new MemoryRepository(); }
};

class Director {
    private:
        Builder* builder;

        public:
            void setBuilder(Builder* new_builder) { builder = new_builder;}

            Application* buildApp() {
                builder->reset();
                builder->setIO();
                builder->setRepository();

                return builder->getProduct();
            }
};