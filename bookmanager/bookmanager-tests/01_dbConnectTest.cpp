#include "catch.hpp"
#include "dbConnect.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>

using namespace dw;
using namespace std;

TEST_CASE("dbConnect - Test getting initial connection.") 
{
   SQLite::Database* connection = db_getConnection();
   REQUIRE(connection->getFilename() == "/home/dean/Programming/Cpp/web/Projects/build/bookmanager/bookmanager-tests/db.sqlite");
   REQUIRE(db_numAvailableConnections() == 0);
   REQUIRE(connection->tableExists("books"));
   
//   SQLite::Statement query(*connection, "SELECT * FROM books WHERE user_id = :userId");
//   query.bind(":userId", 1);
   
//    while (query.executeStep())
//    {
//       std::cout << query.getColumn(1) << std::endl;
//       std::cout << query.getColumn(2) << std::endl;
//       std::cout << query.getColumn(3) << std::endl;
//    }

   REQUIRE(db_numAvailableConnections() == 0);
   db_returnConnection(connection);
   REQUIRE(db_numAvailableConnections() == 1);
   
   db_shutdown();
   REQUIRE(db_numAvailableConnections() == 0);
}

TEST_CASE("dbConnect - Test reusing connection")
{
   SQLite::Database* connection = db_getConnection();
   REQUIRE(connection != nullptr);
   REQUIRE(connection->getFilename() == "/home/dean/Programming/Cpp/web/Projects/build/bookmanager/bookmanager-tests/db.sqlite");
   REQUIRE(connection->tableExists("books"));
   REQUIRE(db_numAvailableConnections() == 0);
   db_returnConnection(connection);
   REQUIRE(db_numAvailableConnections() == 1);
   
   SQLite::Database* connection2 = db_getConnection();
   REQUIRE(db_numAvailableConnections() == 0);
   REQUIRE(connection != nullptr);
   REQUIRE(connection->getFilename() == "/home/dean/Programming/Cpp/web/Projects/build/bookmanager/bookmanager-tests/db.sqlite");
   REQUIRE(connection->tableExists("books"));

   db_returnConnection(connection2);   
   REQUIRE(db_numAvailableConnections() == 1);
   
   db_shutdown();
   REQUIRE(db_numAvailableConnections() == 0);
}
