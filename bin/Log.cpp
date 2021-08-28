#define _CRT_SECURE_NO_WARNINGS
#include "Log.h"

LOG::LOG(): Enabled(false), ConsoleLog(false), FileLog(false), TimeStamp(false)
{
}

LOG::~LOG()
{
	Event("\n------------------------------------------------------\nLOG FINISHED");
}

LOG::LOG(int _BitwiseParameter)
{
	Enabled = false;
	ConsoleLog = false;
	FileLog = false;
	TimeStamp = false;
	FileName = "log_";

	if (_BitwiseParameter & LOG_ENABLED) Enabled = true;
	if (_BitwiseParameter & LOG_TO_FILE) FileLog = true;
	if (_BitwiseParameter & LOG_TO_CONSOLE) ConsoleLog = true;
	if (_BitwiseParameter & LOG_TIME_STAMP) TimeStamp = true;

	Event("LOG STARTED\n------------------------------------------------------");
}

LOG::LOG(std::string _FileName, int _BitwiseParameter)
{
	Enabled = false;
	ConsoleLog = false;
	FileLog = false;
	TimeStamp = false;
	FileName = _FileName;

	if (_BitwiseParameter & LOG_ENABLED) Enabled = true;
	if (_BitwiseParameter & LOG_TO_FILE) FileLog = true;
	if (_BitwiseParameter & LOG_TO_CONSOLE) ConsoleLog = true;
	if (_BitwiseParameter & LOG_TIME_STAMP) TimeStamp = true;

	Event("LOG STARTED\n------------------------------------------------------");
}

void LOG::Error(std::string _Message)
{
	if (Enabled)
	{
		if (ConsoleLog) LogToConsole(LOG_ERROR, _Message);
		if (FileLog) LogToFile(LOG_ERROR, _Message);
	}
}

void LOG::Event(std::string _Message)
{
	if (Enabled)
	{
		if (ConsoleLog) LogToConsole(LOG_EVENT, _Message);
		if (FileLog) LogToFile(LOG_EVENT, _Message);
	}
}

std::string LOG::GetTimeStamp(int _Parameter)
{
	switch (_Parameter)
	{
	case TIME_STAMP_FULL:
	{
	if (TimeStamp)
	{
		std::string TimeStampString{};
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		if (timeinfo->tm_mon + 1 < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_mon + 1);
		TimeStampString += '/';
		if (timeinfo->tm_mday < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_mday);
		TimeStampString += '/';
		TimeStampString += std::to_string(timeinfo->tm_year+1900);
		TimeStampString += " | ";
		if (timeinfo->tm_hour < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_hour);
		TimeStampString += " : ";
		if (timeinfo->tm_min < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_min);
		TimeStampString += " : ";
		if (timeinfo->tm_sec < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_sec);
		TimeStampString += " : ";
		auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		auto sec_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if(millisec_since_epoch - sec_since_epoch * 1000 < 10) TimeStampString += "0";
		if(millisec_since_epoch - sec_since_epoch * 1000 < 100) TimeStampString += '0';
		TimeStampString += std::to_string(millisec_since_epoch - sec_since_epoch * 1000);
		TimeStampString += " | ";

		return TimeStampString;
	}
	}
	break;
	case TIME_STAMP_DATE:
	{
		std::string TimeStampString{};
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		TimeStampString += std::to_string(timeinfo->tm_year + 1900);
		TimeStampString += '-';
		if (timeinfo->tm_mon + 1 < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_mon + 1);
		TimeStampString += '-';
		if (timeinfo->tm_mday < 10) TimeStampString += '0';
		TimeStampString += std::to_string(timeinfo->tm_mday);

		return TimeStampString;
	}
	break;
	}
	return "";
}

void LOG::LogToConsole(int _LogType, std::string _Message)
{
	switch (_LogType)
	{
	case LOG_ERROR:
	{
		std::cout << GetTimeStamp(TIME_STAMP_FULL);
		std::cout << "\033[31mERROR : \033[0m" << _Message << std::endl;
	}
	break;
	case LOG_EVENT:
	{
		std::cout << GetTimeStamp(TIME_STAMP_FULL);
		std::cout << "EVENT : " << _Message << std::endl;
	}
	break;
	}
}

void LOG::LogToFile(int _LogType, std::string _Message)
{
	std::string _FilePath = "Log_Backups/" + FileName + GetTimeStamp(TIME_STAMP_DATE) +".txt";
	std::ofstream OutputFile(_FilePath.c_str(), std::ios_base::out | std::ios_base::app);

	switch (_LogType)
	{
	case LOG_ERROR:
	{
		OutputFile << GetTimeStamp(TIME_STAMP_FULL) << "ERROR : " << _Message << std::endl;
	}
	break;

	case LOG_EVENT:
	{
		OutputFile << GetTimeStamp(TIME_STAMP_FULL) << "EVENT : " << _Message << std::endl;
	}
	break;
	}
	OutputFile.close();
}