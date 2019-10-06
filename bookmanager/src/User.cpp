/*---------  Program Includes  ----------------*/
#include "User.h"
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
User::User(int id,
           const std::string& name, 
           const std::string& email, 
           const std::string& password
          )
     :mId(id), mName(name), mEmail(email), mPassword(password)
{
}

/******************************************************************************
 * Constructor
 ******************************************************************************
 */   
dw::User::User ( const std::string& jsonString )
{
   parseJsonString(jsonString);
}

User& User::operator= (const User& rhs)
{
   if (this != &rhs) // protect against invalid self-assignment
   {
      mId = rhs.id();
      mName = rhs.name();
      mEmail = rhs.email();
      mPassword = rhs.password();
   }
   
   return *this;
}

/******************************************************************************
 * Getters
 ******************************************************************************
 */
int User::id() const
{
   return mId;
}

std::string User::name() const
{
   return mName;
}

std::string User::email() const
{
   return mEmail;
}

std::string User::password() const
{
   return mPassword;
}


/******************************************************************************
 * Name: toJson
 * Desc: Returns the user data as a JSON string
 ******************************************************************************
 */   
string User::toJson() const
{
    nlohmann::json j;
   j["id"] = mId;
   j["name"] = mName;
   j["email"] = mEmail;
   j["password"] = mPassword;
   
   return j.dump();
}

/******************************************************************************
 * Name: toString
 * Desc: Returns the user data as a string with a dash between each element.
 ******************************************************************************
 */   
string User::toString() const
{
   string isReadString = "false";
   
   ostringstream userString;
   userString << mId << " - " << mName << " - " << mEmail << " - " << mPassword;
   return userString.str();
}

/******************************************************************************
 * Name: parseJsonString
 * Desc: Parse the given JSON string to obtain the user data. 
 ******************************************************************************
 */   
void User::parseJsonString(const std::string& jsonString)
{
   auto data = nlohmann::json::parse(jsonString);
   Logger::instance().log(Logger::LogLevel::INFO, "User", "Parse JSON string &.", data);
   //cout << "In User::parseJsonString - Data is " << data << " size " << data.size() << endl;
     
   if(data.size() < 3) {
      throw std::runtime_error("Invalid JSON string. Incorrect number of elements");
   }
   
   // ID is optional. Default to 0
   if(data.find("id") != data.end() && data["id"].is_number_integer()) {
      mId = data["id"];
   } else {
      mId = 0;
   }
   
   if(data.find("name") != data.end() && data["name"].is_string()) {
      mName = data["name"];
   } else {
      throw std::runtime_error("Invalid JSON string. Name must be string.");
   }
   
   if(data.find("email") != data.end() && data["email"].is_string()) {
      mEmail = data["email"];
   } else {
      throw std::runtime_error("Invalid JSON string. email must be string");
   }
   
   if(data.find("password") != data.end() && data["password"].is_string()) {
      mPassword = data["password"];
   } else {
      throw std::runtime_error("Invalid JSON string. password must be string.");
   }
}

} // End namespace dw
