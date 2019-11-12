/*---------  Program Includes  ----------------*/
#include "BookController.h"
#include "BookRepository.h"
#include "Logger.h"
#include "TokenRepository.h"

/*---------  System Includes  -----------------*/
#include <string>

using namespace std;

namespace dw {
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
BookController::BookController()
{

}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
BookController::~BookController()
{
}


/******************************************************************************
 * Name: getBooks
 * Desc: Retrieves all book data for a user. User id is obtained from the token.
 ******************************************************************************
 */   
JsonResponse BookController::getBooks(const std::string& token)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "ENTER getBooks.");
   
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
   ostringstream   json;
   int             userId = userIdFromToken(token);
      
   if(userId) {
      try {
         BookRepository repository;
         vector<Book> books = repository.getAll(userId);
      
         json << "{\"message\":\"OK\", \"books\":[";
      
         int i = 0;
         for(Book& book : books) {
            if(i > 0) {
               json << ",";
            }
            json << book.toJson();
         
            i++;
         }
         code = Pistache::Http::Code::Ok;
      } catch(exception& e) {
         Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "getAll. ERROR: Saving book failed. &", e.what());
         
         code = Pistache::Http::Code::Internal_Server_Error;
         
         json.str("");
         json.clear();
         json << "{\"message\":\"ERROR: Cannot retrieve books\", \"books\":[";
      }
      json << "]}";
   } else {
      code = Pistache::Http::Code::Unauthorized;
      json << "{\"message\":\"User not authorized\", \"books\":[]}";
   }
      
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "LEAVE getBooks. JSON is &.", json.str());
      
   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: getById
 * Desc: Retrieves the data for a book with the given ID.
 ******************************************************************************
 */   
JsonResponse BookController::getById(const std::string& token, int bookId)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "ENTER getById. Token: & Book ID: &", token, to_string(bookId));
   
   int userId = userIdFromToken(token);

   try {
      BookRepository repository;
      Book book = repository.getById(userId, bookId);
      Pistache::Http::Code code = Pistache::Http::Code::Ok;
      string jsonString = R"({"message":"OK", "book":)" + book.toJson() + "}";
      JsonResponse response(jsonString, code);
      return response;
   }
   catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "getByID. ERROR: Retrieving book failed. &", e.what());
      Pistache::Http::Code code = Pistache::Http::Code::Not_Found;
      string jsonString = R"( {"message": "ERROR Book with that id doesn't exist", "book":{}"} )";
      JsonResponse response(jsonString, code);
      return response;
   }   
}

/******************************************************************************
 * Name: remove
 * Desc: Removes a book from the data store
 ******************************************************************************
 */   
JsonResponse BookController::remove(const std::string& token, int bookId)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "remove. JSON: &.", to_string(bookId));
   
   ostringstream json;
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;

   int userId = userIdFromToken(token);
   
   BookRepository repository;
   bool isRemoved = repository.remove(userId,bookId);
      
   if(isRemoved) {
      Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "remove. Book Removed.");
      json << "{\"message\":\"Book removed.\"}";
      code = Pistache::Http::Code::Ok;
   } else {
      json << "{\"message\":\"ERROR. Book not removed.\"}";
      code = Pistache::Http::Code::Internal_Server_Error;
   }
   
   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: search
 * Desc: Search for a book by author, title, or both.
 ******************************************************************************
 */   
JsonResponse BookController::search(const std::string& token, const std::string& searchTypeIn, const std::string& searchTerm)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "search: searchTerm &.", searchTerm);
   
   ostringstream json;
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
   
   int userId = userIdFromToken(token);
   if(userId) {
      try {
         BookRepository::SEARCH_TYPE searchType;
         if(searchTypeIn == "author") {
            searchType = BookRepository::SEARCH_TYPE::AUTHOR;
         } else if(searchTypeIn == "title") {
            searchType = BookRepository::SEARCH_TYPE::TITLE;
         } else {
            searchType = BookRepository::SEARCH_TYPE::BOTH;
         }
         BookRepository repository;
         vector<Book> books = repository.search(userId, searchType, searchTerm);
      
         json << "{\"message\":\"OK\", \"books\":[";
      
         int i = 0;
         for(Book& book : books) {
            if(i > 0) {
               json << ",";
            }
            json << book.toJson();
         
            i++;
         }
         code = Pistache::Http::Code::Ok;
      } catch(exception& e) {
         Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "search. ERROR: Saving book failed. &", e.what());
         
         code = Pistache::Http::Code::Internal_Server_Error;
         
         json.str("");
         json.clear();
         json << "{\"message\":\"ERROR: Cannot retrieve books\", \"books\":[";
      }
      json << "]}";
   } else {
      code = Pistache::Http::Code::Unauthorized;
      json << "{\"message\":\"User not authorized\", \"books\":[]}";
   }
      
   Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "LEAVE search. JSON is &.", json.str());
      
   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: store
 * Desc: Saves a new book in the data store.
 ******************************************************************************
 */   
JsonResponse BookController::store(const std::string& token, const std::string& jsonData)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "store. JSON: &.", jsonData);
   
   long newBookId = 0;
   string message = "";
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
   
   int userId = userIdFromToken(token);
   
   if(userId > 0) {
      try {
         Book book(jsonData);
         book.userId(userId);
         BookRepository repository;
         newBookId = repository.store(book);
      } catch (exception& e) {
         Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "store. ERROR: Saving book failed. &", e.what());
         newBookId = 0;
         message = e.what();
      }
   }
   
   ostringstream json;
   
   if(newBookId) {
      Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "store. Book Saved.");
      json << "{\"message\":\"Book saved.\", \"id\":" << newBookId << "}";
      code = Pistache::Http::Code::Created;
      
   } else {
      json << "{\"message\":\"ERROR. Book not saved\"}";
      code = Pistache::Http::Code::Internal_Server_Error;
   }

   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: update
 * Desc: Updates an existing book.
 ******************************************************************************
 */   
JsonResponse BookController::update(const std::string& token, int bookId, const std::string& jsonData)
{
   Logger::instance().log(Logger::LogLevel::INFO, "BookController", "ENTER - update. Book ID: &, JSON: &.", to_string(bookId), jsonData);
   
   bool isSaved = false;
   bool isAuthorized = false;
   bool isParseError = false;
   string message = "";
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
      
   int userId = userIdFromToken(token);
   
   if(userId > 0) {
      try {
         Book book(jsonData);
         book.bookId(bookId);
         book.userId(userId);
            BookRepository repository;
            isSaved = repository.update(book);
         if(isSaved) {
            isAuthorized = true;
         } else {
            Logger::instance().log(Logger::LogLevel::INFO, "BookController", "update. ERROR: Updating book failed. Book does not belong to user. User Id: &, Book user Id: &", userId, book.userId());
         }
      } catch (exception& e) {
         Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "update. ERROR: Updating book failed. &", e.what());
         isSaved = false;
         isParseError=true;
         message = e.what();
      }
   } else {
         Logger::instance().log(Logger::LogLevel::ERROR, "BookController", "update. ERROR: Updating book failed. Invalid user id: &", userId);
   }
   
   ostringstream json;
      
   if(isSaved) {
       Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "update. Book saved.");
       json << "{\"message\":\"Book updated.\"}";
       code = Pistache::Http::Code::Ok;
   } else if (isParseError || isAuthorized) {
       Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "update. Server Error. Book not saved.");
       json << "{\"message\":\"ERROR. Book not updated. Server error.\"}";
       code = Pistache::Http::Code::Internal_Server_Error;
   } else {
       Logger::instance().log(Logger::LogLevel::DEBUG, "BookController", "update. Book does not belong to user. Book not saved.");
       json << "{\"message\":\"ERROR. Todo does not belong to user, not updated.\"}";
       code = Pistache::Http::Code::Unauthorized;      
   }
   
   return JsonResponse(json.str(), code);
   
}

/******************************************************************************
 * Name: userIdFromToken
 * Desc: Get a user id from a token.
 ******************************************************************************
 */  
int BookController::userIdFromToken(const std::string& token)
{
   int userId = 0;
   TokenRepository tokenRepository;
   
   if(tokenRepository.exists(token)) {
      userId = tokenRepository.getUserIdForToken(token);
   }
   
   return userId;
}

std::string BookController::cleanInput(const std::string& input) const
{
   if(input.size() < 3) {
      return input;
   }
   
   size_t index = 0;
   while((index = input.find("%20", index)) != std::string::npos) {
      
   }
}

} // End namespace dw
