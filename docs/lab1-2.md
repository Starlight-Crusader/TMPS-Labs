# (1) _SOLID_ Principles and (2) Design Patterns
### Course: Techniques and mechanisms of software design
### Author: Kalamaghin Arteom (FAF-211)

## Objectives

    1. Study and understand the SOLID Principles.
    2. Choose a domain, define its main classes/models/entities and choose the appropriate instantiation mechanisms.
    3. Create a sample project that respects SOLID Principles and demonstrates application of common design patterns.

## Theoretical notes

SOLID is a set of five object-oriented design principles intended to make software designs more maintainable, flexible, and easy to understand. The acronym stands for Single Responsibility Principle, Open-Closed Principle, Liskov Substitution Principle, Interface Segregation Principle, and Dependency Inversion Principle. Each principle addresses a specific aspect of software design, such as the organization of responsibilities, the handling of dependencies, and the design of interfaces. By following these principles, software developers can create more modular, testable, and reusable code that is easier to modify and extend over time. These principles are widely accepted and adopted in the software development community and can be applied to any object-oriented programming language.

Design patterns in software design are reusable solutions to common problems that developers encounter during the software development process. These patterns provide proven templates for solving specific design and architectural challenges, promoting code reusability, maintainability, and scalability. Design patterns serve as a shared vocabulary for developers, making it easier to communicate and collaborate on software projects.

There are various categories of design patterns, including creational patterns (e.g., Singleton, Factory Method, Abstract Factory), structural patterns (e.g., Adapter, Bridge, Decorator), and behavioral patterns (e.g., Observer, Strategy, Command). Each pattern addresses a distinct set of concerns and can be applied in different contexts based on the specific requirements of a project.

By employing design patterns, developers can leverage established best practices and design principles, reducing the likelihood of introducing errors or inefficiencies into their code. These patterns also contribute to code that is more modular, flexible, and easier to maintain, ultimately leading to the creation of robust and efficient software systems.

## Implementation

As part of this laboratory work, I decided to develop and implement in C++ the architecture of a simple console application for managing user task notes. Briefly about the functionality: the ability to add and delete tasks, save and read them from text files, automatic deletion and extension of tasks, as well as notifications sending.

### S
The Single Responsibility Principle (SRP) is a fundamental concept in object-oriented programming that states that a class should have only one reason to change, meaning it should have only one responsibility or job. To enforce SRP, developers should design classes with a clear and focused purpose, ensuring that each class has a single responsibility and does not become overly complex or intertwined with unrelated functionality.

While designing the architecture I tried to divide the functionality into components as clearly as possible and the result was the following:

    Application - the main abstraction that aggreagates all the components, within which they interact with each other and the user;
    [view] Scanner/Printer - handling i/o operations as well as input/output data formatting;
    [controller] Task manager - describes and calls task handling procedures;
    [model] Task repository - temporary and permanent data storage;
    [model] Task - the main entity int the application's data model;
    Utility - a special package containing some functions of mixed use.

* The above is only an attempt to categorize all the components in accordance with the MVC arhitecture; there may be adjustments in the future. At this stage, I didn't try to adhere to any architectural design patterns that much.

### O
The Open-Closed Principle (OCP) is a principle in object-oriented programming that states that software entities (classes, modules, functions, etc.) should be open for extension but closed for modification. To enforce the Open-Closed Principle, developers should design their code in a way that allows for the addition of new features or behaviors through inheritance or interface implementations without altering existing code, reducing the risk of introducing bugs in the existing system.

The main way it is achieved in my application is polymarphism. Let's take a look at the Task implementation ...

```cpp
class Task {
	public:
        ...

		virtual int maintenance() = 0;
};

class RegularTask : public Task {
	public:
        ...

		int maintenance() override {
			if (time(0) > expiration_dt) {
				return -1;
			} else {
                return 0;
            }
		}
};

class RecurringTask : public Task {
	public:
        ...

		int maintenance() override {
			if (time(0) - extension_dt > interval) {
				extension_dt = time(0) + interval;
				return 1;
			} else {
                return 0;
            }
		}
};
```

Regular and recurring tasks are two separate types of tasks that the application can manage. These two manifest two different behaviors: one requests deletion from the management after the deadline has passed, while the other extends the deadline and notifies the manager of it. By deciding on them to be particular implementations of some abstraction, I've created a solid base for extending the application's functionality by adding other implementations of the base abstraction corresponding to their own purposes. The same can be said about the implementation of the task repository; at the moment, only one implementation is available - temporary: in the computer memory and available only while the application is running; but with the help of polymorphism, it is possible to add other types of storage (local files, cloud), which will have similar interfaces, but provide permanent storage options.

### L

The Liskov Substitution Principle (LSP) is one of the five SOLID principles of object-oriented programming and design. It states that objects of a derived class should be able to replace objects of the base class without affecting the correctness of the program. In other words, if you have a class hierarchy with inheritance, you should be able to use any subclass wherever you use its superclass.

It can be seen all over the source code how this principle was respected. For example ...

```cpp
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
```

Only RegularTask and RecurringTask can be instantiated, but all the methods depend on the general Task abstraction to enforce the Dependency Inversion Principle (more on this later) and spare me some nerves by freeing me from defining separate interfaces for all the possible implementations of the same abstraction. By defining the parameters as pointers, I've been able to make the C++ functions to manifest desired behavour.

### I

The Interface Segregation Principle (ISP) is one of the SOLID principles of object-oriented design. It states that clients should not be forced to depend on interfaces they don't use. In practice, this means that you should design small, specific interfaces tailored to the needs of the clients that will implement them, rather than creating large, monolithic interfaces that force clients to implement methods they don't need. This helps in keeping code clean, promoting cohesion, and reducing unnecessary dependencies.

Let's look at the Application class ...

```cpp
class Application {
	private:
		Scanner* scanner;
		Printer* printer;
        TaskRepository* repository;
        TaskManager* manager;
    ...
};
```

As you can see ther is a lot of composition in its structure and it requires a few dependency injections to work properly. So ...

```cpp
class Application {
    ...
	public:
		void setup_console_application() {
            scanner = new ConsoleScanner();
            printer = new ConsolePrinter();
            repository = new MemoryRepository();
            manager = new TaskManager();
        }
    ...
};
```

By injecting just some implementations of described above we are able to get a particular configuration of the application that possesses only the needed functionality and do not implement unnecessary interfaces.

### D

The Dependency Inversion Principle (DIP) is a fundamental concept in software design that advocates high-level modules should not depend on low-level modules, but both should depend on abstractions. To enforce DIP, start by defining clear interfaces or abstract classes that represent dependencies and have high-level modules depend on these abstractions rather than concrete implementations. Use techniques like Dependency Injection (DI) or Inversion of Control (IoC) containers to inject dependencies into classes, ensuring that the flow of control is inverted, and the system remains flexible and maintainable.

I mean a lot of DIP was already demonstrated above but let's look at something new, for example memory task repository ...

```cpp
class MemoryRepository : public TaskRepository {
	public:
        std::vector<Task*> records;
        ...
};
```

I've required some data structure to store all the objects from the task group, and you may remember that there are different types of them, so by defining these storage using general abstraction, I've made the record vector capable of handling both types of tasks. The same can be said about the application (which is definitely a high-level model depending on a bucnh of low-level ones) described above, since it all depends on abstraction - I've made a robot that doesn't need different interfaces on its manipulator arm to handle both a hammer and a screwdriver but has a general interface to switch between the tools.

### Factory DP

During the second laboratory, we were puzzled by adding any design patterns and I was able to quickly determine the functionality and apply the Factory design pattern for its intended purpose.

The Factory Design Pattern is a creational design pattern used in software development. It provides an interface for creating objects in a super class but allows subclasses to alter the type of objects that will be created. This pattern promotes encapsulation and flexibility, making it easier to change the types of objects created without affecting the code that uses them, and it's commonly used in scenarios where object creation is complex or involves multiple steps.

As you may have noticed, either in the process of manual testing of functionality or studying the code during the data input application, it expects us to specify which type of task to initialize by inserting _reg_ or _rec_ as the first parameter. Then it is passed along with others to the factory, where it produces the needed object ...

```cpp
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
```

### Abstract Factory DP

The Abstract Factory design pattern is a creational design pattern that provides an interface for creating families of related or dependent objects without specifying their concrete classes. It allows you to abstract the creation of objects and ensures that the created objects are compatible and designed to work together. The pattern consists of an abstract factory interface with multiple concrete factory implementations, each responsible for creating a family of related objects. Clients interact with the abstract factory to obtain objects, and they remain unaware of the specific classes being used.

As part of this project, I decided to use this pattern to create different I/O configurations, which will later be used by the builder to instantiate different versions of the application.

The family of related products in our case is a scanner and printer. There are two variants of this family: for console i/o handling and for file i/o handling. Therefore, the implementation of this factory looks something like this:

```cpp
class IOFactory {
    public:
        virtual Printer* getPrinter() = 0;
        virtual Scanner* getScanner() = 0;
};

class ConsoleIOFactory : public IOFactory {
    public:
        Printer* getPrinter() { return new ConsolePrinter(); }
        Scanner* getScanner() { return new ConsoleScanner(); }
};

class FileIOFactory : public IOFactory {
    public:
        Printer* getPrinter() { return new FilePrinter(""); }
        Scanner* getScanner() { return new FileScanner(); }
};
```

Next, the builder, which we will talk about later, will instantiate a certain type of factory and produce the necessary i/o component for the application it is working on ...

```cpp
class Builder{
    private:
        Application* product;
        IOFactory* factory;

    public:
        ...
        void set_io() { 
            product->scanner = factory->getScanner();
            product->printer = factory->getPrinter();
        }
        ...
};
```

### Builder DP

The Builder design pattern is a creational design pattern that is used to construct complex objects step by step. It separates the construction of an object from its representation, allowing for the same construction process to create different representations. This pattern typically involves a Director class that orchestrates the construction process and a Builder interface with concrete implementations for constructing various parts of the object. It is particularly useful when an object has a large number of optional components or configurations, as it provides a more organized and flexible way to create such objects without the need for a multitude of constructor parameters. The Builder pattern promotes code readability and maintainability by making object construction more explicit and easier to understand.

As previously stated, based on the i/o and storing component configuration, it is possible to create different versions of the application. In our case, there will be two builders: one building a console-based application and the other building a file-based application.

```cpp
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
    ...
};
```

Then I've added the Director that tasks the Builder to build a version of product by implementing some of the production steps ...

```cpp
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
```

### Singleton DP

The Singleton design pattern is a creational design pattern that ensures a class has only one instance and provides a global point of access to that instance. It is useful when you want to restrict the instantiation of a class to a single instance, typically for managing shared resources, configurations, or maintaining a single point of control within an application.

If you look at the manager implementation, you will see that it is more like an interface describing interactions between various application components and doesn't contain any fields, so essentialy, we need just a single instance of this component with global access to it—the perfect candidate to demonstrate the pattern implementation...

```cpp
class TaskManager {
    private:
        TaskManager() {}

        static TaskManager m_instance;

	public:
        static TaskManager& get() {
            return m_instance;
        }

		void create_task(TaskRepository*, Task*);
		void delete_task(TaskRepository*, int);

		void maintain_tasks(TaskRepository*);

		void edit_task(TaskRepository*, int, std::string);
};

TaskManager TaskManager::m_instance;
```

Essentially, we hide the constructor by making it private to prevent direct instantiation of the class and provide a static method to access the unique instance. And in this way, we are able to access this single instance from every place in the program.

```cpp
class Application {
	public:
        ...
		void console_input_test() {
			std::vector<Task*> input = scanner->read_data();
            
            for (Task* input_item : input) { 
                TaskManager::get().create_task(repository, input_item);
            }
		}
        ...
};
```

## Conclusion

In summary, the adherence to SOLID principles and the incorporation of the design patterns in the app's architecture have resulted in a robust and adaptable system. These design choices enhance performance, maintainability, scalability, and code reusability, providing a solid foundation for future development and ensuring the application can evolve effectively to meet changing demands. In my opinion, the deliberate application of SOLID principles and the integration of the factory design pattern have significantly improved the overall quality and longevity of our application's architecture and made my work much more challenging and interesting.

* You may see all the source code developed as part of the implementation of this projec by acessing: _https://github.com/Starlight-Crusader/TMPS-Labs/tree/main_.
