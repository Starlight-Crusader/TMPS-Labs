#include "./controller/application.h"
#include "./creationals/builder.h"

#include <iostream>

int main() {
    Director d; d.setBuilder(new ConsoleBuilder());

	Application* app = d.buildApp();
    app->filePrinterTest();
    // app->basicFunctionalityTest();
}
