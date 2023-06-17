#include "../dependencies.h"
#include <sys/socket.h>
#include <pthread.h>
#define COCKTAIL "Cocktail"
#define FRULLATO "Frullato"

/* Definizione strutture */
typedef struct Drink_struct
{
    char nome_drink[BUFSIZ];
    char descrizione[BUFSIZ];
    int vendite;
    float costo;
    bool frullato;
}Drink_struct;

typedef struct Drink_struct *Drink;

typedef struct DrinkList_struct
{
    Drink drink;
    struct DrinkList_struct *next;
}DrinkList_struct;

typedef struct DrinkList_struct *DrinkList;

typedef struct Ingredient_struct{
    char ingrediente[BUFSIZ];
    char nome_drink[BUFSIZ];
}Ingredient_struct;

typedef struct Ingredient_struct *Ingredient;

typedef struct IngredientList_struct
{
    Ingredient ingredient;
    struct IngredientList_struct *next;

}IngredientList_struct;

typedef struct IngredientList_struct *IngredientList;


/* Definizione metodi */
Drink creaDrink(char *nome_drink,char *descrizione,int vendite,float costo,bool frullato);
DrinkList creaDrinkList(Drink drink);
DrinkList inserisciDrink(DrinkList list,Drink drink);
int lenDrinkList(DrinkList list);
Ingredient creaIngredient(char *ingrediente,char *nome_drink);
IngredientList creaIngredientList(Ingredient ingredient);
IngredientList inserisciIngredient(IngredientList list,Ingredient ingredient);
void deallocaListaIngredient(IngredientList list);
void deallocaLista(DrinkList list);
char* DrinkToString(Drink drink, char *result);
void serializzaDrinkList(DrinkList drinkList, char *result);
char *ingredientToString(Ingredient ingredient,char *result);
void serializzaIngredientList(IngredientList list,char *result);
void printDrink(Drink drink);
void printDrinkList(DrinkList drinkList);
Drink stringToDrink(char data[][BUFSIZ]);




char* drinkToString(Drink drink,char *result);

void printDrink(Drink drink);
void printDrinkList(DrinkList list);
void deallocaLista(DrinkList list);
void serializzaDrinkList(DrinkList drinkList,char *result);
