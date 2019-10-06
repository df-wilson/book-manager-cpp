#include "catch.hpp"

#include "../src/UserController.h"
#include "JsonResponse.h"
#include "../src/TokenRepository.h"
#include <iostream>

using namespace dw;
using namespace std;

const int USER_ID = 1;
const int BOOK_ID = 2;
const int NEW_BOOK_ID = 15;

//string token;

TEST_CASE("Test UserController::registerUser.") 
{
   std::string jsonRequest = R"({"name":"test","email":"test@tester.com","password":"tester"})";
   UserController userController;
   JsonResponse response = userController.registerUser(jsonRequest);
   std::string message = response.message();
   
   REQUIRE(response.code() == Pistache::Http::Code::Created);
   REQUIRE(message.find(R"("message":"ok")") != string::npos);
   REQUIRE(message.find(R"("token":)") != string::npos);
   size_t tokenpos = message.find(R"("token":)");
   std::string token =  message.substr(tokenpos);
   tokenpos = token.find(":\"");
   token = token.substr(tokenpos+1);
   
   // Ensure token is a valid token size. Add two to the size for the extra quotes.
   REQUIRE(token.length() > 12);
   REQUIRE(token.length() < 22);
}

TEST_CASE("Test UserController::LoginUser.")
{
   std::string jsonRequest = R"({"email":"test@tester.com","password":"tester"})";
   UserController userController;
   JsonResponse response = userController.loginUser(jsonRequest);
   std::string message = response.message();
   
   REQUIRE(response.code() == Pistache::Http::Code::Ok);
   REQUIRE(message.find(R"("message":"ok")") != string::npos);
   REQUIRE(message.find(R"("token":)") != string::npos);
   size_t tokenpos = message.find(R"("token":)");
   std::string token =  message.substr(tokenpos);
   tokenpos = token.find(":\"");
   token = token.substr(tokenpos+1);
   
   // Ensure token is a valid token size. Add two to the size for the extra quotes.
   REQUIRE(token.length() > 12);
   REQUIRE(token.length() < 22);
   
   // Test invalid password
   jsonRequest = R"({"email":"test@tester.com","password":"invalid"})";
   response = userController.loginUser(jsonRequest);
   message = response.message();
   
   REQUIRE(response.code() == Pistache::Http::Code::Unauthorized);
   REQUIRE(response.message() == R"({"message":"Login Failed"})");
   
     // Test invalid password
   jsonRequest = R"({"email":"invalid@tester.com","password":"tester"})";
   response = userController.loginUser(jsonRequest);
   message = response.message();
   
   REQUIRE(response.code() == Pistache::Http::Code::Unauthorized);
   REQUIRE(response.message() == R"({"message":"Login Failed"})");   
}
