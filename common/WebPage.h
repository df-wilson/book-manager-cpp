/**
 * @class   WebPage
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 * 
 * Abstract base class for creating web pages.
 */
#ifndef WEBPAGE_H
#define WEBPAGE_H

/*---------  System Includes  -----------------*/

#include <string>

namespace dw {
    
/*---------  Class Definition  ----------------*/

class WebPage
{
public:
   
   /**
    * Constructor and destructor
    */
    WebPage() = default;
    virtual ~WebPage() = default;
    
    /**
     * Override this method to return the page content as a string.
     * 
     * @return The index.html page.
     */
    virtual std::string getPage() = 0;
};
    
}

#endif // WEBPAGE_H
