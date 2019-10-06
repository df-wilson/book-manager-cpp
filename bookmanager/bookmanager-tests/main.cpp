// Let Catch provide main():
//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>

int main( int argc, char* argv[] ) {
  // global setup...

  int result = Catch::Session().run( argc, argv );

  // global clean-up...

  return result;
}

#include "SQLiteCpp/Database.h"
#include "ConfigReader.h"

// Set up the database
TEST_CASE( "1: All test cases reside in other .cpp files (empty)", "[multi-file:1]" ) 
{
   std::cout << "Setting up database\n";
   try
   {
        // Open a database file in create/write mode
      std::string dbPath = dw::ConfigReader::getInstance().getConfig(dw::ConfigReader::Config::DB_PATH); 
        SQLite::Database    db(dbPath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

      db.exec("DROP TABLE IF EXISTS users");

      db.exec(R"(CREATE TABLE IF NOT EXISTS users 
      (
         id integer not null primary key autoincrement, 
         name varchar not null, 
         email varchar not null, 
         password varchar not null, 
         remember_token varchar null, 
         created_at datetime null, 
         updated_at datetime null
      );)");
      
      db.exec("DROP TABLE IF EXISTS tokens");
      db.exec(R"(CREATE TABLE IF NOT EXISTS tokens 
      (
         id integer PRIMARY KEY,
         user_id integer,
         token text NOT NULL,
         expires date NOT NULL
      );)"
      );
      
      db.exec("DROP TABLE IF EXISTS books");
      db.exec(R"(CREATE TABLE IF NOT EXISTS books 
      (
         id integer not null primary key autoincrement,
         user_id integer not null,
         title varchar not null, 
         author varchar not null, 
         year integer null, 
         read tinyint(1) not null, 
         rating integer null, 
         created_at datetime null, 
         updated_at datetime null,
         foreign key(user_id) references users(id)
      );)"
      );
      
      db.exec(R"(INSERT INTO books VALUES(1, 1,'Sorcerer''s Daughter','Terry Brooks',2009,1,4,'03-06-17 20:25:18','03-06-17 20:25:18');)");
      db.exec(R"(INSERT INTO books VALUES(2, 1,'The Expanse','James S.A. Corey',2014,1,5,'03-07-17 00:13:13','03-07-17 00:13:13');)");
      db.exec(R"(INSERT INTO books VALUES(3, 1,'The Stand','Steven King',1985,1,4,'03-07-17 00:21:31','03-07-17 00:21:31');)");
      db.exec(R"(INSERT INTO books VALUES(4, 1,'Omega','Jack McDevitt',2005,0,4,'03-08-17 22:02:41','03-08-17 22:02:41');)");
      db.exec(R"(INSERT INTO books VALUES(5, 1,'The Sword of Shannara','Terry Brooks',1985,0,4,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(6, 1,'Memory And Dream','Charles De Lint',1985,1,5,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(7, 1,'Redshirts','John Scalzi',2015,0,3,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(8, 1,'Test Book','Fred Farmerson',1986,1,3,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(9, 1,'Fuzzy Nation','John Scalzi',1013,1,5,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(10, 1,'Dreams Underfoot','Charles De Lint',1988,1,5,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(11, 1,'It','Steven King',1984,1,4,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(12, 1,'The Churn','James S.A. Corey',2007,0,4,NULL,NULL);)");
      db.exec(R"(INSERT INTO books VALUES(13, 1,'Starhawk','Jack McDevitt',2015,1,4,NULL,NULL);)");
   }
   catch (std::exception& e)
   {
      std::cout << "SQLite exception: " << e.what() << std::endl;
   }
}

