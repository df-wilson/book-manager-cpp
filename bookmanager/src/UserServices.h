/**
 * @class UserServices
 * 
 * Provides user services such as authentication.
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    Nov 16, 2017
 */

#ifndef USERSERVICES_H
#define USERSERVICES_H

/*---------  Program Includes  ----------------*/
#include "User.h"
#include "UserRepository.h"
#include "JsonResponse.h"

/*---------  System Includes  -----------------*/
#include <string>

namespace dw {

class UserServices
{
public:
    UserServices();
    ~UserServices();

    /**
    * Create a new account for a user.
    * 
    * @param user The user details
    * @return A valid user authentication token.
    */    
    std::string 
    registerUser(const User& user);

    /**
    * Login a user. If successful return an authentication token, otherwise return
    * an empty string.
    * 
    * @param email the user's email address
    * @param password the user's password
    * @return A valid user authentication token.
    */    
    std::string 
    loginUser(const std::string& email, const std::string& password);
    
    /**
     * Logout the user with the given token.
     * 
     * @param token the user's token
     * @return true if the user was successfully logged out.
     */
    bool
    logoutUser(std::string token);
    
private:
   /*---------  Private Data   -------------------*/
   UserRepository mUserRepository;
};

} // end namespace dw

#endif // USERSERVICES_H
