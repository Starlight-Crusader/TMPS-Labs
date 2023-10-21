#include <vector>

#include "../misc/utility.h"
#pragma once

class Vector2D {
    public:
        double x;
        double y;

        Vector2D(double x_val, double y_val) : 
            x(x_val), 
            y(y_val) 
        {}

        void set(Vector2D val) {
            x = val.x; y = val.y;
        }

        void add(Vector2D del) {
            x += del.x;
            y += del.y;
        }

        void sub(Vector2D del) {
            x -= del.x;
            y -= del.y;
        }

        void mul(double factor) {
            x *= factor;
            y *= factor;
        }

        std::string str() { 
            return '<' + std::to_string(x) + ", " + std::to_string(y) + '>'; 
        }

        Vector2D get() {
            return Vector2D(x, y);
        }
};

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
        int id;
        std::vector<Vector2D*> vertices;

        Polygon() : id(Utility::generateRandomId()) {
            for (int i = 0; i < 3; i++) {
                vertices.push_back(new Vector2D(0, 0));
            }
        }

        void set(std::vector<Vector2D> new_vert) {
            for (size_t i = 0; i < vertices.size(); ++i) {
                vertices[i]->set(new_vert[i]);
            }
        }

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

        std::string returnGeometryString() {
            std::string str = "P " + std::to_string(id) + " : { <" +
            std::to_string(vertices[0]->x) + ", " + std::to_string(vertices[0]->y) + ">, <" +
            std::to_string(vertices[1]->x) + ", " + std::to_string(vertices[1]->y) + ">, <" +
            std::to_string(vertices[2]->x) + ", " + std::to_string(vertices[2]->y) + "> }";

            return str;
        }

        void eraseData() {
            for (Vector2D* vertex : vertices) {
                delete vertex;
            }
        }
};