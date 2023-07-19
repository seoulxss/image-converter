#pragma once


#include <exception>
#include <string>
#include <sstream>

class Cexcept : public std::exception
{
public:
	Cexcept(unsigned int line, const char* file) noexcept;

	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	unsigned int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOrigString() const noexcept;

private:
	unsigned int line;
	std::string file;

protected:
	mutable std::string whatBuffer;

};
