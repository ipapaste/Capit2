/*
 * ApplicationFactory.hpp
 *
 *  Created on: Dec 1, 2011
 *      Author: issle
 */

#ifndef APPLICATIONFACTORY_HPP_
#define APPLICATIONFACTORY_HPP_

#include "commons/Lock.hpp"
#include "commons/Container.hpp"
#include "commons/Entity.hpp"
#include "commons/Singleton.hpp"
#include "commons/Accessor.hpp"

#include "AbstractApplication.hpp"
#include "applications/HTTPApplication.hpp"
#include "applications/FTPApplication.hpp"

/**
 * The AbstractAccessor provides access to an AbstractApplication pointer.
 */
typedef AbstractAccessor<AbstractApplication*> ApplicationAccessor;

/**
 * The Container contains pairs of Port numbers and ApplicationAccessors.
 */
typedef Container<int,ApplicationAccessor*,NoLock> FactoryPolicy;


typedef ObjectAccessor<HTTPApplication,ApplicationAccessor> HTTPAccessor;
typedef ObjectAccessor<FTPApplication, ApplicationAccessor> FTPAccessor;


class ApplicationFactory:public FactoryPolicy
{
public:
	ApplicationFactory()
	{
		int httpPort =80;
		int ftpPort = 21;
		addItem(&httpPort, new HTTPAccessor);
		addItem(&ftpPort, new FTPAccessor);
	}
};

typedef Singleton<ApplicationFactory> ApplicationFactoryInstance;

#endif /* APPLICATIONFACTORY_HPP_ */
