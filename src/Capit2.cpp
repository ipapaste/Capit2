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
#include "Thread.hpp"
#include "ClientManager.hpp"
#include "PacketSource.hpp"
#include "cluster/Preprocessor.hpp"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <string>
#include "ParameterFactory.hpp"
#include "cluster/Accessor.hpp"
#include "cluster/ClusterManager.hpp"
#include "cluster/Cluster.hpp"

using namespace std;
using namespace boost;

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
	ThreadShell::initialize();
	try
	{
		/**
		 * Initialize the command line manager.
		 */
		TCLAP::CmdLine cmd("Runs the capit emulation system.", ' ', "0.9");

		/**
		 * Argument defining the IP of the targeted host.
		 */
		TCLAP::ValueArg<std::string> serverIp(
				"t",
				"serverIp",
				"The ip of the server running the services targeted for replay.",
				true, "192.168.0.1", "string");

		/**
		 * Argument defining the pcap file to be replayed.
		 */
		TCLAP::ValueArg<std::string> logFile("s", "logFile",
				"The path to the pcap file to replay. If more than one files "
						"are needed, separate them with +.", true,
				"data/sample.pcap", "string");

	    /**
		 * Argument defining the pcap file to be replayed.
	     */
				TCLAP::ValueArg<std::string> filter("f", "filter",
						"Specify a pcap filter expression for the sources.", false,
						"NONE", "string");

		/**
		* Argument defining the pcap file to be replayed.
		*/
		TCLAP::ValueArg<std::string> parameters("p", "cluster_parameters",
				"Specify the list of clustering parameters to use.", false,
				"FTP", "string");

		/**
		* Argument defining application mode.
		*/
		TCLAP::ValueArg<std::string> mode("m", "mode",
				"The execution mode of the application. Modes can be replay and cluster", true,
				"replay", "string");

		/**
		* Argument defining application mode.
		*/
		TCLAP::ValueArg<int> clusterSize("c", "cluster_number",
				"The number of clusters to use.", false,
				3, "string");

		cmd.add(serverIp);
		cmd.add(logFile);
		cmd.add(mode);
		cmd.add(clusterSize);
		cmd.add(parameters);
		cmd.add(filter);

		/**
		 * Parse the argc/argv array.
		 */
		cmd.parse(argc, argv);

		std::string targetIp = serverIp.getValue();
		std::string files = logFile.getValue();
		std::string filterValue = filter.getValue();
		std::string appMode = mode.getValue();

		if(appMode.compare("replay") && appMode.compare("cluster") && appMode.compare("generation"))
		{
			cout<< "Mode can only take one of the following values: replay, cluster, generation" << endl;
			exit(0);
		}

		/**
		 * Lazy load and initialize the client manager singleton.
		 */
		ClientManagerInstance::getInstance()->setTargetIp(targetIp);

		char_separator<char> sep("+");

		tokenizer<char_separator<char> > tokens(files, sep);

		if(!appMode.compare("replay"))
		{
			BOOST_FOREACH(string t, tokens)
			{
				PacketSource* source = new PacketSource();
				source->openSource(t.c_str());

				if(filterValue.compare("NONE") != 0)
				{

					source->setFilter(filterValue);
				}
				else
				{
					cout << "No filter specified." << endl;
				}

				ClientManagerInstance::getInstance()->registerSource();
				ThreadShell::schedule(source);
			}
		}
		else if(!appMode.compare("cluster"))
		{
			int clusterNo = clusterSize.getValue();

			if(clusterNo <= 1)
			{
				cout << "Cluster number must be greater than one." << endl;
				exit(0);
			}


			string params = parameters.getValue();

			char_separator<char> sepp("+");

			tokenizer<char_separator<char> > tokens(params, sepp);

			int size = 0;
			BOOST_FOREACH(string tt, tokens)
			{
				size++;
			}

			if(size <2)
			{
				cout <<"At least two parameter must be specified."<< endl;
				exit(0);
			}

			PcapAdapterShell shell;

			FlowPreproc proc;


			shell.openSource(files.c_str());

			Packet* p = shell.getNextPacket();

			while (p != NULL)
			{
				proc.readInputObject(p);
				p = shell.getNextPacket();
			}

			map<Flow::IdType,Flow*>::iterator it;
			map<Flow::IdType,Flow*>& objects = *proc.getOutputObjects();

			DelayAccessor ac;

			for(it = objects.begin(); it != objects.end(); ++it)
			{
				cout <<"Flow Average Delay:" << ac.getValue(it->second)<< endl;
				cout << it->first << endl;
				map<int,Packet*>& data = *it->second->getInputObjects();

				map<int,Packet*>::iterator in;

				for(in = data.begin(); in != data.end(); ++in)
				{
					//cout << *in->second->getPayload() << endl;
				}

			}

			KMeansManager kms(1,&proc);
			kms.addAccessor(&ac);
			kms.cluster();

			exit(0);
		}

		/**
		 * Wait for all threads to end before shutting down.
		 */
		ThreadShell::join();
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
				<< std::endl;
		return -1;
	}

	return EXIT_SUCCESS;
}
