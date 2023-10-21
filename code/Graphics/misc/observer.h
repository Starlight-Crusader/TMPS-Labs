#include <string>
#include <vector>

#pragma once

class Subscriber {
    public:
        virtual void update(std::string) = 0;
};

class Publisher {
    protected:
        std::vector<Subscriber*> subscribers;

    public:
        void subscribe(Subscriber* sub_to_add) { subscribers.push_back(sub_to_add); }
        void unsubscribe(Subscriber* sub_to_remove) {
            for (int i = 0; i < subscribers.size(); i++) {
                if (subscribers[i] == sub_to_remove) {
                    subscribers.erase(subscribers.begin() + i);
                    break;
                }
            }
        }

        void notify(std::string context) {
            for (Subscriber* sub : subscribers) {
                sub->update(context);
            }
        }
};