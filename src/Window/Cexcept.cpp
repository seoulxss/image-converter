#include "Cexcept.h"

Cexcept::Cexcept(unsigned line, const char* file) noexcept : line(line), file(file)
{
}

const char* Cexcept::what() const noexcept
{
	std::stringstream oss;
	oss << GetType() << std::endl << GetOrigString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Cexcept::GetType() const noexcept
{
	return "CException";
}

unsigned Cexcept::GetLine() const noexcept
{
	return line;
}

const std::string& Cexcept::GetFile() const noexcept
{
	return file;
}

std::string Cexcept::GetOrigString() const noexcept
{

	std::ostringstream oss;
	oss << "[File] " << file << std::endl << "[Line] " << line;
	return oss.str();
}


