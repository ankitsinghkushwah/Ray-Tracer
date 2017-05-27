#ifndef INC_LOGGER_H
#define INC_LOGGER_H

//C++ headers
#include<cstdio>
#include<cstdarg>
#include<memory>

class logger
{
public:
	logger(const char* pHeader);
	~logger();
	void init(const char* pHeader);
	void log(const char* pFormat,...);
private:
	FILE* mFile;
};


#endif