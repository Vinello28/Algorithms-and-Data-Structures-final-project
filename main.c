/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUTO 512

//TODO eliminare tutti i segmentation fault, finire gestione stazioni e pianificaPercorso

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
 * Struttura dati usata per memorizzare le tappe del percorso
 */
typedef struct Percorso{
    int distanza;
    struct Percorso* next;
}Percorso;

/**
 * Funzione di inserimento che mantiene la lista ordinata per autonomia decrescente
 * @param st stazione alla quale aggiungere l'auto
 * @param a autonomia (int)
 */
void aggiungiAutoByDesc(Auto** testa, int a){
    // Creazione di una nuova auto
    Auto* nuovaAuto = (Auto*)malloc(sizeof(Auto));
    nuovaAuto->autonomia = a;
    nuovaAuto->next = NULL;
    // Controllo se la nuova auto debba diventare la testa della lista
    if (*testa == NULL || a > (*testa)->autonomia) {
        nuovaAuto->next = *testa;
        *testa = nuovaAuto;
        return;
    } else{
        // Inserimento dell'auto nella lista in ordine decrescente di autonomia
        int i=0;
        Auto* current = *testa;
        while (i<MAX_AUTO && current->next != NULL && current->next->autonomia > a) {
            current = current->next;
            i++;
        }
        nuovaAuto->next = current->next;
        current->next = nuovaAuto;
        return;
    }
}


/**
 * Funzione che dealloca la lista di auto data una stazione
 * @param head testa della lista di auto
 */
void deallocaAuto(Auto** head) {
    Auto* attuale = *head;
    Auto* tmp;

    while (attuale != NULL) {
        tmp = attuale->next;
        free(attuale);
        attuale = tmp;
    }
    *head = NULL;
}


/**
 * Funzione che dealloca la lista di stazioni (autostrada)
 * @param head testa dell'autostrada
 */
void deallocaStazioni(Stazione** head) {
    Stazione* attuale = *head;
    Stazione* tmp;

    while (attuale != NULL) {
        tmp = attuale->next;
        deallocaAuto(&attuale->head);
        free(attuale);
        attuale = tmp;
    }
    *head = NULL;
}


/**
 * Funzione che dealloca la lista di tappe (percorso migliore)
 * @param head testa del percorso
 */
void deallocaPercorso(Percorso ** head) {
    Percorso* attuale = *head;
    Percorso* tmp;

    while (attuale != NULL) {
        tmp = attuale->next;
        free(attuale);
        attuale = tmp;
    }
    *head = NULL;
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

    if(st != NULL){
        if(st->distanza == dist){
            st->num_auto++;
            aggiungiAutoByDesc(&st->head, autonomia); //condiviso con funzione aggiungiStazione(...)
            int sentinel = printf("aggiunta\n");
            if(sentinel < 0) return;
            return;
        }
        while((st->next != NULL && st->distanza<=dist) || (st->next==NULL && st->distanza==dist)){
            if(st->distanza==dist) {
                if(st->num_auto+1<MAX_AUTO){
                    st->num_auto++;
                    aggiungiAutoByDesc(&st->head, autonomia); //condiviso con funzione aggiungiStazione(...)
                    int sentinel = printf("aggiunta\n");
                    if(sentinel < 0) return;
                    return;
                } else {
                    int sentinel = printf("non aggiunta\n");
                    if(sentinel < 0) return;
                    return;
                }
            }
            else st = st->next;
        }
    } else if(st==NULL){
        int sentinel = printf("non aggiunta\n");
        if(sentinel < 0) return;
        return;
    }
    int sentinel = printf("non aggiunta\n");
    if(sentinel < 0) return;
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
    tmp->distanza = st->distanza;
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

    if(st->next!=NULL) {
        while ((st->next != NULL && st->distanza<=dist) || (st->next==NULL && st->distanza==dist)) {
            if (st->distanza == dist) {
                Auto *tmp = st->head;
                Auto *prev = NULL;

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
                    int sentinel = printf("rottamata\n");
                    if (sentinel < 0) return;
                    return;
                } else {
                    int sentinel = printf("non rottamata\n");
                    if (sentinel < 0) return;
                    return;
                }
            } else st = st->next;
        }
    } else if(st->distanza==dist){
        Auto *tmp = st->head;
        Auto *prev = NULL;

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
            int sentinel = printf("rottamata\n");
            if (sentinel < 0) return;
            return;
        } else {
            int sentinel = printf("non rottamata\n");
            if (sentinel < 0) return;
            return;
        }
    }
    int sentinel = printf("non rottamata\n");
    if (sentinel < 0) return;
}


/**
 * funzione che aggiunge una stazione all'autostrada
 * @param head testa della lista passata da main
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
void aggiungiStazione(Stazione** testa, int distanza, int numeroAuto, int* autonomie) {
    Stazione* newstz = (Stazione*)malloc(sizeof(Stazione));
    newstz->distanza = distanza;
    newstz->num_auto = numeroAuto;
    newstz->head = NULL;
    newstz->prev = NULL;
    newstz->next = NULL;

    if(numeroAuto>=MAX_AUTO){
        free(newstz);
        int sentinel = printf("non aggiunta\n");
        if(sentinel<0)return;
        return;
    }

    //Aggiunta delle autonomie delle auto alla lista delle auto
    for (int i = 0; i < numeroAuto; i++) {
        Auto* nuovaAuto = (Auto*)malloc(sizeof(Auto));
        nuovaAuto->autonomia = autonomie[i];
        nuovaAuto->next = NULL;
        aggiungiAutoByDesc(&newstz->head, autonomie[i]);
    }

    // Inserimento della stazione nella lista in ordine crescente di distanza
    if (*testa == NULL) {
        *testa = newstz;
        int sentinel = printf("aggiunta\n");
        if(sentinel<0)return;
        return;

    } else if (distanza < (*testa)->distanza) {
        newstz->next = *testa;
        (*testa)->prev = newstz;
        *testa = newstz;
        int sentinel = printf("aggiunta\n");
        if(sentinel<0)return;
        return;
    } else {
        Stazione* current = *testa;
        while (current != NULL) {
            if (current->distanza == distanza) {
                int sentinel = printf("non aggiunta\n");
                if (sentinel < 0) return;
                return;
            }
            if (current->next != NULL && current->next->distanza>distanza){ //inserimento tra due stazioni
                current->next->prev = newstz;
                newstz->next=current->next;
                newstz->prev=current;
                current->next=newstz;
                int sentinel = printf("aggiunta\n");
                if(sentinel<0)return;
                return;
            }
            if(current->next==NULL && current->distanza<distanza){
                newstz->prev=current;
                current->next=newstz;
                newstz->next=NULL;
                int sentinel = printf("aggiunta\n");
                if(sentinel<0)return;
                return;
            }
            current = current->next;
        }
        if(current->distanza == distanza){
            deallocaAuto(&newstz->head);
            free(newstz);
            int sentinel = printf("non aggiunta\n");
            if(sentinel<0)return;
            return;
        }
    }
}


/**
 * Funzione che elimina una stazione dalla lista
 * @param head testa della lista passata da main
 * @param dist della stazione da eliminare
 * @return la testa della lista se modificata, altrimenti NULL;
 */
void demolisciStazione(Stazione** head, int dist){
    if (head==NULL) {
        int sentinel = printf("non demolita\n");
        if(sentinel <0) return;
        return;
    }
    Stazione* st = *head;

    while (st != NULL && st->distanza != dist) st = st->next;

    if (st == NULL) {
        int sentinel = printf("non demolita\n");
        if(sentinel <0) return;
        return;
    }
    if (st == *head) *head = st->next;
    if (st->prev != NULL) st->prev->next = st->next;
    if (st->next != NULL) st->next->prev = st->prev;

    deallocaAuto(&st->head);
    free(st);
    int sentinel = printf("demolita\n");
    if(sentinel <0) return;
}


/**
 * Funzione che data una distanza individua una stazione
 * @param head testa della lista delle stazioni
 * @param dist che identifica univocamente la stazione
 * @return stazione cercata
 */
Stazione* cercaStazione(Stazione** head, int dist){
    Stazione* st = *head;
    if(st->next!=NULL) {
        while (st->next->distanza <= dist && st->next != NULL) {
            if (st->distanza == dist) return st;
            st = st->next;
        }
    }
    else {
        if(st->distanza==dist) return st;
        else return NULL;
    }
    return NULL;
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
    if(st==NULL){
        int sentinel = printf("nessun percorso");
        if(sentinel <0) return;
        return;
    }

    Percorso* p = malloc(sizeof(Percorso));

    if(d_start == d_end){               //stazioni coincidenti
        int sentinel = printf("%d", d_start);
        if(sentinel <0) return;
        return;
    }
    if(d_start<d_end){                  //ricerca in avanti
        int d = d_end-d_start;
        int a = st->head->autonomia;
        if (a >= d){
            int sentinel = printf("%d %d", d_start, d_end);
            if(sentinel <0) return;
            return;
        }

        p->distanza = st->distanza;
        p->next = NULL;

        while (st->next != NULL){
            if((a >= (st->next->distanza - st->distanza))){
                if(st->next->head->autonomia >= d){
                    aggiungiTappa(&p, st->next);
                    Stazione* tmp = cercaStazione(head,d_end);
                    if(tmp == NULL){
                        deallocaPercorso(&p);
                        int sentinel = printf("nessun percorso");
                        if(sentinel <0) return;
                        return;
                    }
                    aggiungiTappa(&p, tmp);
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
            else {
                deallocaPercorso(&p);
                int sentinel = printf("nessun percorso");
                if(sentinel <0) return;
                return;
            }
        }
    }
    if(d_start>d_end){                  //ricerca all'indietro
        int d = d_start - d_end;
        int a = st->head->autonomia;
        if (a >= d){
            int sentinel = printf("%d %d", d_start, d_end);
            if(sentinel <0) return;
            return;
        }

        p->distanza = st->distanza;
        p->next = NULL;

        while (st->prev != NULL){
            if((a >= (st->distanza - st->prev->distanza))){
                if(st->prev->head->autonomia >= d){
                    aggiungiTappa(&p, st->prev);
                    Stazione* tmp = cercaStazione(head,d_end);
                    if(tmp == NULL){
                        deallocaPercorso(&p);
                        int sentinel = printf("nessun percorso");
                        if(sentinel <0) return;
                        return;
                    }
                    aggiungiTappa(&p, tmp);
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
            else {
                deallocaPercorso(&p);
                int sentinel = printf("nessun percorso");
                if(sentinel <0) return;
                return;
            }
        }
    }
    int sentinel;
    while (p != NULL){
        sentinel = printf("%d ",p->distanza);
        if(sentinel < 0) return;
        p = p->next;
    }
    printf("\n");
    deallocaPercorso(&p);
}


int main() {
    Stazione *head = NULL;
    FILE* source = stdin;
    char input[20];
    while(fscanf(source, "%s", input) != EOF){
        if(strncmp(input, "aggiungi-stazione", strlen("aggiungi-stazione"))==0) {
            int dist, n_a;
            if(fscanf(source, "%d %d", &dist, &n_a)!=EOF){
                int a[512];
                for(int i = 0; i < n_a; i++)if(fscanf(source, "%d", &a[i])!=EOF)continue;
                aggiungiStazione(&head, dist, n_a, a);
            }
        }
        if(strncmp(input, "aggiungi-auto", strlen("aggiungi-auto"))==0) {
            int dist, a;
            printf("INFO: spacchettamento comando\n");
            if(fscanf(source, "%d %d", &dist, &a) != EOF) aggiungiAuto(&head, dist, a);
        }
        if(strncmp(input, "rottama-auto", strlen("rottama-auto"))==0) {
            int dist, a;
            if(fscanf(source, "%d %d", &dist, &a) != EOF) rottamaAuto(&head, dist, a);
        }
        if(strncmp(input, "demolisci-stazione", strlen("demolisci-stazione"))==0) {
            int dist;
            if(fscanf(source, "%d", &dist)!=EOF) demolisciStazione(&head, dist);
        }
        if(strncmp(input, "pianifica-percorso", strlen("pianifica-percorso"))==0) {
            int start, end;
            if(fscanf(source, "%d %d", &start, &end)!=EOF) pianificaPercorso(&head, start, end);
        }
    }

    deallocaStazioni(&head);
    return 0;
}
