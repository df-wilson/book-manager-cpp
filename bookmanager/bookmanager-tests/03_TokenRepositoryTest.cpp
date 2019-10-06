#include "catch.hpp"
#include "../src/TokenRepository.h"

#include <iostream>
#include <string>

using namespace dw;
using namespace std;

const long NEW_USER_ID = 1;

TEST_CASE("Test token functionality") 
{
   TokenRepository tokenRepository;
   string token = tokenRepository.create(NEW_USER_ID);
   size_t length = token.length();
   cout << "String length " << length << endl;
   REQUIRE(length > 10);
   REQUIRE(length < 20);
   
   long user_id = tokenRepository.getUserIdForToken(token);
   REQUIRE(user_id == NEW_USER_ID);
   
   bool isValid = tokenRepository.exists(token);
   REQUIRE(isValid == true);
   
   // If token already exist return it, don't create a new one.
   string token2 = tokenRepository.create(NEW_USER_ID);
   REQUIRE(token2 == token);
}

