#include "application.h"
#include "./creationals/builder.h"

int main() {
    Director d; d.set_builder(new ConsoleBuilder());

	Application* app = d.build_product();
	app->setup_console_application();

    app->basic_functionality_test();
}
