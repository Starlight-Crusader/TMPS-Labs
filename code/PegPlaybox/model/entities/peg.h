#include <string>
#include <cmath>

#include "box.h"
#include "color.h"

#pragma once

class Peg : public Component {
    public:
        virtual double getParam() = 0;
        virtual void setParam(double) = 0;

        virtual std::string getType() = 0;

        int count() { return 1; }

        virtual std::string buildInfoStr() = 0;
};

class ColoredPeg {
    private:
        Color* color;

    public:
        ColoredPeg() {}
        ColoredPeg(Color* color_to_set) : color(color_to_set) {}

        void setColor(Color* new_color) { color = new_color; }
        std::string getColor() { return color->getColor(); }
};

class RoundPeg : public Peg {
    private:
        double radius;

    public:
        RoundPeg() {}
        RoundPeg(double param_val) : radius(param_val) {}

        double getParam() override { return radius; }
        void setParam(double val) override { radius = val; }

        std::string getType() override { return "ROUND"; }

        std::string buildInfoStr() override {
            return "UNCOLORED round peg of radius = " + std::to_string(radius);
        }
};

class ColoredRoundPeg : public RoundPeg, public ColoredPeg {
    public:
        ColoredRoundPeg(double param_val, Color* color_to_set) : RoundPeg(param_val), ColoredPeg(color_to_set) {}

        std::string buildInfoStr() override {
            return this->getColor() + " round peg of radius = " + std::to_string(this->getParam());
        }
};

class SquarePeg : public Peg {
    private:
        double width;

    public:
        SquarePeg(double param_val) : width(param_val) {}

        double getParam() override { return width; }
        void setParam(double val) override { width = val; }

        std::string getType() override { return "SQUARE"; }

        std::string buildInfoStr() override {
            return "UNCOLORED square peg of width = " + std::to_string(width);
        }
};

class ColoredSquarePeg : public SquarePeg, public ColoredPeg {
    public:
        ColoredSquarePeg(double param_val, Color* color_to_set) : SquarePeg(param_val), ColoredPeg(color_to_set) {}

        std::string buildInfoStr() override {
            return this->getColor() + " square peg of width = " + std::to_string(this->getParam());
        }
};

class SquarePegAdapter : public RoundPeg {
    private:
        Peg* peg;

    public:
        SquarePegAdapter(Peg* peg_to_adapt) : peg(peg_to_adapt) {}

        double getParam() { return peg->getParam() * sqrt(2) / 2; }
};