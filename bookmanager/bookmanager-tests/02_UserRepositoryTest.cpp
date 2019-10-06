#include "catch.hpp"

#include "../src/User.h"
#include "../src/UserRepository.h"
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

using namespace dw;
using namespace std;

const string NAME = "Dean";
const string EMAIL = "test@tester.com";
const string NEW_PASSWORD = "New Password";
const string PASSWORD = "password";
const string JSON = R"({"id": 1, "name": "Dean", "email": "test@tester.com", "password": "password"})";
const long   NEW_USER_ID = 1;

TEST_CASE( "Test User class." ) 
{
    User user(0, NAME, EMAIL, PASSWORD);
    REQUIRE (user.name() == NAME);
    REQUIRE (user.email() == EMAIL);
    REQUIRE (user.password() == PASSWORD);
    
    User userJSON(JSON); 
    REQUIRE (userJSON.name() == NAME);
    REQUIRE (userJSON.email() == EMAIL);
    REQUIRE (userJSON.password() == PASSWORD);

    REQUIRE (user.toString() ==   "0 - Dean - test@tester.com - password");
    
    // Test the toJson functionality. Note the order of the elements is not preserved.
    string json = user.toJson();
    User userCopy(json);
    REQUIRE (userJSON.name() == NAME);
    REQUIRE (userJSON.email() == EMAIL);
    REQUIRE (userJSON.password() == PASSWORD);    
}

TEST_CASE( "Test UserRepository class. Create and get by ID." ) 
{
   User user(0, NAME, EMAIL, PASSWORD);
   UserRepository repository;
    
   long count = repository.count();
   REQUIRE(count == 0);
   
   long id = repository.store(user);
   REQUIRE (id == 1);

   count = repository.count();
   REQUIRE(count == 1);
    
   User user2 = repository.getById(id);
   REQUIRE (user2.id() == 1);
   REQUIRE (user2.name() == NAME);
   REQUIRE (user2.email() == EMAIL);
   REQUIRE (user2.password() == PASSWORD);
}

TEST_CASE("Test UserRepository class. Test updating user information.")
{
   const long userId = 1; 
   UserRepository repository;
   repository.updatePassword(userId, NEW_PASSWORD);
   
   User user = repository.getById(userId);
   REQUIRE (user.id() == userId);
   REQUIRE (user.name() == NAME);
   REQUIRE (user.email() == EMAIL);
   REQUIRE (user.password() == NEW_PASSWORD);
   
   long count = repository.count();
   REQUIRE(count == 1);
}

TEST_CASE("Test UserRepository class. Test checking for valid id/password combination.")
{
   UserRepository repository;
   bool isValid = repository.getUserId(EMAIL, NEW_PASSWORD);
   REQUIRE(isValid == true);
}

TEST_CASE("UserRepository class. Test removing a user from the database.")
{
   UserRepository repository;
   repository.remove(1);
   long count = repository.count();
   REQUIRE(count == 0);
}

