#ifndef DB_CONNECT_H
#define DB_CONNECT_H

#include <SQLiteCpp/SQLiteCpp.h>

#include <memory>

namespace dw {
   
   SQLite::Database* db_getConnection();
   void db_returnConnection(SQLite::Database* database);
   unsigned int db_numAvailableConnections();
   void db_shutdown();
}
#endif
