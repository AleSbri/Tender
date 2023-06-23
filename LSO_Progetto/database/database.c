#include "database.h"
//#include "../dependencies.h"
//#include "../lib/struct/Drink.h"
//#include "../lib/struct/User.h"
//#include <libpq-fe>


PGconn* connect_db(){
    PGconn *conn = PQconnectdb("user=alesilv password=Progetto2023 dbname=tenderdb");
        if (PQstatus(conn) == CONNECTION_BAD) {

        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        PQfinish(conn);
    }
    printf("connesso al databse\n");
    return conn;
}

bool login(PGconn *conn , User user){
    conn = connect_db();
    const char *paramValues[2];
    paramValues[0]=user->username;
    paramValues[1]=user->password;
    char *query= "SELECT * FROM utente WHERE username=$1 AND password=$2";
    PGresult *res = PQexecParams(conn,query,2,NULL,paramValues,NULL,NULL,0);
    if (PQntuples(res) == 0) {
        printf("username o password sbagliati\n");
        PQclear(res);
        PQfinish(conn);
        return false;
    }
    free(user);
    user=creaUserFromDB(PQgetvalue(res,0,0),PQgetvalue(res,0,1),atof(PQgetvalue(res,0,2)));
    printf("Login utente : %s %s\n",PQgetvalue(res,0,0),PQgetvalue(res,0,1));
    PQclear(res);
    PQfinish(conn);
    return true;
}

bool registration(PGconn *conn , User user){
    conn = connect_db();
    const char *paramValues[2];
    paramValues[0]=user->username;
    paramValues[1]=user->password;

    char *query= "INSERT INTO utente (username,password,portafoglio) VALUES ($1,$2,0)";

    PGresult *res = PQexecParams(conn,query,2,NULL,paramValues,NULL,NULL,0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

        printf("registrazione non avvenuta\n");
        PQclear(res);
        PQfinish(conn);
        return false;
    }
    printf("Registrazione utente avvenuta utente : %s %s\n",user->username,user->password);
    PQclear(res);
    PQfinish(conn);
    return true;
}

User getUser(PGconn *conn ,char *username){//potrebbe non servire se si salva tutto in android durante il login
    conn = connect_db();
    const char *paramValues[1];
    paramValues[0]=username;
    char *query ="SELECT * FROM utente WHERE username = $1";
    PGresult *res = PQexecParams(conn,query,1,NULL,paramValues,NULL,NULL,0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

        printf("Utente non esistente\n");
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }
    printf("Utente %s trovato",username);
    User user = creaUserFromDB(PQgetvalue(res,0,0),PQgetvalue(res,0,1),atof(PQgetvalue(res,0,2)));
    PQclear(res);
    PQfinish(conn);
    return user;
}

void addPortafoglio(PGconn *conn,char *username,char *portafoglio){
    conn = connect_db();
    const char *paramValues[2];
    paramValues[0]=portafoglio;
    paramValues[1]=username;
    char *query ="UPDATE utente SET portafoglio = portafoglio + $1 WHERE username = $2";
    PGresult *res = PQexecParams(conn,query,2,NULL,paramValues,NULL,NULL,0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

        printf("Utente non esistente\n");
        PQclear(res);
        PQfinish(conn);
    }
    printf("Soldi aggiunti all'utente %s\n",username);
    PQclear(res);
    PQfinish(conn);
}

bool substractPortafoglio(PGconn *conn,char *username,char *portafoglio){
    conn = connect_db();
    const char *paramValues[2];
    paramValues[0]=portafoglio;
    paramValues[1]=username;
    char *query ="UPDATE utente SET portafoglio = portafoglio - $1 WHERE username = $2";
    PGresult *res = PQexecParams(conn,query,2,NULL,paramValues,NULL,NULL,0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

        printf("Utente non dispone si abbastanza soldi \n");
        PQclear(res);
        PQfinish(conn);
        return false;
    }
    printf("Acquisto avvenuto dell'utente %s\n",username);
    PQclear(res);
    PQfinish(conn);
    return true;
}

DrinkList getDrink (PGconn *conn){
    conn = connect_db();
    char *query ="SELECT * FROM drink order by vendite desc";
    PGresult *res = PQexec(conn,query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("errore database\n");
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }
    int rows = PQntuples(res);
    DrinkList drinks = NULL;
    for(int i=0; i<rows; i++) {
        drinks = inserisciDrink(drinks,creaDrink(
            PQgetvalue(res, i, 0),
            PQgetvalue(res, i, 1),
            atoi(PQgetvalue(res, i, 2)),
            atof(PQgetvalue(res, i, 3)),
            atoi(PQgetvalue(res, i, 4))));
    }

    PQclear(res);
    PQfinish(conn);
    return drinks;
}

DrinkList getDrinksCocktail (PGconn *conn){
    conn = connect_db();
    char *query ="SELECT * FROM drink WHERE frullato = 0 order by vendite desc";
    PGresult *res = PQexec(conn,query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

        printf("errore database\n");
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }
    int rows = PQntuples(res);
    DrinkList drinks = NULL;
    for(int i=0; i<rows; i++) {
        drinks = inserisciDrink(drinks,creaDrink(
          PQgetvalue(res, i, 0),
          PQgetvalue(res, i, 1),
          atoi(PQgetvalue(res, i, 2)),
          atof(PQgetvalue(res, i, 3)),
          atoi(PQgetvalue(res, i, 4))));
    }

    PQclear(res);
    PQfinish(conn);
    return drinks;
}

DrinkList getDrinksFrullato (PGconn *conn){
    conn = connect_db();
    char *query ="SELECT * FROM drink WHERE frullato = 1 order by vendite desc";
    PGresult *res = PQexec(conn,query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

        printf("errore database\n");
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }
    int rows = PQntuples(res);
    DrinkList drinks = NULL;
    for(int i=0; i<rows; i++) {
      drinks = inserisciDrink(drinks,creaDrink(
          PQgetvalue(res, i, 0),
          PQgetvalue(res, i, 1),
          atoi(PQgetvalue(res, i, 2)),
          atof(PQgetvalue(res, i, 3)),
          atoi(PQgetvalue(res, i, 4))));
    }

    PQclear(res);
    PQfinish(conn);
    return drinks;
}

void buyListDrink(PGconn *conn,DrinkList list){
    conn = connect_db();
    DrinkList tmp = list;
    int len = lenDrinkList(tmp);
    for (int i = 0 ; i < len ; i++){

        const char *paramValues[1];
        paramValues[0]=tmp->drink->nome_drink;
        char *query ="UPDATE drink SET vendite = vendite + 1 WHERE nome = $1";
        PGresult *res = PQexecParams(conn,query,1,NULL,paramValues,NULL,NULL,0);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("errore database\n");
            PQclear(res);
            PQfinish(conn);
        }
        tmp = tmp ->next;
        PQclear(res);
    }
    printf("aggiornate vendite ");
    PQfinish(conn);
}


void buyDrinkFromName(PGconn *conn,char *nome_drink){
    conn = connect_db();
    const char *paramValues[1];
    paramValues[0]=nome_drink;
    char *query ="UPDATE drink SET vendite = vendite + 1 WHERE nome = $1";
    PGresult *res = PQexecParams(conn,query,1,NULL,paramValues,NULL,NULL,0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("errore database\n");
        PQclear(res);
        PQfinish(conn);
    }
    PQclear(res);
    PQfinish(conn);
}

//**************************************************************************************************//

IngredientList getIngredient(PGconn *conn,char *nome_drink){
    conn = connect_db();
    const char *paramValues[0];
    paramValues[0]=nome_drink;
    char *query ="SELECT * FROM ingredient WHERE nome_drink = $1";
    PGresult *res = PQexecParams(conn,query,1,NULL,paramValues,NULL,NULL,0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

        printf("nome drink inesistente\n");
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }

    int rows = PQntuples(res);
    IngredientList ingredients = NULL;
    for(int i=0; i<rows; i++) {
        ingredients = inserisciIngredient(ingredients,creaIngredient(
            PQgetvalue(res, i, 0),PQgetvalue(res, i, 1)));
    }
    PQclear(res);
    PQfinish(conn);
    return ingredients;
}

IngredientList getIngredientFromDrinksList(PGconn *conn,DrinkList drinks){
    conn = connect_db();
    IngredientList ingredients = NULL;
    DrinkList tmp = drinks;
    int len = lenDrinkList(tmp);
    for (int i = 0 ; i < len ; i++){

        const char *paramValues[0];
        paramValues[0]=tmp->drink->nome_drink;
        char *query ="SELECT * FROM ingredient WHERE nome_drink=$1";
        PGresult *res = PQexecParams(conn,query,1,NULL,paramValues,NULL,NULL,0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("errore database\n");
            PQclear(res);
            PQfinish(conn);
        }
        int rows = PQntuples(res);
        for(int i=0; i<rows; i++) {
            ingredients = inserisciIngredient(ingredients,creaIngredient(
                PQgetvalue(res, i, 0),PQgetvalue(res, i, 1)));
        }
        tmp = tmp ->next;
        PQclear(res);
    }
    PQfinish(conn);
    return ingredients;
}
