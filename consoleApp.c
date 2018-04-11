#include <stdio.h>
#include <stdlib.h>
#include "libpq-fe.h"

PGresult *res;

void do_exit(PGconn *conn) {

	PQfinish(conn);
	exit(1);
}


void ExecuteQuery(PGconn* conn,char* query){
    PQexec(conn, "set search_path to gsrtc;");
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    	fprintf(stderr, "Query failed: %s",PQerrorMessage(conn));
   	  do_exit(conn);
	}
	else{
  		 int rows = PQntuples(res);
  		 int ncols = PQnfields(res);
      	 for (int i=0; i<ncols; i++){
   		 char *name = PQfname(res, i);
   		 printf("%s ", name);
   	 }
   	 printf("\n");
   	 for(int i=0; i<rows; i++) {
   		 for(int j=0;j<ncols;j++){
   			 printf("%s ",PQgetvalue(res, i, j));
   		 }
   		 printf("\n");
   	 }
	}
}


void ExecuteUpdate(PGconn* conn,char* command){
    PQexec(conn, "set search_path to gsrtc;");
    res = PQexec(conn, command);
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
   	 fprintf(stderr, "%.6s failed: %s",command,PQerrorMessage(conn));
   	 do_exit(conn);
    }
    else{
   	 printf("successful\n");
    }
}


int main()
{
  const char *conninfo = "dbname=201501140 user=201501140 password=201501140 hostaddr=10.100.71.21 port=5432";
  PGconn *conn = PQconnectdb(conninfo);
	if (PQstatus(conn) != CONNECTION_OK){
    	fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
    	do_exit(conn);
	}

	char *user = PQuser(conn);
	char *db_name = PQdb(conn);

	printf("User: %s\n", user);
	printf("Database name: %s\n", db_name);

	while(1){
   	 char command[1000];
   	 printf("Enter Command in a single line\n");
   	 fgets (command, 1000, stdin);
   	 if(command[0]=='S' || command[0]=='s'){
   		 ExecuteQuery(conn,command);
   	 }
   	 else{
   		 ExecuteUpdate(conn,command);
   	 }
   	 PQclear(res);
	}
	PQfinish(conn);


	return 0;
}
