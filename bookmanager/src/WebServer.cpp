/*---------  Program Includes  ----------------*/
#include "WebServer.h"
#include "BookController.h"
#include "UserController.h"
#include "IndexPage.h"
#include "JsonResponse.h"
#include "dbConnect.h"
#include "Logger.h"

/*---------  System Includes  --------------*/
#include <iostream>
#include <memory>

using namespace std;

namespace dw {
   
/******************************************************************************
 * Constructor
 ******************************************************************************
 */
WebServer::WebServer(Pistache::Address addr, std::string serverpath)
          : mServerPath(serverpath),
            mHttpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr))
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "Construct.");

    //mBookController = std::make_shared<BookController>();
    //mUserController = std::make_shared<UserController>();
    
    mIndexPage = std::make_shared<IndexPage>();
}

/******************************************************************************
 * Destructor
 ******************************************************************************
 */
WebServer::~WebServer()
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "Destruct.");
   
   std::cout << "Shutting down server" << std::endl;
   db_shutdown();
   shutdown();
}

/******************************************************************************
 * Name: init
 * Desc: Initialize the webserver.
 ******************************************************************************
 */   
void WebServer::init(size_t threads)
{
    Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "Num threads &.", threads);
    
    auto options = Pistache::Http::Endpoint::options()
        .threads(threads);
        
    mHttpEndpoint->init(options);
    
    setupRoutes();
}

/******************************************************************************
 * Name: shutdown
 * Desc: Stop the webserver.
 ******************************************************************************
 */ 
void WebServer::shutdown()
{
    mHttpEndpoint->shutdown();
}

/******************************************************************************
 * Name: start
 * Desc: Start the webserver.
 ******************************************************************************
 */ 
void WebServer::start()
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "start");
   
    mHttpEndpoint->setHandler(router.handler());
    mHttpEndpoint->serve();
}


/*----------------- Private Methods ---------------------*/

/******************************************************************************
 * Name: setupRoutes
 * Desc: Setup the routes that the webserver handles.
 ******************************************************************************
 */ 
void WebServer::setupRoutes()
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "setupRoutes");
   
    Pistache::Rest::Routes::Get(router, 
                "/", 
                Pistache::Rest::Routes::bind(&WebServer::handleIndex, this));
    Pistache::Rest::Routes::Get(router, 
                "/css/*", 
                Pistache::Rest::Routes::bind(&WebServer::serveCss, this));
    Pistache::Rest::Routes::Get(router, 
                "/fonts/*", 
                Pistache::Rest::Routes::bind(&WebServer::serveFonts, this));
    Pistache::Rest::Routes::Get(router, 
                "/img/*", 
                Pistache::Rest::Routes::bind(&WebServer::serveImage, this));
    Pistache::Rest::Routes::Get(router, 
                "/js/*", 
                Pistache::Rest::Routes::bind(&WebServer::serveJs, this));
   
    // Api Routes
    Pistache::Rest::Routes::Delete(router, 
                "/api/v1/books/:id", 
                Pistache::Rest::Routes::bind(&WebServer::handleDeleteBook, this));
    
    Pistache::Rest::Routes::Get(router, 
                "/api/v1/books/:id", 
                Pistache::Rest::Routes::bind(&WebServer::handleGetBookById, this));
    
    Pistache::Rest::Routes::Get(router, 
                "/api/v1/books", 
                Pistache::Rest::Routes::bind(&WebServer::handleGetBooks, this));
    
    Pistache::Rest::Routes::Post(router,
                 "/api/v1/books",
                 Pistache::Rest::Routes::bind(&WebServer::handlePostBooks, this));
   
    Pistache::Rest::Routes::Put(router,
                 "/api/v1/books/:id",
                 Pistache::Rest::Routes::bind(&WebServer::handlePutBooks, this));
   
    Pistache::Rest::Routes::Post(router,
                 "/api/v1/auth/login",
                 Pistache::Rest::Routes::bind(&WebServer::handlePostLogin, this));

    Pistache::Rest::Routes::Post(router,
                 "/api/v1/auth/register",
                 Pistache::Rest::Routes::bind(&WebServer::handlePostRegister, this));

    Pistache::Rest::Routes::Post(router,
                 "/api/v1/auth/logout",
                 Pistache::Rest::Routes::bind(&WebServer::handleLogout, this));

    // Catch unknown pages
    Pistache::Rest::Routes::Get(router, 
                "/*", 
                Pistache::Rest::Routes::bind(&WebServer::serveUnknown, this));
    
}

/******************************************************************************
 * Name: handleIndex
 * Desc: Handle the index route.
 ******************************************************************************
 */ 
void WebServer::handleIndex(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleIndex");
   
   response.send(Pistache::Http::Code::Ok, mIndexPage->getPage());
}

/******************************************************************************
 * Name: serveCss
 * Desc: Serve the css file requests.
 ******************************************************************************
 */ 
void WebServer::serveCss(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   auto splat = request.splatAt(0).as<std::string>();
    std::string cssFile = mServerPath + "/css/" + splat;
    
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "serveCss(). Serving &.", cssFile);
    
   try {
      auto result = Pistache::Http::serveFile(response, cssFile.c_str());
      result.then([](ssize_t bytes) 
         {},
         Pistache::Async::NoExcept
      );
   } catch(exception& e) {
      Logger::instance().log(Logger::LogLevel::ERROR, "WebServer", "ERROR: serveCss(). File not found &.", cssFile);
      std::cout << "The requested css file not found. " << cssFile << std::endl;
   }
}

/******************************************************************************
 * Name: serveFonts
 * Desc: Serve the font file requests.
 ******************************************************************************
 */ 
void WebServer::serveFonts(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   auto splat = request.splatAt(0).as<std::string>();
    std::string fontFile = mServerPath + "/fonts/" + splat;
    
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "serveFonts. Serving &.", fontFile);
   
   try {
   auto result = Pistache::Http::serveFile(response, fontFile.c_str());
   result.then([](ssize_t bytes) 
      {},
      Pistache::Async::NoExcept
   );
   } catch(exception& e) {
      Logger::instance().log(Logger::LogLevel::ERROR, "WebServer", "serveFonts(). ERROR: File not found &.", fontFile);
   }
}

/******************************************************************************
 * Name: serveImage
 * Desc: Handles the image requests. Files expected in /img directory.
 ******************************************************************************
 */
void WebServer::serveImage(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   auto splat = request.splatAt(0).as<std::string>();
   std::string imgFile = mServerPath + "/img/" + splat;
    
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "serveImg(). Serving &.", imgFile);
   
   try {
      auto result = Pistache::Http::serveFile(response, imgFile.c_str());
      result.then([](ssize_t bytes) 
         {},
         Pistache::Async::NoExcept
      );
   } catch(exception& e) {
      Logger::instance().log(Logger::LogLevel::ERROR, "WebServer", "serveImage(). ERROR: File not found &.", imgFile);
   }
}

/******************************************************************************
 * Name: serveJs
 * Desc: Handles the javascript requests. Files expected in /js directory.
 ******************************************************************************
 */
void WebServer::serveJs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   auto splat = request.splatAt(0).as<std::string>();
   std::string jsFile = mServerPath + "/js/" + splat;
    
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "serveJs(). Serving &.", jsFile);
   
   try {
      response.headers().add<Pistache::Http::Header::ContentType>(MIME(Text, Javascript));
      auto result = Pistache::Http::serveFile(response, jsFile.c_str());
      result.then([](ssize_t bytes) 
         {},
         Pistache::Async::NoExcept
      );
   } catch(exception& e) {
      Logger::instance().log(Logger::LogLevel::ERROR, "WebServer", "serveJs(). ERROR: File not found &.", jsFile);
   }
}

/******************************************************************************
 * Name: handleGetBooks
 * Desc: Handles delete requests.
 ******************************************************************************
 */  
void WebServer::handleDeleteBook(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   int id = request.param(":id").as<int>();
   std::string token = getUrlToken(request);
      
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleDeleteBook(). Serving &.", to_string(id));
   
   try {
      BookController controller;
      JsonResponse jsonResponse = controller.remove(token, id);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleDeleteBook(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Error occurred when deleting book.");
   }
}

/******************************************************************************
 * Name: handleGetBooks
 * Desc: Handles GET request for all books.
 ******************************************************************************
 */  
void WebServer::handleGetBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleGetBooks().");

   std::string token = getUrlToken(request);
   
   BookController controller;
   JsonResponse jsonResponse = controller.getBooks(token);
   response.setMime(MIME(Application, Json));
   response.send(jsonResponse.code(), jsonResponse.message());
}

/******************************************************************************
 * Name: handleGetBookById
 * Desc: Handles GET requests for single book.
 ******************************************************************************
 */  
void WebServer::handleGetBookById(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   std::string token = getUrlToken(request);
   int id = request.param(":id").as<int>();
   
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleGetBookById(). Message: &.", to_string(id));
   
   BookController controller;
   JsonResponse jsonResponse = controller.getById(token, id);
   
   response.setMime(MIME(Application, Json));
   response.send(jsonResponse.code(), jsonResponse.message());
}

/******************************************************************************
 * Name: handlePostBooks
 * Desc: Handles POST requests.
 ******************************************************************************
 */  
void WebServer::handlePostBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   std::string message = request.body();
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostBooks(). Message: &.", message);
  
   std::string token = getUrlToken(request);
   
   try {
      BookController controller;
      JsonResponse jsonResponse = controller.store(token, message);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostBooks(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Server error occurred when adding book.");
   }
}

/******************************************************************************
 * Name: handlePutBooks
 * Desc: Handles PUT requests.
 ******************************************************************************
 */   
void WebServer::handlePutBooks(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   int id = request.param(":id").as<int>();
   
   std::string message = request.body();
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePutBooks(). Message: &.", message);
  
   std::string token = getUrlToken(request);
      
   try {
      BookController controller;
      JsonResponse jsonResponse = controller.update(token, id, message);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePutBooks(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Server error occurred when updating adding book.");
   }
}

/******************************************************************************
 * Name: handlePostLogin
 * Desc: Handles the POST request /api/v1/login.
 ******************************************************************************
 */
void 
WebServer::handlePostLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   std::string message = request.body();
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostLogin(). Message: &.", message);
  
   try {
      UserController controller;
      JsonResponse jsonResponse = controller.loginUser(message);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostLogin(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Server error occurred when adding book.");
   }
}

/******************************************************************************
 * Name: handlePostRegister
 * Desc: Handles the POST request /api/v1/register.
 ******************************************************************************
 */
void 
WebServer::handlePostRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   std::string message = request.body();
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostRegister(). Message: &.", message);
  
   try {
      UserController controller;
      JsonResponse jsonResponse = controller.registerUser(message);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handlePostRegister(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Server error occurred when adding book.");
   }
}

/******************************************************************************
 * Name: handleLogout
 * Desc: Handles the POST request /api/v1/logout.
 ******************************************************************************
 */
void 
WebServer::handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   std::string message = request.body();
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleLogout().Message: &.", message);
     
   try {
      UserController controller;
      JsonResponse jsonResponse = controller.logoutUser(message);
      response.setMime(MIME(Application, Json));
      response.send(jsonResponse.code(), jsonResponse.message());
   } catch (exception& e) {
      Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "handleLogout(). ERROR: &.", e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, "Server error occurred when logging out user.");
   }
}

/******************************************************************************
 * Name: serveUnknown
 * Desc: Handles requests the server does not know how to serve.
 ******************************************************************************
 */ 
void WebServer::serveUnknown(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
   Logger::instance().log(Logger::LogLevel::INFO, "WebServer", "serveUnknown(). Page not found: &.", request.resource());
   
   response.send(Pistache::Http::Code::Not_Found, "Page not found");
}

/******************************************************************************
 * Name: getUrlToken
 * Desc: Extract the token from the URL.
 ******************************************************************************
 */ 
std::string 
WebServer::getUrlToken(const Pistache::Rest::Request& request)
{
   std::string token("");
      
   if(request.query().has("token")) {
   auto queryVal = request.query().get("token");
   token = queryVal.getOrElse("");
   }
   
   return token;
}

} // End namespace dw
