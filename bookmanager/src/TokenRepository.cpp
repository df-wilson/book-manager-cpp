#include "TokenRepository.h"
#include "ConfigReader.h"
#include "Logger.h"

#include <climits>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace std;

namespace dw {
   
const string SELECT_SQL = "SELECT user_id FROM tokens where token = ?";
const string INSERT_SQL = "INSERT INTO tokens (user_id, token, expires) VALUES (?,?,datetime('now'))";
const string EXISTS_SQL = "SELECT 1 FROM tokens WHERE token = ?";
const string FIND_TOKEN_SQL = "SELECT token FROM tokens WHERE user_id = ?";
const string REMOVE_SQL = "DELETE FROM tokens WHERE token = ?";
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
TokenRepository::TokenRepository()
{
   string dbPath("");
   
   try {
      dbPath = ConfigReader::getInstance().getConfig(ConfigReader::Config::DB_PATH); 
      Logger::instance().log(Logger::LogLevel::INFO, "TokenRepository", "Constructor. Database path is &.", dbPath);
   }
   catch(std::out_of_range& e) {
      e.what();
      Logger::instance().log(Logger::LogLevel::ERROR, "TokenRepository", "Constructor. ERROR Exception: &.", e.what());
      abort();
   }
   
   mDb = make_shared<SQLite::Database>(dbPath, 
                                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
TokenRepository::~TokenRepository()
{
}

/******************************************************************************
 * Name: create
 * Description: Create a new token and save it to the database.
 ******************************************************************************
 */
string 
TokenRepository::create(unsigned int userId)
{   
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "create(). User ID: &.", userId);
   
   string token = getTokenForUserId(userId);
   
   if(token.length() == 0) {
      token = longToHex(generateRandomNum()) + std::to_string(userId);
      
      SQLite::Statement statement(*mDb, INSERT_SQL);
      statement.bind(1, userId);
      statement.bind(2, token);
      
      int result = statement.exec();
      
      if(result) {      
         Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "create(). Token created.");
      } else {
         Logger::instance().log(Logger::LogLevel::ERROR, "TokenRepository", "create(). ERROR token not saved.");
         token = "";
      }      
   }
   
   return token;
}

/******************************************************************************
 * Name: exists
 * Description: Check to see if a token exists.
 ******************************************************************************
 */
bool 
TokenRepository::exists(string token) 
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "exists() ENTER. Token: &.", token);
   
   bool exists = false;
   
   SQLite::Statement statement(*mDb, EXISTS_SQL);
   statement.bind(1, token);
   
   int result = statement.executeStep();
   if(result) {
      exists = true;
   } else {
      exists = false;
   }
   
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "exists() LEAVE. Exists: &.", exists);
   
   return exists;
}

/******************************************************************************
 * Name: getTokenForUserId
 * Description: Get the token for a user ID.
 ******************************************************************************
 */
string 
TokenRepository::getTokenForUserId(unsigned int userId)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "getTokenForUserId() ENTER. UserId: &.", userId);
   
   string token = "";
   
   SQLite::Statement query(*mDb, FIND_TOKEN_SQL);
   query.bind(1, (long long)userId);
   
   if(query.executeStep()) {
      token = query.getColumn(0).getString();
   }
   
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "getTokenForUserId() LEAVE. token: &.", token);
   
   return token;
}

/******************************************************************************
 * Name: getUserIdForToken
 * Description: Get the user ID for a given token.
 ******************************************************************************
 */
long 
TokenRepository::getUserIdForToken(const std::string& token)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "getUserIdForToken() ENTER. token: &.", token);
   long user_id = 0;
   
   SQLite::Statement query(*mDb, SELECT_SQL);
   query.bind(1, token);
   
   if (query.executeStep())
   {
      user_id = query.getColumn(0).getInt();
   }
 
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "getUserIdForToken() LEAVE. UserId: &.", user_id);
   
   return user_id;
}

bool
TokenRepository::remove(std::string token)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "remove().");
   
   SQLite::Statement query(*mDb, REMOVE_SQL);
   query.bind(1, token);
   
   int affectedRows = query.exec();

   return (affectedRows > 0);
}

/******************************************************************************
 * Name: longToHex
 * Description: Private. Convert a long int to its hex value.
 ******************************************************************************
 */
string 
TokenRepository::longToHex(long n)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "TokenRepository", "longToHex(). long val: &.", n);
   
   stringstream ss;
   ss << std::hex << n;
   return ss.str();
}

/******************************************************************************
 * Name: generateRandomNum
 * Description: Private. Generate a random number.
 ******************************************************************************
 */
long 
TokenRepository::generateRandomNum()
{
   
   mt19937 eng(static_cast<unsigned long>(time(nullptr)));
   uniform_int_distribution<long> dist(LONG_MAX/2, LONG_MAX);
   
   return dist(eng);
}

} // end namespace dw
