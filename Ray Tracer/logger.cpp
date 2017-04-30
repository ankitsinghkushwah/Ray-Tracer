//Declaration header
#include"logger.h"

//C++ headers
#include<limits>



	logger::logger(const char* pHeader)
		: 
		mFile(nullptr)
	{
		init(pHeader);
	}


	logger::~logger()
	{
		fclose(mFile);
	}


	void logger::init(const char* pHeader)
	{
		mFile = fopen("Logs.txt","w");

		if (!mFile){
			printf("Failed to create a log mFile");
			return;
		}

		fprintf(mFile,pHeader);
		fprintf(mFile,"\n");
	}


	void logger::log(const char* pFormat,...)
	{
		va_list list;
		va_start(list,pFormat);
		vfprintf(mFile,pFormat,list);
		fprintf(mFile, "\n");
		va_end(list);
	}
