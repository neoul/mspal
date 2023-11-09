#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <odbc_connector.h>
#include <wpal.h>

using namespace std;

int use_odbc()
{
    registerODBCConnector();
    string conn = "Driver=ODBC Driver 18 for SQL Server;Server=tcp:192.168.100.130,1433;UID=TestUser;PWD=TestUser;Database=GameDB;TrustServerCertificate=yes;Encrypt=Yes";
    Poco::Data::Session session(ODBCKey, conn);
    try
    {
        Poco::Dynamic::Var intValue(123);
        Poco::Data::Statement stmt(session);
        int result;
        stmt << "{? = call spGetAccountProfileIcon (?)}", Poco::Data::Keywords::out(result);
        stmt, Poco::Data::Keywords::bind(intValue);
        stmt.execute();
        Poco::Data::RecordSet rs(stmt);
        std::cout << "result: " << result << std::endl;
        std::size_t cols = rs.columnCount();

        const std::string name = "ProfileIconId";
        // print all column names
        for (std::size_t col = 0; col < cols; ++col)
        {
            std::cout << rs.columnName(col) << std::endl;
        }

        // iterate over all rows and columns
        for (auto it = rs.begin(); it != rs.end(); ++it)
            std::cout << it->get(0).toString() << *it << " ";
    }
    catch (Poco::Data::DataException &ex)
    {
        std::cerr << "Database Error: " << ex.displayText() << std::endl;
    }
    unregisterODBCConnector();
    return 0;
}