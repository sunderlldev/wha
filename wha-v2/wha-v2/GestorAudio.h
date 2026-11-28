#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include "AudioMotor.h"

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
	void* motorHandle;

	static std::vector<unsigned char>& BufferPaso() {
		static std::vector<unsigned char> buf;
		return buf;
	}
	static std::vector<unsigned char>& BufferDialogo() {
		static std::vector<unsigned char> buf;
		return buf;
	}
	static std::vector<unsigned char>& BufferAccion() {
		static std::vector<unsigned char> buf;
		return buf;
	}
	static bool& SfxHabilitadoFlag() {
		static bool flag = true;
		return flag;
	}
	static bool& SfxInicializadoFlag() {
		static bool init = false;
		return init;
	}
	static ULONGLONG& UltimoPasoMs() {
		static ULONGLONG ultimo = 0;
		return ultimo;
	}
	static ULONGLONG& UltimoDialogoMs() {
		static ULONGLONG ultimo = 0;
		return ultimo;
	}
	static ULONGLONG& UltimoAccionMs() {
		static ULONGLONG ultimo = 0;
		return ultimo;
	}

	static void EscribirEntero32(std::vector<unsigned char>& v, int val) {
		v.push_back((unsigned char)(val & 0xFF));
		v.push_back((unsigned char)((val >> 8) & 0xFF));
		v.push_back((unsigned char)((val >> 16) & 0xFF));
		v.push_back((unsigned char)((val >> 24) & 0xFF));
	}

	static void EscribirEntero16(std::vector<unsigned char>& v, short val) {
		v.push_back((unsigned char)(val & 0xFF));
		v.push_back((unsigned char)((val >> 8) & 0xFF));
	}

	static void FinalizarWav(std::vector<unsigned char>& out, const std::vector<short>& muestras, int sampleRate) {
		out.clear();
		int dataSize = (int)muestras.size() * 2;
		out.reserve(44 + dataSize);
		const char* riff = "RIFF";
		const char* wave = "WAVEfmt ";
		const char* data = "data";
		for (int i = 0; i < 4; i++) out.push_back((unsigned char)riff[i]);
		EscribirEntero32(out, 36 + dataSize);
		for (int i = 0; i < 8; i++) out.push_back((unsigned char)wave[i]);
		EscribirEntero32(out, 16);
		EscribirEntero16(out, 1);
		EscribirEntero16(out, 1);
		EscribirEntero32(out, sampleRate);
		EscribirEntero32(out, sampleRate * 2);
		EscribirEntero16(out, 2);
		EscribirEntero16(out, 16);
		for (int i = 0; i < 4; i++) out.push_back((unsigned char)data[i]);
		EscribirEntero32(out, dataSize);
		for (size_t i = 0; i < muestras.size(); i++) {
			EscribirEntero16(out, muestras[i]);
		}
	}

	static void ConstruirSfxPaso(std::vector<unsigned char>& out) {
		int sr = 22050;
		int n = (sr * 55) / 1000;
		std::vector<short> m(n, 0);
		double fase = 0.0;
		double filtro = 0.0;
		unsigned int rng = 424242u;
		for (int i = 0; i < n; i++) {
			double t = (double)i / (double)n;
			double freq = 148.0 - 72.0 * t;
			fase += (2.0 * 3.14159265358979 * freq) / (double)sr;
			rng = rng * 1664525u + 1013904223u;
			double r = ((double)(rng % 2000) / 1000.0) - 1.0;
			filtro = filtro * 0.68 + r * 0.32;
			double env = (1.0 - t) * std::sqrt(1.0 - t);
			if (i < 110) env *= (double)i / 110.0;
			double s = (std::sin(fase) * 0.42 + filtro * 0.58) * env * 0.34;
			m[i] = (short)(s * 32767.0);
		}
		FinalizarWav(out, m, sr);
	}

	static void ConstruirSfxDialogo(std::vector<unsigned char>& out) {
		int sr = 22050;
		int n = (sr * 22) / 1000;
		std::vector<short> m(n, 0);
		double fase = 0.0;
		double filtro = 0.0;
		unsigned int rng = 135791u;
		for (int i = 0; i < n; i++) {
			double t = (double)i / (double)n;
			double freq = 185.0 - 55.0 * t;
			fase += (2.0 * 3.14159265358979 * freq) / (double)sr;
			rng = rng * 1664525u + 1013904223u;
			double r = ((double)(rng % 2000) / 1000.0) - 1.0;
			filtro = filtro * 0.75 + r * 0.25;
			double env = std::exp(-5.0 * t) * (1.0 - t);
			if (i < 35) env *= (double)i / 35.0;
			double s = (std::sin(fase) * 0.72 + filtro * 0.28) * env * 0.18;
			m[i] = (short)(s * 32767.0);
		}
		FinalizarWav(out, m, sr);
	}

	static void ConstruirSfxAccion(std::vector<unsigned char>& out) {
		int sr = 22050;
		int n = (sr * 85) / 1000;
		std::vector<short> m(n, 0);
		double f1 = 0.0;
		double f2 = 0.0;
		for (int i = 0; i < n; i++) {
			double t = (double)i / (double)n;
			f1 += (2.0 * 3.14159265358979 * 293.66) / (double)sr;
			f2 += (2.0 * 3.14159265358979 * 440.0) / (double)sr;
			double env = std::exp(-4.0 * t) * (1.0 - t);
			if (i < 65) env *= (double)i / 65.0;
			double s = (std::sin(f1) * 0.6 + std::sin(f2) * 0.4) * env * 0.18;
			m[i] = (short)(s * 32767.0);
		}
		FinalizarWav(out, m, sr);
	}

	static void AsegurarSfx() {
		if (SfxInicializadoFlag()) return;
		SfxInicializadoFlag() = true;
		ConstruirSfxPaso(BufferPaso());
		ConstruirSfxDialogo(BufferDialogo());
		ConstruirSfxAccion(BufferAccion());
	}

	void cerrarStream() {
		if (motorHandle) {
			AudioMotor_Detener(motorHandle);
			motorHandle = NULL;
		}
		cargada = false;
		pistaActual = "";
	}

	bool intentarRuta(const std::string& ruta) {
		void* nuevo = AudioMotor_Iniciar(ruta.c_str(), repetir, 0.48f);
		if (!nuevo) return false;
		cerrarStream();
		motorHandle = nuevo;
		cargada = true;
		pistaActual = ruta;
		return true;
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
		motorHandle = NULL;
		AsegurarSfx();
	}

	~GestorAudio() {
		detener();
	}

	static void SetSfxHabilitado(bool activo) {
		SfxHabilitadoFlag() = activo;
	}

	static void ReproducirSfxPaso() {
#ifdef _WIN32
		if (!SfxHabilitadoFlag()) return;
		ULONGLONG ahora = GetTickCount64();
		if (ahora - UltimoPasoMs() < 130) return;
		AsegurarSfx();
		if (!BufferPaso().empty()) {
			UltimoPasoMs() = ahora;
			PlaySoundA((LPCSTR)&BufferPaso()[0], NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
#endif
	}

	static void ReproducirSfxDialogo() {
#ifdef _WIN32
		if (!SfxHabilitadoFlag()) return;
		ULONGLONG ahora = GetTickCount64();
		if (ahora - UltimoDialogoMs() < 85) return;
		AsegurarSfx();
		if (!BufferDialogo().empty()) {
			UltimoDialogoMs() = ahora;
			PlaySoundA((LPCSTR)&BufferDialogo()[0], NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
#endif
	}

	static void ReproducirSfxAccion() {
#ifdef _WIN32
		if (!SfxHabilitadoFlag()) return;
		ULONGLONG ahora = GetTickCount64();
		if (ahora - UltimoAccionMs() < 75) return;
		AsegurarSfx();
		if (!BufferAccion().empty()) {
			UltimoAccionMs() = ahora;
			PlaySoundA((LPCSTR)&BufferAccion()[0], NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
#endif
	}

	bool reproducir(const std::string& archivo, bool loop) {
#ifdef _WIN32
		repetir = loop;
		std::string exeDir = directorioExe();
		std::vector<std::string> candidatos;
		candidatos.push_back("audio\\" + archivo);
		candidatos.push_back(archivo);
		if (!exeDir.empty()) {
			candidatos.push_back(exeDir + "audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\wha-v2\\wha-v2\\audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\wha-v2\\wha-v2\\audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\..\\wha-v2\\wha-v2\\audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\audio\\" + archivo);
			candidatos.push_back(exeDir + "..\\..\\..\\audio\\" + archivo);
		}
		candidatos.push_back("..\\..\\wha-v2\\audio\\" + archivo);
		candidatos.push_back("..\\..\\..\\wha-v2\\wha-v2\\audio\\" + archivo);
		for (size_t i = 0; i < candidatos.size(); i++) {
			if (intentarRuta(candidatos[i])) return true;
		}
		return false;
#else
		(void)archivo;
		(void)loop;
		return false;
#endif
	}

	bool reproducirNivel(int numNivel) {
		if (numNivel == 2) return reproducir("nivel2.mp3", true);
		if (numNivel == 3) return reproducir("nivel3.mp3", true);
		return reproducir("nivel1.mp3", true);
	}

	bool reproducirMinijuego() {
		return reproducir("minijuego.mp3", true);
	}

	void actualizar() {
	}

	void pausar() {
		if (cargada && motorHandle) AudioMotor_Pausar(motorHandle);
	}

	void reanudar() {
		if (cargada && motorHandle) AudioMotor_Reanudar(motorHandle);
	}

	void detener() {
#ifdef _WIN32
		PlaySoundA(NULL, NULL, 0);
#endif
		cerrarStream();
	}

	bool estaCargado() const { return cargada; }
};
