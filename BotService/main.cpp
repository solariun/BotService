//============================================================================
// Name        : BotService.cpp
// Author      : Gustavo Campos
// Version     :
// Copyright   : All rights reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <Util.hpp>
#include <SHA1.hpp>


#include <DBAccess.hpp>
#include <Sqlite3DBAccess.hpp>
#include <iostream>
using namespace std;

int main()
{
    Sqlite3DBAccess dataBase;
    
    cout << "starting up SHA1(teste):" << sha1 ("teste") << endl;
    
    
    try
    {
        dataBase.open("test.db");
        
        try
        {
            DBColumns mapColumns = dataBase.TableRows("test");
            
            if (mapColumns.size() == 0)
            {
                cout << "Table test not found, creating it...." << endl;
                Sqlite3DBQuery query(dataBase);
                
                query.query("CREATE TABLE test (Id integer primary key autoincrement not null, data text);");
                
                int nReturn = query.exec();
                
                cout << "Affected: (" << nReturn << ")" << endl;
                
                mapColumns = dataBase.TableRows("test");
                
                cout << "Table test created." << endl;
            }
            
            for (auto data : mapColumns)
            {
                cout << "  -> [" << data.first << "]=(" << data.second << ")" << endl;
            }
            
            cout << "test database: Number columns: [" << mapColumns.size() << "]" << endl;
            
            Sqlite3DBQuery query(dataBase);
            
            int nReturn;
            
            query.query("insert into test (data) values (strftime('%Y-%m-%d %H:%M:%S','now'));");
            nReturn = query.exec();
            cout << "Affected: (" << nReturn << ")" << endl;
            
            
            query.query("insert into test (data) values (?);");
            query.bind(1, "bind test");
            nReturn = query.exec();
            cout << "Affected: (" << nReturn << ")" << endl;
            
            mapColumns = query.query ("select * from test;");
            
            while (query.step ())
            {
                for (auto column : mapColumns)
                {
                    cout << query.getString(column.second) << "\t";
                }
                
                cout << endl;
            }
            
        }
        catch (DBAccessException& ex)
        {
            cerr << "Sqlite3Error: " << ex.what() << endl;
            
            Sqlite3DBQuery query (dataBase);
        }
        
    }
    catch (DBAccessException& ex)
    {
        cerr << "Sqlite3Error: " << ex.what() << endl;
    }
    
}
