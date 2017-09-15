/*
 * mysql-odbc.cpp
 *
 *  Created on: 2017年9月16日
 *      Author: cplusplus
 */


/*
 * main.cpp
 *
 *  Created on: 2017年9月4日
 *      Author: cplusplus
 */

#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <iostream>
using namespace std;

#include "otl/otlv4config.h"


otl_connect db; // connect object
void select(const int af1)
{
 otl_stream i(50, // buffer size may be > 1
              "select phone from f_user ",
              db // connect object
             );
   // create select stream


 char phone[30];
 // C++11 (or higher) compiler
 for(auto& it : i){
  it>>phone;
  cout<<"phone="<<phone<<endl;
 }

}
int main(int argc , char* argv[]){
	 otl_connect::otl_initialize(); // initialize ODBC environment
	 try{

	  db.rlogon("UID=root;PWD=root;DSN=myodbc3"); // connect to ODBC
	//  db.rlogon("scott/tiger@mysql35"); // connect to ODBC, alternative format
	                                    // of connect string

	  select(8); // select records from the table

	 }

	 catch(otl_exception& p){ // intercept OTL exceptions
	  cerr<<p.msg<<endl; // print out error message
	  cerr<<p.stm_text<<endl; // print out SQL that caused the error
	  cerr<<p.sqlstate<<endl; // print out SQLSTATE message
	  cerr<<p.var_info<<endl; // print out the variable that caused the error
	 }

	 db.logoff(); // disconnect from ODBC
	return 0;
}






