#pragma once
//����� ��� ������ �������� � �������
class uexception {
public:
	wchar_t buf[1024];
	// ������ ����� NEW ����� ��� ���������, ��������
	// � ��� ������ ��� ���� ��������
public:
	uexception(const wchar_t* str) {
		wcsncpy_s(this->buf, sizeof(this->buf), str,
			sizeof(this->buf));
	}
	const wchar_t* what() { return this->buf; }
};