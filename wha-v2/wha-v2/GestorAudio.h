#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOGDI
#define NOGDI
#endif
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

class GestorAudio {
private:
	bool cargada;
	bool repetir;
	std::string pistaActual;

	void cerrarAlias() {
#ifdef _WIN32
		mciSendStringA("stop whaMusica", NULL, 0, NULL);
		mciSendStringA("close whaMusica", NULL, 0, NULL);
#endif
		cargada = false;
		pistaActual = "";
	}

	bool intentarRuta(const std::string& ruta) {
#ifdef _WIN32
		if (GetFileAttributesA(ruta.c_str()) == INVALID_FILE_ATTRIBUTES) return false;
		cerrarAlias();
		std::string comandoOpen = "open \"" + ruta + "\" type mpegvideo alias whaMusica";
		MCIERROR error = mciSendStringA(comandoOpen.c_str(), NULL, 0, NULL);
		if (error != 0) {
			comandoOpen = "open \"" + ruta + "\" alias whaMusica";
			error = mciSendStringA(comandoOpen.c_str(), NULL, 0, NULL);
		}
		if (error != 0) return false;
		mciSendStringA("setaudio whaMusica volume to 380", NULL, 0, NULL);
		if (mciSendStringA("play whaMusica from 0", NULL, 0, NULL) == 0) {
			cargada = true;
			pistaActual = ruta;
			return true;
		}
		cerrarAlias();
		return false;
#else
		(void)ruta;
		return false;
#endif
	}

	std::string directorioExe() {
#ifdef _WIN32
		char ruta[MAX_PATH];
		if (GetModuleFileNameA(NULL, ruta, MAX_PATH) > 0) {
			std::string s(ruta);
			size_t pos = s.find_last_of("\\/");
			if (pos != std::string::npos) return s.substr(0, pos + 1);
		}
#endif
		return "";
	}

public:
	GestorAudio() {
		cargada = false;
		repetir = true;
	}

	~GestorAudio() {
		detener();
	}

	bool reproducir(const std::string& archivo, bool loop) {
#ifdef _WIN32
		repetir = loop;
		std::string exeDir = directorioExe();
		std::vector<std::string> candidatos;
		candidatos.push_back(archivo);
		candidatos.push_back("audio\\" + archivo);
		if (!exeDir.empty()) {
			candidatos.push_back(exeDir + archivo);
			candidatos.push_back(exeDir + "audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\..\\wha\\wha\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\..\\..\\wha\\wha\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\..\\wha\\wha\\wha\\" + archivo);
		}
		candidatos.push_back("..\\..\\..\\wha\\wha\\" + archivo);
		candidatos.push_back("..\\..\\..\\..\\wha\\wha\\" + archivo);
		candidatos.push_back("..\\..\\..\\..\\..\\wha\\wha\\" + archivo);
		candidatos.push_back("..\\..\\..\\..\\wha\\wha\\wha\\" + archivo);
		for (size_t i = 0; i < candidatos.size(); i++) {
			if (intentarRuta(candidatos[i])) return true;
		}
#endif
		return false;
	}

	bool estaCargado() { return cargada; }

	bool reproducirNivel(int numeroNivel) {
		return reproducir("nivel" + std::to_string(numeroNivel) + ".mp3", true);
	}

	bool reproducirMinijuego() {
		return reproducir("minijuego.mp3", true);
	}

	void actualizar() {
#ifdef _WIN32
		if (cargada && repetir) {
			char estado[64];
			memset(estado, 0, sizeof(estado));
			mciSendStringA("status whaMusica mode", estado, sizeof(estado), NULL);
			if (strcmp(estado, "stopped") == 0) {
				mciSendStringA("play whaMusica from 0", NULL, 0, NULL);
			}
		}
#endif
	}

	void pausar() {
#ifdef _WIN32
		if (cargada) mciSendStringA("pause whaMusica", NULL, 0, NULL);
#endif
	}

	void reanudar() {
#ifdef _WIN32
		if (cargada) mciSendStringA("resume whaMusica", NULL, 0, NULL);
#endif
	}

	void detener() {
#ifdef _WIN32
		cerrarAlias();
#endif
	}
};
