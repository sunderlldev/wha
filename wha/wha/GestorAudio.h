#ifndef GestorAudio_h
#define GestorAudio_h

#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class GestorAudio;
static GestorAudio* g_instanciaAudioGlobal = nullptr;
static BOOL WINAPI ManejadorCierreConsola(DWORD tipo);
#endif

class GestorAudio {
private:
#ifdef _WIN32
	bool cargada;
	bool procesoActivo;
	PROCESS_INFORMATION piFallback;
	HANDLE hJobAudio;
	std::string pistaActual;
#endif
public:
	GestorAudio() {
#ifdef _WIN32
		cargada = false;
		procesoActivo = false;
		pistaActual = "";
		memset(&piFallback, 0, sizeof(piFallback));
		hJobAudio = CreateJobObjectA(NULL, NULL);
		if (hJobAudio != NULL) {
			JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;
			memset(&jeli, 0, sizeof(jeli));
			jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
			SetInformationJobObject(hJobAudio, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
		}
		g_instanciaAudioGlobal = this;
		SetConsoleCtrlHandler(ManejadorCierreConsola, TRUE);
#endif
	}

	~GestorAudio() {
		detenerMusica();
#ifdef _WIN32
		if (g_instanciaAudioGlobal == this) {
			g_instanciaAudioGlobal = nullptr;
		}
		if (hJobAudio != NULL) {
			CloseHandle(hJobAudio);
			hJobAudio = NULL;
		}
#endif
	}

	bool reproducirArchivo(const std::string& ruta, int volumen = 500) {
#ifdef _WIN32
		detenerMusica();
		mciSendStringA("close musicaFondo", NULL, 0, NULL);
		char rutaAbs[MAX_PATH];
		if (GetFullPathNameA(ruta.c_str(), MAX_PATH, rutaAbs, NULL) == 0) {
			return false;
		}
		if (GetFileAttributesA(rutaAbs) == INVALID_FILE_ATTRIBUTES) {
			return false;
		}
		char tempDir[MAX_PATH];
		std::string rutaReproducir = std::string(rutaAbs);
		if (GetTempPathA(MAX_PATH, tempDir) > 0) {
			std::string nombreSolo = "pista.mp3";
			size_t sep = ruta.find_last_of("\\/");
			if (sep != std::string::npos) {
				nombreSolo = ruta.substr(sep + 1);
			} else {
				nombreSolo = ruta;
			}
			std::string destTemp = std::string(tempDir) + "wha_" + nombreSolo;
			if (CopyFileA(rutaAbs, destTemp.c_str(), FALSE)) {
				rutaReproducir = destTemp;
			}
		}
		char rutaCorta[MAX_PATH];
		std::string rutaMCI = rutaReproducir;
		if (GetShortPathNameA(rutaReproducir.c_str(), rutaCorta, MAX_PATH) > 0) {
			rutaMCI = std::string(rutaCorta);
		}
		std::string cmdOpen = "open \"" + rutaMCI + "\" type mpegvideo alias musicaFondo";
		MCIERROR err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
		if (err != 0) {
			cmdOpen = "open \"" + rutaMCI + "\" alias musicaFondo";
			err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
		}
		if (err == 0) {
			std::string cmdVol = "setaudio musicaFondo volume to " + std::to_string(volumen);
			mciSendStringA(cmdVol.c_str(), NULL, 0, NULL);
			MCIERROR playErr = mciSendStringA("play musicaFondo from 0", NULL, 0, NULL);
			if (playErr == 0) {
				cargada = true;
				pistaActual = rutaMCI;
				return true;
			}
		}

		DWORD miPid = GetCurrentProcessId();
		std::string vbsRuta = std::string(tempDir) + "wha_play.vbs";
		std::ofstream vbs(vbsRuta.c_str());
		if (vbs.is_open()) {
			int volPorc = volumen / 10;
			if (volPorc < 10) volPorc = 10;
			if (volPorc > 100) volPorc = 100;
			vbs << "On Error Resume Next\n";
			vbs << "Set w = CreateObject(\"WMPlayer.OCX\")\n";
			vbs << "If Err.Number = 0 Then\n";
			vbs << "  w.settings.autoStart = True\n";
			vbs << "  w.settings.setMode \"loop\", True\n";
			vbs << "  w.settings.volume = " << volPorc << "\n";
			vbs << "  w.URL = \"" << rutaReproducir << "\"\n";
			vbs << "  w.controls.play\n";
			vbs << "  Set wmi = GetObject(\"winmgmts:\")\n";
			vbs << "  Do While True\n";
			vbs << "    Set pList = wmi.ExecQuery(\"Select ProcessId From Win32_Process Where ProcessId = " << miPid << "\")\n";
			vbs << "    If pList.Count = 0 Then\n";
			vbs << "      w.controls.stop\n";
			vbs << "      WScript.Quit\n";
			vbs << "    End If\n";
			vbs << "    WScript.Sleep 1000\n";
			vbs << "  Loop\n";
			vbs << "End If\n";
			vbs.close();

			STARTUPINFOA si;
			memset(&si, 0, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			memset(&piFallback, 0, sizeof(piFallback));

			std::string cmdWscript = "wscript.exe //B //nologo \"" + vbsRuta + "\"";
			if (CreateProcessA(NULL, (LPSTR)cmdWscript.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &piFallback)) {
				if (hJobAudio != NULL) {
					AssignProcessToJobObject(hJobAudio, piFallback.hProcess);
				}
				DWORD waitRes = WaitForSingleObject(piFallback.hProcess, 200);
				if (waitRes == WAIT_TIMEOUT) {
					procesoActivo = true;
					cargada = true;
					pistaActual = rutaReproducir;
					return true;
				} else {
					CloseHandle(piFallback.hProcess);
					CloseHandle(piFallback.hThread);
					memset(&piFallback, 0, sizeof(piFallback));
				}
			}
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
		if (cargada && !procesoActivo) {
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
		}
		if (procesoActivo) {
			TerminateProcess(piFallback.hProcess, 0);
			CloseHandle(piFallback.hProcess);
			CloseHandle(piFallback.hThread);
			procesoActivo = false;
			memset(&piFallback, 0, sizeof(piFallback));
		}
		pistaActual = "";
#endif
	}

	void pausarMusica() {
#ifdef _WIN32
		if (cargada && !procesoActivo) {
			mciSendStringA("pause musicaFondo", NULL, 0, NULL);
		}
#endif
	}

	void reanudarMusica() {
#ifdef _WIN32
		if (cargada && !procesoActivo) {
			mciSendStringA("resume musicaFondo", NULL, 0, NULL);
		}
#endif
	}

	void setVolumen(int volumen) {
#ifdef _WIN32
		if (cargada && !procesoActivo) {
			std::string cmdVol = "setaudio musicaFondo volume to " + std::to_string(volumen);
			mciSendStringA(cmdVol.c_str(), NULL, 0, NULL);
		}
#else
		(void)volumen;
#endif
	}
};

#ifdef _WIN32
static BOOL WINAPI ManejadorCierreConsola(DWORD tipo) {
	(void)tipo;
	if (g_instanciaAudioGlobal != nullptr) {
		g_instanciaAudioGlobal->detenerMusica();
	}
	return FALSE;
}
#endif

#endif
