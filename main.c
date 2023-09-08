/**
 * Progetto di algoritmi e strutture dati
 * Vianello Gabriele
 * Cod.Persona 10704549
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUTO 512
#define DELETED 1918
#define BLOCK 8098813

int not_interesting=0;

/**
 * Auto rappresentate dalla seguente struttura dati
 */
typedef struct Auto{
    unsigned int autonomia;
    struct Auto* next;
}Auto;


/**
 * l'autostrada è rappresentata da una lista doppiamente concatenata di stazioni
 */
typedef struct Stazione{
    unsigned int distanza;
    unsigned int num_auto;
    Auto *head;             //puntatore alla prima cella del vettore delle autonomie (una per auto)
    struct Stazione* next;
    struct Stazione* prev;
}Stazione;

/**
 * Struttura dati usata per memorizzare le tappe del percorso
 */
typedef struct Tappa{
    unsigned int distanza;
    Stazione* ref;
    struct Tappa* next;
    struct Tappa* prev;
}Tappa;

/**
 * Stuttura dati utilizzata per tenere traccia del percorso temporaneo e ottimale
 */
typedef struct Percorso{
    unsigned int n_tappe;
    unsigned int coeff_dist;
    Tappa* tappe;
}Percorso;


/**
 * Aggiunge una tappa al percorso temporaneo
 * @param percorso puntatore al percorso temporaneo
 * @param distanza della nuova tappa
 * @param st puntatore alla stazione alla quale è associata la tappa
 */
void aggiungiTappa(Percorso* percorso, Stazione* st) {
    Tappa* nuovaTappa = (Tappa*)malloc(sizeof(Tappa));
    if (nuovaTappa != NULL) {
        nuovaTappa->distanza = st->distanza;
        nuovaTappa->ref = st;
        nuovaTappa->next = NULL;
        nuovaTappa->prev = NULL;
    }
    if (percorso->tappe == NULL) percorso->tappe = nuovaTappa;
    else {
        Tappa* corrente = percorso->tappe;
        while (corrente->next != NULL) corrente = corrente->next;
        corrente->next = nuovaTappa;
        nuovaTappa->prev=corrente;
    }
    percorso->n_tappe++;
    percorso->coeff_dist+=nuovaTappa->distanza;
}

/**
 * Aggiunge una tappa al percorso temporaneo in testa alla lista delle tappe
 * @param percorso puntatore al percorso temporaneo
 * @param distanza della nuova tappa
 * @param st puntatore alla stazione alla quale è associata la tappa
 */
void aggiungiTappaInTesta(Percorso* percorso, Stazione* st) {
    Tappa* nuovaTappa = (Tappa*)malloc(sizeof(Tappa));
    nuovaTappa->distanza = st->distanza;
    nuovaTappa->ref = st;
    nuovaTappa->prev = NULL;
    nuovaTappa->next = NULL;

    if(percorso->tappe==NULL) {
        percorso->tappe = nuovaTappa;
        percorso->n_tappe++;
        percorso->coeff_dist += nuovaTappa->distanza;
    }else{
        Tappa* tmp = percorso->tappe;
        percorso->tappe=nuovaTappa;
        percorso->tappe->next=tmp;
        tmp->prev=nuovaTappa;
        percorso->n_tappe++;
        percorso->coeff_dist += nuovaTappa->distanza;
    }
}

/**
 * Elimina l'ultima tappa inserita (quella in testa) dal percorso temporaneo
 * @param percorso puntatore al percorso temporaneo
 */
void eliminaUltimaTappa(Percorso* percorso) {
    if (percorso->tappe == NULL) return;

    Tappa* judge_jury_executor = percorso->tappe; //tappa da eliminare

    if (judge_jury_executor->next != NULL) {
        percorso->tappe = judge_jury_executor->next;
        percorso->tappe->prev = NULL;

    } else percorso->tappe = NULL; //altrimenti la lista delle tappe diventa vuota

    percorso->n_tappe--;
    percorso->coeff_dist -= judge_jury_executor->distanza;

    free(judge_jury_executor); //effettiva eliminazione della tappa
}

/**
 * Funzione di inserimento che mantiene la lista ordinata per autonomia decrescente
 * @param st stazione alla quale aggiungere l'auto
 * @param a autonomia (int)
 */
void aggiungiAutoByDesc(Auto** testa, unsigned int a) {
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
 * Funzione che dealloca la lista di auto data una stazione
 * @param head testa della lista di auto
 */
void deallocaAuto(Auto** head) {
    if(*head==NULL)return;
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
    if(*head==NULL)return;
    Stazione* attuale = *head;
    Stazione* tmp;

    while (attuale != NULL) {
        tmp = attuale->next;
        deallocaAuto(&attuale->head);
        attuale->head = NULL;
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
    if(*head==NULL)return;
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
 * Funzione che rimuove l'ultima tappa del percorso provvisorio per cercare
 * nuove strade da percorrere.
 * @param percorso struct del percorso temporaneo
 */
unsigned int rimuoviUltimaTappa(Percorso* percorso) {
    percorso->n_tappe--;
    Tappa* tmp = percorso->tappe;
    while (tmp->next->next != NULL) tmp = tmp->next;
    unsigned int dist=tmp->distanza;
    tmp =tmp->next;
    percorso->coeff_dist-=tmp->distanza;
    free(tmp);
    return dist;
}


/**
 * Funzione che aggiunge un auto scorrendo le stazioni fino a cercare (se esiste) quella desiderata,
 * poi la passa a metodo che si preoccupa di inserire l'auto mantenendo l'ordinamento decrescente
 * @param testa testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param autonomia dell'auto da inserire
 */
void aggiungiAuto(Stazione** testa, unsigned int dist, unsigned int autonomia){
    if(*testa == NULL) {
        not_interesting=printf("non aggiunta\n");
        return;
    }

    Stazione* st = *testa;

    if(st != NULL){
        if(st->distanza == dist){
            if(st->num_auto<MAX_AUTO){
                st->num_auto++;
                aggiungiAutoByDesc(&st->head, autonomia);
                not_interesting=printf("aggiunta\n");
                return;
            } else {
                not_interesting=printf("non aggiunta\n");
                return;
            }
        }
        while((st->next != NULL && st->distanza<=dist) || (st->next==NULL && st->distanza==dist)){
            if(st->distanza==dist) {
                if(st->num_auto<MAX_AUTO){
                    st->num_auto++;
                    aggiungiAutoByDesc(&st->head, autonomia);
                    not_interesting=printf("aggiunta\n");
                    return;
                } else {
                    not_interesting=printf("non aggiunta\n");
                    return;
                }
            }
            else st = st->next;
        }
    } else if(st==NULL){
        not_interesting=printf("non aggiunta\n");
        return;
    }
    not_interesting=printf("non aggiunta\n");
}


/**
 * Funzione che elimina un auto dalla lista di una precisa stazione
 * @param testa testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param a autonomia dell'auto da eliminare.
 */
void rottamaAuto(Stazione** testa, unsigned int dist, unsigned int a){
    if(*testa == NULL) {
        not_interesting=printf("non rottamata\n");
        return;
    }

    Stazione* st= *testa;

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
                    st->num_auto--;
                    not_interesting=printf("rottamata\n");
                    return;
                } else {
                    not_interesting=printf("non rottamata\n");
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
            st->num_auto--;
            not_interesting=printf("rottamata\n");
            return;
        } else {
            not_interesting=printf("non rottamata\n");
            return;
        }
    }
    not_interesting=printf("non rottamata\n");
}


/**
 * funzione che aggiunge una stazione all'autostrada
 * @param head testa della lista passata da main
 * @param dist dall'origine
 * @param n_a  numero di automobili presenti
 * @param v_a  puntatore al vettore delle automobili
 */
void aggiungiStazione(Stazione** testa, unsigned int distanza, unsigned int numeroAuto, unsigned int* autonomie) {

    if(numeroAuto>MAX_AUTO){
        not_interesting=printf("non aggiunta\n");
        return;
    }

    Stazione* n_st = malloc(sizeof(Stazione));
    n_st->distanza = distanza;
    n_st->num_auto = numeroAuto;
    n_st->head = NULL;
    n_st->prev = NULL;
    n_st->next = NULL;

    //Aggiunta auto
    for (int i = 0; i < numeroAuto; i++) {
        Auto* nuovaAuto = malloc(sizeof(Auto));
        nuovaAuto->autonomia = autonomie[i];
        nuovaAuto->next = NULL;
        aggiungiAutoByDesc(&n_st->head, autonomie[i]);
    }

    //inserimento stazione in ordine crescente
    if (*testa == NULL) {
        *testa = n_st;
        not_interesting=printf("aggiunta\n");
        return;

    } else if (distanza < (*testa)->distanza) {
        n_st->next = *testa;
        (*testa)->prev = n_st;
        *testa = n_st;
        not_interesting=printf("aggiunta\n");
        return;
    } else {
        Stazione* current = *testa;
        while (current != NULL) {
            if (current->distanza == distanza) {
                not_interesting=printf("non aggiunta\n");
                return;
            }
            if (current->next != NULL && current->next->distanza>distanza){ //inserimento tra due stazioni
                current->next->prev = n_st;
                n_st->next=current->next;
                n_st->prev=current;
                current->next=n_st;
                not_interesting=printf("aggiunta\n");
                return;
            }
            if(current->next==NULL && current->distanza<distanza){
                n_st->prev=current;
                current->next=n_st;
                n_st->next=NULL;
                not_interesting=printf("aggiunta\n");
                return;
            }
            current = current->next;
        }
        if(current->distanza == distanza){
            deallocaAuto(&n_st->head);
            free(n_st);
            not_interesting=printf("non aggiunta\n");
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
void demolisciStazione(Stazione** head, unsigned int dist){
    if (head==NULL) {
        not_interesting=printf("non demolita\n");
        return;
    }
    Stazione* st = *head;

    while (st != NULL && st->distanza != dist) st = st->next;

    if (st == NULL) {
        not_interesting=printf("non demolita\n");
        return;
    }
    if (st == *head) *head = st->next;
    if (st->prev != NULL) st->prev->next = st->next;
    if (st->next != NULL) st->next->prev = st->prev;

    deallocaAuto(&st->head);
    free(st);
    not_interesting=printf("demolita\n");
}


/**
 * Funzione che data una distanza individua una stazione
 * @param head testa della lista delle stazioni
 * @param dist che identifica univocamente la stazione
 * @return stazione cercata
 */
Stazione* cercaStazione(Stazione** head, unsigned int dist) {
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
 * Stampa in stdout il percorso ottimale, cioè quello con minor numero
 * di tappe e con quest'ultime più vicine all'inizio dell'autostrada.
 * @param percorso lista mono-direzionale di tappe
 */
void stampaPercorso(Tappa* percorso) {
    while (percorso->next != NULL) {
        printf("%d ", percorso->distanza);
        percorso = percorso->next;
    }
    not_interesting=printf("%d\n", percorso->distanza);
    deallocaTappa(&percorso);
}


/**
 * Funzione di ricerca iterativa che prende il percorso minimo quando
 * la direzione è verso l'inizio dell'autostrada.
 * @param start stazione di partenza
 * @param end stazione di arrivo
 * @return percorso ottimale
 */
Percorso ricercaPercorsoIndietro(Stazione* start, Stazione* end){
    Percorso migliore;
    migliore.n_tappe = 10000;
    migliore.coeff_dist = 10000;
    migliore.tappe = NULL;

    Stazione* observed = NULL; //prima tappa attualmente in esame
    // NOTA: la sua scelta determina un possibile insieme di percorsi

    Stazione* temp = end->next;
    while(temp !=NULL && temp->distanza < start->distanza) {
        if(temp->head!=NULL && temp->distanza-end->distanza<=temp->head->autonomia)observed=temp;
        temp=temp->next;
    }

    if(observed==NULL){ //se non è possibile arrivare alla fine, allora l'esecuzione termina
        migliore.n_tappe=DELETED;          //NOTA: assicurarsi che questo controllo non crei problemi
        return migliore;                   //delega stampa "nessun percorso" a funzione predisposta
    }

    Tappa* pt2=NULL;

    while (observed->distanza > end->distanza){

        //nuovo percorso temporaneo, ogni nuova tappa di partenza corrisponde ad almeno un nuovo possibile percorso
        Percorso attuale;
        attuale.n_tappe=0;
        attuale.coeff_dist=0;
        attuale.tappe=NULL;

        aggiungiTappaInTesta(&attuale, end);
        Tappa* pt1=attuale.tappe;
        aggiungiTappaInTesta(&attuale, observed);

        if(attuale.tappe==NULL)return migliore;

        Stazione* nuova = observed; //la incremento fino ad arrivare alla fine;
        if(nuova->next->head==NULL) while(nuova->next->head==NULL)nuova=nuova->next;  //inizializzazione di next
        else nuova=nuova->next;

        Stazione* tmp= NULL; //possibile nuova tappa
        Stazione* vecchia = observed; //la aggiorno SSE ne trovo una che riesce ad arrivare alla precedente tappa | altrimenti torno indietro | se == a tmp ricomincio

        int exitstat=0;

        while (nuova != NULL && nuova->distanza <= start->distanza) {

            if (nuova->distanza - vecchia->distanza <= nuova->head->autonomia) tmp = nuova; //nuova stazione intermedia

            if ((tmp != NULL && nuova->distanza == start->distanza &&
                nuova->distanza - tmp->distanza <= nuova->head->autonomia)
                || (tmp == NULL && nuova->distanza==start->distanza && start->distanza-vecchia->distanza<=nuova->head->autonomia)) {

                if (tmp!=NULL && tmp->distanza != nuova->distanza) {
                    aggiungiTappaInTesta(&attuale, tmp);
                    aggiungiTappaInTesta(&attuale, nuova);
                } else aggiungiTappaInTesta(&attuale, start);

                if (attuale.n_tappe <= migliore.n_tappe) { //sequenza finale scelta percorso
                    exitstat=BLOCK;

                    if (attuale.n_tappe < migliore.n_tappe) {
                        deallocaTappa(&migliore.tappe);
                        migliore.tappe = attuale.tappe;
                        migliore.n_tappe = attuale.n_tappe;
                        migliore.coeff_dist = attuale.coeff_dist;
                        pt2 = pt1;
                    } else {
                        while (pt2->prev != NULL && pt2->distanza == pt1->distanza) {
                            pt2 = pt2->prev;
                            pt1 = pt1->prev;
                        }
                        if (pt1->distanza < pt2->distanza) {
                            deallocaTappa(&migliore.tappe);
                            migliore.tappe = attuale.tappe;
                            migliore.n_tappe = attuale.n_tappe;
                            migliore.coeff_dist = attuale.coeff_dist;
                            pt2 = pt1;
                        }
                    }
                    break;
                }
            }else if (nuova->next == NULL && nuova->distanza!=start->distanza)break;

            if (attuale.n_tappe >= migliore.n_tappe)break; //condizioni di uscita

            if (tmp != NULL && nuova->distanza==start->distanza && tmp->distanza != vecchia->distanza
            && start->head->autonomia<start->distanza-vecchia->distanza) {
                aggiungiTappaInTesta(&attuale, tmp);
                vecchia = tmp;
                nuova = tmp;
            } else if(nuova->distanza==start->distanza && tmp == NULL){   //parte di codice deputata alla ricerca di nuove vie una volta che l'attuale non è più percorribile
                eliminaUltimaTappa(&attuale);   //elimina l'ultima tappa inserita

                if (attuale.tappe == NULL)break;
                Tappa* x = attuale.tappe;   //nuova tappa di riferimento
                tmp = vecchia;

                while (tmp->head == NULL || tmp->head->autonomia < tmp->distanza - x->distanza) {
                    if (tmp->distanza > x->distanza) {
                        aggiungiTappaInTesta(&attuale, tmp);
                        vecchia = tmp;
                        nuova = tmp->next;
                    } else if (x->distanza > observed->distanza) {
                        x = x->prev;
                        eliminaUltimaTappa(&attuale);
                        vecchia = x->ref;
                    } else break;

                    if(x->distanza==observed->distanza)exitstat=BLOCK;

                    tmp = tmp->prev;
                }
                if (exitstat == BLOCK)break;
            }

            if(nuova->next!=NULL && exitstat != BLOCK){
                if (nuova->next->head == NULL && nuova->next->distanza != start->distanza) {
                    nuova = nuova->next;
                    while (nuova->head == NULL)nuova = nuova->next;
                } else nuova = nuova->next;
            }else break;
        }

        while (observed->prev != NULL && observed->prev->head == NULL && observed->prev->distanza < end->distanza) {
            observed = observed->prev;
        }
        if (observed->prev != NULL && observed->prev->distanza)observed = observed->prev;
        else break;
    }

    if(migliore.n_tappe<3 || migliore.n_tappe==10000)migliore.n_tappe=DELETED;
    else{    //parte di codice che "aggiusta" laddove richiesto il percorso ottimale
        Tappa* tmp = migliore.tappe;

        while(tmp->next->next!=NULL){
            if(tmp->distanza-tmp->next->distanza<tmp->ref->head->autonomia){
                Stazione* lory = tmp->ref; //scorre le possibili nuove tappe
                if(lory->prev->head!=NULL)lory=lory->prev;
                while(lory->distanza>tmp->next->next->distanza&&tmp->distanza-lory->distanza<=tmp->ref->head->autonomia){
                    if(tmp->next->next->distanza !=end->distanza &&lory->head->autonomia>= lory->distanza-tmp->next->next->distanza && lory->distanza-lory->head->autonomia <= tmp->next->distanza-tmp->next->ref->head->autonomia){
                        
                        tmp->next->distanza=lory->distanza;
                        tmp->next->ref=lory;
                    }
                    if(lory->prev->head!=NULL)lory=lory->prev;
                    else while(lory->prev->head==NULL)lory=lory->prev;
                }
            }
            tmp=tmp->next;
        }

        while(tmp->prev->prev!=NULL){
            if(tmp->prev->prev->distanza-tmp->prev->distanza<tmp->prev->prev->ref->head->autonomia){
                Stazione* lory = tmp->prev->ref; //scorre le possibili nuove tappe

                while(lory->distanza>tmp->distanza && tmp->prev->prev->distanza-lory->distanza<=tmp->prev->prev->ref->head->autonomia){
                    if(lory->head->autonomia >= lory->distanza-tmp->distanza){

                        tmp->prev->distanza=lory->distanza;
                        tmp->prev->ref=lory;
                    }
                    if(lory->prev->head!=NULL)lory=lory->prev;
                    else while(lory->prev->head==NULL)lory=lory->prev;
                }
            }
            tmp=tmp->prev;
        }
    }
    return migliore;
}

/**
 * Funzione di ricerca iterativa che prende il percorso minimo quando
 * la direzione è verso la fine dell'autostrada.
 * @param start stazione di partenza
 * @param end stazione di arrivo
 * @return percorso ottimale
 */
Percorso ricercaPercorsoInAvanti(Stazione* start, Stazione* end) {
    Percorso migliore;
    migliore.n_tappe = 10000;
    migliore.coeff_dist = 10000;
    migliore.tappe = NULL;

    Stazione *actual = start;

    while (start->distanza + start->head->autonomia >= actual->distanza&&actual->next!=NULL) {
        Percorso temp;
        temp.n_tappe = 0;
        temp.coeff_dist = 0;
        temp.tappe = NULL;

        if (actual != NULL && actual->next != NULL && actual->head->autonomia > 0 && (actual->head->autonomia >= actual->next->distanza - actual->distanza ||
                                            actual->distanza == end->distanza)) {
            Stazione *new_st = actual->next;
            Stazione *old_st = actual;

            aggiungiTappa(&temp, start);
            aggiungiTappa(&temp, old_st);

            int tmp_auto = old_st->head->autonomia;

            while (new_st != NULL && new_st->distanza <= end->distanza && tmp_auto >= new_st->distanza - new_st->prev->distanza) {
                if (temp.n_tappe > migliore.n_tappe || (migliore.n_tappe==temp.n_tappe&&migliore.coeff_dist < temp.coeff_dist)) break; //condizioni di uscita

                if(new_st->prev!=NULL)tmp_auto -= new_st->distanza - new_st->prev->distanza;

                if((new_st->head==NULL || new_st->head->autonomia==0) && new_st->distanza!=end->distanza){
                    while((new_st->head==NULL || new_st->head->autonomia==0) && new_st->next!=NULL && tmp_auto > 0) {
                        new_st=new_st->next;
                        tmp_auto -= new_st->distanza - new_st->prev->distanza;
                    }
                    if(tmp_auto==0&&new_st->head==NULL)break;
                }

                if (tmp_auto >= 0 && (new_st == end || new_st->head->autonomia + new_st->distanza >= end->distanza)) {
                    aggiungiTappa(&temp, new_st);

                    if (new_st->distanza != end->distanza)
                        aggiungiTappa(&temp, end);

                    if (migliore.n_tappe > temp.n_tappe ||
                        (migliore.n_tappe == temp.n_tappe && migliore.coeff_dist > temp.coeff_dist)) {
                        deallocaTappa(&migliore.tappe);
                        migliore = temp;
                        break;
                    }
                }

                if (tmp_auto <= 0 || new_st->next->distanza - new_st->distanza > tmp_auto) {
                    if (new_st->next != NULL && old_st->head->autonomia <= new_st->next->distanza - old_st->distanza) {
                        aggiungiTappa(&temp, new_st);
                        tmp_auto = new_st->head->autonomia;
                        old_st = new_st;

                    } else {
                        new_st = old_st;
                        old_st = old_st->prev;
                        unsigned int tail = rimuoviUltimaTappa(&temp);

                        while (old_st->head->autonomia < old_st->next->distanza - old_st->distanza) {
                            old_st = old_st->prev;
                            new_st = new_st->prev;
                            if (old_st->distanza == tail)tail = rimuoviUltimaTappa(&temp);
                        }
                        aggiungiTappa(&temp, old_st);
                        tmp_auto = old_st->prev->head->autonomia;

                        if (old_st != NULL && actual != NULL && old_st->distanza < actual->distanza)break;
                    }
                }
                new_st = new_st->next;
            }
       }
        actual = actual->next;
    }
    if(migliore.n_tappe<3||migliore.n_tappe==10000)migliore.n_tappe=DELETED;
    else{
        Tappa* tmp = migliore.tappe;
        Tappa* k = migliore.tappe;
        if(k->distanza==tmp->next->distanza){
            tmp = tmp->next;
            migliore.tappe=migliore.tappe->next;
            migliore.tappe->prev=NULL;
            free(k);
        }
        while(tmp->next!=NULL)tmp =tmp->next;
        while(tmp->distanza != migliore.tappe->distanza){
            Stazione* s = NULL;
            if(tmp->prev==NULL)break;
            else s = tmp->prev->ref;
            while(s!=NULL && tmp->prev->prev!=NULL && s->distanza>tmp->prev->prev->distanza){
                if(s->head!=NULL && s->distanza<tmp->prev->distanza && s->distanza+s->head->autonomia >= tmp->distanza){
                    tmp->prev->distanza=s->distanza;
                    tmp->prev->ref=s;
                }
                s=s->prev;
            }
            tmp = tmp->prev;
        }

        Tappa* z = migliore.tappe;
        if(z->next->next->distanza - z->distanza <= z->ref->head->autonomia){
            z=z->next;
            z->next->prev=migliore.tappe;
            migliore.tappe->next=z->next;
            free(z);
        }
    }
    return migliore;
}


/**
 * Principale funzione del programma, si occupa di cercare il percorso con meno tappe tra due stazioni date,
 * si assume che queste siano sempre presenti
 * @param head testa della lista delle stazioni
 * @param d_start distanza stazione di partenza
 * @param d_end distanza stazione arrivo
 */
void pianificaPercorso(Stazione** head, unsigned int d_start, unsigned int d_end) {
    //trova le stazioni di partenza e arrivo
    Stazione* stazionePartenza = cercaStazione(head, d_start);
    Stazione* stazioneArrivo = cercaStazione(head, d_end);

    //controlla che le stazioni di partenza e arrivo siano valide
    if (stazionePartenza == NULL || stazioneArrivo == NULL || stazionePartenza->num_auto==0
        ||stazionePartenza->head==NULL || stazionePartenza->head->autonomia==0) {
        not_interesting=printf("nessun percorso\n");
        return;
    }
    int var = d_end-d_start;
    if(var == 0){
        not_interesting=printf("%d\n", d_start);
        return;
    }
    Percorso ottimale;
    if(var>0)ottimale = ricercaPercorsoInAvanti(stazionePartenza, stazioneArrivo);
    else ottimale = ricercaPercorsoIndietro(stazionePartenza, stazioneArrivo);
    if(ottimale.n_tappe==DELETED){
        not_interesting=printf("nessun percorso\n");
        return;
    }
    stampaPercorso(ottimale.tappe);
}


int main() {
    Stazione *head = NULL;
    char c;
    char input[20];
    int indice = 0;
    while ((c = getchar_unlocked()) != EOF) {
        if (c == '\n' || c == ' ') {
            input[indice] = '\0';
            indice = 0;

            if (strcmp(input, "aggiungi-stazione") == 0) {
                unsigned int dist, n_a;
                not_interesting=scanf("%d %d", &dist, &n_a);
                unsigned int a[512];
                for (int i = 0; i < n_a; i++) {
                    not_interesting=scanf(" %d", &a[i]);
                }
                aggiungiStazione(&head, dist, n_a, a);

            } else if (strcmp(input, "aggiungi-auto") == 0) {
                unsigned int dist, a;
                not_interesting=scanf("%d %d", &dist, &a);
                aggiungiAuto(&head, dist, a);

            } else if (strcmp(input, "rottama-auto") == 0) {
                unsigned int dist, a;
                not_interesting=scanf("%d %d", &dist, &a);
                rottamaAuto(&head, dist, a);

            } else if (strcmp(input, "demolisci-stazione") == 0) {
                unsigned int dist;
                not_interesting=scanf("%d", &dist);
                demolisciStazione(&head, dist);

            } else if (strcmp(input, "pianifica-percorso") == 0) {
                unsigned int start, end;
                not_interesting=scanf("%d %d", &start, &end);
                pianificaPercorso(&head, start, end);

            }
        } else {
            input[indice++] = c;
        }
    }
    deallocaStazioni(&head);
    return 0;
}
