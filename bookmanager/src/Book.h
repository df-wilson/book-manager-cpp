/**
 * @class Book
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 * 
 * Description:
 * Stores the data for a book object. The object can be constructed either by passing
 * in the individual items or via a JSON string. If a valid book object cannot be 
 * created fron the JSON string, an std::runtime_error exception is thrown. 
 * 
 * There is no default constructor, so an empty or default object cannot be created.
 * 
 * Once the object is created, it cannot be modified.
 * 
 * The individual components may be retrieved, or the object can be retrieved as a 
 * JSON string. 
 * 
 */
#ifndef BOOK_H
#define BOOK_H

/*---------  System Includes  -----------------*/
#include <string>

namespace dw {
   
class Book final
{
public:
   
   /*---------  Public Functions  ---------------*/
   
   /**
    * Constructors and Destructors
    */
   Book() = delete;
   Book(int id,
        int user_id,
        const std::string& title, 
        const std::string& author, 
        const std::string& year, 
        bool isRead, 
        int rating);
   Book(const std::string& jsonString);
   virtual ~Book() = default;
   
   /**
    * Get methods.
    */
   int id() const;
   std::string author() const;
   std::string title() const;
   std::string year() const;
   bool read() const;
   int rating() const;
   int userId() const;
   
   /**
    * Set methods
    */
   void bookId(int bookId) { mId = bookId; }
   void userId(int userId) { mUserId = userId; }
   
   /**
    * Get the book data as a string.
    * 
    * @return the book data as a string.
    */
   std::string toString() const;
   
   /**
    * Get the object data a string in JSON format.
    * 
    * @return the book data as a string in JSON format.
    */
   std::string toJson() const;
   
private:
   
   /*---------  Private Functions ---------------*/
   
   /**
    * Parses a JSON string containing book data. If the JSON data is invalid or does not
    * contain the expected data, an std::runtime_error exception is thrown.
    */
   void parseJsonString(const std::string& jsonString);
  
   
   /*---------  Private Data  ------------------*/
   
   int         mId;
   int         mUserId;
   std::string mTitle;
   std::string mAuthor;
   std::string mYear;
   bool        mIsRead;
   int         mRating;
};
   
}


#endif // BOOK_H
