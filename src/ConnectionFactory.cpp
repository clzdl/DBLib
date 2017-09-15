/*
 * ConnectionFactory.cpp
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */


#include "ConnectionFactory.h"
namespace DBLIB{

ConnectionFactory::ConnectionFactory(std::string connString)
:m_connString(connString)
{}
ConnectionFactory::~ConnectionFactory(){

}

otl_connect* ConnectionFactory::create()
{
	otl_connect* conn = new otl_connect();
	conn->rlogon(m_connString.c_str() , false);
	return conn;
}
}

