#pragma once
//класс для вывода эксепшна в юникоде
class uexception {
public:
	wchar_t buf[1024];
	// Лишний вызов NEW может все испортить, например
	// у нас память или стек кончился
public:
	uexception(const wchar_t* str) {
		wcsncpy_s(this->buf, sizeof(this->buf), str,
			sizeof(this->buf));
	}
	const wchar_t* what() { return this->buf; }
};