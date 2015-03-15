/* Socket.io client C++ demo
 * Evan Shimizu, June 2012
 *
 * Demonstrates basic usage of the socket.io client.
 * Sets up a connection, registers an event handler, and sends an event.
 * Uses the test.js server provided with this example code.
 * Note that the events the server sends back are not bound to anything, but are logged
 * in the output of the application with full logging enabled.
 */

//#include "stdafx.h"

#include <socket_io_client.hpp>
#include <thread>
#include <chrono>
#include <boost/thread/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
using namespace socketio;

int main(int /*argc*/, char* /*argv*/ []) {
   // websocket++ expects urls to begin with "ws://" not "http://"
   std::string uri = "ws://127.0.0.1:8080";

   try {
      // Create and link handler to websocket++ callbacks.
      socketio_client_handler_ptr handler(new socketio_client_handler());
      client<websocketpp::config::asio_client>::connection_ptr con;
      client<websocketpp::config::asio_client>  endpoint;//(handler);
      handler->set_client(&endpoint);

      // Set log level. Leave these unset for no logging, or only set a few for selective logging.
      
      endpoint.get_elog().set_channels(websocketpp::log::elevel::rerror|
				       websocketpp::log::elevel::fatal |
				       websocketpp::log::elevel::warn);
      endpoint.get_alog().set_channels(websocketpp::log::alevel::devel);

      std::string socket_io_uri = handler->perform_handshake(uri);
      websocketpp::lib::error_code ec;
      con = endpoint.get_connection(socket_io_uri, ec);
      if (ec) {
	std::cout << "> Connect initialization error: " << ec.message() << std::endl;
	return -1;
      }
      // The previous two lines can be combined:
      // con = endpoint.get_connection(handler->perform_handshake(uri));

      endpoint.connect(con);

      boost::thread t(boost::bind(&client<websocketpp::config::asio_client>::run, &endpoint));

      // Wait for a sec before sending stuff
      while (!handler->connected()) {
	std::this_thread::sleep_for (std::chrono::seconds(1));	 
      }

      handler->bind_event("example", &socketio_events::example);

      // After connecting, send a connect message if using an endpoint
      handler->connect_endpoint("/chat");

      std::getchar();

      // Then to connect to another endpoint with the same socket, we call connect again.
      handler->connect_endpoint("/news");

      std::getchar();

      handler->emit("test", "hello!");

      std::getchar();

      endpoint.stop();
   }
   catch (std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      std::getchar();
   }
}
