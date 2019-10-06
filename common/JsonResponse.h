/**
 * @class   JsonResponse
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 * 
 * Contains the response message and HTTP code.
 */

#ifndef JSONRESPONSE_H
#define JSONRESPONSE_H

/*---------  System Includes  -----------------*/
#include "pistache/http_defs.h"

class JsonResponse
{
public:
   
   /*---------  Public Methods  ------------------*/
   
   /**
    * Constructor.
    * 
    * @param message The response message
    * @param code the HTTP response code
    */
   JsonResponse(std::string message, Pistache::Http::Code code) : mMessage(message), mCode(code) 
   {
      
   }
   
   /**
    * Get methods.
    */
   std::string message() { return mMessage; }
   Pistache::Http::Code code() { return mCode; }
  
private:
   
   /*---------  Private Data   -------------------*/
   
   std::string mMessage;
   Pistache::Http::Code mCode; 
};

#endif // JSONRESPONSE_H
