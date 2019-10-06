/**
 * @class UserController
 * 
 * Handles the HTTP requests for the Book Manager App.
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    Nov 14, 2017
 */
#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

/*---------  Program Includes  ----------------*/
#include "JsonResponse.h"
#include "UserServices.h"

/*---------  System Includes  -----------------*/
#include <iostream>
#include <memory>
#include <string>

namespace dw {
    
class UserController
{
public:
    
    /*---------  Public Methods  ------------------*/

    /**
     * Constructors and Destructors
     */
    UserController();
    ~UserController();
      
    /**
     * Creates a new user and logs the user in. Returns the response to return to the user.
     * The response contains a JSON string with the authentication token and 200 response code
     * if successful. If the user could not be registered, an error message and 401 code.
     * 
     * @param jsonData string The registration parameters as a JSON string.
     * @return JSON Response
     */
    JsonResponse 
    registerUser (std::string jsonData);
    
    /**
     * Login a user and return a token.
     * 
     * @param jsonData The JSON data sent in the request.
     * @return JSON Response
     */
    JsonResponse 
    loginUser(std::string jsonData);
    
    /**
     * Logout a user.
     * 
     * @param token The token of the user to logout.
     * @return JSON Response
     */
    JsonResponse
    logoutUser(std::string token);
    
private:
   
   /*---------  Private Data   -------------------*/
};

} // End namespace dw

#endif // USERCONTROLLER_H
