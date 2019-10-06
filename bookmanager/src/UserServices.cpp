/*---------  Program Includes  ----------------*/
#include "Logger.h"
#include "UserServices.h"
#include "UserRepository.h"
#include "TokenRepository.h"
#include "User.h"

/*---------  System Includes  -----------------*/
#include <string>


using namespace std;

namespace dw {
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
UserServices::UserServices()
{

}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
UserServices::~UserServices()
{

}

/******************************************************************************
 * Name: registerUser
 * Desc: Create a new user in the database, then create and return a token for 
 * the user. 
 ******************************************************************************
 */  
string 
UserServices::registerUser(const User& user)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserServices", "registerUser. ENTER.");
      
   string token("");
   long newId = mUserRepository.store(user);
   
   if(newId) {
      TokenRepository tokenRepository;
      token = tokenRepository.create(newId);
      
   }
   
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserServices", "registerUser. LEAVE - Token &", token);

   return token;
}

/******************************************************************************
 * Name: loginUser
 * Desc: Login a user and return an authentication token. 
 ******************************************************************************
 */
string 
UserServices::loginUser(const string& email, const string& password)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserServices", "loginUser. ENTER. Email: &", email);
     
   string token("");
   TokenRepository tokenRepository;
   UserRepository userRepository;
   
   long userId = userRepository.getUserId(email, password);
   if(userId) {
      TokenRepository tokenRepository;
      token = tokenRepository.create(userId);      
   }
   
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserServices", "loginUser. LEAVE");
   
   return token;
}

/******************************************************************************
 * Name: logoutUser
 * Desc: Logout a user and remove the authentication token from the database. 
 ******************************************************************************
 */
bool
UserServices::logoutUser(std::string token)
{
   Logger::instance().log(Logger::LogLevel::DEBUG, "UserServices", "logoutUser. ENTER.");
     
   TokenRepository tokenRepository;
   bool isSuccessful = tokenRepository.remove(token);
   
   return isSuccessful;
}


} // end namespace dw
