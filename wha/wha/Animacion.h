#ifndef Animacion_h
#define Animacion_h

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

class Animacion {
public:
	static void animarTexto(const std::string& texto, int velocidadMs = 25) {
		for (size_t i = 0; i < texto.length(); i++) {
			std::cout << texto[i] << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(velocidadMs));
		}
		std::cout << std::endl;
	}

	static void animarDialogo(const std::string& hablante, const std::string& texto, int velocidadMs = 25) {
		std::cout << "[" << hablante << "]: ";
		std::cout.flush();
		for (size_t i = 0; i < texto.length(); i++) {
			std::cout << texto[i] << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(velocidadMs));
		}
		std::cout << std::endl;
	}
};

#endif
