/**
 * @class WebServer
 * 
 * Setup the server, create the routes, and manage user requests.
 * 
 * @author  Dean Wilson
 * @version 1.1
 * @date    Feb 2, 2018
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

/*---------  System Includes  --------------*/
#include <memory>
#include <string>

#include "pistache/net.h"
#include "pistache/http.h"
#include "pistache/peer.h"
#include "pistache/http_headers.h"
#include "pistache/cookie.h"
#include "pistache/endpoint.h"
#include "pistache/router.h"

/*---------  Program Includes  ----------------*/
#include "BookController.h"
#include "UserController.h"
#include "IndexPage.h"

namespace dw {
    
class WebServer
{
public:
   
   /*---------  Public Methods  ------------------*/
   
   /**
    * Constructors and Destructors
    */
    WebServer(Pistache::Address addr, std::string serverpath);
    virtual ~WebServer();
    
    /**
     * Initialize the web server.
     * 
     * @param threads The number of threads to use. Defaults to 2.
     */
    void init(size_t threads = 2);
    
    /**
     * Start the webserver.
     */
    void start();
    
    /**
     * Stop the webserver.
     */
    void shutdown();

private:
   
   /*----------------- Private Methods  ------------*/
   
    void handleIndex(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleGetBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handlePostBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handlePutBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleGetBookById(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleDeleteBook(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handlePostLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handlePostRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    
    void setupRoutes();
    void serveCss(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void serveFonts(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void serveImage(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void serveJs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void serveUnknown(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    
    std::string getUrlToken(const Pistache::Rest::Request& request);
    
    
   /*----------------- Private Data  -----------------------*/
    const std::string mServerPath;
    std::shared_ptr<Pistache::Http::Endpoint> mHttpEndpoint;
    Pistache::Rest::Router router;
    
    // Web page objects
    std::shared_ptr<WebPage> mIndexPage;
};
    
}

#endif // WEBSERVER_H
