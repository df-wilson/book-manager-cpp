/**
 * @class BookController
 * 
 * Handles the HTTP requests for the Book Manager App.
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 */

#ifndef BOOKCONTROLLER_H
#define BOOKCONTROLLER_H

/*---------  Program Includes  ----------------*/
#include "JsonResponse.h"

/*---------  System Includes  -----------------*/
#include <iostream>
#include <string>

namespace dw {
   
/*---------  Class Definition  ----------------*/

class BookController final
{
public:
   
   /*---------  Public Methods  ------------------*/
   
   /**
    * Constructors and Destructors
    */
   BookController();
   virtual ~BookController();
  
   /**
    * Handle the GET request /api/vi/books. The book data is returned as a JSON string in
    * the form:
    * {"message":"OK", "books":[{"author":"[string]]","id":[int],"rating":[int],"read":[bool],"title":"[string]","year":"[string]"},...]}
    * 
    * @param token the users authentication token
    * @return the HTTP code and message to send to the client
    */
   JsonResponse getBooks(const std::string& token);

   /**
    * Handle the GET request /api/vi/books/id
    * 
    * @param token the users authentication token
    * @return the HTTP code and message to send to the client
    */
   JsonResponse getById(const std::string& token, int bookId);
   
   /**
    * Handles the DELETE request to remove a book from the datastore.
    * 
    * @param token the users authentication token
    * @param bookId The ID of the book to remove from the data store.
    * @return the HTTP code and message to send to the client
    */
   JsonResponse remove(const std::string& token, int bookId);
   
   /**
    * Handles the POST request. The book data is expected to be in JSON format in the form:
    * {"title":"[title]","author":"[author]","year":"[year]","read":[bool],"rating":[0 to 5]}
    *
    * @param token the users authentication token 
    * @param jsonData The book data in JSON format.
    * @return the HTTP code and message to send to the client
    */
   JsonResponse store(const std::string& token, const std::string& jsonData);
   
   /**
    * Handles the PUT request. Updates an existing book in the data store. The book data is 
    * expected to be in JSON format in the form:
    * {"title":"[title]","author":"[author]","year":"[year]","read":[bool],"rating":[0 to 5]}
    *
    * @param token the users authentication token
    * @param bookId The ID of the book to update.
    * @param jsonData The book data in JSON format.
    * @return the HTTP code and message to send to the client
    */ 
   JsonResponse update(const std::string& token, int bookId, const std::string& jsonData);
   
private:
   
   /*---------  Private Data   -------------------*/
   
   
   /*---------  Private Methods ------------------*/
   
   int userIdFromToken(const std::string& token);
};

}

#endif // BOOKCONTROLLER_H
