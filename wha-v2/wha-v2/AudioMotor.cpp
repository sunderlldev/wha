#include "AudioMotor.h"

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <string>
#include <vector>
#include <cstring>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "ole32.lib")

namespace {

	struct EstadoStream {
		CRITICAL_SECTION cs;
		HANDLE hThread;
		volatile LONG detener;
		volatile LONG pausado;
		HWAVEOUT hWaveOut;
		std::string ruta;
		bool repetir;
		float volumen;

		EstadoStream() {
			InitializeCriticalSection(&cs);
			hThread = NULL;
			detener = 0;
			pausado = 0;
			hWaveOut = NULL;
			repetir = true;
			volumen = 0.48f;
		}

		~EstadoStream() {
			DeleteCriticalSection(&cs);
		}
	};

	std::wstring ConvertirRutaWide(const std::string& ruta) {
		if (ruta.empty()) return std::wstring();
		int len = MultiByteToWideChar(CP_ACP, 0, ruta.c_str(), -1, NULL, 0);
		if (len <= 0) return std::wstring();
		std::wstring w(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, ruta.c_str(), -1, &w[0], len);
		while (!w.empty() && w.back() == L'\0') w.pop_back();
		return w;
	}

	bool ProbarRutaLegible(const std::string& ruta) {
		if (GetFileAttributesA(ruta.c_str()) == INVALID_FILE_ATTRIBUTES) return false;
		HRESULT hrCom = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		HRESULT hrMf = MFStartup(MF_VERSION);
		bool ok = false;
		if (SUCCEEDED(hrMf)) {
			std::wstring wRuta = ConvertirRutaWide(ruta);
			IMFSourceReader* reader = NULL;
			if (!wRuta.empty() && SUCCEEDED(MFCreateSourceReaderFromURL(wRuta.c_str(), NULL, &reader)) && reader) {
				ok = true;
				reader->Release();
			}
			MFShutdown();
		}
		if (SUCCEEDED(hrCom)) CoUninitialize();
		return ok;
	}

	DWORD WINAPI HiloStreamMusica(LPVOID param) {
		EstadoStream* st = (EstadoStream*)param;
		if (!st) return 0;

		HRESULT hrCom = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		HRESULT hrMf = MFStartup(MF_VERSION);

		std::wstring wRuta = ConvertirRutaWide(st->ruta);
		IMFSourceReader* reader = NULL;
		IMFMediaType* partialType = NULL;
		IMFMediaType* actualType = NULL;
		WAVEFORMATEX* pWfx = NULL;
		UINT32 cbWfx = 0;
		HANDLE hWaveEvt = NULL;
		HWAVEOUT hWave = NULL;

		if (SUCCEEDED(hrMf) && !wRuta.empty()) {
			if (SUCCEEDED(MFCreateSourceReaderFromURL(wRuta.c_str(), NULL, &reader)) && reader) {
				if (SUCCEEDED(MFCreateMediaType(&partialType)) && partialType) {
					partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
					partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
					partialType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
					partialType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 2);
					partialType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);
					reader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, partialType);
					partialType->Release();
					partialType = NULL;
				}
				if (SUCCEEDED(reader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &actualType)) && actualType) {
					MFCreateWaveFormatExFromMFMediaType(actualType, &pWfx, &cbWfx);
					actualType->Release();
					actualType = NULL;
				}
			}
		}

		if (pWfx && pWfx->wFormatTag == WAVE_FORMAT_PCM && pWfx->wBitsPerSample == 16) {
			hWaveEvt = CreateEventA(NULL, FALSE, FALSE, NULL);
			if (hWaveEvt && waveOutOpen(&hWave, WAVE_MAPPER, pWfx, (DWORD_PTR)hWaveEvt, 0, CALLBACK_EVENT) == MMSYSERR_NOERROR) {
				EnterCriticalSection(&st->cs);
				st->hWaveOut = hWave;
				if (InterlockedCompareExchange(&st->pausado, 0, 0) != 0) {
					waveOutPause(hWave);
				}
				LeaveCriticalSection(&st->cs);

				const int NUM_BUFS = 4;
				WAVEHDR hdrs[NUM_BUFS];
				std::vector<BYTE> bufs[NUM_BUFS];
				for (int i = 0; i < NUM_BUFS; i++) {
					std::memset(&hdrs[i], 0, sizeof(WAVEHDR));
					hdrs[i].dwFlags = WHDR_DONE;
				}

				int slot = 0;
				bool finArchivo = false;
				while (InterlockedCompareExchange(&st->detener, 0, 0) == 0 && !finArchivo) {
					if (InterlockedCompareExchange(&st->pausado, 0, 0) != 0) {
						Sleep(20);
						continue;
					}

					if (!(hdrs[slot].dwFlags & WHDR_DONE)) {
						WaitForSingleObject(hWaveEvt, 30);
						continue;
					}

					if (hdrs[slot].dwFlags & WHDR_PREPARED) {
						waveOutUnprepareHeader(hWave, &hdrs[slot], sizeof(WAVEHDR));
					}

					DWORD streamIdx = 0, flags = 0;
					LONGLONG ts = 0;
					IMFSample* sample = NULL;
					HRESULT hrRead = reader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &streamIdx, &flags, &ts, &sample);

					if (FAILED(hrRead)) {
						if (sample) sample->Release();
						break;
					}

					if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
						if (sample) sample->Release();
						if (st->repetir) {
							PROPVARIANT pos;
							PropVariantInit(&pos);
							pos.vt = VT_I8;
							pos.hVal.QuadPart = 0;
							reader->SetCurrentPosition(GUID_NULL, pos);
							PropVariantClear(&pos);
							continue;
						}
						finArchivo = true;
						break;
					}

					if (!sample) continue;

					IMFMediaBuffer* buf = NULL;
					if (SUCCEEDED(sample->ConvertToContiguousBuffer(&buf)) && buf) {
						BYTE* ptr = NULL;
						DWORD len = 0;
						if (SUCCEEDED(buf->Lock(&ptr, NULL, &len)) && len > 0) {
							bufs[slot].resize(len);
							std::memcpy(&bufs[slot][0], ptr, len);
							short* s = (short*)&bufs[slot][0];
							DWORD count = len / 2;
							float vol = st->volumen;
							for (DWORD i = 0; i < count; i++) {
								int scaled = (int)(s[i] * vol);
								if (scaled > 32767) scaled = 32767;
								else if (scaled < -32768) scaled = -32768;
								s[i] = (short)scaled;
							}
							std::memset(&hdrs[slot], 0, sizeof(WAVEHDR));
							hdrs[slot].lpData = (LPSTR)&bufs[slot][0];
							hdrs[slot].dwBufferLength = len;
							if (waveOutPrepareHeader(hWave, &hdrs[slot], sizeof(WAVEHDR)) == MMSYSERR_NOERROR) {
								waveOutWrite(hWave, &hdrs[slot], sizeof(WAVEHDR));
								slot = (slot + 1) % NUM_BUFS;
							}
							buf->Unlock();
						}
						buf->Release();
					}
					sample->Release();
				}

				if (InterlockedCompareExchange(&st->detener, 0, 0) == 0) {
					for (int waitIter = 0; waitIter < 80; waitIter++) {
						bool pendientes = false;
						for (int i = 0; i < NUM_BUFS; i++) {
							if ((hdrs[i].dwFlags & WHDR_PREPARED) && !(hdrs[i].dwFlags & WHDR_DONE)) {
								pendientes = true;
								break;
							}
						}
						if (!pendientes || InterlockedCompareExchange(&st->detener, 0, 0) != 0) break;
						WaitForSingleObject(hWaveEvt, 25);
					}
				}

				EnterCriticalSection(&st->cs);
				st->hWaveOut = NULL;
				LeaveCriticalSection(&st->cs);

				waveOutReset(hWave);
				for (int i = 0; i < NUM_BUFS; i++) {
					if (hdrs[i].dwFlags & WHDR_PREPARED) {
						waveOutUnprepareHeader(hWave, &hdrs[i], sizeof(WAVEHDR));
					}
				}
				waveOutClose(hWave);
			}
			if (hWaveEvt) CloseHandle(hWaveEvt);
		}

		if (pWfx) CoTaskMemFree(pWfx);
		if (reader) reader->Release();
		if (SUCCEEDED(hrMf)) MFShutdown();
		if (SUCCEEDED(hrCom)) CoUninitialize();
		return 0;
	}
}

void* AudioMotor_Iniciar(const char* ruta, bool repetir, float volumen) {
	if (!ruta || !ProbarRutaLegible(ruta)) return NULL;
	EstadoStream* st = new EstadoStream();
	st->ruta = ruta;
	st->repetir = repetir;
	st->volumen = volumen;
	st->hThread = CreateThread(NULL, 0, HiloStreamMusica, st, 0, NULL);
	if (!st->hThread) {
		delete st;
		return NULL;
	}
	return st;
}

void AudioMotor_Pausar(void* handle) {
	EstadoStream* st = (EstadoStream*)handle;
	if (!st) return;
	InterlockedExchange(&st->pausado, 1);
	EnterCriticalSection(&st->cs);
	if (st->hWaveOut) waveOutPause(st->hWaveOut);
	LeaveCriticalSection(&st->cs);
}

void AudioMotor_Reanudar(void* handle) {
	EstadoStream* st = (EstadoStream*)handle;
	if (!st) return;
	InterlockedExchange(&st->pausado, 0);
	EnterCriticalSection(&st->cs);
	if (st->hWaveOut) waveOutRestart(st->hWaveOut);
	LeaveCriticalSection(&st->cs);
}

void AudioMotor_Detener(void* handle) {
	EstadoStream* st = (EstadoStream*)handle;
	if (!st) return;
	InterlockedExchange(&st->detener, 1);
	InterlockedExchange(&st->pausado, 0);
	EnterCriticalSection(&st->cs);
	if (st->hWaveOut) waveOutRestart(st->hWaveOut);
	LeaveCriticalSection(&st->cs);
	if (st->hThread) {
		WaitForSingleObject(st->hThread, 2500);
		CloseHandle(st->hThread);
		st->hThread = NULL;
	}
	delete st;
}
#else
void* AudioMotor_Iniciar(const char* ruta, bool repetir, float volumen) { (void)ruta; (void)repetir; (void)volumen; return NULL; }
void AudioMotor_Pausar(void* handle) { (void)handle; }
void AudioMotor_Reanudar(void* handle) { (void)handle; }
void AudioMotor_Detener(void* handle) { (void)handle; }
#endif
