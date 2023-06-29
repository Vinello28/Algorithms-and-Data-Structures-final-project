/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_AUTO 512


typedef struct Auto{
    int autonomia;
    struct Auto* next;
}Auto;

/**
 * l'autostrada è rappresentata da una lista doppiamente concatenata di stazioni
 */
typedef struct Stazione{
    int distanza;
    int num_auto;
    Auto *head;         //puntatore alla prima cella del vettore delle autonomie (una per auto)
    struct Stazione* next;
    struct Stazione* prev;
}Stazione;

Stazione head;
Stazione* last;

//prototipi
void addAutoInOrder(Stazione *st, int a);

/**
 * Aggiunge un auto alla stazione indicata
 */
void aggiungiAuto(int dist, int autonomia){
    Stazione* st=&head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            addAutoInOrder(st, autonomia);
            return;
        }
        else st = st->next;
    }
}

/**
 * Funzione di inserimento che mantiene la lista ordinata per autonomia decrescente
 * @param st stazione alla quale aggiungere l'auto
 * @param a autonomia (int)
 */
void addAutoInOrder(Stazione* st, int a){
    Auto* newAuto = malloc(sizeof(Auto));
    newAuto->autonomia = a;

    //head insertion
    if (st->head == NULL || a > st->head->autonomia) {
        newAuto->next = st->head;
        st->head = newAuto;
        printf("aggiunta");
        return;
    } else {
        //insertion
        int i=0;
        Auto* current = st->head;
        while (current->next != NULL && a <= current->next->autonomia) {
            if(i<MAX_AUTO) current = current->next;
            else{
                printf("non aggiunta");
                return;
            }
            i++;
        }
        newAuto->next = current->next;
        current->next = newAuto;
        printf("aggiunta");
        return;
    }
}

/**
 *
 */
void removeInOrder(){
    //TODO: rimozione delle auto in ordine
}



/**
 * funzione che aggiunge una stazione all'autostrada
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
void aggiungiStazione(int dist, int n_a, char* autonomie){
    Stazione* st = malloc(sizeof(Stazione));
    st->distanza=dist;
    st->num_auto=n_a;
    st->autonomie=v_a;

    //la seguente parte di codice aggiunge le stazioni in sequenza senza tener conto della distanza
    st->prev=last;
    st->next=NULL;
    last = st;
    //TODO devono essere in ordine?
}

void headInit(){
    head.distanza=0;
    head.prev=NULL;
    head.num_auto=0;
    last = &head;
}


int main() {
    printf("Hello, World!\n");
    return 0;
}
