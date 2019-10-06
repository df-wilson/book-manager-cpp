
/*---------  Program Includes  ---------------*/
#include "ConfigReader.h"

/*---------  System Includes  --------------*/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

namespace dw {

const char* CONFIG_FILE_LOCATION = "config.cfg";

/******************************************************************************
 * Constructor
 ******************************************************************************
 */
ConfigReader::ConfigReader()
{
   readInConfigs();
}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
ConfigReader::~ConfigReader()
{

}

/******************************************************************************
 * Name: configEnumToString
 * Description: Gets the string value of a config enum.
 ******************************************************************************
 */
string
ConfigReader::configEnumToString(ConfigReader::Config configIn) const
{
   string config("NONE");

   switch(configIn) {
      case Config::LOG_LEVEL:
         config = "LOG_LEVEL";
         break;
         
      case Config::DB_PATH:
         config = "DB_PATH";
         break;
         
      default:
         config = "NONE";
         break;
   }
   
   return config;
}

/******************************************************************************
 * Name: configEnumToString
 * Description: Gets the string value of a config enum.
 ******************************************************************************
 */
ConfigReader::Config 
ConfigReader::configStringToEnum(const string& configString) const
{
   Config config = Config::NONE;

   if(configString == "NONE")
   {
      config = Config::NONE;
   }
   else if (configString == "DB_PATH")
   {
      config = Config::DB_PATH;
   }
   else if (configString == "LOG_LEVEL")
   {
      config = Config::LOG_LEVEL;
   }
   else
   {
      config = Config::NONE;
   }

   return config;
}

/******************************************************************************
 * Name: getConfig
 * Description: Get the configuration value as a string.
 ******************************************************************************
 */
string 
ConfigReader::getConfig(ConfigReader::Config config) const
{
   auto configIter = mConfigCache1.find(config);
   if(configIter == mConfigCache1.end()) {
      string logLevel = configEnumToString(config);
      string error = "No such configuration found: " + logLevel;
      throw(out_of_range(error));
   }

   string configValue = configIter->second;

   return configValue;
}

/******************************************************************************
 * Name: getInstance
 * Description: Get an instance of the config reader.
 ******************************************************************************
 */
ConfigReader& ConfigReader::getInstance()
{
   static ConfigReader mInstance;

   return mInstance;
}

/**
 * Name: readInConfigs
 * Description: Read in configurations from file
 */
void ConfigReader::readInConfigs()
{
   string line("");
   string error = "ConfigReader::readInConfigs. Can't open file specified. "; 

   ifstream fin;
   try {
      //fin.open(CONFIG_FILE_LOCATION);
      fin.open("config.cfg");
   }
   catch (ifstream::failure e) {
      std::cout << error << e.what() << endl;
   }
   if(!fin.is_open())
   {
      throw invalid_argument(error.c_str());
   }

   while (fin.good() && !fin.eof())
   {
      getline(fin, line);
      storeConfig(line);
   }

   fin.close();

   for(auto it = mConfigCache1.cbegin(); it != mConfigCache1.cend(); ++it) {
      //cout << "Config Map Values" << it->first << ": " << it->second << endl;
      cout << "Config Map Values " << it->second << endl;
   }
}

/**
 * Name: storeConfig
 * Description:
 * Store 1 configuration variable in the configuration cache
 * Parses the configLine string on = to find the configuration and it's value. 
 * - A line beginning with # is considered a comment.
 * - An empty line or line with just whitepace is ignored.
 * - A line with no = delimeter is ignored.
 *      Examples:
 *      LogLevel=DEBUG
 *      # This is a comment
 */
void ConfigReader::storeConfig(const string& configLine)
{
   // First check if this is a comment.
   if(configLine.front() != '#')
   {
      string::size_type index = configLine.find("=");
      if(index != string::npos)
      {
         string configuration = configLine.substr(0, index);
         string value = configLine.substr(index+1);

         //Get rid of any whitepace
         configuration.erase( std::remove_if(configuration.begin(), configuration.end(), ::isspace ), configuration.end());
         value.erase(std::remove_if( value.begin(), value.end(), ::isspace ), value.end());

         // Add into the cache if neither string is empty
         if(!configuration.empty() && !value.empty())
         {
            mConfigCache[configuration] = value;
            Config configEnum = configStringToEnum(configuration);
            if(configEnum != Config::NONE)
            {
               mConfigCache1[configEnum] = value;
            }
         }
      }
   }
}

} // end namespace dw
