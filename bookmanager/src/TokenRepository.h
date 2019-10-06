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
#ifndef TOKENREPOSITORY_H
#define TOKENREPOSITORY_H

/*--------  System Includes  --------------*/
#include <memory>
#include <string>
#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>

namespace dw {
   
class TokenRepository
{
public:
   /*-----------  Public Constants  ----------------*/
   const long NO_ID = 0;
   
   /*-----------  Public Functions  ----------------*/
   
   /**
    * Constructor and destructor
    */
   TokenRepository();
   virtual ~TokenRepository();

   /**
    * Remove all expired tokens from the database.
    */
   void 
   clearExpiredTokens();
      
   /**
    * @param userId the user ID to create the token for.
    */
   std::string 
   create(unsigned int userId);

   /**
    * Check to see if a token exists in the database.
    * @param token the token to see if exists in the database.
    * @return bool
    */
   bool 
   exists(std::string token);
   
   /**
    * Find the token for the given user Id.
    * @param userId
    * @return the token for the given user.
    */
   std::string 
   getTokenForUserId(unsigned int userId);
   
   /**
    * Find the user ID for a given token.
    * 
    * @param token the token to find the ID for.
    * @return the user ID
    */
   long 
   getUserIdForToken(const std::string& token);

   /**
    * Remove the given token from the database.
    * 
    * @param token The token to remove from the database.
    * @remove return true if the token was deleted.
    */
   bool
   remove(std::string token);
   
private:
   /*-----------  Private Functions  ---------------*/
   std::string 
   longToHex(long n);
   
   long 
   generateRandomNum();
   
   /*-----------  Private Data    ------------------*/
   std::shared_ptr<SQLite::Database> mDb;
};

} // end namespace dw
#endif
