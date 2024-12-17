// SQLInjection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <locale>
#include <tuple>
#include <vector>


#include "sqlite3.h"

// Helper function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

/*
   The reason this logic has been seperated from run_query is so that custom
   whitelist verfication functions can be written for different queries.
   For example, a password string does not have the same whitelist of
   characters as the name string does and if there was a query that 
   contained a user entered password it would need to be valdated 
   with a different technique.
*/
bool whitelistNameField(const std::string& userInput)
{
    /*
          The name field of an SQL query should not contain any special characters.
          If it does have an = character or single or double quotes or anything
          else that is commonoly used in SQL injection we log it as an injection attempt
          and return early without executing the query.
       */
    for (auto& c : userInput)
    {
        if (!isalnum(c))
        {
            return false;
        }
    }
    return true;
}

/*
    Currently only two queries are used, and therefore allowed, in this program.
    More queries can be added to the vector in this function to allow them.
*/
bool validQuery(const std::string& query)
{
    // The only acceptable queries
    std::vector<std::string> validQueries = { "SELECT * from USERS", "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME=" };

    // Automatically reject any query that is not in the "whitelist"
    for (auto& q : validQueries)
    {
        if (!(toLowerCase(query).find(toLowerCase(q)) == std::string::npos))
        {
            return true;
        }
    }
    return false;
}

/*
   Used to find the "Base" version of a query containing user input
   Right now we only support one such query but more could be added
   as needed.
*/
std::string getQuery(const std::string& query)
{
    std::vector<std::string> validQueries = {"SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME=" };
    std::string err = "";
    
    // return the query that contains the matching substring
    for (auto& q : validQueries)
    {
        if (toLowerCase(query).find(toLowerCase(q)) != std::string::npos)
        {
            return q;
        }
    }
    return err;
}

// DO NOT CHANGE
typedef std::tuple<std::string, std::string, std::string> user_record;
const std::string str_where = " where ";

// DO NOT CHANGE
static int callback(void* possible_vector, int argc, char** argv, char** azColName)
{
  if (possible_vector == NULL)
  { // no vector passed in, so just display the results
    for (int i = 0; i < argc; i++) 
    {
      std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
  }
  else
  {
    std::vector< user_record >* rows =
      static_cast<std::vector< user_record > *>(possible_vector);

    rows->push_back(std::make_tuple(argv[0], argv[1], argv[2]));
  }
  return 0;
}

// DO NOT CHANGE
bool initialize_database(sqlite3* db)
{
  char* error_message = NULL;
  std::string sql = "CREATE TABLE USERS(" \
    "ID INT PRIMARY KEY     NOT NULL," \
    "NAME           TEXT    NOT NULL," \
    "PASSWORD       TEXT    NOT NULL);";

  int result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    std::cout << "Failed to create USERS table. ERROR = " << error_message << std::endl;
    sqlite3_free(error_message);
    return false;
  }
  std::cout << "USERS table created." << std::endl;

  // insert some dummy data
  sql = "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (1, 'Fred', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (2, 'Barney', 'Rubble');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (3, 'Wilma', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (4, 'Betty', 'Rubble');";

  result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    std::cout << "Data failed to insert to USERS table. ERROR = " << error_message << std::endl;
    sqlite3_free(error_message);
    return false;
  }

  return true;
}

bool run_query(sqlite3* db, std::string& sql, std::vector< user_record >& records, bool containsUserInput)
{
  // TODO: Fix this method to fail and display an error if there is a suspected SQL Injection
  //  NOTE: You cannot just flag 1=1 as an error, since 2=2 will work just as well. You need
  //  something more generic

  // Clear any prior results
  records.clear();

  if (!validQuery(sql))
  {
      std::cout << "[SQL ERROR]: invalid SQL query" << '\n';
      return false;
  }

   // If the input query contains no user input we can go ahead and execute it
   if (!containsUserInput)
   {
       char* error_message;
       if (sqlite3_exec(db, sql.c_str(), callback, &records, &error_message) != SQLITE_OK)
       {
           std::cout << "Data failed to be queried from USERS table. ERROR = " << error_message << std::endl;
           sqlite3_free(error_message);
           return false;
       }
   }
   else // The query contains user input that must be checked
   {
       std::string baseQuery = getQuery(sql);

       if (baseQuery.length() < 1)
       {
           std::cout << "[ERROR]: Query not found";
           return false;
       }

       // Grab all of the user input beyond the end of the base query
       std::string userInput;
       userInput = sql.substr(baseQuery.length(), sql.length());

       // Get rid of all ' characters
       userInput.erase(std::remove(userInput.begin(), userInput.end(), '\''), userInput.end());

       // Right now we are only suppoorting queries with NAME as user input so we can use the corresponding whitelist function to check for SQL Injection
       if (!whitelistNameField(userInput))
       {
           std::cout << "\n[SQL ERROR]: The Submitted Query Contains a Possible SQL Injection Attempt! \n";
           return false;
       }

       // Prepared query
       std::string preparedQuery = baseQuery.append(" ?");

       /*
          If the user input has reached this point, it is likely safe, but we will still
          bind the user input into a prepared statement which safeguards against
          all possible injections
       */ 

       // pointer to an sql statement
       sqlite3_stmt* sqlStatement;

       /*
           Create the prepared statement using the prepared query
           
           NOTE: for some reason whoever wrote the sqlite3_prepare_v2()
           decided to make the string length argument a signed int.
           To avoid the conversion from size_t to int warning I've used a cast,
           but this is only okay because we know exactly how long a query that 
           reaches this part of the code is and it wont exceed INT_MAX.
       */
       sqlite3_prepare_v2(
           db,
           preparedQuery.c_str(),
           (int)preparedQuery.length(),
           &sqlStatement,
           nullptr
       );

       /*
          Bind the user input text into the prepared query.The user input may or may not contain an SQL injection
          but it will fail either way because the user input is treated as a text variable and is not part of the SQL statement
       */         
       sqlite3_bind_text(
           sqlStatement,
           1,
           userInput.c_str(),
           (int)preparedQuery.length(),
           SQLITE_STATIC
       );

       // the final query value
       std::string query = sqlite3_expanded_sql(sqlStatement);

       char* error_message;
       if (sqlite3_exec(db, query.c_str(), callback, &records, &error_message) != SQLITE_OK)
       {
           std::cout << "Data failed to be queried from USERS table. ERROR = " << error_message << std::endl;
           sqlite3_free(error_message);

           // Destroy statement
           sqlite3_finalize(sqlStatement);

           return false;
       }

       // Destroy statement
       sqlite3_finalize(sqlStatement);
   }

  return true;
}

// DO NOT CHANGE
bool run_query_injection(sqlite3* db, const std::string& sql, std::vector< user_record >& records)
{
  std::string injectedSQL(sql);
  std::string localCopy(sql);

  // we work on the local copy because of the const
  std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
  if(localCopy.find_last_of(str_where) >= 0)
  { // this sql has a where clause
    if(localCopy.back() == ';')
    { // smart SQL developer terminated with a semicolon - we can fix that!
      injectedSQL.pop_back();
    }

    switch (rand() % 4)
    {
    case 1:
      injectedSQL.append(" or 2=2;");
      break;
    case 2:
      injectedSQL.append(" or 'hi'='hi';");
      break;
    case 3:
      injectedSQL.append(" or 'hack'='hack';");
      break;
    case 0:
    default:
      injectedSQL.append(" or 1=1;");
      break;
    }
  }
  
  return run_query(db, injectedSQL, records, true);
}


// DO NOT CHANGE
void dump_results(const std::string& sql, const std::vector< user_record >& records)
{
  std::cout << std::endl << "SQL: " << sql << " ==> " << records.size() << " records found." << std::endl;

  for (auto record : records)
  {
    std::cout << "User: " << std::get<1>(record) << " [UID=" << std::get<0>(record) << " PWD=" << std::get<2>(record) << "]" << std::endl;
  }
}

// DO NOT CHANGE
void run_queries(sqlite3* db)
{
  char* error_message = NULL;

  std::vector< user_record > records;

  // query all
  std::string sql = "SELECT * from USERS";
  if (!run_query(db, sql, records, false)) return;
  dump_results(sql, records);

  //  query 1
  sql = "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred'";
  if (!run_query(db, sql, records, true)) return;
  dump_results(sql, records);

  //  run query 1 with injection 5 times
  for (auto i = 0; i < 5; ++i)
  {
    if (!run_query_injection(db, sql, records)) continue;
    dump_results(sql, records);
  }
}

// You can change main by adding stuff to it, but all of the existing code must remain, and be in the
// in the order called, and with none of this existing code placed into conditional statements
int main()
{
  // initialize random seed:
  srand((unsigned int)time(nullptr));

  int return_code = 0;
  std::cout << "SQL Injection Example" << std::endl;

  // the database handle
  sqlite3* db = NULL;
  char* error_message = NULL;

  // open the database connection
  int result = sqlite3_open(":memory:", &db);

  if(result != SQLITE_OK)
  {
    std::cout << "Failed to connect to the database and terminating. ERROR=" << sqlite3_errmsg(db) << std::endl;
    return -1;
  }

  std::cout << "Connected to the database." << std::endl;

  // initialize our database
  if(!initialize_database(db))
  {
    std::cout << "Database Initialization Failed. Terminating." << std::endl;
    return_code = -1;
  }
  else
  {
    run_queries(db);
  }

  // close the connection if opened
  if(db != NULL)
  {
    sqlite3_close(db);
  }

  return return_code;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
