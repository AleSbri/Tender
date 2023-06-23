#include "lib/com/socket_server_lib.h"
#include <netdb.h>
int main(int argc, char const *argv[])
{
    char buffer[BUFSIZ];
    char nickname[BUFSIZ];
    int new_socket;
    struct sockaddr_in client_addr;
    pthread_t thread;
    int server_fd = init_server();

    printf("\n******* Benvenuto in Tender *************\n");
    printf("** L'app che ti permette di acquistare bevande **\n");
	
    while (true) {
        socklen_t client_addr_len = sizeof(struct sockaddr_in);
        if((new_socket = accept(server_fd,(struct sockaddr *)&client_addr,&client_addr_len)) < 0) perror("errore durante l'accept");
        printf("nuova connessione stabilita con %s\n",inet_ntoa(client_addr.sin_addr));
        if((pthread_create(&thread,NULL,gestisci_richiesta,&new_socket)) < 0) perror("errore nella creazione del thread");
    }
}

int init_server() {
    int server_fd;
    struct sockaddr_in address;
    int option = 1;
    int addr_len;

    if((server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("creazione socket non riuscita");
        return EXIT_FAILURE;
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    init_address(&address);
    addr_len = sizeof(address);

    if(bind(server_fd,(struct sockaddr *)&address,addr_len) < 0) {
        perror("binding socket non riuscita");
        return EXIT_FAILURE;
    }
	char host[256];
	gethostname(host,256);
	printf("%s",host);
	struct hostent *hostinfo;
	hostinfo = gethostbyname(host);
	char *ip = inet_ntoa(*(struct in_addr*)hostinfo->h_addr);
	printf("%s",ip);
    listen(server_fd,5);
    return server_fd;
}

void init_address(struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_port = htons(8080);
    address->sin_addr.s_addr = INADDR_ANY;
}


void deserialize_data(char data[][BUFSIZ],char *token) {
    char *strtoken = strtok(token,";");
    size_t i = 0;
    while (strtoken != NULL) {
            strcpy(data[i++],strtoken);
            strtoken = strtok(NULL,";");
        }
}

void leggiClient(int socket, char *client) {
    char buffer[BUFSIZ] = {""};
    if(read(socket,buffer,BUFSIZ) < 0) perror("errore nella lettura del nickname del client\n");
    printf("richiesta in arrivo dal client %s",buffer);
    buffer[strlen(buffer) -1] = '\0';
    strcpy(client,buffer);
}

void inviaRespOk(int socket) {
    char *resp = "ok\n";
    if(send(socket,resp,strlen(resp),0) < 0) perror("errore nell'invio della risposta al client\n");
}

void leggiRichiesta(int socket, char *richiesta) {
    char buffer[BUFSIZ] = {""};
    if(read(socket,buffer,BUFSIZ) < 0) perror("errore nella lettura della richiesta del client\n");
    buffer[strlen(buffer) -1] = '\0';
    strcpy(richiesta,buffer);
    printf("tipo richiesta: %s\n",richiesta);
}

//********************************************************************
//REGISTRAZIONE
//********************************************************************

User initUser(int socket) {
    char data[2][BUFSIZ] = {""};
    char userClient[BUFSIZ] = {""};
    leggiDatiClient(socket,userClient);
    deserialize_data(data,userClient);
    return stringToUser(data);
}

void registrazioneUtente(int socket){
    User user = initUser(socket);
    PGconn *conn;
    if(registration(conn,user)){
        if(send(socket,"Registration\n",strlen("Registration\n"),0) < 0) perror("invio non riuscito");
    }
    if(send(socket,"NoRegistration\n",strlen("NoRegistration\n"),0) < 0) perror("invio non riuscito");
}


//FINE REGISTRAZIONE
//***********************************************************

//***********************************************************
//LOGIN
//***********************************************************

    void leggiDatiClient(int socket, char* userClient) {
        char buffer[BUFSIZ] = {""};
        if(read(socket,buffer,sizeof(buffer)) < 0) perror("errore nella lettura dati client\n");
        buffer[strlen(buffer) - 1] = '\0';
        strcpy(userClient,buffer);
    }

    void checkLogin(int socket){
        char datiClient[2][BUFSIZ];
        char loginClient[BUFSIZ] = {""};
        leggiDatiClient(socket,loginClient);
        deserialize_data(datiClient,loginClient);
        User user=creaUser(datiClient[0],datiClient[1]);
        PGconn *conn;
        if(login(conn,user)){
            char result[BUFSIZ]={""};
            UserToString(user,result);
            if(send(socket,result,strlen(result),0) < 0) perror("invio non riuscito"); //invio user con anche il portafoglio cosi da salvare in android senza fare altre richieste
        }else
          if(send(socket,"noLogin\n",strlen("noLogin\n"),0) < 0) perror("invio non riuscito");
    }

//FINE LOGIN
//****************************************************

//**************************************************
//Aggiungi Denaro
    void addDenaro(int socket){
        char datiClient[2][BUFSIZ];
        char dati[BUFSIZ] = {""};
        leggiDatiClient(socket,dati);
        deserialize_data(datiClient,dati);
        PGconn *conn;
        addPortafoglio(conn,datiClient[0],datiClient[1]);
    }
//**************************************************

//GET DRINKS
    void inviaListaDrink(int socket){
        PGconn *conn;
        DrinkList list = getDrink(conn);
        if(list == NULL)
          if(send(socket,"errore database",strlen("errore database"),0) < 0) perror("invio non riuscito");

        char result[BUFSIZ] ={""};
        serializzaDrinkList(list,result);
        strcat(result,"\n");
        if(send(socket,result,strlen(result),0) < 0) perror("invio non riuscito");
        char richiestaIngredienti[BUFSIZ] = {""};
        leggiRichiesta(socket,richiestaIngredienti);
        if(strcmp(richiestaIngredienti,"ingredienti")==0){
            IngredientList ingredients = getIngredientFromDrinksList(conn,list);
            if(ingredients == NULL) if(send(socket,"errore database",strlen("errore database"),0) < 0) perror("invio non riuscito");
            char serializeIngredients[BUFSIZ] = {""};
            serializzaIngredientList(ingredients,serializeIngredients);
            if(send(socket,serializeIngredients,strlen(serializeIngredients),0) < 0) perror("invio non riuscito");
        }
    }
//*************************************************

//GET DRINKS COCKTAIL
    void inviaListaDrinkCocktail(int socket){
        PGconn *conn;
        DrinkList list = getDrinksCocktail(conn);

        char result[BUFSIZ] ={""};
        serializzaDrinkList(list,result);
        strcat(result,"\n");
        if(send(socket,result,strlen(result),0) < 0) perror("invio non riuscito");

        char richiestaIngredienti[BUFSIZ] = {""};
        leggiRichiesta(socket,richiestaIngredienti);
        if(strcmp(richiestaIngredienti,"ingredienti")==0){
            IngredientList ingredients = getIngredientFromDrinksList(conn,list);
            char serializeIngredients[BUFSIZ] = {""};
            serializzaIngredientList(ingredients,serializeIngredients);
            if(send(socket,serializeIngredients,strlen(serializeIngredients),0) < 0) perror("invio non riuscito");
        }
    }
//**************************************************

//GET DRINKS FRULLATO
    void inviaListaDrinkFrullato(int socket){
        PGconn *conn;
        DrinkList list = getDrinksFrullato(conn);

        char result[BUFSIZ] ={""};
        serializzaDrinkList(list,result);
        strcat(result,"\n");
        if(send(socket,result,strlen(result),0) < 0) perror("invio non riuscito");

        char richiestaIngredienti[BUFSIZ] = {""};
        leggiRichiesta(socket,richiestaIngredienti);
        if(strcmp(richiestaIngredienti,"ingredienti")==0){
            IngredientList ingredients = getIngredientFromDrinksList(conn,list);
            char serializeIngredients[BUFSIZ] = {""};
            serializzaIngredientList(ingredients,serializeIngredients);
            if(send(socket,serializeIngredients,strlen(serializeIngredients),0) < 0) perror("invio non riuscito");
        }
    }
//**************************************************

//BUY DRINKS // CLIENT SEND TOTALE -> QUERY DATABASE PER VEDERE SE È POSSIBILE L'ACQUISTO E POI FARSI MANDARE UN ALTRA RICHIESTA DAL CLIENT E AGGIORNARE DATABASE VENDITE
    void buyDrinks (int socket){
        char datiClient[2][BUFSIZ];
        char dati[BUFSIZ] = {""};
        char richiestaUpdateVednite[BUFSIZ] = {""};
        leggiDatiClient(socket,dati);
        deserialize_data(datiClient,dati);
        PGconn *conn;
        if(substractPortafoglio(conn,datiClient[0],datiClient[1])){
            inviaRespOk(socket);
            leggiRichiesta(socket,richiestaUpdateVednite);
            if(strcmp(richiestaUpdateVednite,"update_vendite")==0){
                inviaRespOk(socket);

                char nomiDrinks[BUFSIZ] = {""};
                leggiDatiClient(socket,nomiDrinks);
                //DESERIALIZE E UPDATE DRINK VENDITE
                char *strtoken = strtok(nomiDrinks,";");
                size_t i = 0;
                while (strtoken != NULL) {
                    PGconn *connUpdate;
                    buyDrinkFromName(conn,strtoken);
                    strtoken = strtok(NULL,";");
                }
            }
        }
        if(send(socket,"soldi_insufficienti\n",strlen("soldi_insufficienti\n"),0) < 0) perror("invio non riuscito");

    }

// PROBLEMA GET INGREDIENTS PER OGNI RICHIESTA HOME COCKTAIL E FRULLATO //
// Risolto vedere se funziona // client deve fare una richiesta per gli ingredienti una volta ricevuti i drinks //


void *gestisci_richiesta(void *arg) {
    int socket = *(int*)arg;
    char client[BUFSIZ] = {""};
    char richiesta[BUFSIZ] = {""};

    leggiClient(socket,client);
    printf("\n");
    inviaRespOk(socket);
    leggiRichiesta(socket,richiesta);
    if(strcmp(richiesta,"AggiungiDenaro") == 0) {
        inviaRespOk(socket);
        addDenaro(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");

    } else if (strcmp(richiesta,"registrazione") == 0) {
        inviaRespOk(socket);
        registrazioneUtente(socket);
        close(socket);
        printf("disconessione con %s avvenuta\n",client);
        printf("**************************************\n");
    }else if(strcmp(richiesta,"login") == 0){
        inviaRespOk(socket);
        checkLogin(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");

    }else if(strcmp(richiesta,"home") == 0) {
        inviaListaDrink(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");

    } else if(strcmp(richiesta,"cocktail") == 0) {
        inviaListaDrinkCocktail(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");

    } else if(strcmp(richiesta,"frullato") == 0) {
        inviaListaDrinkFrullato(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");

    }else if(strcmp(richiesta,"buy") == 0){
        inviaRespOk(socket);
        buyDrinks(socket);
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");
    }else {
        printf("operazione non supportata\n");
        close(socket);
        printf("disconnessione con %s avvenuta\n",client);
        printf("**************************************\n");
    };
}
