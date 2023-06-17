#include "../dependencies.h"

typedef struct User_struct
{
    char username[BUFSIZ];
    char password[BUFSIZ];
    float portafoglio;
}User_struct;

typedef struct User_struct *User;

User creaUser(char *username,char *password);
User creaUserFromDB(char *username,char *password,float portafoglio);
void printUser(User user);
User stringToUser(char data[][BUFSIZ]);
char* UserToString(User user, char *result);
