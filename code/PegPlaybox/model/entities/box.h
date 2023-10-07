#include <vector>

#pragma once

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