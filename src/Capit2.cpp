//============================================================================
// Name        : Capit2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Packet.hpp"
#include <tclap/CmdLine.h>
#include "Thread.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "PacketSource.hpp"

using namespace std;


int main(int argc, char* argv[]) {

	ThreadShell::initialize();

	try
		{
			// Define the command line object, and insert a message
			// that describes the program. The "Command description message"
			// is printed last in the help text. The second argument is the
			// delimiter (usually space) and the last one is the version number.
			// The CmdLine object parses the argv array based on the Arg objects
			// that it contains.
			TCLAP::CmdLine cmd("Runs the capit replay system.", ' ', "0.9");

			// Define a value argument and add it to the command line.
			// A value arg defines a flag and a type of value that it expects,
			// such as "-n Bishop".
			TCLAP::ValueArg<std::string> serverIp("t","serverIp","The ip of the server running the services targeted for replay.",true,"192.168.0.1","string");
			TCLAP::ValueArg<std::string> logFile("f","logFile","The path to the pcap file to replay.",true,"data/sample.pcap","string");
			// Add the argument nameArg to the CmdLine object. The CmdLine object
			// uses this Arg to parse the command line.
			cmd.add(serverIp);
			cmd.add(logFile);

			// Define a switch and add it to the command line.
			// A switch arg is a boolean argument and only defines a flag that
			// indicates true or false.  In this example the SwitchArg adds itself
			// to the CmdLine object as part of the constructor.  This eliminates
			// the need to call the cmd.add() method.  All args have support in
			// their constructors to add themselves directly to the CmdLine object.
			// It doesn't matter which idiom you choose, they accomplish the same thing.
			TCLAP::SwitchArg serverLog("s","server_log","If this log has been captured server-side.", cmd, false);

			// Parse the argv array.
			cmd.parse( argc, argv );

			// Get the value parsed by each arg.
			std::string ip = serverIp.getValue();
			std::string file = logFile.getValue();
			bool isServerLog = serverLog.getValue();

			std::cout << "Running for target: " << ip << " with log: " << file << std::endl;

			ClientManagerInstance::getInstance()->setTargetIp(&ip);

			PacketSource source;

			source.openSource(file.c_str());

			ThreadShell::schedule(&source);
			ThreadShell::join();
		}
		catch (TCLAP::ArgException &e)  // catch any exceptions
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
			return -1;
		}

	return EXIT_SUCCESS;
}
