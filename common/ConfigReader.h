/**
 * @class ConfigReader
 * 
 * This is a singleton class to manage configurations. All configurations are 
 * read in during initialization and cached. 
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    June 15, 2013
 */
#ifndef CONFIGREADER_H
#define CONFIGREADER_H

/*---------  System Includes  --------------*/
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

namespace dw {
   
/*---------  Class Declaration -------------*/
/**
 * This is a singleton class to manage configurations. All configurations are 
 * read in during initialization and cached. 
 * Usage:
 * try
 *    {
 *       std::string configValue = ConfigReader::getInstance().getConfig(ConfigReader::Config::LOG_LEVEL);
 *    }
 *    catch (out_of_range e)
 *    {
 *       // ....
 *    }
 * 
 */
class ConfigReader
{
public:
   
   /**
    * The available configurations
    * 
    */
   enum class Config : char 
   {
      NONE,
      LOG_LEVEL,
      DB_PATH
   };
   
   /*---------  Public Functions  ---------------*/
   
   /**
    * Get the value of a configuration.
    * 
    * @param config [in] enum value of the configuration value to get.
    * @return string value of the configuration.
    * @throws std::out_of_range
    */
    std::string 
    getConfig(dw::ConfigReader::Config config) const;


   /**
    * Get an instance of the configuration reader.
    * 
    * @return An instance of ConfigReader
    */
    static ConfigReader& getInstance();

private:
   /*---------  Private Data    -----------------*/
   
    std::string mLogLevel;
    static ConfigReader mInstance;
    std::map<std::string, std::string> mConfigCache;
    std::map<Config, std::string> mConfigCache1;


   /*---------  Private Functions ---------------*/
   
    ConfigReader();
    ConfigReader(const ConfigReader& other) = delete;
    virtual ~ConfigReader();
    ConfigReader& operator=(const ConfigReader& other) = delete;
    bool operator==(const ConfigReader& other) = delete;

    
   /**
    * Converts a configuration enum to its string representation
    * 
    * @param config [in] The configuration enum to convert to a string.
    * @return The string representation of the enum.
    */
   std::string configEnumToString(Config config) const;

   
  /**
   * Converts the passed in string to its enum representation. If the string does not match
   * an enum value then Config::NONE is returned.
   * 
   * @param configString [in] The configuration string to convert to its enum representation.
   * @return The configuration enum value.
   */
   Config configStringToEnum(const std::string& configString) const;

   
  /**
   * Parses a string on = to find the configuration and it's value. A line
   * beginning with # is considered a comment.
   * Examples:
   * LogLevel=DEBUG
   * # This is a comment.
   * 
   * @param configLine A string in the form config=value or #comment
   * @return void
   */
   void storeConfig(const std::string& configLine);
   
   
   /**
    * Read in all configurations and cache the values.
    * 
    * @return void
    */
   void readInConfigs();
};

} // End Namespace

#endif // CONFIGREADER_H
