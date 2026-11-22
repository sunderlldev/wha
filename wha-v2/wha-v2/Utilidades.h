#pragma once

#include <string>

using namespace System;

static String^ ToManaged(const std::string& texto) {
	if (texto.empty()) return "";
	array<Byte>^ bytes = gcnew array<Byte>((int)texto.size());
	for (int i = 0; i < (int)texto.size(); i++) {
		bytes[i] = (Byte)(unsigned char)texto[i];
	}
	return System::Text::Encoding::UTF8->GetString(bytes);
}

static String^ ToManaged(const char* texto) {
	if (texto == nullptr) return "";
	return ToManaged(std::string(texto));
}

static int ClampEntero(int valor, int minimo, int maximo) {
	if (valor < minimo) return minimo;
	if (valor > maximo) return maximo;
	return valor;
}

static float ClampFlotante(float valor, float minimo, float maximo) {
	if (valor < minimo) return minimo;
	if (valor > maximo) return maximo;
	return valor;
}
