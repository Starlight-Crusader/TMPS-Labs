#include "peg.h"
#include "box.h"

#pragma once

class Hole {
    private:
        double radius;

    public:
        Hole(double rad_val) : radius(rad_val) {}

        double getParam() { return radius; }
        void setParam(double val) { radius = val; }

        bool fits(RoundPeg* peg) {
            return radius >= peg->getParam();
        }

        void push(Box* box, Peg* peg) {
            box->add(peg);
        }
};