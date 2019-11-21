/**
 * @class BookRepository
 * 
 * Handles storing, updating and retrieving from the book data store.
 * One database connection is shared for all get requests. All database writes create
 * a new connection and close it when finished.
 * 
 * @author  Dean Wilson
 * @version 1.1
 * @date    Feb 25, 2017
 */
#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

/*---------  Program Includes  ----------------*/
#include "Book.h"

/*--------  System Includes  --------------*/
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <memory>
#include <string>
#include <vector>

namespace dw {

class BookRepository
{
   
public:
   
   enum SEARCH_TYPE
   {
      AUTHOR,
      BOTH,
      TITLE
   };
   /*-----------  Public Functions  ----------------*/
   
   /**
    * Constructor and destructor
    */
   BookRepository();
   virtual ~BookRepository();
  
   /**
    * Gets all stored books.
    * 
    * @param userId the user ID of the books to return
    * @return std::vector<Book>
    */
   std::vector<Book> getAll(int userId);
   
   /**
    * Get the stored book object for the given id.
    * @throws out_of_range exception if the book cannot be retrieved.
    * 
    * @param userId The id of the user requesting the book 
    * @param bookId The id of the book to retrieve.
    * @return The book data.
    */
    Book getById(int userId, int bookId);
   
   /**
    * Delete the book with the given id from the data store. Returns true if successful,
    * otherwise returns false.
    * 
    * @param userId the id of the user who is requesting the delete
    * @param bookId the id of the book to delete
    * @return true if the book was removed, otherwise false.
    */
   bool remove(int userId, int bookId);
  
   
   /**
    * Search for books that contain the search term in either the author's name or book title. Only books
    * that belong to the user are returned.
    * 
    * @param user_id the id of the user doing the search.
    * @param searchTerm the string to search for.
    * @param searchType the type of search to do
    * @return std::vector< dw::Book > The list of books that match the search criteria.
    */
   std::vector<Book> search(int user_id, SEARCH_TYPE searchType, std::string searchTerm);
   
   /**
    * Store a new book object in the data store.
    * 
    * @param book The book to store.
    * @return true if the book was stored, false otherwise.
    */
   long store(const Book& book);
   
   /**
    * Update a book object in the data store.
    * 
    * @param book the updated book object.
    * @return true if the book was successfully updated, false otherwise.
    */
   bool update(const Book& book);
  
private:
   
   /*-----------  Private Data    ------------------*/
   
   SQLite::Database* mDb;
   std::string mDbPath;

};

} // End namespace dw

#endif // BOOKREPOSITORY_H
