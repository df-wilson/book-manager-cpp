/**
 * @class IndexPage
 * 
 * Creates the index.html page.
 * 
 * @author  Dean Wilson
 * @version 1.0
 * @date    March 25, 2017
 */

#ifndef INDEXPAGE_H
#define INDEXPAGE_H

/*---------  Program Includes  ----------------*/
#include "WebPage.h"

/*---------  System Includes  -----------------*/
#include <string>

namespace dw {
   
/*---------  Class Definition  ----------------*/

class IndexPage :  public WebPage
{
public:
   /*---------  Public Methods  ------------------*/
   
   /**
    * Constructor and destructor
    */
    IndexPage();
    virtual ~IndexPage() = default;
    
    
    /**
     * Get the index.html page as a string.
     * 
     * @return The index.html page.
     */
    std::string getPage() override;
    
private:
   
   /*---------  Public Methods  ------------------*/
   
    void createPage();
    
   /*---------  Private Data   -------------------*/
   
    std::string mPage;
};

} // End namespace dw

#endif // INDEXPAGE_H
