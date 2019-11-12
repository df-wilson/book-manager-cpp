/**
 * @class BookRepository
 * 
 * Handles storing, updating and retrieving from the todo data store.
 * One database connection is shared for all get requests. All database writes create
 * a new connection and close it when finished.
 * 
 * @author  Dean Wilson
 * @version 1.1
 * @date    Feb 25, 2017
 */
#ifndef TODOREPOSITORY_H
#define TODOREPOSITORY_H

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
    * @return std::vector<todo>
    */
   std::vector<Book> getAll(int userId);
   
   /**
    * Get the stored todo object for the given id.
    * @throws out_of_range exception if the todo cannot be retrieved.
    * 
    * @param userId The id of the user requesting the todo 
    * @param TodoId The id of the todo to retrieve.
    * @return The todo data.
    */
    Book getById(int userId, int TodoId);
   
   /**
    * Delete the todo with the given id from the data store. Returns true if successful,
    * otherwise returns false.
    * 
    * @param userId the id of the user who is requesting the delete
    * @param TodoId the id of the todo to delete
    * @return true if the todo was removed, otherwise false.
    */
   bool remove(int userId, int TodoId);
  
   
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
    * Store a new todo object in the data store.
    * 
    * @param todo The todo to store.
    * @return true if the todo was stored, false otherwise.
    */
   long store(const Book& todo);
   
   /**
    * Update a todo object in the data store.
    * 
    * @param todo the updated todo object.
    * @return true if the todo was successfully updated, false otherwise.
    */
   bool update(const Book& todo);
  
private:
   
   /*-----------  Private Data    ------------------*/
   
   SQLite::Database* mDb;
   std::string mDbPath;

};

} // End namespace dw

#endif // BOOKREPOSITORY_H
