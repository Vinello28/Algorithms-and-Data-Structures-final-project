/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <malloc.h>

/**
 * l'autostrada è rappresentata da una lista doppiamente concatenata di stazioni
 */
typedef struct Stazione{
    int distanza;
    int num_auto;
    int** autonomie;         //puntatore alla prima cella del vettore delle autonomie (una per auto)
    struct Stazione* next;
    struct Stazione* prev;
}Stazione;

Stazione head;
Stazione* last;

/**
 * funzione che aggiunge una stazione all'autostrada
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
void aggiungiStazione(int dist, int n_a, int** v_a){
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

Stazione* previousStation(int dist){
    //TODO ritorna la stazione prima della distanza data
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
