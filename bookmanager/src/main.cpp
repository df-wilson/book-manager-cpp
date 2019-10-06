
#include <iostream>
#include <string>
#include "ConfigReader.h"
#include "Logger.h"
#include "WebServer.h"
#include "pistache/http.h"

using namespace std;
using namespace Pistache;
using namespace dw;

int main(int argc, char *argv[]) {
    
    // Defaults. Can be overridden by command line args. 
    Pistache::Port port(9080);
    int threads = 2;

    // First arg is port, second is number of threads to use.
    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            threads = std::stol(argv[2]);
    }

    // Find the server's working directory
    string serverpath(argv[0]);
    serverpath = serverpath.substr(0, serverpath.find_last_of("/"));

    // Set up the config
    try
   {
      string configValue = ConfigReader::getInstance().getConfig(ConfigReader::Config::LOG_LEVEL);
      Logger::instance().log(Logger::LogLevel::INFO, "main", "Config init &.", configValue);
   }
   catch (out_of_range e) {
         cout << "Config failed to initialize." << endl;
         return EXIT_FAILURE;
   }
   
   
    cout << "Server home " << serverpath << endl;
    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << threads << " threads" << endl;
    cout << "Port: 9080 " << endl;

    Pistache::Address addr(Pistache::Ipv4::any(), port);
    dw::WebServer server(addr, serverpath);

    server.init(threads);
    server.start();
    
    server.shutdown();
    
   return EXIT_SUCCESS;
}

