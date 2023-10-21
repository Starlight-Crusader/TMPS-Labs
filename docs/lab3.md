# (3) Structural Design Patterns
### Course: Techniques and mechanisms of software design
### Author: Kalamaghin Arteom (FAF-211)

## Objectives

    1. Study and understand the Structural Design Patterns.
    2. As a continuation of the previous laboratory work, think about the functionalities that ythis system will need to provide to the user.
    3. Implement some additional functionalities, or create a new project using structural design patterns.

## Theoretical notes

Structural design patterns are a subset of software design patterns that deal with organizing and structuring classes and objects. They primarily address how objects and classes can be composed to create flexible and efficient software architectures. These patterns help in managing relationships between different components of a software system, thereby enhancing code reusability, scalability, and maintainability. By promoting sound architectural practices, structural design patterns contribute to the overall robustness and adaptability of software systems. Some common examples of structural design patterns include the Adapter, Bridge, Composite, and Proxy patterns, each serving specific purposes in structuring software components.

## Implementation

As part of this laboratory work, I decided to develop and implement in C++ the architecture of a simple project of a peg playbox: there are pegs of different shapes, colors and parameters, round holes that pegs are able to go through and boxes that are able to collect pegs and smaller boxes.

### Bridge

Let's start from the simplest. The bridge structural design pattern is a software design pattern that separates an object's abstraction from its implementation, allowing them to vary independently. This pattern is particularly useful when you want to avoid a permanent binding between the abstraction and its implementation, making the system more flexible and adaptable.

```cpp
class Color {
    public:
        virtual std::string getColor() = 0;
};

class RedColor : public Color {
    public:
        std::string getColor() override { return "RED"; }
};

class BlueColor : public Color {
    public:
        std::string getColor() override { return "BLUE"; }
};
```

The Color class is the bridge that makes possible to assign colors to the pegs.

```cpp
class ColoredPeg {
    private:
        Color* color;

    public:
        ColoredPeg() {}
        ColoredPeg(Color* color_to_set) : color(color_to_set) {}

        void setColor(Color* new_color) { color = new_color; }
        std::string getColor() { return color->getColor(); }
};
```

### Decorator

The decorator structural design pattern is a pattern that allows you to add behavior or responsibilities to objects dynamically without altering their core structure. It involves attaching additional classes, or decorators, to an object, which can modify its functionality, making it a flexible way to extend object behavior in a granular and reusable manner. By implementing the ColoredPeg component mentioned previously, I was able to implement a coloring wrapper for every type of peg.

```cpp
class ColoredRoundPeg : public RoundPeg, public ColoredPeg {
    public:
        ColoredRoundPeg(double param_val, Color* color_to_set) : RoundPeg(param_val), ColoredPeg(color_to_set) {}

        std::string buildInfoStr() override {
            return this->getColor() + " round peg of radius = " + std::to_string(this->getParam());
        }
};
```

### Adapter

The wrapper structural design pattern, also known as the adapter pattern, allows objects with incompatible interfaces to work together. It involves creating a wrapper class that acts as an intermediary, translating one interface into another, thus enabling these incompatible components to collaborate seamlessly. In this case, the Hole class is able to accept only RoundPeg objects, but in reality, a square peg of small enough size is capable of going through a big enough round hole. So I implemented a wrapper that makes collaboration between Hole and SquarePeg possible.

```cpp
class SquarePegAdapter : public RoundPeg {
    private:
        Peg* peg;

    public:
        SquarePegAdapter(Peg* peg_to_adapt) : peg(peg_to_adapt) {}

        double getParam() { return peg->getParam() * sqrt(2) / 2; }
};
```

### Composite

The composite structural design pattern is a pattern that lets you compose objects into tree structures to represent part-whole hierarchies. It allows clients to treat individual objects and compositions of objects uniformly, making it useful for creating complex structures from simple components. As was mentioned before, Box objects should be able to contain not only pegs but also boxes of smaller size—explicit tree structure: boxes—nodes, pegs—leaves. To check that everything works properly, we'll push a few boxes with pegs and separate pegs in the big box, and at the end, we will count all the pegs that ended up in the root box.

```cpp
class Component {
    public:
        virtual void add(Component* new_comp) {}
        virtual int count() = 0;
};

class Box : public Component {
    private:
        std::vector<Component*> components;

    public:
        void add(Component* new_comp) { components.push_back(new_comp); }
        int count() {
            int result = 0;

            for (Component* comp : components) {
                result += comp->count();
            }

            return result;
        }
};
```

```cpp
class Peg : public Component {
    public:
        int count() { return 1; }
        ...
};
```

## Conclusion

In conclusion, the implementation and demonstration of the adapter, bridge, decorator, and composite structural design patterns in this laboratory work showcased the versatility and power of these design patterns in solving a variety of software engineering challenges. 

The adapter pattern allowed to seamlessly integrate components with different interfaces, simplifying the interaction between disparate systems. The bridge pattern helped decouple abstractions from implementations, making this code more flexible and extensible. The decorator pattern enabled us to dynamically add new functionalities to objects, enhancing code reusability and maintainability. Finally, the composite pattern allowed to construct complex structures from simple components, promoting a hierarchical organization of this objects.

Overall, the successful application of these structural design patterns demonstrated their effectiveness in improving code modularity, flexibility, and maintainability, which are essential qualities in building robust and scalable software systems. These patterns serve as valuable tools in the software developer's toolbox, offering elegant solutions to common design challenges.