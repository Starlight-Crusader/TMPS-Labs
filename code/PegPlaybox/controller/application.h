#include <iostream>
#include <string>
#include <vector>

#include "../model/entities/peg.h"
#include "../model/entities/hole.h"
#include "../model/entities/box.h"
#include "../model/entities/color.h"

class Application {
    public:
        static void awbTest() /* adapter, wrapper, bridge */ {
            Hole* filter = new Hole(4.0);

            std::vector<Peg*> pegs;
            pegs.push_back(new RoundPeg(1.0));
            pegs.push_back(new ColoredRoundPeg(2.0, new RedColor));
            pegs.push_back(new ColoredRoundPeg(7.0, new BlueColor));
            pegs.push_back(new ColoredSquarePeg(5.0, new RedColor));
            pegs.push_back(new ColoredSquarePeg(7.0, new BlueColor));

            std::cout << "Filter of radius = " << filter->getParam() << ':' <<'\n';

            for (Peg* peg : pegs) {
                std::cout << peg->buildInfoStr() << " : ";
                if (peg->getType() == "SQUARE") {
                    SquarePeg* square_peg = dynamic_cast<SquarePeg*>(peg);

                    std::cout << "is of the wrong shape";

                    if (filter->fits(new SquarePegAdapter(square_peg))) {
                        std::cout << " but it will fit." << '\n';
                    } else {
                        std::cout << " and it won't fit." << '\n';
                    }
                } else if (peg->getType() == "ROUND") {
                    RoundPeg* round_peg = dynamic_cast<RoundPeg*>(peg);

                    if (filter->fits(round_peg)) {
                        std::cout << "will fit." << '\n';
                    } else {
                        std::cout << "won't fit." << '\n';
                    }
                } else {
                    std::cout << "is of unknown type!";
                }
            }
        }

        static void compositeTest() {
            Component* big_box = new Box();
            Component* small_box = new Box();

            Hole* big_box_filter = new Hole(4.0);
            Hole* small_box_filter = new Hole(2.0);

            std::vector<Peg*> pegs;
            pegs.push_back(new RoundPeg(2.0));
            pegs.push_back(new RoundPeg(4.0));
            pegs.push_back(new RoundPeg(6.0));

            for (Peg* peg : pegs) {
                std::cout << peg->buildInfoStr() << " : ";
                if (small_box_filter->fits(dynamic_cast<RoundPeg*>(peg))) {
                    std::cout << "fits in the small box." << '\n';
                    small_box_filter->push(dynamic_cast<Box*>(small_box), peg);
                } else {
                    std::cout << "won't fit in thre small box ";
                    if (big_box_filter->fits(dynamic_cast<RoundPeg*>(peg))) {
                        std::cout << "but fits in the big box." << '\n';
                        big_box_filter->push(dynamic_cast<Box*>(big_box), peg);
                    } else {
                        std::cout << "and won't fit in the big box so it is discarded." << '\n';
                    }
                }
            }

            big_box->add(small_box);
            std::cout << "We put small box into the big box... ";
            std::cout << big_box->count() << " pegs ended up in the big box." << '\n';
        }
};