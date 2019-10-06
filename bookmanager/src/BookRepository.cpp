
/*---------  Program Includes  ----------------*/
#include "BookRepository.h"
#include "ConfigReader.h"
#include "Book.h"
#include "Logger.h"
#include "dbConnect.h"

/*--------  System Includes  --------------*/
#include <iostream>
#include <vector>


using namespace std;

namespace dw {
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
BookRepository::BookRepository()
{
   try {
      mDbPath = ConfigReader::getInstance().getConfig(ConfigReader::Config::DB_PATH); 
      Logger::instance().log(Logger::LogLevel::INFO, "BookRepository", "Constructor. Database path is &.", mDbPath);
   }
   catch(out_of_range& e) {
      e.what();
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "Constructor. ERROR Exception: &.", e.what());
      abort();
   }
   
   //mDb = make_shared<SQLite::Database>(mDbPath, 
   //                                    SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   
   mDb = db_getConnection();
}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
BookRepository::~BookRepository()
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookRepository", "Desstructor.");
   db_returnConnection(mDb);
}

/******************************************************************************
 * Name: getAll
 * Description: Return all stored books in a vector.
 ******************************************************************************
 */
vector<Book> BookRepository::getAll(int userId)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "getAll()");
   
   vector<Book> books;
   try 
   {
      SQLite::Statement query(*(mDb), "SELECT * FROM books WHERE user_id = :userId");
      query.bind(":userId", userId);
      
      while (query.executeStep())
      {
         Book book(query.getColumn(0),
                   query.getColumn(1), 
                   query.getColumn(2), 
                   query.getColumn(3), 
                   query.getColumn(4), 
                   (int)query.getColumn(5), 
                   query.getColumn(6));
         
         books.push_back(book);
      }
   }
   catch (exception& e)
   {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "getAll(). ERROR Exception: &.", e.what());
      throw;
   }
   
   return books;
}

/******************************************************************************
 * Name: getById
 * Description: Return the stored book with the given id.
 ******************************************************************************
 */
Book BookRepository::getById(int userId, int todoId)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "getById(). User ID: & Book ID &.", to_string(userId), to_string(todoId));
   
   bool hasResults = false;
   
   SQLite::Statement query(*(mDb), "SELECT id, user_id, title, author, year, read, rating FROM books WHERE id = :id AND user_id = :user_id");
   query.bind(":id", todoId);
   query.bind(":user_id", userId);
   
   try 
   {
      hasResults = query.executeStep();
   }
   catch (exception& e)
   {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "getById(). ERROR Exception: &.", e.what());
      throw out_of_range("Book with that id does not exist for user.");
   }
   
   if(hasResults == false) {
      query.reset();
      throw out_of_range("Book with that id does not exist for user.");
   }
   
   Book book(query.getColumn(0),
             query.getColumn(1), 
             query.getColumn(2), 
             query.getColumn(3),
             query.getColumn(4), 
             (int)query.getColumn(5), 
             query.getColumn(6));
   query.reset();
   
   return book;
}

/******************************************************************************
 * Name: remove
 * Description: Remove a book from the data store.
 ******************************************************************************
 */
bool BookRepository::remove(int userId, int todoId)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "remove(). Book ID: & User ID &.", to_string(todoId), to_string(userId));
   
   bool isRemoved = false;
   
   string deleteQuery = "DELETE FROM books WHERE id = ? AND user_id = ?";
   
   // Create a new database connection when storing a new book. Connection will be closed
   // when db goes out-of-scope.
   //
   SQLite::Database db(mDbPath, 
                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   SQLite::Statement query(db, deleteQuery);
   query.bind(1, todoId);
   query.bind(2, userId);
   int result = query.exec();
   
   if(result) {
      isRemoved = true;
   }
   
   return isRemoved;
}

/******************************************************************************
 * Name: store
 * Description: Store a new book in the data store.
 ******************************************************************************
 */
long BookRepository::store(const Book& book)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "store(). Book data: &.", book.toString());

   long newId = 0;
   
   string insertQuery = "INSERT INTO books (user_id, title, author, year, read, rating) VALUES (?,?,?,?,?,?)";
   
   // Create a new database connection when storing a new book. Connection will be closed
   // when db goes out-of-scope.
   //
   SQLite::Database db(mDbPath, 
                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   SQLite::Statement query(db, insertQuery);
   
   query.bind(1, book.userId());
   query.bind(2, book.title());
   query.bind(3, book.author());
   query.bind(4, book.year());
   query.bind(5, book.read());
   query.bind(6, book.rating());
   
   int result = query.exec();
   
   if(result) {
      newId = db.getLastInsertRowid();
      
      Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "store(). Book created. Id is: &.", to_string(newId));
   } else {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "store(). ERROR book not saved.");
   }
   
   return newId;
}

/******************************************************************************
 * Name: update
 * Description: Update a new book in the data store.
 ******************************************************************************
 */
bool BookRepository::update(const Book& book) {
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "update(). Book data: &.", book.toString());

   // Ensure reasonable ID
   if (book.id() < 1 || book.userId() < 1) {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "update(). Invalid user ID or Book ID");
      return false;
   }
   
   bool isSaved = false;
   
   string updateQuery = "UPDATE books set title=?, author=?, year=?, read=?, rating=? WHERE id=? AND user_id=?";
   
   // Create a new database connection when updating a book. Connection will be closed
   // when db goes out-of-scope.
   //
   SQLite::Database db(mDbPath, 
                       SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
   SQLite::Statement query(db, updateQuery);
   
   query.bind(1, book.title());
   query.bind(2, book.author());
   query.bind(3, book.year());
   query.bind(4, book.read());
   query.bind(5, book.rating());
   query.bind(6, book.id());
   query.bind(7, book.userId());
   
   int result = query.exec();
   
   if(result) {
      isSaved = true;
      Logger::instance().log(Logger::LogLevel::DEBUG, "BookRepository", "update(). Book has been updated.");
   } else {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookRepository", "update(). ERROR book not updated.");
      isSaved = false;
   }
   
   return isSaved;
}

} // End Namespace dw

