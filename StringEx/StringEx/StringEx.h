#pragma once
#include <istream>
#include <ostream>
#include <assert.h>

class String
{
	public:
		String(const char* Str = nullptr);
		String(const String &Str);

		String& operator=(const String &Str);
		bool operator==(const String &Str) const;
		bool operator != (const String &Str) const;

		String operator+(const String &Str) const;
		String& operator+=(const String &Str);

		char& operator[](size_t n) const { return m_data[n > m_len - 1 ? m_len - 1 : n]; };

		int find(const String & src, int pos) { return 0;/*TBD*/ };
		size_t replacaeAll(const String & src, const String & dest) { return 0; /*TBD*/ };

		size_t size() const { return m_len; }
		const char* c_str() const { return m_data; }
		
		~String();
		
	private:
		char* m_data;
		size_t m_len;
};