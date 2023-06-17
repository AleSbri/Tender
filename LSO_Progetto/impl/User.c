#include "../lib/struct/User.h"

User creaUser(char *username, char *password) {
    User user = (User) malloc(sizeof(struct User_struct));
    strcpy(user->username,username);
    strcpy(user->password,password);
    user->portafoglio=0;
    return user;
}

User creaUserFromDB(char *username,char *password,float portafoglio){
    User user = (User) malloc(sizeof(struct User_struct));
    strcpy(user->username,username);
    strcpy(user->password,password);
    user->portafoglio=portafoglio;
    return user;
}
void printUser(User user) {
    printf("%s:%s:%lf\n",user->username,user->password,user->portafoglio);
}


User stringToUser(char data[][BUFSIZ]) {
    char username[BUFSIZ] = {""};
    char password[BUFSIZ] = {""};
    strcpy(username,data[0]);
    strcpy(password,data[1]);
    return creaUser(username,password);
}

char* UserToString(User user, char *result) {
    sprintf(result,"%s;%s;%f\n",user->username,user->password,user->portafoglio);
    return result;
}
