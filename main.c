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

typedef struct Percorso{
    Stazione x;
    struct Percorso* next;
}Percorso;

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


void deallocaStazioni(Stazione* head) {//TODO da usare per deallocare l'autostrada.
    Stazione* attuale = head;
    Stazione* tmp;

    while ( != NULL) {
        temp = current->next;
        free(current);
        current = temp;
    }

    head = NULL;
}


/**
 * Funzione che aggiunge un auto scorrendo le stazioni fino a cercare (se esiste) quella desiderata,
 * poi la passa a metodo che si preoccupa di inserire l'auto mantenendo l'ordinamento decrescente
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param autonomia dell'auto da inserire
 */
void aggiungiAuto(Stazione** head, int dist, int autonomia){
    Stazione* st = *head;
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
 * Funzione che si occupa di aggiungere una stazione al percorso in esame
 * @param head testa della lista passata da cercaPercorso
 * @param st stazione da aggiungere
 */
void aggiungiTappa(Percorso **head, Stazione* st){
    Percorso* p = *head;
    while(p->next != NULL){
        p=p->next;
    }
    Percorso* tmp = malloc(sizeof(Percorso));
    tmp->x = *st;
    tmp->next=NULL;
    p->next=tmp;
}

/**
 * Funzione che elimina un auto dalla lista di una precisa stazione
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param a autonomia dell'auto da eliminare.
 */
void rottamaAuto(Stazione** head, int dist, int a){
    Stazione* st= *head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            Auto* tmp = st->head;
            Auto* prev = NULL;

            //ciclo di ricerca dell'auto
            while (tmp != NULL && tmp->autonomia != a) {
                prev = tmp;
                tmp = tmp->next;
            }

            //se è stata trovata:
            if (tmp != NULL) {
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
void aggiungiStazione(Stazione** head, int dist, int n_a, int* a) {
    Stazione* st = malloc(sizeof(Stazione));
    st->distanza = dist;
    st->num_auto = n_a;
    st->prev = NULL;
    st->next = NULL;

    //Creazione e inserimento delle auto nella lista ordinata per autonomie decrescenti
    for (int i = 0; i < n_a; i++) addAutoInOrder(st, a[i]);

    if (*head == NULL) {
        //se lista vuota -> st diventa la testa
        *head = st;
        printf("aggiunta\n");
        return;
    } else if (dist < (*head)->distanza) {
        // Inserimento in testa alla lista
        st->next = *head;
        (*head)->prev = st;
        *head = st;
        printf("aggiunta\n");
        return;
    } else if (dist == (*head)->distanza) {
        printf("non aggiunta\n");
        return;
    } else {
        // Inserimento nel mezzo o in coda
        Stazione* tmp = *head;
        while (tmp->next != NULL && dist > tmp->next->distanza) {
            if (tmp->next->distanza == dist) {
                printf("non aggiunta\n");
                return;
            }
            tmp = tmp->next;
        }
        st->next = tmp->next;
        st->prev = tmp;
        if (tmp->next != NULL) {
            tmp->next->prev = st;
        }
        tmp->next = st;
        printf("aggiunta\n");
        return;
    }
}



/**
 * Funzione che elimina una stazione dalla lista
 * @param head testa della lista passata da main
 * @param dist della stazione da eliminare
 * @return la testa della lista se modificata, altrimenti NULL;
 */
void demolisciStazione(Stazione** head, int dist){
    Stazione* st = *head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) {
            if (head == NULL || st == NULL) {
                // La lista è vuota o la stazione da eliminare è nulla
                printf("non demolita");
                return;
            }

            //Se la stazione da eliminare è la testa della lista
            if (*head == st) *head = st->next;

            //Collega il nodo precedente al nodo successivo
            if (st->prev != NULL) st->prev->next = st->next;

            //Collega il nodo successivo al nodo precedente
            if (st->next != NULL) st->next->prev = st->prev;

            //Dealloca la memoria della stazione eliminata
            free(st);
            printf("demolita");
            return;
        }
        else st = st->next;
    }
    printf("non demolita");
}


/**
 * Funzione che data una distanza individua una stazione
 * @param head testa della lista delle stazioni
 * @param dist che identifica univocamente la stazione
 * @return stazione cercata
 */
Stazione* cercaStazione(Stazione** head, int dist){
    Stazione* st = *head;
    while(st->next->distanza<=dist && st->next != NULL){
        if(st->distanza==dist) return st;
        st = st->next;
    }
    return *head; //da rivedere poiche non arriverà a questo punto.
}

/**
 * Principale funzione del programma, si occupa di cercare il percorso con meno tappe tra due stazioni date,
 * si assume che queste siano sempre presenti
 * @param head testa della lista delle stazioni
 * @param d_start distanza stazione di partenza
 * @param d_end distanza stazione arrivo
 */
void pianificaPercorso(Stazione** head, int d_start, int d_end){
    Stazione* st = cercaStazione(head, d_start);
    Percorso* p = malloc(sizeof(Percorso));

    if(d_start == d_end){               //stazioni coincidenti
        printf("%d", d_start);
        return;
    }
    if(d_start<d_end){                  //ricerca in avanti
        int d = d_end-d_start;
        int a = st->head->autonomia;
        if (a >= d){
            printf("%d %d", d_start, d_end);
            return;
        }

        p->x = *st;
        p->next = NULL;

        while (st->next != NULL){
            if((a >= (st->next->distanza - st->distanza))){
                if(st->next->head->autonomia >= d){
                    aggiungiTappa(&p, st->next);
                    aggiungiTappa(&p, cercaStazione(head,d_end));
                    break;
                }
                if((a - ((st->next->distanza - st->distanza))) < st->next->head->autonomia){
                    a=st->next->head->autonomia;
                    aggiungiTappa(&p, st->next);
                }else a = a - ((st->next->distanza - st->distanza));
                //decrementa la distanza attuale
                d = d-st->next->distanza;
                //passa al prossimo elemento della lista
                st=st->next;
            }
            else{
                printf("nessun percorso");
            }
        }
    }
    if(d_start>d_end){                  //ricerca all'indietro
        int d = d_start - d_end;
        int a = st->head->autonomia;
        if (a >= d){
            printf("%d %d", d_start, d_end);
            return;
        }

        p->x = *st;
        p->next = NULL;

        while (st->prev != NULL){
            if((a >= (st->distanza - st->prev->distanza))){
                if(st->prev->head->autonomia >= d){
                    aggiungiTappa(&p, st->prev);
                    aggiungiTappa(&p, cercaStazione(head,d_end));
                    break;
                }
                if((a - (st->distanza - st->prev->distanza)) < st->prev->head->autonomia){
                    a=st->prev->head->autonomia;
                    aggiungiTappa(&p, st->prev);
                }else a = a - (st->distanza - st->prev->distanza);
                //decrementa la distanza attuale
                d = d-st->prev->distanza;
                //passa ad elemento precedente della lista
                st=st->prev;
            }
            else{
                printf("nessun percorso");
            }
        }
      //TODO stampa percorsi e deallocazioni varie
    }


    //TODO printf percorsi
}



int main() {
    printf("Hello, World!\n");
    return 0;
}
