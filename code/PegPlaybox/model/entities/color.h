#include <string>

#pragma once

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