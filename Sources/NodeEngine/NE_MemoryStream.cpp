#include "NE_MemoryStream.hpp"
#include "NE_Debug.hpp"

namespace NE
{

#ifdef _WIN32
#define SIZEOFWCHAR 2
#else
#define SIZEOFWCHAR 4
#endif

static_assert (sizeof (bool) == 1, "invalid size for bool");
static_assert (sizeof (char) == 1, "invalid size for char");
static_assert (sizeof (wchar_t) == SIZEOFWCHAR, "invalid size for wchar_t");
static_assert (sizeof (char16_t) == 2, "invalid size for double");
static_assert (sizeof (char32_t) == 4, "invalid size for double");
static_assert (sizeof (double) == 8, "invalid size for double");
static_assert (sizeof (unsigned char) == 1, "invalid size for unsigned char");
static_assert (sizeof (short) == 2, "invalid size for short");
static_assert (sizeof (size_t) == 8, "invalid size for size_t");
static_assert (sizeof (int) == 4, "invalid size for int");
static_assert (sizeof (float) == 4, "invalid size for float");
static_assert (sizeof (double) == 8, "invalid size for double");

template <typename StringType, typename CharType>
static Stream::Status ReadString (MemoryInputStream& stream, StringType& val)
{
	size_t count = 0;
	if (stream.Read (count) != Stream::Status::NoError) {
		return stream.GetStatus ();
	}

	CharType* str = new CharType[count + 1];
	stream.Read ((char*) str, count * sizeof (CharType));
	str[count] = 0;

	val = str;
	delete[] str;

	return stream.GetStatus ();
}

template <typename StringType, typename CharType>
static Stream::Status WriteString (MemoryOutputStream& stream, const StringType& val)
{
	stream.Write (val.length ());
	stream.Write ((const char*) val.c_str (), val.length () * sizeof (CharType));
	return stream.GetStatus ();
}

Stream::Status ReadString (MemoryInputStream& stream, std::string& val)
{
	return ReadString<std::string, char> (stream, val);
}

Stream::Status ReadString (MemoryInputStream& stream, std::wstring& val)
{
	std::u32string u32Val;
	Stream::Status status = ReadString<std::u32string, char32_t> (stream, u32Val);
	val.clear ();
	for (char32_t ch : u32Val) {
		val += (wchar_t) ch;
	}
	return status;
}

static Stream::Status WriteString (MemoryOutputStream& stream, const std::string& val)
{
	return WriteString<std::string, char> (stream, val);
}

static Stream::Status WriteString (MemoryOutputStream& stream, const std::wstring& val)
{
	std::u32string u32Val (val.begin (), val.end ());
	return WriteString<std::u32string, char32_t> (stream, u32Val);
}

MemoryInputStream::MemoryInputStream (const std::vector<char>& buffer) :
	InputStream (),
	buffer (buffer),
	position (0)
{
	
}

MemoryInputStream::~MemoryInputStream ()
{
	
}

Stream::Status MemoryInputStream::Read (bool& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (char& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (unsigned char& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (short& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (size_t& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (int& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (float& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (double& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (std::string& val)
{
	return ReadString (*this, val);
}

Stream::Status MemoryInputStream::Read (std::wstring& val)
{
	return ReadString (*this, val);
}

void MemoryInputStream::Read (char* dest, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	if (DBGERROR (position + size > buffer.size ())) {
		status = Status::Error;
		return;
	}
	std::copy (buffer.begin () + position, buffer.begin () + position + size, dest);
	position += size;
}

MemoryOutputStream::MemoryOutputStream () :
	OutputStream (),
	buffer ()
{
	
}

MemoryOutputStream::~MemoryOutputStream ()
{
	
}

const std::vector<char>& MemoryOutputStream::GetBuffer () const
{
	return buffer;
}

Stream::Status MemoryOutputStream::Write (const bool& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const char& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const unsigned char& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const short& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const size_t& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const int& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const float& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const double& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const std::string& val)
{
	return WriteString (*this, val);
}

Stream::Status MemoryOutputStream::Write (const std::wstring& val)
{
	return WriteString (*this, val);
}

void MemoryOutputStream::Write (const char* source, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	buffer.insert (buffer.end (), source, source + size);
}

}
