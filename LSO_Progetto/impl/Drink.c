#include "../lib/struct/Drink.h"

Drink creaDrink(char *nome_drink,char *descrizione,int vendite,float costo,bool frullato) {
        Drink drink = (Drink) malloc(sizeof(struct Drink_struct));
        drink->frullato=frullato;
        strcpy(drink->nome_drink,nome_drink);
        strcpy(drink->descrizione,descrizione);
        drink->vendite=vendite;
        drink->costo=costo;
        return drink;
}

DrinkList creaDrinkList(Drink drink) {
    if(drink != NULL) {
        DrinkList dr = (DrinkList) malloc(sizeof(struct DrinkList_struct));
        dr->drink = drink;
        dr->next = NULL;
        return dr;
    }
    return NULL;
}

DrinkList inserisciDrink(DrinkList list, Drink drink) {
    if(list == NULL) return creaDrinkList(drink);
    list->next = inserisciDrink(list->next,drink);
    return list;
}

int lenDrinkList(DrinkList list){
    if(list == NULL) return 0;
    return lenDrinkList(list->next) + 1;
}

Ingredient creaIngredient(char *ingredient,char *nome_drink){
    Ingredient in = (Ingredient) malloc(sizeof(struct Ingredient_struct));
    strcpy(in->ingrediente,ingredient);
    strcpy(in->nome_drink,nome_drink);
    return in;
}

IngredientList creaIngredientList(Ingredient ingredient){
    if(ingredient != NULL) {
        IngredientList in = (IngredientList) malloc(sizeof(struct IngredientList_struct));
        in->ingredient = ingredient;
        in->next = NULL;
        return in;
    }
    return NULL;
}

IngredientList inserisciIngredient(IngredientList list,Ingredient ingredient){
    if(list == NULL) return creaIngredientList(ingredient);
    list->next = inserisciIngredient(list->next,ingredient);
    return list;
}
void deallocaListaIngredient(IngredientList list) {
    if(list == NULL) return;
    deallocaListaIngredient(list->next);
    free(list);
}

void deallocaLista(DrinkList list) {
    if(list == NULL) return;
    deallocaLista(list->next);
    free(list);
}

char* DrinkToString(Drink drink, char *result) {
    sprintf(result,"%s;%s;%d;%lf;%d\n",drink->nome_drink,drink->descrizione,drink->vendite,drink->costo,drink->frullato);
    return result;
}

void serializzaDrinkList(DrinkList drinkList, char *result) {
    if(drinkList != NULL) {
        char buffer[BUFSIZ];
        strcat(result,DrinkToString(drinkList->drink,buffer));
        serializzaDrinkList(drinkList->next,result);
    }
}

char *ingredientToString(Ingredient ingredient,char *result){
    sprintf(result,"%s;%s\n",ingredient->ingrediente,ingredient->nome_drink);
    return result;
}

void serializzaIngredientList(IngredientList list,char *result){
    if(list != NULL) {
        char buffer[BUFSIZ];
        strcat(result,ingredientToString(list->ingredient,buffer));
        serializzaIngredientList(list->next,result);
    }
}