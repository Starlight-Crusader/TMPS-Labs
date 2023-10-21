# (4) Behavioral Design Patterns
### Course: Techniques and mechanisms of software design
### Author: Kalamaghin Arteom (FAF-211)

## Objectives

    1. Study and understand the Behavioral Design Patterns.
    2. As a continuation of the previous laboratory work, think about what communication between software entities might be involed in your system.
    3. Create a new Project or add some additional functionalities using behavioral design patterns.

## Theoretical notes

Behavioral design patterns in software engineering are a set of solutions that address how objects interact and communicate in a system. They offer strategies for managing the behavior and responsibilities of objects, promoting flexibility and reusability. Common behavioral patterns include the Observer pattern, Command pattern, Strategy pattern, and Chain of Responsibility pattern, among others. These patterns enhance the modularity and maintainability of software systems by fostering loose coupling and adaptable interactions between objects.

## Implementation

As part of this laboratory work, I decided to develop and implement in C++ the architecture of a simple project of graphic software: there are 2D polygons of various sizes and positions; the user can create, delete, and transform polygons (translate, rotate, and scale) by typing commands in a simple shell language that I implemented; the information about the geometry created is shown on the screen; the user can also read shell-history and undo transformations.

## Observer

The Observer pattern establishes a one-to-many relationship between objects, allowing one object (the subject) to notify multiple observers when its state changes. Observers can automatically update in response to these changes, promoting loose coupling and a dynamic, event-driven communication model. This is the simplest implementation, shell-history (Subscriber) observes CLI (Publisher) events and saves each command the user executes.

```cpp
// misc/observer.h

class Subscriber {
    public:
        virtual void update(std::string) = 0;
};

class Publisher {
    protected:
        std::vector<Subscriber*> subscribers;

    public:
        void subscribe(Subscriber* sub_to_add) { subscribers.push_back(sub_to_add); }
        void unsubscribe(Subscriber* sub_to_remove) {
            for (int i = 0; i < subscribers.size(); i++) {
                if (subscribers[i] == sub_to_remove) {
                    subscribers.erase(subscribers.begin() + i);
                    break;
                }
            }
        }

        void notify(std::string context) {
            for (Subscriber* sub : subscribers) {
                sub->update(context);
            }
        }
};

// controller/application.h

Application::Application() {
    history = new CLIHistory();
    data = new AppData(history);
    cli = new CLI(data);
    cli->subscribe(history);
}

// view/cli.h

class CLI : public Publisher {
    private:
        AppData* ad_ref;

    public:
        Command* scanCommand() {
            std::string line; std::getline(std::cin, line);

            if(line.length() == 0) {
                return nullptr;
            }

            this->notify(line);
            ...
        }
        ...
};

// model/history.h

class CLIHistory : public Subscriber {
    private:
        std::vector<std::string> history;

    public:
        void update(std::string last) {
            history.push_back(last);
        }
        ...
};
```

## Memento

The Memento pattern captures the internal state of an object without violating encapsulation, creating a "snapshot" that can be restored later. It is useful for implementing features like undo functionality, and it ensures that an object can revert to a previous state without exposing its internal details. Through the use of this technology, I was able to incorporate undo capabilities into the application, allowing the user to reverse the most recent transformation they made to a polygon.

```cpp
// model/geometry.h

class PolygonMemento {
    private:
        std::vector<Vector2D> vertices_state;

    public:
        PolygonMemento(std::vector<Vector2D> state) : vertices_state(state) {}

        std::vector<Vector2D> getState() {
            return vertices_state;
        }
};

class Polygon {
    public:
        PolygonMemento save() { 
            std::vector<Vector2D> state;

            for (Vector2D* vertex : vertices) {
                state.push_back(vertex->get());
            }
            
            return PolygonMemento(state); 
        };

        void restore(PolygonMemento m) {
            std::vector<Vector2D> state = m.getState();

            for (int i = 0; i < 3; i++) {
                vertices[i]->set(state[i]);
            }
        }
        ...
};

// model/history.h

class Caretaker {
    private:
        Polygon* originator;
        std::vector<PolygonMemento> history;

    public:
        Caretaker(Polygon* originator_assigned) : originator(originator_assigned) {
            history.clear();
        }

        int getOrigId() { return originator->id; }

        void save() {
            PolygonMemento m = originator->save();
            history.push_back(m);
        }

        void undo() {
            PolygonMemento m = history.back(); history.pop_back();
            originator->restore(m);
        }
};
```

## Strategy

The Strategy pattern defines a family of interchangeable algorithms or behaviors and allows them to be selected at runtime. This flexibility enables a client to choose the appropriate strategy without altering its structure, making it valuable for scenarios where different algorithms or strategies can be applied interchangeably. Making all of the different transformations over polygons into strategies with the same structure, referencing the polygon over which they will be executed and outlining the actions to be taken over polygon vertices, was a fine application.

```cpp
// model/transformations.h

class Transformation {
    protected:
        Polygon* target;

    public:
        Transformation(Polygon* target_ref) : target(target_ref) {}

        void transform() {}
};

class Translation : public Transformation {
    public:
        Translation(Polygon* target_ref) : Transformation(target_ref) {}

        void transform(Vector2D del) {
            for (Vector2D* vertex : target->vertices) {
                vertex->add(del);
            }
        }
};

class Rotation : public Transformation {
    public:
        Rotation(Polygon* target_ref) : Transformation(target_ref) {}

        void transform(double theta, Vector2D origin) {
            for (Vector2D* vertex : target->vertices) {
                double x_new = cos(theta) * (vertex->x - origin.x) - sin(theta) * (vertex->y - origin.y) + origin.x;
                double y_new = sin(theta) * (vertex->x - origin.x) + cos(0) * (vertex->y - origin.y) + origin.y;

                vertex->set(Vector2D(x_new, y_new));
            }
        }
};

class Scale : public Transformation {
    public:
        Scale(Polygon* target_ref) : Transformation(target_ref) {}

    void transform(double factor) {
        for (Vector2D* vertex : target->vertices) {
            vertex->mul(factor);
        }
    }
};
```

## Command

The Command pattern encapsulates requests as objects, allowing for parameterization and queuing of these requests. It separates the sender of a request from the object that performs the action, providing a flexible and extensible way to handle various operations and transactions in a system. We actually have user-inserted commands in the application, so I've decided to introduce the eponymous pattern to loosen the coupling between the CLI and the model: After reading user input, the CLI creates or instantiates a command, which the controller subsequently runs.

```cpp
// controller/command.h

class Command {
    protected:
        AppData* app_data_ref;
        std::vector<std::string> params;

    public:
        Command() {}
        Command(AppData* ad_ref) : app_data_ref(ad_ref) {}
        Command(AppData* ad_ref, std::vector<std::string> param_set) : app_data_ref(ad_ref), params(param_set) {}

        virtual void execute() = 0;
};

class CreateCommand : public Command {
    public:
        CreateCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            std::vector<Vector2D> new_vert;

            new_vert.push_back(Vector2D(stod(params[0]), stod(params[1])));
            new_vert.push_back(Vector2D(stod(params[2]), stod(params[3])));
            new_vert.push_back(Vector2D(stod(params[4]), stod(params[5])));

            Polygon* newPol = new Polygon(); newPol->set(new_vert);
            app_data_ref->add(newPol);
        }
};

class DeleteCommand : public Command {
    public:
        DeleteCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->remove(stoi(params[0]));
        }
};

class TransformationCommand : public Command {
    public:
        TransformationCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->getCaretaker(stoi(params[0]))->save();

            if (params[1] == "-t") {
                Translation transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(Vector2D(stod(params[2]), stod(params[3])));
            } else if (params[1] == "-r") {
                Rotation transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(stod(params[2]), Vector2D(stod(params[3]), stod(params[4])));
            } else if (params[1] == "-s") {
                Scale transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(stod(params[2]));
            } else {
                // Return/Display an error ...
            }
        }  
};

class UndoCommand : public Command {
    public:
        UndoCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->getCaretaker(stoi(params[0]))->undo();
        }
};

class BreakCommand : public Command {
    public:
        BreakCommand(AppData* ad_ref) : Command(ad_ref) {}

        void execute() override { 
            app_data_ref->eraseData();
            delete app_data_ref->getHistRef(); 
            exit(0); 
        }
};

class HistoryCommand : public Command {
    public:
        HistoryCommand(AppData* ad_ref) : Command(ad_ref) {}

        void execute() override {
            app_data_ref->getHistRef()->print();
        }
};

class ClearCommand : public Command {
    public:
        ClearCommand() {}

        void execute() override { system("clear"); }
};

// controller/application.h

void Application::main() {
    while(1) {
        cli->mainPrint();
        Command* command = cli->scanCommand();
        
        if (command != nullptr) {
            command->execute();
        } else {
            cli->displayError("Unknown command!");
        }
    }
}
```

## Conclusion

In summary, this lab has allowed me to explore and apply essential behavioral design patterns, including Observer, Memento, Command, and Strategy. These patterns have proven to be effective tools for enhancing the flexibility and maintainability of software systems. My experience in implementing these patterns equips me with valuable insights into designing more adaptable and efficient software, which will undoubtedly benefit my future software development projects.