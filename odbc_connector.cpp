#ifndef _POCO_DATA_ODBC_CONNECTOR_H_
#define _POCO_DATA_ODBC_CONNECTOR_H_
#include <string>
#include <Poco/Data/ODBC/Connector.h>

void registerODBCConnector()
{
    Poco::Data::ODBC::Connector::registerConnector();
}

void unregisterODBCConnector()
{
    Poco::Data::ODBC::Connector::unregisterConnector();
}

const std::string ODBCKey = Poco::Data::ODBC::Connector::KEY;
#endif // _POCO_DATA_ODBC_CONNECTOR_H_