#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../../database/database.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int init_server();
void init_address(struct sockaddr_in *address);
void deserialize_data(char data[][BUFSIZ],char *token);
void leggiClient(int socket, char* client);
void inviaRespOk(int socket);
void leggiRichiesta(int socket, char* richiesta);
User initUser(int socket);
void registrazioneUtente(int socket);
void leggiDatiClient(int socket, char* userClient);
void checkLogin(int socket);
void addDenaro(int socket);
void inviaListaDrink(int socket);
void inviaListaDrinkCocktail(int socket);
void inviaListaDrinkFrullato(int socket);
void buyDrinks (int socket);

void *gestisci_richiesta(void *arg);


