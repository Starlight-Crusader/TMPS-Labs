#include <string>
#include <vector>

#include "../model/appdata.h"

#include "../model/geometry.h"

#pragma once

class Command {
    protected:
        AppData* app_data_ref;
        std::vector<std::string> params;

    public:
        Command() {}
        Command(AppData* ad_ref) : app_data_ref(ad_ref) {}
        Command(AppData* ad_ref, std::vector<std::string> param_set) : app_data_ref(ad_ref), params(param_set) {}

        virtual void execute() = 0;
};

class CreateCommand : public Command {
    public:
        CreateCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            std::vector<Vector2D> new_vert;

            new_vert.push_back(Vector2D(stod(params[0]), stod(params[1])));
            new_vert.push_back(Vector2D(stod(params[2]), stod(params[3])));
            new_vert.push_back(Vector2D(stod(params[4]), stod(params[5])));

            Polygon* newPol = new Polygon(); newPol->set(new_vert);
            app_data_ref->add(newPol);
        }
};

class DeleteCommand : public Command {
    public:
        DeleteCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->remove(stoi(params[0]));
        }
};

class TransformationCommand : public Command {
    public:
        TransformationCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->getCaretaker(stoi(params[0]))->save();

            if (params[1] == "-t") {
                Translation transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(Vector2D(stod(params[2]), stod(params[3])));
            } else if (params[1] == "-r") {
                Rotation transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(stod(params[2]), Vector2D(stod(params[3]), stod(params[4])));
            } else if (params[1] == "-s") {
                Scale transformation(app_data_ref->getPolygon(stoi(params[0])));
                transformation.transform(stod(params[2]));
            } else {
                // Return/Display an error ...
            }
        }  
};

class UndoCommand : public Command {
    public:
        UndoCommand(AppData* ad_ref, std::vector<std::string> param_set) : Command(ad_ref, param_set) {}

        void execute() override {
            app_data_ref->getCaretaker(stoi(params[0]))->undo();
        }
};

class BreakCommand : public Command {
    public:
        BreakCommand(AppData* ad_ref) : Command(ad_ref) {}

        void execute() override { 
            app_data_ref->eraseData();
            delete app_data_ref->getHistRef(); 
            exit(0); 
        }
};

class HistoryCommand : public Command {
    public:
        HistoryCommand(AppData* ad_ref) : Command(ad_ref) {}

        void execute() override {
            app_data_ref->getHistRef()->print();
        }
};

class ClearCommand : public Command {
    public:
        ClearCommand() {}

        void execute() override { system("clear"); }
};