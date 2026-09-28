#ifndef CuartoRicheh_h
#define CuartoRicheh_h

#include <vector>
#include <string>

class CuartoRicheh {
public:
	static void cargarMatriz(std::vector<std::string>& matriz) {
		matriz.clear();
		matriz.push_back("+--------------------------------------------------------------------------------+");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|  +-----+                                  +------------------------------+     |");
		matriz.push_back("|  |     |                                  |                              |     |");
		matriz.push_back("|  |     |                                  |                              |     |");
		matriz.push_back("|  |  *  |                                  |                              |     |");
		matriz.push_back("|  |     |                                  |                              |     |");
		matriz.push_back("|  +--   +----------------------------------+                              |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     +-------------------------------------+                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           +------------------------------+     |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("+--------------------------------------------------------------------------------+");
	}
};

#endif