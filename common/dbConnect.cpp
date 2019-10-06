#include "dbConnect.h"

#include "ConfigReader.h"
#include "Logger.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace dw {
   
std::vector<SQLite::Database*> connections;

SQLite::Database* db_getConnection()
{
   std::string mDbPath;
   SQLite::Database* connection = nullptr;
   
   if(connections.size() == 0) {
      try {
         mDbPath = dw::ConfigReader::getInstance().getConfig(dw::ConfigReader::Config::DB_PATH); 
         dw::Logger::instance().log(dw::Logger::LogLevel::INFO, "dbConnect", "db_getConnection: Database path is &.", mDbPath);
      }
      catch(std::out_of_range& e) {
         e.what();
         dw::Logger::instance().log(dw::Logger::LogLevel::ERROR, "dbConnect", "db_getConnection: ERROR Exception: &.", e.what());
         abort();
      }
      
      connection = new SQLite::Database(mDbPath, 
                     SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   } else {
      connection = connections.back();
      connections.pop_back();
   }
   
   dw::Logger::instance().log(dw::Logger::LogLevel::DEBUG, "dbConnect", "db_getConnection: LEAVE - Open connections &.", connections.size());
   
   return connection;
}

unsigned int db_numAvailableConnections()
{
   return connections.size();
}

void db_returnConnection(SQLite::Database* connection)
{
   bool exists = false;
   
   for(SQLite::Database* storedConnection : connections) {
      if(connection == storedConnection) {
            dw::Logger::instance().log(dw::Logger::LogLevel::ERROR, "dbConnect", "db_returnConnection: Connection already stored.");
         exists = true;
         break;
      }
   }
   
   if(connection != nullptr && exists == false) {
      connections.push_back(connection);
   }
   
   dw::Logger::instance().log(dw::Logger::LogLevel::DEBUG, "dbConnect", "db_returnConnection: LEAVE - Open connections &.", connections.size());
   
   return;
}

void db_shutdown()
{
   dw::Logger::instance().log(dw::Logger::LogLevel::DEBUG, "dbConnect", "db_shutdown: ENTER - Open connections &.", connections.size());
   
   for(SQLite::Database* connection : connections) {
      if(connection) {
         delete connection;
         connection = nullptr;
      }
   }
   
   connections.clear();
   
   dw::Logger::instance().log(dw::Logger::LogLevel::DEBUG, "dbConnect", "db_shutdown: LEAVE - Open connections &.", connections.size());
}

} // End namespace dw
