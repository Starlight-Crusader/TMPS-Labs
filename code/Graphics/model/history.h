#include <vector>

#include "../model/geometry.h"
#include "../misc/observer.h"

#pragma once

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

class CLIHistory : public Subscriber {
    private:
        std::vector<std::string> history;

    public:
        void update(std::string last) {
            history.push_back(last);
        }

        void print() {
            int c = 0;

            for (std::string line : history) {
                std::cout << c << ". " << line << '\n';
                c++;
            }
        }
};