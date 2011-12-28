/*
 * ParameterFactory.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef PARAMETERFACTORY_HPP_
#define PARAMETERFACTORY_HPP_

#include "commons/Lock.hpp"
#include "commons/Container.hpp"
#include "commons/Entity.hpp"
#include "commons/Singleton.hpp"
#include "cluster/Preproccessor.hpp"
#include "cluster/Accessor.hpp"


/**
 * The AbstractAccessor provides access to an Access porointer.
 */
typedef AbstractAccessor<float*> ParametersAccessor;

/**
 * The Container contains pairs of Port numbers and ApplicationAccessors.
 */
typedef Container<string,ParametersAccessor*,NoLock> ParamPolicy;

class ParametersFactory:public ParamPolicy
{
public:
	ParametersFactory()
	{
		//int httpPort =80;
		//int ftpPort = 21;
		//addItem(&httpPort, new HTTPAccessor);
		//addItem(&ftpPort, new FTPAccessor);
	}
};

typedef Singleton<ParametersFactory> ParametersFactoryInstance;

#endif /* PARAMETERFACTORY_HPP_ */
