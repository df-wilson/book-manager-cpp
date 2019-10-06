#include "catch.hpp"
#include "../src/UserServices.h"

#include <iostream>
#include <string>

using namespace dw;
using namespace std;

const string NAME = "Dean";
const string EMAIL = "test@tester.com";
const string PASSWORD = "Password";
const long   NEW_USER_ID = 1;
const long   INVALID_USER_ID = 999;
const string INVALID_PASSWORD = "Invalid";
    
TEST_CASE("Test Registering user.") 
{
   User user(0, NAME, EMAIL, PASSWORD);
   UserServices userServices;
   string token = userServices.registerUser(user);
   
   REQUIRE(token.length() > 10);
   REQUIRE(token.length() < 20);
}

TEST_CASE("Test login functionality")
{
   UserServices userServices;
   string token = userServices.loginUser(EMAIL, PASSWORD);
      
   REQUIRE(token.length() > 10);
   REQUIRE(token.length() < 20);
   
   token = userServices.loginUser(EMAIL, INVALID_PASSWORD);
   REQUIRE(token.length() == 0);
}
