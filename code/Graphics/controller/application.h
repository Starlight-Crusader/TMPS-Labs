#include "../model/appdata.h"
#include "../view/cli.h"

class Application {
    private:
        AppData* data;
        CLI* cli;
        CLIHistory* history;

    public:
        Application() {
            history = new CLIHistory();
            data = new AppData(history);
            cli = new CLI(data);
            cli->subscribe(history);
        }

        void main() {
            while(1) {
                cli->mainPrint();
                Command* command = cli->scanCommand();

                if (command != nullptr) {
                    command->execute();
                } else {
                    cli->displayError("Unknown command!");
                }
            }
        }
};

// CREATE 0 0 1 0 0 1
// TRANSFORM {id} -t 1 0
// TRANSFORM {id} -r 0.5 0 0
// TRANSFORM {id} -s 2