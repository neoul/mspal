#include <iostream>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <iomanip>

#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/ODBC/Connector.h>
#include <wpal.h>

using namespace std;

void qweqw(Poco::Dynamic::Var &qwe)
{
    qwe = 123;
}

int main()
{
    Poco::Data::ODBC::Connector::registerConnector();

    constexpr char *connectionString = "Driver=ODBC Driver 18 for SQL Server;Server=tcp:192.168.100.130,1433;UID=TestUser;PWD=TestUser;Database=GameDB;TrustServerCertificate=yes;Encrypt=Yes";

    Poco::Data::Session session(Poco::Data::ODBC::Connector::KEY, connectionString);

    try
    {
        _variant_t qqq;
        qqq.n1.n2.vt = VT_I8;
        qqq.n1.n2.n3.llVal = 20000000799;
        Poco::Dynamic::Var intValue;
        qweqw(intValue);
        // intValue = qqq;
        intValue = qqq.n1.n2.n3.llVal;

        Poco::Data::Statement stmt(session);
        int result;
        stmt << "{? = call spGetAccountProfileIcon (?)}", Poco::Data::Keywords::out(result);
        stmt, Poco::Data::Keywords::bind(intValue);
        // delete intValue;

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

        // Access columns by name or index
        Poco::Dynamic::Var columnValue = rs.row(0).get(0);
        std::cout << "11?? " << columnValue.toString() << std::endl;
        Poco::Dynamic::Var columnValue2 = rs["ProfileMarkIcon"];
        std::cout << "22?? " << columnValue2.toString() << std::endl;
    }
    catch (Poco::Data::DataException &ex)
    {
        std::cerr << "Database Error: " << ex.displayText() << std::endl;
    }

    return 0;
}