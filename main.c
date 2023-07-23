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

    // Inserimento dell'auto nella lista in ordine decrescente di autonomia
    Auto* current = *testa;
    while (current->next != NULL && current->next->autonomia > a) {
        current = current->next;
    }
    nuovaAuto->next = current->next;
    current->next = nuovaAuto;
}


/**
 * ausiliaria per ricerca percorso
 * @param a
 * @param b
 * @return
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
void aggiungiTappa(Tappa **head, Stazione* st){
    Tappa* p = *head;
    if (p == NULL) {
        //inserimento in testa
        *head = malloc(sizeof(Tappa));
        (*head)->distanza = st->distanza;
        (*head)->next = NULL;
        return;
    } else {
        //scorre fino alla fine
        while (p->next != NULL) p = p->next;

        //inserimento in coda
        p->next = malloc(sizeof(Tappa));
        p->next->distanza = st->distanza;
        p->next->next = NULL;
    }
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

    Tappa* p = (*percorso)->tappe;
    while (p != NULL) {
        if (printf("%d ", p->distanza) < 0) return;
        p = p->next;
    }
    if (printf("\n") < 0) return;

    // Dealloca il percorso dopo averlo stampato
    deallocaTappa(&((*percorso)->tappe));
    free(*percorso);
    *percorso = NULL;
}


/**
 * Funzione che salva le sequenza di tappe percorribili ottimale
 * @param p percorso ottimale (old)
 * @param t possibile percorso ottimale (new)
 */
void scambiaPercorsi(Percorso* p, Tappa* t){
    if(t==NULL) return;
    Tappa* tmp = t;
    int i = 0;
    while (tmp!=NULL){
        i++;
        tmp = tmp->next;
    }
    if (i<p->n_tappe){
        p->tappe=t;
        p->n_tappe=i;
        return;
    }
}



//TODO: implementare correttamente guardando le foto del vecchio codice su ipad
void ricercaPercorsi(Stazione* stazioneCorrente, int distanzaFinale, int autonomia, Tappa* percorsoParziale, Percorso** percorsoOttimale) {
    if (stazioneCorrente->distanza + autonomia >= distanzaFinale) {
        int distanzaPercorsa = stazioneCorrente->distanza + autonomia;

        if (distanzaPercorsa < (*percorsoOttimale)->n_tappe) {
            // Scambia il percorso ottimale con il nuovo percorso parziale
            scambiaPercorsi(*percorsoOttimale, percorsoParziale);
        }

        // Non deallocare il percorso parziale qui, verrà deallocato dopo l'intera esplorazione
        return;
    }

    Stazione* stazioneSuccessiva = stazioneCorrente->next;
    while (stazioneSuccessiva != NULL && stazioneSuccessiva->distanza <= stazioneCorrente->distanza + autonomia) {
        int nuovaAutonomia = autonomia - (stazioneSuccessiva->distanza - stazioneCorrente->distanza);

        // Controlla se possiamo aggiungere l'auto con la maggiore autonomia disponibile
        Auto* autoCorrente = stazioneSuccessiva->head;
        if (autoCorrente != NULL) {
            nuovaAutonomia = max(nuovaAutonomia, autoCorrente->autonomia);
        }

        // Aggiungi la stazione al percorso parziale
        Tappa* nuovaTappa = malloc(sizeof(Tappa));
        nuovaTappa->distanza = stazioneSuccessiva->distanza;
        nuovaTappa->next = NULL;

        Tappa* percorsoParzialeCopia = percorsoParziale; // Salva una copia del percorso parziale attuale

        if (percorsoParziale == NULL)
            percorsoParziale = nuovaTappa;
        else {
            Tappa* tappaAttuale = percorsoParziale;
            while (tappaAttuale->next != NULL) {
                tappaAttuale = tappaAttuale->next;
            }
            tappaAttuale->next = nuovaTappa;
        }

        // Chiamata ricorsiva per esplorare le stazioni successive
        ricercaPercorsi(stazioneSuccessiva, distanzaFinale, nuovaAutonomia, percorsoParziale, percorsoOttimale);

        // Rimuovi l'ultima tappa per provare altre possibilità
        if (percorsoParziale != percorsoParzialeCopia) {
            deallocaTappa(&percorsoParziale); // Dealloca solo se è stato creato un nuovo percorso parziale
        }

        // Dealloca le tappe dopo aver completato la ricerca per questo possibile percorso
        deallocaTappa(&nuovaTappa);

        // Passa alla stazione successiva
        stazioneSuccessiva = stazioneSuccessiva->next;
    }
}

/**
 * Principale funzione del programma, si occupa di cercare il percorso con meno tappe tra due stazioni date,
 * si assume che queste siano sempre presenti
 * @param head testa della lista delle stazioni
 * @param d_start distanza stazione di partenza
 * @param d_end distanza stazione arrivo
 */
void pianificaPercorso(Stazione** head, int d_start, int d_end) {
    printf("INFO: entrato in pianifica percorso\n");
    // Trova le stazioni di partenza e arrivo
    Stazione* stazionePartenza = cercaStazione(head, d_start);
    Stazione* stazioneArrivo = cercaStazione(head, d_end);

    // Controlla che le stazioni di partenza e arrivo siano valide
    if (stazionePartenza == NULL || stazioneArrivo == NULL) {
        if(printf("nessun percorso\n")<0) return;
        return;
    }
    printf("INFO: post-check stazioni partenza e arrivo\n");


    //Crea il percorso ottimale e il percorso parziale
    Percorso* percorsoOttimale = malloc(sizeof(Percorso));
    percorsoOttimale->n_tappe = 10000;                              //Inizializza a un valore molto grande
    percorsoOttimale->tappe = NULL;
    printf("INFO: post creazione nuovo percorso\n");

    Tappa* percorsoParziale = NULL;

    printf("INFO: chiamo ricerca percorsi\n");
    // Chiama la funzione ricorsiva per trovare il percorso ottimale
    ricercaPercorsi(stazionePartenza, d_end, 0, percorsoParziale, &percorsoOttimale);
    printf("INFO: percorsi trovati\n");

    if (percorsoOttimale==NULL) {
        printf("INFO: non trovati\n");
        if(printf("nessun percorso\n")<0) return;
        return;
    }

    // Stampa il percorso ottimale
    stdoutPercorso(&percorsoOttimale);

    // Dealloca il percorso ottimale
    deallocaTappa(&percorsoOttimale->tappe);
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
