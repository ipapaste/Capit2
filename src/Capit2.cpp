//============================================================================
// Name        : Capit2.cpp
// Author      : John Papastergiou
// Version     :
// Copyright   : Your copyright notice
// Description : Capit2, security deterministic replay tool.
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tclap/CmdLine.h>
#include "commons/concurrent/Daemon.hpp"
#include "commons//tools/String.hpp"
#include "ClientManager.hpp"
#include "PacketSource.hpp"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <string>
//#include "FlowManager.hpp"
#include "CapitMarkovParser.hpp"
#include "CapitInputParser.hpp"
//#include "model/input/SourceManager.hpp"

using namespace std;
using namespace boost;


void loadMarkov(string markov)
{
	CapitMarkovParser* p = new CapitMarkovParser(markov);
	p->read();
}

void loadSource(string source)
{
	CapitInputParser* p = new CapitInputParser(source);
	p->read();
}



/**
 * This is the main entry point for the application. To start
 * the application properly the following steps must be done
 * in the correct order.
 *
 * 1) The thread management system must be initialized. By doing
 * that we ensure that the daemon thread is up and running, and
 * is ready to handle all thread scheduling.
 *
 * 2) Generate all the command line arguments that need to be
 * defined in order to have all the necessary information that
 * are needed to execute a replay service. A replay service is
 * defined by one or more log files and target host. The log
 * files contain the packets that have been captured server side
 * or client side. Those packets will get filtered and replayed.
 * The target host defines the remote ( or local ) machine towards
 * which we are going to replay the traffic captured as packets
 * inside the log file(s). The replay service is independent of
 * the location at which the log was generated. For example a pcap
 * log is the same weather it was captured in a client machine
 * generating the packets or in a server machine receiving the
 * packets. In both cases the log produced is actually the same
 * since packets have the same format at the source of their
 * generation as those that will arrive at the destination. To
 * make it more clear a packet generated at 192.168.0.1 and
 * targeted towards 192.168.0.2 will have source : 192.168.0.1
 * and destination : 192.168.0.2 wherever in the network it
 * gets captured. Thus we do not need an argument defining the
 * location-type of the log file.
 *
 * 3)The argc/argv arrays are parsed into variables representing
 * the previously mentioned arguments.
 *
 * 4) The client manager singleton gets lazy loaded and initialized
 * with the target host IP Address that was read from the arguments.
 * This is necessary as the ClientManager acts as a filter towards
 * a pcap source, filtering out packets that do not target the
 * desired host but correspond to different hosts or different
 * applications.
 *
 * 5) A PacketSource object is created for every log file name
 * argument and gets initialized. Each PacketSource acts as an
 * independent thread that feeds packets from the pcap file to
 * the ClientManager.
 *
 * 6) The thread management system joins(), waiting for all other
 * threads to finish executing before it allows the application
 * to exit.
 */
int main(int argc, char* argv[])
{
	/**
	 * Initializing the thread management system.
	 */
	try
	{
		/**
		 * Initialize the command line manager.
		 */
		TCLAP::CmdLine cmd("Runs the capit emulation system.", ' ', "0.9");

		/**
		 * Argument defining the IP of the targeted host.
		 */
		TCLAP::ValueArg<std::string> targetArg(
				"t",
				"serverIp",
				"The ip of the server running the services targeted for replay.",
				true, "192.168.0.1", "string");

		/**
		 * Argument defining the pcap file to be replayed.
		 */
		TCLAP::ValueArg<std::string> sourceArg("s", "sourceFile",
				"The path to the xml source file.", true,
				"data/config/source.xml", "string");

		/**
		* Argument defining the pcap file to be replayed.
		*/
		TCLAP::ValueArg<std::string> markovArg("d", "markovSource",
				"The path to the xml file containing the Markov Chain state-entities.", true,
						"data/config/markov.xml", "string");

	    /**
		 * Argument defining the pcap file to be replayed.
	     */
		TCLAP::ValueArg<std::string> filterArg("f", "filter",
				"Specify a pcap filter expression for the sources.", false,
				"NONE", "string");

		/**
		* Argument defining application mode.
		*/
		TCLAP::ValueArg<std::string> modeArg("m", "mode",
				"The execution mode of the application. Modes can be replay and cluster", true,
				"replay", "string");


		cmd.add(targetArg);
		cmd.add(sourceArg);
		cmd.add(markovArg);
		cmd.add(modeArg);
		cmd.add(filterArg);

		/**
		 * Parse the argc/argv array.
		 */
		cmd.parse(argc, argv);

		std::string target = targetArg.getValue();
		std::string source = sourceArg.getValue();
		std::string markov = markovArg.getValue();
		std::string filter= filterArg.getValue();
		std::string mode = modeArg.getValue();

		if(!String::areEqual(mode,"replay") && !String::areEqual(mode,"extract"))
		{
			cout<< "Mode can only take one of the following values: replay, extract." << endl;
			exit(0);
		}


		/**
		 * Lazy load and initialize the client manager singleton.
		 */
		ClientManagerInstance::getInstance().setTargetIp(target);



		loadMarkov(markov);
		loadSource(source);

		if(String::areEqual(mode,"replay"))
		{
			SourceManager::getInstance().replay();
		}
		else
		{
			SourceManager::getInstance().extract();
			exit(0);
		}
		/**
		 * Wait for all threads to end before shutting down.
		 */
		Daemon daemon;
		daemon.execute(99999999);
		daemon.join();
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
				<< std::endl;
		return -1;
	}

	return EXIT_SUCCESS;
}
