#pragma once
#include <iostream>
#include <cstring>

class Exception
{
private:
	char* file; //��� �����
	char* func; //��� �������
	int line;   //����� ������
	char* desc; //��������� ��������

public:
	Exception(const char* _file, const char* _func, int _line, const char* _desc);
	Exception(const Exception& e);
	~Exception();

	void print() const;
	friend std::ostream& operator<<(std::ostream& os, const Exception& e)
	{
		os << "File: " << e.file << '\n';
		os << "Function: " << e.func << '\n';
		os << "Line: " << e.line << '\n';
		os << "Message: " << e.desc << '\n';
		return os;
	}
};