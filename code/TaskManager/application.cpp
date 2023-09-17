#include "application.h"
#include "./creationals/builder.h"

#include <iostream>

int main() {
    Director d; d.set_builder(new ConsoleBuilder());

	Application* app = d.build_app();
    app->basic_functionality_test();
}
