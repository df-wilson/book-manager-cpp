/**
 * @class UserRepository
 * 
 * Handles storing, updating and retrieving data from the user data store.
 * One database connection is shared for all get requests. All database writes create
 * a new connection and close it when finished.
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    November 19, 2017
 */

#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

/*---------  Program Includes  ----------------*/
#include "User.h"

/*--------  System Includes  --------------*/
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

namespace dw {
 
class UserRepository
{
public:
   /*-----------  Public Functions  ----------------*/
   
   /**
    * Constructor and destructor
    */
   UserRepository();
   virtual ~UserRepository();
   
   /**
    * Disable copying
    */
   UserRepository(const UserRepository &rhs) = delete;
   void operator=(const UserRepository &rhs) = delete;

   /**
   * Get the number of user records in the datastore.
   * 
   * @return long int
   */
   long count();
   
   /**
    * Get all user information for the given user id. If the user is not found 
    * id of the returned user id will be 0.
    * 
    * @param id The user id to find the data for.
    * @return User
    */
   User getById(unsigned int id);
      
   /**
   * Check if the given user ID / password combination is in the database. Returns 
   * true if found, otherwise false.
   * 
   * @param email The user's email address
   * @param password The user's password
   * @return long
   */
   long getUserId(std::string email, std::string password);
   
   /**
   * Remove the user with the given id from the database.
   * 
   * @param id the user id
   */
   void remove(unsigned int id);
   
   /**
    * Store a new user object in the data store.
    * 
    * @param user The user to store.
    * @return New user ID if successful, otherwise 0. 
    */
   long store(const User& user);
  
   /**
   * Update the user's password.
   * 
   * @param id       the user's id
   * @param password the new password
   * @return bool
   */
   bool updatePassword(unsigned int id, const std::string &password);

   
private:
   /*-----------  Private Data    ------------------*/
   
   const std::string mAuthenticateQuery = "SELECT id FROM users WHERE email=? AND password=?";
   
   SQLite::Database* mDb;
   std::shared_ptr<SQLite::Statement> mQuery;
   std::string mDbPath;

};

} // End namespace dw

#endif
