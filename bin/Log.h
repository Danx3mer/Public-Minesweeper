#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>


#define LOG_ENABLED		0x0001
#define LOG_TO_FILE		0x0010
#define LOG_TO_CONSOLE	0x0100
#define LOG_TIME_STAMP	0x1000


class LOG
{
public:
				LOG();
			   ~LOG();
				LOG(int _BitwiseParameter);
				LOG(std::string _FileName, int _BitwiseParameter);
	void		Error(std::string _Message);
	void		Event(std::string _Message);

private:
	std::string GetTimeStamp(int _Parameter);
	void		LogToConsole(int _LogType, std::string _Message);
	void		LogToFile(int _LogType, std::string _Message);

private:
	#define		LOG_ERROR		1
	#define		LOG_EVENT		2
	#define		TIME_STAMP_FULL 3
	#define		TIME_STAMP_DATE 4

private:
	bool		Enabled;
	bool		FileLog;
	bool		ConsoleLog;
	bool		TimeStamp;
	std::string FileName;
};