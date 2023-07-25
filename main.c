/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Struttura dati usata per memorizzare le tappe del percorso
 */
typedef struct Tappa{
    int distanza;
    struct Tappa* next;
}Tappa;

typedef struct Percorso{
    int n_tappe;
    Tappa* tappe;
    struct Percorso* next;
}Percorso;


/**
 * Funzione di inserimento che mantiene la lista ordinata per autonomia decrescente
 * @param st stazione alla quale aggiungere l'auto
 * @param a autonomia (int)
 */
 void aggiungiAutoByDesc(Auto** testa, int a) {
    Auto* nuovaAuto = malloc(sizeof(Auto));
    nuovaAuto->autonomia = a;
    nuovaAuto->next = NULL;

    //Check testa
    if (*testa == NULL || a > (*testa)->autonomia) {
        nuovaAuto->next = *testa;
        *testa = nuovaAuto;
        return;
    }

    //inserimento per ordine decrescente di autonomia
    Auto* current = *testa;
    while (current->next != NULL && current->next->autonomia > a) {
        current = current->next;
    }
    nuovaAuto->next = current->next;
    current->next = nuovaAuto;
}


/**
 * funzione che restituisce il massimo tra due interi in ingresso
 * @param a primo valore
 * @param b secondo valore
 * @return valore maggiore
 */
int max(int a, int b) {
    return (a > b) ? a : b;
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
void deallocaTappa(Tappa** head) {
    Tappa* attuale = *head;
    Tappa* tmp;

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
            if(printf("aggiunta\n")<0)return;
            return;
        }
        while((st->next != NULL && st->distanza<=dist) || (st->next==NULL && st->distanza==dist)){
            if(st->distanza==dist) {
                if(st->num_auto+1<MAX_AUTO){
                    st->num_auto++;
                    aggiungiAutoByDesc(&st->head, autonomia);
                    if(printf("aggiunta\n")<0)return;
                    return;
                } else {
                    if(printf("non aggiunta\n")<0)return;
                    return;
                }
            }
            else st = st->next;
        }
    } else if(st==NULL){
        if(printf("non aggiunta\n")<0)return;
        return;
    }
    if(printf("non aggiunta\n")<0)return;
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

                while (tmp != NULL && tmp->autonomia != a) { //ricerca auto
                    prev = tmp;
                    tmp = tmp->next;
                }

                if (tmp != NULL) {        //se trovo auto
                    if (prev == NULL)st->head = tmp->next;
                    else prev->next = tmp->next;
                    free(tmp);
                    if(printf("rottamata\n")<0)return;
                    return;
                } else {
                    if(printf("non rottamata\n")<0)return;
                    return;
                }
            } else st = st->next;
        }
    } else if(st->distanza==dist){
        Auto *tmp = st->head;
        Auto *prev = NULL;

        while (tmp != NULL && tmp->autonomia != a) { //ricerca auto
            prev = tmp;
            tmp = tmp->next;
        }

        if (tmp != NULL) {    //se auto trovata
            if (prev == NULL)st->head = tmp->next;
            else prev->next = tmp->next;
            free(tmp);
            if(printf("rottamata\n")<0)return;
            return;
        } else {
            if(printf("non rottamata\n")<0)return;
            return;
        }
    }
    if(printf("non rottamata\n")<0)return;
}


/**
 * funzione che aggiunge una stazione all'autostrada
 * @param head testa della lista passata da main
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
void aggiungiStazione(Stazione** testa, int distanza, int numeroAuto, int* autonomie) {
    Stazione* n_st = (Stazione*)malloc(sizeof(Stazione));
    n_st->distanza = distanza;
    n_st->num_auto = numeroAuto;
    n_st->head = NULL;
    n_st->prev = NULL;
    n_st->next = NULL;

    if(numeroAuto>=MAX_AUTO){
        free(n_st);
        if(printf("non aggiunta\n")<0)return;
        return;
    }

    //Aggiunta delle autonomie delle auto alla lista delle auto
    for (int i = 0; i < numeroAuto; i++) {
        Auto* nuovaAuto = (Auto*)malloc(sizeof(Auto));
        nuovaAuto->autonomia = autonomie[i];
        nuovaAuto->next = NULL;
        aggiungiAutoByDesc(&n_st->head, autonomie[i]);
    }

    // Inserimento della stazione nella lista in ordine crescente di distanza
    if (*testa == NULL) {
        *testa = n_st;
        if(printf("aggiunta\n")<0)return;
        return;

    } else if (distanza < (*testa)->distanza) {
        n_st->next = *testa;
        (*testa)->prev = n_st;
        *testa = n_st;
        if(printf("aggiunta\n")<0)return;
        return;
    } else {
        Stazione* current = *testa;
        while (current != NULL) {
            if (current->distanza == distanza) {
                if(printf("non aggiunta\n")<0)return;
                return;
            }
            if (current->next != NULL && current->next->distanza>distanza){ //inserimento tra due stazioni
                current->next->prev = n_st;
                n_st->next=current->next;
                n_st->prev=current;
                current->next=n_st;
                if(printf("aggiunta\n")<0)return;
                return;
            }
            if(current->next==NULL && current->distanza<distanza){
                n_st->prev=current;
                current->next=n_st;
                n_st->next=NULL;
                if(printf("aggiunta\n")<0)return;
                return;
            }
            current = current->next;
        }
        if(current->distanza == distanza){
            deallocaAuto(&n_st->head);
            free(n_st);
            if(printf("non aggiunta\n")<0)return;
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
        if(printf("non demolita\n")<0)return;
        return;
    }
    Stazione* st = *head;

    while (st != NULL && st->distanza != dist) st = st->next;

    if (st == NULL) {
        if(printf("non demolita\n")<0)return;
        return;
    }
    if (st == *head) *head = st->next;
    if (st->prev != NULL) st->prev->next = st->next;
    if (st->next != NULL) st->next->prev = st->prev;

    deallocaAuto(&st->head);
    free(st);
    if(printf("demolita\n")<0)return;
}


/**
 * Funzione che data una distanza individua una stazione
 * @param head testa della lista delle stazioni
 * @param dist che identifica univocamente la stazione
 * @return stazione cercata
 */
Stazione* cercaStazione(Stazione** head, int dist) {
    Stazione* st = *head;
    while (st != NULL) {
        if (st->distanza == dist) return st;
        if (st->next != NULL && st->next->distanza <= dist) {
            st = st->next;
        } else {
            break;
        }
    }
    return NULL;
}


/**
 * Funzione che si occupa di mandare in stdout il percorso ottimale
 * @param percorso puntatore al puntatore del percorso ottimale
 */
void stdoutPercorso(Percorso** percorso) {
    if (*percorso == NULL || (*percorso)->tappe == NULL) {
        if (printf("nessun percorso\n") < 0) return;
        return;
    }
    int i = (*percorso)->n_tappe;
    int distanze[i];
    Tappa* p = (*percorso)->tappe;

    while(p!=NULL){
        distanze[i-1] = p->distanza;
        i--;
        p=p->next;
    }
    for(int j = 0; j<(*percorso)->n_tappe; j++)if (printf("%d ", distanze[j]) < 0) return;
    if (printf("\n") < 0) return;

    //deallocazione percorso
    deallocaTappa(&((*percorso)->tappe));
    free(*percorso);
    *percorso = NULL;
}


/**
 * Funzione che copia le sequenza di tappe percorribili ottimale
 * @param source testa dell'elenco di tappe da copiare
 * @return copia del percorso
 */
Tappa* copiaPercorso(Tappa* source) {
    if (source == NULL) {
        return NULL;
    }

    Tappa* newTappa = malloc(sizeof(Tappa));
    newTappa->distanza = source->distanza;
    newTappa->next = copiaPercorso(source->next);
    return newTappa;
}

/**
 * Data la testa della lista di tappe di un percorso la dealloca
 * @param head testa lista (contenuta in un percorso)
 */
void deallocaPercorso(Tappa* head) {
    while (head != NULL) {
        Tappa* tmp = head;
        head = head->next;
        free(tmp);
    }
}

/**
 * Funzione che sostiuisce il vecchio percorso ottimale con quello nuovo (ex parziale)
 * @param percorsoOttimale percorso che sarà sostituito
 * @param percorsoParziale nuovo percorso ottimale
 */
void scambiaPercorsi(Percorso** percorsoOttimale, Tappa* percorsoParziale) {
    if (*percorsoOttimale != NULL) {
        deallocaPercorso((*percorsoOttimale)->tappe);
        free(*percorsoOttimale);
    }

    *percorsoOttimale = malloc(sizeof(Percorso));
    (*percorsoOttimale)->n_tappe = 0;
    (*percorsoOttimale)->tappe = copiaPercorso(percorsoParziale);
    Tappa* t = (*percorsoOttimale)->tappe;
    while (t != NULL) {
        (*percorsoOttimale)->n_tappe++;
        t = t->next;
    }
}


/**
 * Funzione per ricerca ricorsiva dei percorsi ottimali, che vengono poi aggiunti in apposita struttura dati
 * @param st_corrente nella prima iterazione è quella di partenza, poi viene usata per tenere traccia per st. corrente
 * @param dist_final distanza alla quale deve "arrivare" l'utente
 * @param autonomia dell'auto attualmente in uso
 * @param p_parziale percorso che si sta attualmente percorrendo
 * @param p_ottimale miglior percorso trovate fino all'istante attuale
 */
void ricercaPercorsiInAvanti(Stazione* st_corrente, int dist_final, int autonomia, Tappa* p_parziale, Percorso** p_ottimale) {
    if (st_corrente->distanza + autonomia >= dist_final) {   //fine ricorsione poiché ho trovato l'ultima tappa del percorso

        Tappa* n_tappa = malloc(sizeof(Tappa));  //aggiunge stazione finale al percorso parziale
        n_tappa->distanza = dist_final;
        n_tappa->next = p_parziale;
        p_parziale = n_tappa;

        int tappe = 0;
        Tappa* tmp= p_parziale;
        while(tmp!=NULL){      //conta le tappe
            tappe++;
            tmp = tmp->next;
        }
        if (tappe < (*p_ottimale)->n_tappe) scambiaPercorsi(p_ottimale, p_parziale);
        return;
    }
    //se stazione corrente è ultima stazione dell'autostrada
    if (st_corrente->next == NULL) return;
    //se stazione corrente non ha auto o tutte le auto hanno autonomia nulla
    if (st_corrente->head == NULL || st_corrente->head->autonomia == 0) return;

    Stazione* st_successiva = st_corrente->next;
    while (st_successiva != NULL && st_successiva->distanza <= st_corrente->distanza + autonomia) {
        int n_autonomia = autonomia - (st_successiva->distanza - st_corrente->distanza);

        //check cambio auto
        Auto* auto_attuale = st_successiva->head;
        if (auto_attuale != NULL) n_autonomia = max(n_autonomia, auto_attuale->autonomia);

        //aggiunge stazione in percorso parziale
        Tappa* nuovaTappa = malloc(sizeof(Tappa));
        nuovaTappa->distanza = st_successiva->distanza;
        nuovaTappa->next = p_parziale;
        p_parziale = nuovaTappa;

        //ricorsione per stazioni successive
        ricercaPercorsiInAvanti(st_successiva, dist_final, n_autonomia, p_parziale, p_ottimale);

        p_parziale = p_parziale->next;
        st_successiva = st_successiva->next;
    }
}


//TODO: ricerca percorsi all'indietro

/**
 * Principale funzione del programma, si occupa di cercare il percorso con meno tappe tra due stazioni date,
 * si assume che queste siano sempre presenti
 * @param head testa della lista delle stazioni
 * @param d_start distanza stazione di partenza
 * @param d_end distanza stazione arrivo
 */
void pianificaPercorso(Stazione** head, int d_start, int d_end) {
    //trova le stazioni di partenza e arrivo
    Stazione* stazionePartenza = cercaStazione(head, d_start);
    Stazione* stazioneArrivo = cercaStazione(head, d_end);

    //controlla che le stazioni di partenza e arrivo siano valide
    if (stazionePartenza == NULL || stazioneArrivo == NULL) {
        if(printf("nessun percorso\n")<0) return;
        return;
    }

    //allocazione percorso ottimale e parziale
    Percorso* percorsoOttimale = malloc(sizeof(Percorso));
    percorsoOttimale->n_tappe = 10000000;
    percorsoOttimale->tappe = NULL;
    Tappa* percorsoParziale = NULL;

    //allocazione prima tappa del percorso parziale prima tappa (partenza)
    Tappa* nuovaTappa = malloc(sizeof(Tappa));
    nuovaTappa->distanza = stazionePartenza->distanza;
    nuovaTappa->next = percorsoParziale;
    percorsoParziale = nuovaTappa;

    //chiama la funzione ricorsiva per trovare il percorso ottimale
    ricercaPercorsiInAvanti(stazionePartenza, d_end, stazionePartenza->head->autonomia, percorsoParziale,
                            &percorsoOttimale);

    if (percorsoOttimale==NULL || percorsoOttimale->n_tappe==10000) {
        if(printf("nessun percorso\n")<0) return;
        return;
    }

    stdoutPercorso(&percorsoOttimale); //stdout del percorso migliore
    free(percorsoOttimale);
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
