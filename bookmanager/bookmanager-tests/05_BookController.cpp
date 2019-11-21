#include "catch.hpp"

#include "JsonResponse.h"
#include "Logger.h"
#include "../src/BookController.h"
#include "../src/TokenRepository.h"

#include <iostream>
#include <string>

using namespace dw;
using namespace std;

const int USER_ID = 1;
const int BOOK_ID = 2;
const int NEW_BOOK_ID = 15;

string token;

TEST_CASE("Test BookController::getBooks.") 
{
   TokenRepository tokenRepository;
   token = tokenRepository.create(USER_ID);
   
   BookController bookController;
   JsonResponse jsonResponse = bookController.getBooks(token);
   REQUIRE(jsonResponse.message() == 
   R"({"message":"OK", "books":[{"author":"Terry Brooks","id":1,"rating":4,"read":true,"title":"Sorcerer's Daughter","userId":1,"year":"2009"},{"author":"James S.A. Corey","id":2,"rating":5,"read":true,"title":"The Expanse","userId":1,"year":"2014"},{"author":"Steven King","id":3,"rating":4,"read":true,"title":"The Stand","userId":1,"year":"1985"},{"author":"Jack McDevitt","id":4,"rating":4,"read":false,"title":"Omega","userId":1,"year":"2005"},{"author":"Terry Brooks","id":5,"rating":4,"read":false,"title":"The Sword of Shannara","userId":1,"year":"1985"},{"author":"Charles De Lint","id":6,"rating":5,"read":true,"title":"Memory And Dream","userId":1,"year":"1985"},{"author":"John Scalzi","id":7,"rating":3,"read":false,"title":"Redshirts","userId":1,"year":"2015"},{"author":"Fred Farmerson","id":8,"rating":3,"read":true,"title":"Test Book","userId":1,"year":"1986"},{"author":"John Scalzi","id":9,"rating":5,"read":true,"title":"Fuzzy Nation","userId":1,"year":"1013"},{"author":"Charles De Lint","id":10,"rating":5,"read":true,"title":"Dreams Underfoot","userId":1,"year":"1988"},{"author":"Steven King","id":11,"rating":4,"read":true,"title":"It","userId":1,"year":"1984"},{"author":"James S.A. Corey","id":12,"rating":4,"read":false,"title":"The Churn","userId":1,"year":"2007"},{"author":"Jack McDevitt","id":13,"rating":4,"read":true,"title":"Starhawk","userId":1,"year":"2015"}]})");
  
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
}

TEST_CASE("Test BookController::getById.")
{
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test getById - ENTER");
   
   BookController bookController;
   JsonResponse jsonResponse = bookController.getById(token, BOOK_ID);

   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() ==
      R"({"message":"OK", "book":{"author":"James S.A. Corey","id":2,"rating":5,"read":true,"title":"The Expanse","userId":1,"year":"2014"}})");
   
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test getById - LEAVE");
}

TEST_CASE("Test BookController::store.")
{
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test store - ENTER");
   
   std::string jsonRequest = R"({"title":"New Title","author":"New Author","year":"2018","read":true,"rating":5})";
   BookController bookController;
   JsonResponse jsonResponse = bookController.store(token, jsonRequest);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Created);
   REQUIRE(jsonResponse.message() ==   R"({"message":"Book saved.", "id":15})");
   
   jsonResponse = bookController.getById(token, NEW_BOOK_ID);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() ==
      R"({"message":"OK", "book":{"author":"New Author","id":15,"rating":5,"read":true,"title":"New Title","userId":1,"year":"2018"}})");
   
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test store - LEAVE");
}

TEST_CASE("Test BookController::update.")
{
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test update - ENTER");
   
   std::string jsonRequest = R"({"userId":1,"title":"Updated Title","author":"Updated Author","year":"2017","read":false,"rating":4})";
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test update - Token is: &", token);
   
   BookController bookController;
   JsonResponse jsonResponse = bookController.getById(token, NEW_BOOK_ID);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() ==
      R"({"message":"OK", "book":{"author":"New Author","id":15,"rating":5,"read":true,"title":"New Title","userId":1,"year":"2018"}})");

   jsonResponse = bookController.update(token, NEW_BOOK_ID, jsonRequest);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() ==   R"({"message":"Book updated."})");
   
   jsonResponse = bookController.getById(token, NEW_BOOK_ID);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() ==
      R"({"message":"OK", "book":{"author":"Updated Author","id":15,"rating":4,"read":false,"title":"Updated Title","userId":1,"year":"2017"}})");
   
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test update - LEAVE");
}

TEST_CASE("Test BookController::remove")
{
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test remove - ENTER");
   
   BookController bookController;
   JsonResponse jsonResponse = bookController.remove(token, NEW_BOOK_ID);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   
   jsonResponse = bookController.getById(token, NEW_BOOK_ID);
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Not_Found);
   REQUIRE(jsonResponse.message() == R"( {"message": "ERROR Book with that id doesn't exist", "book":{}"} )");

   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test remove - LEAVE");
}

TEST_CASE("Test BookController::search")
{
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test search - ENTER");
   
   //std::string jsonRequest = R"({"search":""})";
   BookController bookController;
   JsonResponse jsonResponse = bookController.search(token, "author", "Jack");
   
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() == R"({"message":"OK", "books":[{"author":"Jack McDevitt","id":4,"rating":4,"read":false,"title":"Omega","userId":1,"year":"2005"},{"author":"Jack McDevitt","id":13,"rating":4,"read":true,"title":"Starhawk","userId":1,"year":"2015"}]})");
   
   jsonResponse = bookController.search(token, "title", "Jack");
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() == R"({"message":"OK", "books":[]})");
   
   jsonResponse = bookController.search(token, "title", "Terry%20Brooks");
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() == R"({"message":"OK", "books":[{"author":"Terry Brooks","id":5,"rating":4,"read":false,"title":"The Sword of Shannara","userId":1,"year":"1985"}]})");
   
   jsonResponse = bookController.search(token, "title", "Sword");
   REQUIRE(jsonResponse.code() == Pistache::Http::Code::Ok);
   REQUIRE(jsonResponse.message() == R"({"message":"OK", "books":[{"author":"Terry Brooks","id":5,"rating":4,"read":false,"title":"The Sword of Shannara","userId":1,"year":"1985"}]})");
   
   Logger::instance().log(Logger::LogLevel::INFO, "TEST 05_BookController", "Test search - LEAVE");
}
