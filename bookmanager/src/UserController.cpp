/*---------  Program Includes  ----------------*/
#include "Logger.h"
#include "UserController.h"
#include "UserServices.h"
#include "User.h"

/*---------  System Includes  -----------------*/
#include <memory>
#include <string>
#include "json.hpp"

namespace dw {

/******************************************************************************
 * Constructor
 ******************************************************************************
 */
UserController::UserController()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
UserController::~UserController()
{
}

/******************************************************************************
 * Name: registerUser
 * Desc: Parse the given JSON string to obtain the user data. 
 ******************************************************************************
 */   
JsonResponse 
UserController::registerUser(std::string jsonData)
{
   Logger::instance().log(Logger::LogLevel::INFO, "UserController", "registerUser. JSON: &.", jsonData);
   
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
   std::ostringstream json;
   
   User user(jsonData);
   UserServices userServices;
   std::string token = userServices.registerUser(user);
   
   if(token.size() > 0) {
      json << "{\"message\":\"ok\", \"token\":\"" << token << "\"}";
      code = Pistache::Http::Code::Created;
   } else {
      Logger::instance().log(Logger::LogLevel::INFO, "UserController", "registerUser. Invalid token: &.", token);
      json << "{\"message\":\"ERROR. User not registered\"}";
      code = Pistache::Http::Code::Internal_Server_Error;
   }
   
   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: loginUser
 * Desc: Parse the given JSON string for user credentials and attempt to log in.
 ******************************************************************************
 */   
JsonResponse
UserController::loginUser(std::string jsonData)
{
   Logger::instance().log(Logger::LogLevel::INFO, "UserController", "login. JSON: &.", jsonData);
   
   // Parse the json string to get the username and password
   auto data = nlohmann::json::parse(jsonData);
   std::string email;
   std::string password;
   
   if(data.size() < 2) {
      throw std::runtime_error("Invalid JSON string. Incorrect number of elements");
   }
   
   if(data.find("email") != data.end() && data["email"].is_string()) {
      email = data["email"];
   } else {
      throw std::runtime_error("Invalid JSON string. Email must be string.");
   }
   
   if(data.find("password") != data.end() && data["password"].is_string()) {
      password = data["password"];
   } else {
      throw std::runtime_error("Invalid JSON string. Password must be string.");
   }
   
   // Login the user and return the response
   UserServices userServices;
   std::string response = userServices.loginUser(email, password);

   std::ostringstream json;
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;
      
   if(response.length() > 0) {
      Logger::instance().log(Logger::LogLevel::DEBUG, "UserController", "loginUser. token: &.", response);
      json << "{\"message\":\"ok\", \"token\":\"" << response << "\"}";
      code = Pistache::Http::Code::Ok;
         
   } else {
      json << "{\"message\":\"Login Failed\"}";
      code = Pistache::Http::Code::Unauthorized;
   }
   
   return JsonResponse(json.str(), code);
}

/******************************************************************************
 * Name: logoutUser
 * Desc: Parse the given JSON string for user credentials and attempt to log in.
 ******************************************************************************
 */
JsonResponse
UserController::logoutUser(std::string jsonData)
{
   Logger::instance().log(Logger::LogLevel::INFO, "UserController", "logout.");
   
   std::string token;
   bool isSuccessful = false;

   // Parse the json string to get the token
   auto data = nlohmann::json::parse(jsonData);   
   if(data.size() != 1) {
      throw std::runtime_error("Invalid JSON string. Incorrect number of elements");
   }
   
   if(data.find("token") != data.end() && data["token"].is_string()) {
      token = data["token"];
      isSuccessful = true;
   }
      
   UserServices userServices;
   if(isSuccessful) {
      isSuccessful = userServices.logoutUser(token);
   }
   
   std::ostringstream json;
   Pistache::Http::Code code = Pistache::Http::Code::Internal_Server_Error;

   if (isSuccessful) {
      json << "{\"message\":\"ok\"}";
      code = Pistache::Http::Code::Ok;
   } else {
      json << "{\"message\":\"not found\"}";
      code = Pistache::Http::Code::Not_Found;      
   }
            
   return JsonResponse(json.str(), code);
}

} // End namespace dw
