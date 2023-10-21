#include <math.h>

#include "../model/geometry.h"

#pragma once

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