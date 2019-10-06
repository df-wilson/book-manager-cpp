/**
 * @class User
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 * 
 * Description:
 * Stores the data for a user object. The object can be constructed either by passing
 * in the individual items or via a JSON string. If a valid user object cannot be 
 * created fron the JSON string, an std::runtime_error exception is thrown. 
 * 
 * There is no default constructor, so an empty or default object cannot be created.
 * 
 * Once the object is created, it cannot be modified.
 * 
 * The individual components may be retrieved, or the object can be retrieved as a 
 * JSON string. 
 * 
 */
#ifndef USER_H
#define USER_H

/*---------  System Includes  -----------------*/
#include <string>

namespace dw {
   
class User final
{
   public:
   
   /*---------  Public Functions  ---------------*/
   
   /**
    * Constructors and Destructors
    */
   User() = delete;
   User(int id,
        const std::string& name, 
        const std::string& email, 
        const std::string& password);
   explicit User(const std::string& jsonString);
   virtual ~User() = default;
   
   User& operator= (const User& other);
   
   /**
    * Get methods.
    */
   int id() const;
   std::string name() const;
   std::string email() const;
   std::string password() const;
   
   /**
    * Get the user data as a string.
    * 
    * @return the user data as a string.
    */
   std::string toString() const;
   
   /**
    * Get the object data a string in JSON format.
    * 
    * @return the user data as a string in JSON format.
    */
   std::string toJson() const;
   
private:
   
   /*---------  Private Functions ---------------*/
   
   /**
    * Parses a JSON string containing user data. If the JSON data is invalid or does not
    * contain the expected data, an std::runtime_error exception is thrown.
    */
   void parseJsonString(const std::string& jsonString);
  
   
   /*---------  Private Data  ------------------*/
   
   int         mId;
   std::string mName;
   std::string mEmail;
   std::string mPassword;
};

} // End namespace dw

#endif // USER_H
