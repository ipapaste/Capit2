#include <iostream>
#include <cstdlib>
#include <iostream>
#include <boost/asio/ssl/rfc2818_verification.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
using boost::asio::ip::tcp;

int main()
{


	// Create a context that uses the default paths for finding CA certificates.
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

	// Open a socket and connect it to the remote host.
	boost::asio::io_service io_service;
	boost::asio::ssl::stream<tcp::socket> sock(io_service, ctx);
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("192.168.0.5", "https");
	boost::asio::connect(sock.lowest_layer(), resolver.resolve(query));
	sock.lowest_layer().set_option(tcp::no_delay(true));

	// Perform SSL handshake and verify the remote host's certificate.
	sock.set_verify_mode(boost::asio::ssl::verify_none);
	sock.set_verify_callback(boost::asio::ssl::rfc2818_verification("192.168.0.5"));
	sock.handshake(boost::asio::ssl::stream<tcp::socket>::client);

	return 0;
}
