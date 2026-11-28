#ifndef MapaNivel2_h
#define MapaNivel2_h

#include <vector>
#include <string>

class MapaNivel2 {
public:
    static void cargarMatriz(std::vector<std::string>& matriz) {
        matriz.clear();
        matriz.reserve(62);
        matriz.push_back(R"(VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V     A-----------------------C                                                                                            V)");
        matriz.push_back(R"(V     |.......................|       VVVVVVVVVVVVVVVVVVVVVVVV        VVVVVVVVVVVVVVVVVVVVVV                               V)");
        matriz.push_back(R"(V     |.......................|                                                                                            V)");
        matriz.push_back(R"(V     |.......................|                                                                                            V)");
        matriz.push_back(R"(V     |........................                                                                                            V)");
        matriz.push_back(R"(V     |........................                                                                                            V)");
        matriz.push_back(R"(V     |........................       A-----------------------C       A---------------------C                              V)");
        matriz.push_back(R"(V     |.......................|       |.......................|       |.....................|                              V)");
        matriz.push_back(R"(V     |.......................|       |.......................|       |.....................|                              V)");
        matriz.push_back(R"(V     |.......................|       |.......................|       |.....................|                              V)");
        matriz.push_back(R"(V     B---------.....---------D       .........................       .......................                              V)");
        matriz.push_back(R"(V                                     .........................       .......................                              V)");
        matriz.push_back(R"(V                                     .........................       .......................     A------.....------C      V)");
        matriz.push_back(R"(V                                     |.......................|       |.....................|     |.................|      V)");
        matriz.push_back(R"(V                                     |.......................|       |.....................|     |.................|      V)");
        matriz.push_back(R"(V                                     |.......................|       |.....................|     |.................|      V)");
        matriz.push_back(R"(V                                     B---------....----------D       B-------....----------D     ..................|      V)");
        matriz.push_back(R"(V                                                                                                 ..................|      V)");
        matriz.push_back(R"(V                                                                                                 ..................|      V)");
        matriz.push_back(R"(V                                                                                                 ..................|      V)");
        matriz.push_back(R"(V     A----....-----C                                                                             |.................|      V)");
        matriz.push_back(R"(V     |.............|                                                                             |.................|      V)");
        matriz.push_back(R"(V     |.............|                                                                             |.................|      V)");
        matriz.push_back(R"(V     |..............                 A---------....----------C                                   |.................|      V)");
        matriz.push_back(R"(V     |..............                 |.......................|       A-------....--------C       B-----------------D      V)");
        matriz.push_back(R"(V     |..............                 |.......................|       |...................|                                V)");
        matriz.push_back(R"(V     |..............                 .........................       |...................|                                V)");
        matriz.push_back(R"(V     |.............|                 .........................       ....................|                                V)");
        matriz.push_back(R"(V     |.............|                 .........................       ....................|                                V)");
        matriz.push_back(R"(V     |.............|                 .........................       ....................|                                V)");
        matriz.push_back(R"(V     B-------------D                 |.......................|       ....................|                                V)");
        matriz.push_back(R"(V                                     |.......................|       |...................|                                V)");
        matriz.push_back(R"(V                                     |.......................|       |...................|                                V)");
        matriz.push_back(R"(V                                     B---------....----------D       |...................|                                V)");
        matriz.push_back(R"(V                                                                     B-------....--------D                                V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                     VVVVVVVV     VVVVVVVVV                               V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V     A---------....----------C                                                                                            V)");
        matriz.push_back(R"(V     |.......................|       VVVVVVVVV      VVVVVVVVVVVVVVVVVVVVV      VVVVV                                      V)");
        matriz.push_back(R"(V     |.......................|       V.............................................V                                      V)");
        matriz.push_back(R"(V     |.......................|       V.............................................V                                      V)");
        matriz.push_back(R"(V     |........................        .............................................                                       V)");
        matriz.push_back(R"(V     |........................        .............................................                                       V)");
        matriz.push_back(R"(V     |........................        .............................................                                       V)");
        matriz.push_back(R"(V     |.......................|        .............................................                                       V)");
        matriz.push_back(R"(V     |.......................|       V.............................................V                                      V)");
        matriz.push_back(R"(V     |.......................|       V.............................................V                                      V)");
        matriz.push_back(R"(V     B-----------------------D       V.............................................V                                      V)");
        matriz.push_back(R"(V                                     VVVVVVVVVVVVVVVVVVVV       VVVVVVVVVVVVVVVVVVVV                                      V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(V                                                                                                                          V)");
        matriz.push_back(R"(VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV)");
    }
};

#endif
