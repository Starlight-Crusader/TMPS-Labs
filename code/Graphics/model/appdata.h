#include <vector>
#include <algorithm>

#include "geometry.h"
#include "history.h"
#include "transformations.h"

#pragma once

class AppData {
    private:
        std::vector<Polygon*> graphics;
        std::vector<Caretaker*> state_history;
        CLIHistory* cli_hist_ref;

    public:
        AppData(CLIHistory* hist_ref) {
            graphics.clear(); state_history.clear();
            cli_hist_ref = hist_ref;
        }

        CLIHistory* getHistRef() {
            return cli_hist_ref;
        }

        void add(Polygon* p) { 
            graphics.push_back(p);
        
            Caretaker* pol_ct = new Caretaker(p);
            state_history.push_back(pol_ct);
        }

        void remove(int rem_id) {
            for (int i = 0; i < graphics.size(); i++) {
                if (graphics[i]->id == rem_id) {
                    graphics[i]->eraseData();
                    delete graphics[i];

                    delete state_history[i];

                    graphics.erase(graphics.begin() + i);
                    state_history.erase(state_history.begin() + i);
                }
            }
        }

        Polygon* getPolygon(int pol_id) {
            for (Polygon* pol : graphics) {
                if (pol->id == pol_id) {
                    return pol;
                }
            }
        }

        Caretaker* getCaretaker(int pol_id) {
            for (Caretaker* ct : state_history) {
                if (ct->getOrigId() == pol_id) {
                    return ct;
                }
            }
        }

        std::vector<Polygon*> getGraphics() { return graphics; }

        void eraseData() {
            for (int i = 0; i < graphics.size(); i++) {
                graphics[i]->eraseData();
                delete graphics[i];
                delete state_history[i];
            }
        }
};