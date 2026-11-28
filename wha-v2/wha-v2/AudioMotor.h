#pragma once

void* AudioMotor_Iniciar(const char* ruta, bool repetir, float volumen);
void AudioMotor_Pausar(void* handle);
void AudioMotor_Reanudar(void* handle);
void AudioMotor_Detener(void* handle);
