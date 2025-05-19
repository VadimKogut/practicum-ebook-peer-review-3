#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <sstream>

using namespace std;

struct Person {
    string name;
    int age;
};

struct DatabaseQueryParams {
    int min_age;
    int max_age;
    string_view name_filter;
};

struct DatabaseConnectionParams {
    string_view db_name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;
};

class DatabaseQueryBuilder {
public:
    explicit DatabaseQueryBuilder(const DatabaseQueryParams& params) 
        : params_(params) {}

    string Build() const {
        ostringstream query_str;
        query_str << "from Persons "
                 << "select Name, Age "
                 << "where Age between " << params_.min_age 
                 << " and " << params_.max_age << " "
                 << "and Name like '%" << params_.name_filter << "%'";
        return query_str.str();
    }

private:
    DatabaseQueryParams params_;
};

class DatabaseLoader {
public:
    explicit DatabaseLoader(const DatabaseConnectionParams& conn_params)
        : conn_params_(conn_params) {}

    vector<Person> LoadPersons(const DatabaseQueryParams& query_params) {
        DBConnector connector(conn_params_.allow_exceptions, conn_params_.log_level);
        DBHandler db = Connect(connector);
        
        if (!conn_params_.allow_exceptions && !db.IsOK()) {
            return {};
        }

        DatabaseQueryBuilder builder(query_params);
        DBQuery query(builder.Build());

        vector<Person> persons;
        for (auto [name, age] : db.LoadRows<string, int>(query)) {
            persons.push_back({move(name), age});
        }
        return persons;
    }

private:
    DBHandler Connect(DBConnector& connector) const {
        if (conn_params_.db_name.starts_with("tmp."s)) {
            return connector.ConnectTmp(conn_params_.db_name, 
                                      conn_params_.connection_timeout);
        }
        return connector.Connect(conn_params_.db_name, 
                               conn_params_.connection_timeout);
    }

    DatabaseConnectionParams conn_params_;
};

vector<Person> LoadPersons(DatabaseConnectionParams conn_params, 
                          DatabaseQueryParams query_params) {
    DatabaseLoader loader(conn_params);
    return loader.LoadPersons(query_params);
}
