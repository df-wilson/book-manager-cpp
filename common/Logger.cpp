
/*---------  Program Includes  ---------------*/
#include "Logger.h"
#include "ConfigReader.h"

/*---------  System Includes  --------------*/
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace dw {

Logger Logger::mInstance;

///////////////////////////////////////////////////////////////////////////////
// Function: Constructor
// Description: None
///////////////////////////////////////////////////////////////////////////////

Logger::Logger()
{
	mOutputStream.open("logfile.log", std::ofstream::out | std::ofstream::app);
	if(mOutputStream.good())
	{
		try {
			string logLevel = ConfigReader::getInstance().getConfig(ConfigReader::Config::LOG_LEVEL); 
			mLogLevel = stringToLogLevel(logLevel);
               log(LogLevel::INFO, "Logger", "Logger initialized at level &.", logLevel);
		}
		catch(out_of_range& e) {
			e.what();
			cerr << "Logger: " << e.what() ;
			abort();
		}
		
		mIsFileOpen = true;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Function: Destructor
// Description: None
///////////////////////////////////////////////////////////////////////////////

Logger::~Logger()
{
	if(mIsFileOpen) mOutputStream.close();
}


///////////////////////////////////////////////////////////////////////////////
// Public Function: Instance
// Description: Get an instance of the logger.
///////////////////////////////////////////////////////////////////////////////

Logger& Logger::instance()
{
	
	return mInstance;
}


///////////////////////////////////////////////////////////////////////////////
// Public Function: log
// Description: Log a simple string message.
///////////////////////////////////////////////////////////////////////////////


void Logger::log(Logger::LogLevel level, 
                 const std::string& module, 
                 const std::string& message) const
{
	const size_t MAX_BUFFER_SIZE = 180;
	system_clock::time_point today = system_clock::now();
	time_t temp = system_clock::to_time_t (today);
	tm* localTimestamp = localtime(&temp);
   char buffer[MAX_BUFFER_SIZE+1] = {0};
	//strftime(buffer, sizeof(MAX_BUFFER_SIZE), "%F %T", localTimestamp);
	//strftime(buffer, sizeof(buffer), "%Y-%m-%D %H:%M:%S", localTimestamp);
	strftime(buffer, sizeof(buffer), "%F %T", localTimestamp);
	buffer[MAX_BUFFER_SIZE] = 0;
	
	// TODO: Use this when supported
	//mOutputStream << put_time(localTimestamp, "%F %T") << " [" << module << "] " << message << endl;

	if(true == isMessageAtLevelToWrite(level))
	{
		mOutputStream << buffer << " [" << module << "] " << message << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Private Function: isMessageAtLevelToWrite
// Description: Determines if the input log level should be printed based on
//              the currently set log level of the logger.
///////////////////////////////////////////////////////////////////////////////

bool Logger::isMessageAtLevelToWrite(Logger::LogLevel levelIn) const
{
	bool isCorrectLevel = false;
	
	switch(levelIn)
	{
		case LogLevel::DEBUG:
			if(mLogLevel == LogLevel::DEBUG ||
                     mLogLevel == LogLevel::ERROR)
                  {
                     isCorrectLevel = true;
                  }
			break;
			
		case LogLevel::ERROR:
			if(mLogLevel != LogLevel::NONE) {
                     isCorrectLevel = true;
                  }
			break;
			
		case LogLevel::INFO:
			if(mLogLevel == LogLevel::ERROR ||
                     mLogLevel == LogLevel::DEBUG ||
			   mLogLevel == LogLevel::INFO)
			{
				isCorrectLevel = true;
			}
			break;
			
		case LogLevel::NONE:
			isCorrectLevel = false;
			break;
			
		default:
			isCorrectLevel = false;
			log(LogLevel::ERROR, "Logger", "Invalid log level in isMessageAtLevelToWrite");
			break;
	}
	
	return isCorrectLevel;
}

///////////////////////////////////////////////////////////////////////////////
// Private Function:isMessageAtLevelToWrite 
// Description: Return true if the passed in string value represents a log level
//              configured to write to the log file.
///////////////////////////////////////////////////////////////////////////////

bool Logger::isMessageAtLevelToWrite(const std::string& levelIn) const
{
	LogLevel level = stringToLogLevel(levelIn);
	return isMessageAtLevelToWrite(level);
}

///////////////////////////////////////////////////////////////////////////////
// Private Function: stringToLogLevel
// Description: Convert a string value to a log level enum
///////////////////////////////////////////////////////////////////////////////

Logger::LogLevel Logger::stringToLogLevel(const std::string& levelIn) const
{
	Logger::LogLevel level = LogLevel::NONE;
	
	if(levelIn == "DEBUG") {
		level = LogLevel::DEBUG;
	}	
	else if(levelIn == "ERROR") {
		level = LogLevel::ERROR;
	}
	else if(levelIn == "INFO") {
		level = LogLevel::INFO;
	}
	else if(levelIn == "NONE") {
		level = LogLevel::NONE;
	}
	else {
		level = LogLevel::NONE;
	}
	
	return level;
}


///////////////////////////////////////////////////////////////////////////////
// Private Function: splitStringAt
// Description: 
///////////////////////////////////////////////////////////////////////////////
std::vector< std::string > Logger::splitStringAt(const std::string& stringIn, const char splitTokenIn) const
{
	vector<string> tokens;
	size_t pos      = 0;
	size_t startPos = 0;
	string subString;
	while(pos != string::npos) {
		pos = stringIn.find(splitTokenIn, startPos);
		if(pos == 0) {
			subString = "";
		}
		else {
			subString = stringIn.substr(startPos, (pos - startPos));
		}

		tokens.push_back(subString);
		startPos = pos+1;
	}
	
	return tokens;
}



} // End namespace dw
