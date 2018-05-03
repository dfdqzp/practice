#include "StringEx.h"
#include <iostream>

int main()
{
	// test case 1 - operator ==
	{
		String S1("aabbc1x!+*&^%");
		String S2("aabbc1x!+*&^%");
		S1 == S2 ? std::cout << "case 1 Pass.\n" : std::cout << "case 1 Fail.\n";
	}

	// test case 2 - operator =
	{
		String S1("aabbc1x!+*&^%");
		String S2 = S1;
		S1 == S2 ? std::cout << "case 2 Pass.\n" : std::cout << "case 2 Fail.\n";
	}

	// test case 3 - operator copy
	{
		String S1("aabbc1x!+*&^%");
		String S2(S1);
		S1 == S2 ? std::cout << "case 3 Pass.\n" : std::cout << "case 3 Fail.\n";
	}

	// test case 4 - operator +
	{
		String S1("123456789");
		String S2("123456789");
		String S3 = S1 + S2;
		S3 == String("123456789123456789") ? std::cout << "case 4 Pass.\n" : std::cout << "case 4 Fail.\n";
	}

	// test case 5 - operator +=
	{
		String S1("123456789");
		String S2("123456789");
		S2 += S1;
		S2 == String("123456789123456789") ? std::cout << "case 5 Pass.\n" : std::cout << "case 5 Fail.\n";
	}

    return 0;
}

// constructor
String::String(const char * Str)
{
	if (!Str)
	{
		m_len = 0;
		m_data = new char[1], assert(nullptr != m_data);
		*m_data = '\0';
	}
	else
	{
		m_len = strlen(Str);
		m_data = new char[m_len + 1], assert(nullptr != m_data);
		strncpy_s(m_data, m_len + 1, Str, m_len);
		m_data[m_len] = '\0';
	}
}

// constructor
String::String(const String & Str)
{
	m_len = Str.size();
	m_data = new char[m_len + 1], assert(nullptr != m_data);
	strncpy_s(m_data, m_len + 1, Str.c_str(), m_len);
	m_data[m_len] = '\0';
}

// operator
String & String::operator = (const String & Str)
{
	if (this == &Str)
		return *this;

	delete [] m_data;
	m_len = Str.m_len;
	m_data = new char[m_len + 1], assert(nullptr != m_data);
	strncpy_s(m_data, m_len + 1, Str.c_str(), m_len);

	return *this;
}

// operator
bool String::operator == (const String & Str) const
{
	return this->m_len != Str.m_len || 0 == strcmp(this->m_data, Str.m_data);
}

// operator
bool String::operator != (const String & Str) const
{
	return !(*this == Str);
}

// operator
String String::operator + (const String & Str) const
{
	String SS;
	SS.m_len = m_len + Str.size();
	SS.m_data = new char[SS.m_len + 1], assert(nullptr != m_data);
	strncpy_s(SS.m_data, SS.m_len + 1, this->c_str(), this->m_len);
	strcat_s(SS.m_data, SS.m_len + 1, Str.m_data);
	SS.m_data[SS.m_len] = '\0';

	return SS;
}

// operator
String & String::operator += (const String & Str)
{
	m_len += Str.m_len;
	char *tmp = new char[m_len + 1];
	assert(nullptr != tmp);

	strncpy_s(tmp, m_len + 1, m_data, m_len);
	strcat_s(tmp, m_len + 1, Str.m_data);
	tmp[m_len] = '\0';

	delete [] m_data;
	m_data = tmp;
	return *this;
}

//destructor
String::~String()
{
	m_len = 0;
	delete [] m_data;
}
