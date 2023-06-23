#include "../lib/dependencies.h"
#include "../lib/struct/Drink.h"
#include "../lib/struct/User.h"
#include "libpq-fe.h"

bool checkConnection(PGconn *conn);
PGconn* connect_db();
bool login(PGconn *conn , User user);
bool registration(PGconn *conn , User user);
User getUser(PGconn *conn ,char *username);//potrebbe non servire
void addPortafoglio(PGconn *conn,char *username,char *portafoglio);
bool substractPortafoglio(PGconn *conn,char *username,char *portafoglio);
DrinkList getDrink (PGconn *conn);
DrinkList getDrinksCocktail (PGconn *conn);
DrinkList getDrinksFrullato (PGconn *conn);
void buyListDrink(PGconn *conn,DrinkList list);//potrebbe non servire
void buyDrinkFromName(PGconn *conn,char *nome_drink);
IngredientList getIngredient(PGconn *conn,char *nome_drink);
IngredientList getIngredientFromDrinksList(PGconn *conn,DrinkList drinks);
