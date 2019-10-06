/**
 * @class Logger
 * 
 * Singleton class to handle logging.Example usage:
 * 
 *    log(Logger::LogLevel::ERROR, "module", "Message with param1 & and param2 & and param3 &.", "test", 4, 7.5);
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    June 15, 2013
 */

#ifndef LOGGER_H
#define LOGGER_H

/*---------  System Includes  --------------*/
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace dw {

/*---------  Class Declaration -------------*/

class Logger
{
public:

   /*---------  Public Enums      ---------------*/

   /**
    * @enum LogLevel
    */
    enum class LogLevel : char {NONE, ERROR, INFO, DEBUG};

   /*---------  Public Methods  ---------------*/
   
   /**
    * Returns an instance of the logger.
    * 
    * @return dw::Logger&
    */
   static Logger& instance();

   /**
    * Log a message with a module name and a string message.
    * 
    * @param level   The log level a which this message is logged. 
    * @param module  The module which this message applies to.
    * @param message The log message.
    * 
    * @return void
    */
   void log(const LogLevel level, 
            const std::string& module, 
            const std::string& message) const;


   /**
    * Log a message with a module name and string message with 1 additional parameters.
    * Parameters may be standard C++ types such as int, float, char, string. C style strings
    * may need to be cast to std::string.
    * Example usage:
    * log(Logger::LogLevel::ERROR, "module", "Message with param1 &.", "test");
    * 
    * @param level   The log level a which this message is logged. 
    * @param module  The module which this message applies to.
    * @param message The log message.
    * @param param   An extra parameter to add to the message
    * 
    * @return void
    */
   template <typename T>
   void log(LogLevel level, 
            const std::string& module, 
            const std::string& message,
            const T& param) const
   {
      std::stringstream ss;
      std::vector<std::string> substrings = splitStringAt(message, '&');
      int numSubstrings = substrings.size();

      if(numSubstrings != 2) {
         log(LogLevel::ERROR, "Logger", "Error - invalid num parameters for log message &.", message);
      }
      else {
         ss << substrings[0] << param << substrings[1];
         log(level, module, ss.str());
      }
   }

   /**
    * Log a message with a module name and string message with 2 additional parameters.
    * Parameters may be standard C++ types such as int, float, char, string. C style strings
    * may need to be cast to std::string.
    * Example usage:
    * log(Logger::LogLevel::ERROR, "module", "Message with param1 & and param2 &.", "test", 4);
    * 
    * @param level   The log level a which this message is logged. 
    * @param module  The module which this message applies to.
    * @param message The log message.
    * @param param1   An extra parameter to add to the message
    * @param param2   An extra parameter to add to the message
    * 
    * @return void
    */
   template <typename T>
   void log(LogLevel level, 
            const std::string& module, 
            const std::string& message,
            const T& param1,
            const T& param2) const
   {
      std::stringstream ss;
      std::vector<std::string> substrings = splitStringAt(message, '&');
      int numSubstrings = substrings.size();

      if(numSubstrings > 3) {
         log(LogLevel::ERROR, "Logger", "Error - invalid num parameters for log message &.", message);
      }
      else {
         ss << substrings[0] << param1 << substrings[1] << param2 << substrings[2];
         log(level, module, ss.str());
      }
   }

   /**
    * Log a message with a module name and string message with 3 additional parameters.
    * Parameters may be standard C++ types such as int, float, char, string. C style strings
    * may need to be cast to std::string.
    * Example usage:
    * log(Logger::LogLevel::ERROR, "module", "Message with param1 & and param2 & and param3 &.", "test", 4, 7.5);
    * 
    * @param level   The log level a which this message is logged. 
    * @param module  The module which this message applies to.
    * @param message The log message.
    * @param param1   An extra parameter to add to the message
    * @param param2   An extra parameter to add to the message
    * 
    * @return void
    */
   template <typename T>
   void log(LogLevel level, 
            const std::string& module, 
            const std::string& message,
            const T& param1,
            const T& param2,
            const T& param3) const
   {
      std::stringstream ss;
      std::vector<std::string> substrings = splitStringAt(message, '&');
      int numSubstrings = substrings.size();

      if(numSubstrings > 4) {
         log(LogLevel::ERROR, "Logger", "Error - invalid num parameters for log message &.", message);
      }
      else {
         ss << substrings[0] << param1 << substrings[1] << param2 << substrings[2] << param3 << substrings[3];
         log(level, module, ss.str());
      }
   }

   /**
    * @brief Set the log level.
    * 
    * @param level The level at which the logger will log messages.
    * @return void
    * 
    * @see LogLevel for valid values.
    */
   void setLogLevel(LogLevel level)
   {
      mLogLevel = level;
   }

private:

   /*---------  Private Members    --------------*/
   static Logger mInstance;
   bool mIsFileOpen = false;
   LogLevel mLogLevel = LogLevel::ERROR;
   mutable std::ofstream mOutputStream;

   /*---------  Private Methods    ---------------*/
   Logger();
   Logger(const Logger& other) = delete;
   virtual ~Logger();

   /**
    * Determine if this message is at the correct level to write to the log
    * file by comparing the level to the level the logger is set to.
    * DEBUG - written only if the logger is set to DEBUG
    * INFO  - written if the logger is set to INFO or DEBUG
    * ERROR - written if the logger is set to INFO, DEBUG or ERROR
    * NONE  - newver written
    * 
    * @param levelIn the log level to compare with the current logger setting
    * @return bool
    */
   bool isMessageAtLevelToWrite(LogLevel levelIn) const;
   bool isMessageAtLevelToWrite(const std::string& levelIn) const;
   
   std::vector< std::string > splitStringAt(const std::string& stringIn, const char splitTokenIn) const;
   
   LogLevel stringToLogLevel(const std::string& levelIn) const;
};

} // End namespace dw

#endif // LOGGER_H
