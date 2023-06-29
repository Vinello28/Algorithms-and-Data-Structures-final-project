/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_AUTO 512

/**
 * Auto rappresentate dalla seguente struttura dati
 */
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
    Auto *head;             //puntatore alla prima cella del vettore delle autonomie (una per auto)
    struct Stazione* next;
    struct Stazione* prev;
}Stazione;


/**
 * Funzione di inserimento che mantiene la lista ordinata per autonomia decrescente
 * @param st stazione alla quale aggiungere l'auto
 * @param a autonomia (int)
 */
int addAutoInOrder(Stazione* st, int a){
    Auto* newAuto = malloc(sizeof(Auto));
    newAuto->autonomia = a;

    //head insertion
    if (st->head == NULL || a > st->head->autonomia) {
        newAuto->next = st->head;
        st->head = newAuto;
        return 1;
    } else {
        //insertion
        int i=0;
        Auto* current = st->head;
        while (current->next != NULL && a <= current->next->autonomia) {
            if(i<MAX_AUTO) current = current->next;
            else{
                return 0;
            }
            i++;
        }
        newAuto->next = current->next;
        current->next = newAuto;
        return 1;
    }
}

/**
 * Funzione che aggiunge un auto scorrendo le stazioni fino a cercare (se esiste) quella desiderata,
 * poi la passa a metodo che si preoccupa di inserire l'auto mantenendo l'ordinamento decrescente
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param autonomia dell'auto da inserire
 */
void aggiungiAuto(Stazione* head, int dist, int autonomia){
    Stazione* st = head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            int x = addAutoInOrder(st, autonomia); //condiviso con funzione aggiungiStazione(...)
            if(x==1)printf("aggiunta\n");
            else printf("non aggiunta\n");
            return;
        }
        else st = st->next;
    }
}



/**
 * Funzione che elimina un auto dalla lista di una precisa stazione
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param a autonomia dell'auto da eliminare.
 */
void rottamaAuto(Stazione* head, int dist, int a){
    Stazione* st= head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            Auto* tmp = st->head;
            Auto* prev = NULL;

            //ciclo di ricerca dell'auto
            while (tmp != NULL && tmp->autonomia != a) {
                prev = tmp;
                tmp = tmp->next;
            }

            if (tmp != NULL) {                //se è stata trovata:
                if (prev == NULL)st->head = tmp->next;
                else prev->next = tmp->next;
                free(tmp);
                printf("rottamata");
                return;
            }
            else{
                printf("non rottamata");
                return;
            }
        }
        else st = st->next;
    }
}



/**
 * funzione che aggiunge una stazione all'autostrada
 * @param head testa della lista passata da main
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
Stazione* aggiungiStazione(Stazione* head, int dist, int n_a, int* a){
    Stazione* st = malloc(sizeof(Stazione));
    st->distanza = dist;
    st->num_auto=n_a;
    for(int i = 0; i < n_a; i++) addAutoInOrder(st, a[i]);
    st->prev = NULL;
    st->next = NULL;

    if (head == NULL) {
        //La lista è vuota -> st diventa la testa
        head = st;
        printf("aggiunta");
        return head;
    } else if (dist < head->distanza) {
        //Inserimento in testa alla lista
        st->next = head;
        head->prev = st;
        head = st;
        printf("aggiunta");
        return head;
    } else if(dist == head->distanza){
        printf("non aggiunta");
        return NULL;
    } else {
        //Inserimento nel mezzo o in coda
        Stazione* tmp = head;
        while (tmp->next != NULL && dist > tmp->next->distanza) {
            if(tmp->next->distanza==dist){
                printf("non aggiunta");
                return NULL;
            }
            tmp = tmp->next;
        }
        st->next = tmp->next;
        st->prev = tmp;
        if (tmp->next != NULL) tmp->next->prev = st;
        tmp->next = st;
        printf("aggiunta");
        return NULL;
    }
}


/**
 * Funzione che elimina una stazione dalla lista
 * @param head testa della lista passata da main
 * @param dist della stazione da eliminare
 * @return la testa della lista se modificata, altrimenti NULL;
 */
Stazione* demolisciStazione(Stazione* head, int dist){
    Stazione* st = head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            if (head == NULL || st == NULL) {
                // La lista è vuota o la stazione da eliminare è nulla
                printf("non demolita");
                return NULL;
            }

            //Se la stazione da eliminare è la testa della lista
            if (head == st) head = st->next;

            //Collega il nodo precedente al nodo successivo
            if (st->prev != NULL) st->prev->next = st->next;

            //Collega il nodo successivo al nodo precedente
            if (st->next != NULL) st->next->prev = st->prev;

            //Dealloca la memoria della stazione eliminata
            free(st);
            printf("demolita");
            return head;
        }
        else st = st->next;
    }
    printf("non demolita");
    return NULL;
}


//TODO: algoritmo di pianificazione del percorso
void pianificaPercorso(){
    //TODO: implementazione
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
