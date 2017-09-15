/*
 * IConnectonFactory.h
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */

#ifndef INCLUDE_CONNECTIONFACTORY_H_
#define INCLUDE_CONNECTIONFACTORY_H_


#include <string>
#include "otl/otlv4config.h"

namespace DBLIB{


class ConnectionFactory
{
public:
	ConnectionFactory(std::string connString);
	~ConnectionFactory();
	otl_connect* create();

private:
	std::string m_connString;
};

}



#endif /* INCLUDE_CONNECTIONFACTORY_H_ */
