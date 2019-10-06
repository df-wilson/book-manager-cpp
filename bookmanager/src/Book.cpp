
/*---------  Program Includes  ----------------*/
#include "Book.h"
#include "Logger.h"

/*---------  System Includes  -----------------*/
#include <iostream>
#include <string>
#include "json.hpp"

using namespace std;

namespace dw {

/******************************************************************************
 * Constructor
 ******************************************************************************
 */
Book::Book(int id,
           int userId,
           const std::string& title, 
           const std::string& author, 
           const std::string& year, 
           bool isRead,
           int rating
          )
     :mId(id), mUserId(userId), mTitle(title), mAuthor(author), mYear(year), mIsRead(isRead), mRating(rating)
{
}

/******************************************************************************
 * Constructor
 ******************************************************************************
 */   
Book::Book(const std::string& jsonString)
{
   parseJsonString(jsonString);
}

/******************************************************************************
 * Getters
 ******************************************************************************
 */
int Book::id() const
{
   return mId;
}

std::string Book::author() const
{
   return mAuthor;
}

bool Book::read() const
{
   return mIsRead;
}

std::string Book::title() const
{
   return mTitle;
}

std::string Book::year() const
{
   return mYear;
}

int Book::rating() const
{
   return mRating;
}

int Book::userId() const
{
   return mUserId;
}

/******************************************************************************
 * Name: toJson
 * Desc: Returns the book data as a JSON string
 ******************************************************************************
 */   
string Book::toJson() const
{
    nlohmann::json j;
   j["id"] = mId;
   j["userId"] = mUserId;
   j["title"] = mTitle;
   j["author"] = mAuthor;
   j["year"] = mYear;
   j["read"] = mIsRead;
   j["rating"] = mRating;
   
   return j.dump();
}

/******************************************************************************
 * Name: toString
 * Desc: Returns the book data as a string with a dash between each element.
 ******************************************************************************
 */   
string Book::toString() const
{
   string isReadString = "false";
   if(mIsRead) {
      isReadString = "true";
   }
   
   ostringstream bookString;
   bookString << mId << " - " << mUserId << " - " << mTitle << " - " << mAuthor << " - " << mYear << " - " << mIsRead << " - " << mRating;
   return bookString.str();
}

/******************************************************************************
 * Name: parseJsonString
 * Desc: Parse the given JSON string to obtain the book data. 
 ******************************************************************************
 */   
void Book::parseJsonString(const std::string& jsonString)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "Book", "Parse JSON string. JSON string &.", jsonString);
   
   auto data = nlohmann::json::parse(jsonString);
   Logger::instance().log(Logger::LogLevel::DEBUG, "Book", "Parse JSON string. Parsed data is &.", data);
     
   if(data.size() < 5) {
      throw std::runtime_error("Invalid JSON string. Incorrect number of elements");
   }
   
   // ID is optional. Default to 0
   if(data.find("id") != data.end() && data["id"].is_number_integer()) {
      mId = data["id"];
   } else {
      mId = 0;
   }
   
   // User ID is optional. Default to 0
   if(data.find("userId") != data.end() && data["userId"].is_number_integer()) {
      mUserId = data["userId"];
   } else {
      mUserId = 0;
   }
   
   if(data.find("title") != data.end() && data["title"].is_string()) {
      mTitle = data["title"];
   } else {
      throw std::runtime_error("Invalid JSON string. Title must be string");
   }
   
   if(data.find("author") != data.end() && data["author"].is_string()) {
      mAuthor = data["author"];
   } else {
      throw std::runtime_error("Invalid JSON string. Author must be string.");
   }
   
   if(data.find("year") != data.end() && data["year"].is_string()) {
      mYear = data["year"];
   } else {
      throw std::runtime_error("Invalid JSON string. year must be string.");
   }
   
   if(data.find("read") != data.end() && data["read"].is_boolean()) {
      mIsRead = data["read"];
   } else {
      throw std::runtime_error("Invalid JSON string. isRead must be boolean");
   }
   
   if(data.find("rating") != data.end() && data["rating"].is_number_integer()) {
      mRating = data["rating"];
      if(mRating < 0 || mRating > 5) {
         throw std::runtime_error("Invalid JSON string. Rating must be integer between 0 and 5.");
      }
   } else {
      throw std::runtime_error("Invalid JSON string. Rating must be an integer.");
   }
}

} // End namespace dw
