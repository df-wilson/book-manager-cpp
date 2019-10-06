/*---------  Program Includes  ----------------*/
#include "UserRepository.h"
#include "ConfigReader.h"
#include "dbConnect.h"
#include "Logger.h"

/*--------  System Includes  --------------*/
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <iostream>

using namespace std;

namespace dw {
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
UserRepository::UserRepository()
{
   try {
      mDbPath = ConfigReader::getInstance().getConfig(ConfigReader::Config::DB_PATH); 
      Logger::instance().log(Logger::LogLevel::INFO, "UserRepository", "Constructor. Database path is &.", mDbPath);
   }
   catch(std::out_of_range& e) {
      e.what();
      Logger::instance().log(Logger::LogLevel::ERROR, "UserRepository", "Constructor. ERROR Exception: &.", e.what());
      abort();
   }
   
//   mDb = make_shared<SQLite::Database>(mDbPath, 
//                                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
mDb = db_getConnection();

}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
UserRepository::~UserRepository()
{
   db_returnConnection(mDb);
}

/******************************************************************************
 * Name: count
 * Description: Returns the number of users in the database.
 ******************************************************************************
 */
long 
UserRepository::count()
{
   string sql = "SELECT COUNT(1) FROM users";
   long count = 0;
      
   SQLite::Statement query(*mDb, sql);
   
   if (query.executeStep())
   {
      count = query.getColumn(0).getInt();
   }       
   
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserRepository", "count(). Num rows: &.", count);
      
   return count;
}

/******************************************************************************
 * Name: getById
 * Description: Get the user info for the given ID.
 ******************************************************************************
 */
User 
UserRepository::getById(unsigned int id)
{
   string sql = "SELECT * FROM users WHERE id = ?";
   
   SQLite::Statement query(*mDb, sql);
   query.bind(1, (long long)id);
   
   if (query.executeStep())
   {
      User user(query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText()
               );
      return user;
   }
 
   User user(0,"","","");
   return user;
}

/******************************************************************************
 * Name: getUserId
 * Description: Get the user ID for the given string and password.
 ******************************************************************************
 */
long 
UserRepository::getUserId(string email, string password)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, 
                          "UserRepository", 
                          "getUserId(). Email: & Password: &.", email, password);
      
   long userId = 0;
   
   SQLite::Statement query(*mDb, mAuthenticateQuery);
   query.bind(1, email);
   query.bind(2, password);
   
   if (query.executeStep())
   {
      userId = query.getColumn(0).getInt();
   }

   Logger::instance().log(Logger::LogLevel::DEBUG, 
                          "UserRepository", 
                          "getUserId() returning User ID: &.", userId);

   return userId;
}

/******************************************************************************
 * Name: remove
 * Description: Remove a user from the database.
 ******************************************************************************
 */
void 
UserRepository::remove(unsigned int id)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserRepository", "remove(). User Id: &.", id);
      
   string sql = "DELETE FROM users WHERE id = ?";
   SQLite::Database db(mDbPath, 
                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   SQLite::Statement query(db, sql);
   query.bind(1, (long long)id);
   int result = query.exec();
   
   if(result != 1) {
      Logger::instance().log(Logger::LogLevel::ERROR, "UserRepository", "remove(). ERROR: &.", result);
   }
}

/******************************************************************************
 * Name: store
 * Description: Store a new user in the database.
 ******************************************************************************
 */
long 
UserRepository::store(const User& user)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserRepository", "store(). User data: &.", user.toString());

   long newId = 0;
   
   string insertQuery = "INSERT INTO users (name, email, password, created_at, updated_at) VALUES (?,?,?,datetime('now'),datetime('now'))";
   
   // Create a new database connection when storing a new user. Connection will be closed
   // when db goes out-of-scope.
   //
   SQLite::Database db(mDbPath, 
                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   SQLite::Statement query(db, insertQuery);
   
   query.bind(1, user.name());
   query.bind(2, user.email());
   query.bind(3, user.password());
   
   int result = query.exec();
   
   if(result) {
      newId = db.getLastInsertRowid();
      
      Logger::instance().log(Logger::LogLevel::DEBUG, "UserRepository", "store(). User created. Id is: &.", to_string(newId));
   } else {
      Logger::instance().log(Logger::LogLevel::ERROR, "UserRepository", "store(). ERROR user not saved.");
      newId = 0;
   }
   
   return newId;
}

/******************************************************************************
 * Name: updatePassword
 * Description: Update a user's password.
 ******************************************************************************
 */
bool 
UserRepository::updatePassword(unsigned int id, const std::string& password)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserRepository", "updatePassword(). Id: &.", id);
   
   bool isUpdated = false;
   string sql = "UPDATE users SET password=? WHERE id=?";
   
   SQLite::Statement query(*mDb, sql);

   query.bind(1, password);
   query.bind(2, (long long)id);

   int result = query.exec();
   
   if(result) {
      isUpdated = true;
   }
   
   return isUpdated;
}

} // End namespace dw
