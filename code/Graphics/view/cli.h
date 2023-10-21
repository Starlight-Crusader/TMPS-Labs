#include <vector>
#include <string>

#include "../controller/command.h"
#include "../model/appdata.h"
#include "../misc/utility.h"
#include "../misc/observer.h"

#pragma once

class CLI : public Publisher {
    private:
        AppData* ad_ref;

    public:
        CLI(AppData* ad_assigned) : ad_ref(ad_assigned) {}

        AppData* getAD() { return ad_ref; }
        void setAD(AppData* new_ad) { ad_ref = new_ad; }

        Command* scanCommand() {
            std::string line; std::getline(std::cin, line);

            if(line.length() == 0) {
                return nullptr;
            }

            this->notify(line);

            std::vector<std::string> tokens = Utility::tokenizeLine(line);
            std::vector<std::string> param_set(tokens.begin()+1, tokens.end());

            Command* command;

            if (tokens[0] == "CREATE") {
                command = new CreateCommand(ad_ref, param_set);
            } else if (tokens[0] == "DELETE") {
                command = new DeleteCommand(ad_ref, param_set);
            } else if (tokens[0] == "UNDO") {
                command = new UndoCommand(ad_ref, param_set);
            } else if (tokens[0] == "TRANSFORM") {
                command = new TransformationCommand(ad_ref, param_set);
            } else if (tokens[0] == "CLEAR") {
                command = new ClearCommand();
            } else if (tokens[0] == "BREAK") {
                command = new BreakCommand(ad_ref);
            } else if (tokens[0] == "HISTORY") {
                command = new HistoryCommand(ad_ref);
            } else {
                command = nullptr;
            }

            return command;
        }

        void mainPrint() {
            if (ad_ref->getGraphics().size() > 0) {
                std::cout << "====================" << '\n';
                std::cout << "GEOMETRY DATA:" << '\n';

                for (Polygon* pol : ad_ref->getGraphics()) {
                    std::cout << pol->returnGeometryString() << '\n';
                }
            }

            std::cout << ">>> ";
        }

        void displayError(std::string msg) {
            std::cout << "ERROR: " << msg << '\n';
        }
};