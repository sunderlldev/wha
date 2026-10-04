#ifndef GestorAudio_h
#define GestorAudio_h

#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

class GestorAudio {
private:
#ifdef _WIN32
	bool cargada;
	std::string pistaActual;
#endif
public:
	GestorAudio() {
#ifdef _WIN32
		cargada = false;
		pistaActual = "";
#endif
	}

	~GestorAudio() {
		detenerMusica();
	}

	bool reproducirArchivo(const std::string& ruta, int volumen = 500) {
#ifdef _WIN32
		detenerMusica();
		char rutaAbs[MAX_PATH];
		if (GetFullPathNameA(ruta.c_str(), MAX_PATH, rutaAbs, NULL) == 0) {
			return false;
		}
		if (GetFileAttributesA(rutaAbs) == INVALID_FILE_ATTRIBUTES) {
			return false;
		}
		char rutaCorta[MAX_PATH];
		if (GetShortPathNameA(rutaAbs, rutaCorta, MAX_PATH) == 0) {
			strncpy(rutaCorta, rutaAbs, MAX_PATH);
		}
		std::string cmdOpen = "open \"" + std::string(rutaCorta) + "\" type mpegvideo alias musicaFondo";
		MCIERROR err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
		if (err != 0) {
			cmdOpen = "open \"" + std::string(rutaCorta) + "\" alias musicaFondo";
			err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
		}
		if (err == 0) {
			std::string cmdVol = "setaudio musicaFondo volume to " + std::to_string(volumen);
			mciSendStringA(cmdVol.c_str(), NULL, 0, NULL);
			mciSendStringA("play musicaFondo from 0", NULL, 0, NULL);
			cargada = true;
			pistaActual = std::string(rutaCorta);
			return true;
		}
#else
		(void)ruta;
		(void)volumen;
#endif
		return false;
	}

	bool reproducirMusica(const std::string& nombreBase, int volumen = 500) {
		if (reproducirArchivo(nombreBase, volumen)) return true;
#ifdef _WIN32
		char exePath[MAX_PATH];
		if (GetModuleFileNameA(NULL, exePath, MAX_PATH) > 0) {
			std::string sExe(exePath);
			size_t lastSlash = sExe.find_last_of("\\/");
			if (lastSlash != std::string::npos) {
				std::string exeDir = sExe.substr(0, lastSlash + 1);
				if (reproducirArchivo(exeDir + nombreBase, volumen)) return true;
				if (reproducirArchivo(exeDir + "..\\" + nombreBase, volumen)) return true;
				if (reproducirArchivo(exeDir + "..\\..\\" + nombreBase, volumen)) return true;
				if (reproducirArchivo(exeDir + "..\\..\\wha\\" + nombreBase, volumen)) return true;
				if (reproducirArchivo(exeDir + "..\\..\\wha\\wha\\" + nombreBase, volumen)) return true;
				if (reproducirArchivo(exeDir + "..\\..\\..\\" + nombreBase, volumen)) return true;
			}
		}
#endif
		if (reproducirArchivo("wha/" + nombreBase, volumen)) return true;
		if (reproducirArchivo("wha/wha/" + nombreBase, volumen)) return true;
		if (reproducirArchivo("../" + nombreBase, volumen)) return true;
		if (reproducirArchivo("../../" + nombreBase, volumen)) return true;
		if (reproducirArchivo("../../wha/wha/" + nombreBase, volumen)) return true;
		if (reproducirArchivo("../../../" + nombreBase, volumen)) return true;
		return false;
	}

	bool reproducirNivel(int numeroNivel, int volumen = 500) {
		std::string archivo = "nivel" + std::to_string(numeroNivel) + ".mp3";
		if (reproducirMusica(archivo, volumen)) return true;
		return reproducirMusica("musicaFondo.mp3", volumen);
	}

	bool reproducirMinijuego(int volumen = 500) {
		if (reproducirMusica("minijuego.mp3", volumen)) return true;
		return false;
	}

	void actualizar() {
#ifdef _WIN32
		if (cargada) {
			char estado[64];
			memset(estado, 0, sizeof(estado));
			mciSendStringA("status musicaFondo mode", estado, sizeof(estado), NULL);
			if (strcmp(estado, "stopped") == 0) {
				mciSendStringA("play musicaFondo from 0", NULL, 0, NULL);
			}
		}
#endif
	}

	void detenerMusica() {
#ifdef _WIN32
		if (cargada) {
			mciSendStringA("stop musicaFondo", NULL, 0, NULL);
			mciSendStringA("close musicaFondo", NULL, 0, NULL);
			cargada = false;
			pistaActual = "";
		}
#endif
	}

	void pausarMusica() {
#ifdef _WIN32
		if (cargada) {
			mciSendStringA("pause musicaFondo", NULL, 0, NULL);
		}
#endif
	}

	void reanudarMusica() {
#ifdef _WIN32
		if (cargada) {
			mciSendStringA("resume musicaFondo", NULL, 0, NULL);
		}
#endif
	}

	void setVolumen(int volumen) {
#ifdef _WIN32
		if (cargada) {
			std::string cmdVol = "setaudio musicaFondo volume to " + std::to_string(volumen);
			mciSendStringA(cmdVol.c_str(), NULL, 0, NULL);
		}
#else
		(void)volumen;
#endif
	}
};

#endif
