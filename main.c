/**
 * Progetto di algoritmi e strutture dati
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUTO 512
#define DELETED 1918

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
        if(printf("non aggiunta\n")>0)return;
        return;
    }

    Stazione* st = *testa;

    if(st != NULL){
        if(st->distanza == dist){
            if(st->num_auto<MAX_AUTO){
                st->num_auto++;
                aggiungiAutoByDesc(&st->head, autonomia);
                if(printf("aggiunta\n")<0)return;
                return;
            } else {
                if(printf("non aggiunta\n")<0)return;
                return;
            }
        }
        while((st->next != NULL && st->distanza<=dist) || (st->next==NULL && st->distanza==dist)){
            if(st->distanza==dist) {
                if(st->num_auto<MAX_AUTO){
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
 * @param testa testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param a autonomia dell'auto da eliminare.
 */
void rottamaAuto(Stazione** testa, unsigned int dist, unsigned int a){
    if(*testa == NULL) {
        if(printf("non rottamata\n")>0)return;
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
            st->num_auto--;
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
void aggiungiStazione(Stazione** testa, unsigned int distanza, unsigned int numeroAuto, unsigned int* autonomie) {
    Stazione* n_st = malloc(sizeof(Stazione));
    n_st->distanza = distanza;
    n_st->num_auto = numeroAuto;
    n_st->head = NULL;
    n_st->prev = NULL;
    n_st->next = NULL;

    if(numeroAuto>MAX_AUTO){
        free(n_st);
        if(printf("non aggiunta\n")<0)return;
        return;
    }

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
void demolisciStazione(Stazione** head, unsigned int dist){
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
    if(printf("%d\n", percorso->distanza)>0)NULL;
    deallocaTappa(&percorso);
}

/**
 * Sistema eventuali errori commessi dal ciclo di ricerca del percorso migliore
 * @param attuale percorso attualmente in esame
 */
void aggiustaPercorsoDesc(Percorso* attuale){
    if(attuale->n_tappe>=4){
        Tappa* tmp = attuale->tappe;

        while(tmp->next!=NULL)tmp = tmp->next;

        Tappa* ultima=tmp;
        Tappa* again=tmp;
        Tappa* hihi = tmp;

        while(ultima->prev->prev->prev!=NULL) {
            Stazione *st = ultima->prev->prev->ref->next;
            while (st->distanza < ultima->prev->prev->prev->distanza && st->distanza - st->head->autonomia >=
                                                                        ultima->prev->prev->distanza -
                                                                        ultima->prev->prev->ref->head->autonomia)
                st = st->next;
            if (st->distanza == ultima->prev->prev->prev->distanza) st = ultima->prev->prev->ref;
            ultima->prev->prev->distanza = st->distanza;
            ultima->prev->prev->ref = st;
            ultima=ultima->prev;
        }
        //sotto il vecchio codice
        while(tmp->prev->prev->distanza < attuale->tappe->distanza){
            Stazione* s=tmp->ref->next;

            while(s->distanza<tmp->prev->distanza){

                if(s->head!=NULL && s->distanza<s->head->autonomia+tmp->distanza){ //non entra
                    if(tmp->prev->prev->distanza-tmp->prev->prev->ref->head->autonomia <= s->distanza){
                        tmp->prev->distanza=s->distanza;
                        tmp->prev->ref=s;
                    }

                    if(tmp->prev->prev->distanza < attuale->tappe->distanza && tmp->prev->prev->distanza-tmp->prev->prev->ref->head->autonomia > s->distanza){
                        Stazione* p = NULL;
                        int op=tmp->prev->prev->prev->distanza-tmp->prev->prev->prev->ref->head->autonomia;

                        if(tmp->prev->prev->prev!=NULL && op<tmp->prev->prev->distanza) {
                            p=tmp->prev->prev->ref->prev;
                            while (p->distanza < tmp->prev->distanza && p->distanza>=op) {
                                if (p->head != NULL && p->distanza - p->head->autonomia <= s->distanza) {
                                    tmp->prev->prev->distanza = p->distanza;
                                    tmp->prev->prev->ref = p;
                                    tmp->prev->distanza = s->distanza;
                                    tmp->prev->ref = s;
                                    break;
                                }
                                p = p->prev;
                            }
                        }else{
                            p=tmp->prev->prev->ref->next;
                            while (p->distanza < tmp->prev->prev->prev->distanza) {
                                if (p->head != NULL && p->distanza - p->head->autonomia <= s->distanza) {
                                    tmp->prev->prev->distanza = p->distanza;
                                    tmp->prev->prev->ref = p;
                                    tmp->prev->distanza = s->distanza;
                                    tmp->prev->ref = s;
                                    break;
                                }
                                p = p->next;
                            }
                        }
                    }
                }
                s=s->next;
            }
            tmp=tmp->prev;
        }

        //uso tmp che corrisponde ad ultima tappa
        //devo assicurarmi che tra le tappe precedenti alle attuali siano tutte a "gittata" maggiore
        Tappa* lst_check=hihi;
        Tappa* mid_check=lst_check->prev;
        Tappa* fst_check= mid_check->prev;

        while(fst_check->prev!=NULL){
            Stazione* mario = lst_check->ref->next;
            Stazione* luigi = fst_check->ref;

            while(mario->distanza<mid_check->distanza && luigi->distanza<fst_check->prev->distanza){

                if(luigi->distanza-luigi->head->autonomia<=mario->distanza && mario->distanza-mario->head->autonomia <= lst_check->distanza){
                    mid_check->distanza=mario->distanza;
                    mid_check->ref=mario;

                }else if(fst_check->prev!=NULL){
                    luigi=luigi->next;
                    while(luigi->distanza<fst_check->prev->distanza){
                        if(fst_check->prev->distanza-fst_check->prev->ref->head->autonomia<=luigi->distanza) {
                            if (luigi->distanza - luigi->head->autonomia <= mario->distanza && mario->distanza-mario->head->autonomia <= lst_check->distanza) {
                                fst_check->distanza = luigi->distanza;
                                fst_check->ref = luigi;
                                mid_check->distanza = mario->distanza;
                                mid_check->ref = mario;
                            }
                        }
                        luigi=luigi->next;
                    }
                }

                mario=mario->next;
                luigi = fst_check->ref; //check per cambiare prima stazione
            }
            lst_check=lst_check->prev;
            mid_check=lst_check->prev;
            fst_check= mid_check->prev;

        }

        tmp = attuale->tappe;
        while(tmp->next->next!=NULL){
            for(Stazione* t = tmp->next->ref; t->distanza>tmp->distanza-tmp->ref->head->autonomia; t=t->prev){
                if(t->head==NULL) continue;
                if(t->distanza-tmp->next->next->distanza<=t->head->autonomia){
                    tmp->next->distanza=t->distanza;
                    tmp->next->ref=t;
                }
                if(t->prev==NULL)break;
            }
            tmp = tmp->next;
        }

        if(again->prev->prev->distanza-again->distanza<=again->prev->prev->ref->head->autonomia){
            Tappa* judge_jury_executor = again->prev;
            again->prev->prev->next=again;
            again->prev=again->prev->prev;
            free(judge_jury_executor);
        }

        attuale->coeff_dist=0;
        Tappa* iterator=attuale->tappe;
        while(iterator!=NULL){
            attuale->coeff_dist+=iterator->distanza;
            iterator=iterator->next;
        }
    }
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

    Stazione* actual = start;
    unsigned int autonomia = start->head->autonomia;

    while (start->distanza-autonomia <= actual->distanza && actual->prev!=NULL){
        Percorso temp;
        temp.n_tappe=0;
        temp.coeff_dist=0;
        temp.tappe=NULL;

        if (actual != NULL && actual->prev != NULL && actual->head!=NULL && actual->head->autonomia>0 && (actual->head->autonomia>=actual->distanza - actual->prev->distanza || actual->distanza==end->distanza)) {
            Stazione* new_st = actual->prev;
            Stazione* old_st = actual;


            if(temp.tappe != NULL) {
                temp.n_tappe=0;
                temp.coeff_dist=0;
                deallocaTappa(&temp.tappe);
            }
            aggiungiTappa(&temp, start);
            if(old_st->distanza!=start->distanza)aggiungiTappa(&temp, old_st);

            int tmp_auto = old_st->head->autonomia;

            while (new_st != NULL && new_st->distanza >= end->distanza && tmp_auto >= new_st->next->distanza - new_st->distanza) {

                if (temp.n_tappe > migliore.n_tappe) break; //condizioni di uscita

                if(new_st->next!=NULL) tmp_auto -= new_st->next->distanza - new_st->distanza;

                if((new_st->head==NULL || new_st->head->autonomia==0) && new_st->distanza!=end->distanza){
                    while((new_st->head==NULL || new_st->head->autonomia==0) && tmp_auto > 0) {
                        new_st=new_st->prev;
                        tmp_auto -= new_st->next->distanza - new_st->distanza;
                    }
                    if(tmp_auto==0&&new_st->head==NULL)break;
                }

                if (tmp_auto >= 0 && (new_st == end || (new_st->head!=NULL && new_st->distanza - new_st->head->autonomia <= end->distanza))) {
                    if(new_st->distanza==end->distanza)aggiungiTappa(&temp, new_st);
                    else {
                        Stazione *t = end->next;
                        while(t->distanza <= new_st->distanza){
                            if(t->head != NULL && old_st->distanza - old_st->head->autonomia <= t->distanza && t->distanza - t->head->autonomia <= end->distanza){
                                aggiungiTappa(&temp, t);
                                break;
                            }
                            t=t->next;
                        }
                        aggiungiTappa(&temp, end);
                    }
                    aggiustaPercorsoDesc(&temp);

                    if (migliore.n_tappe > temp.n_tappe || (migliore.n_tappe==temp.n_tappe && migliore.coeff_dist > temp.coeff_dist)) {
                        deallocaTappa(&migliore.tappe);
                        migliore = temp;
                        break;
                    }
                    break;
                }

                if (tmp_auto <= 0 || new_st->distanza - new_st->prev->distanza > tmp_auto) {
                    if (new_st->prev != NULL && old_st->head->autonomia <= new_st->prev->distanza - old_st->distanza) {
                        aggiungiTappa(&temp, new_st);
                        tmp_auto = new_st->head->autonomia;
                        old_st = new_st;

                    } else {
                        new_st=old_st;
                        old_st = old_st->next;
                        unsigned int tail = rimuoviUltimaTappa(&temp);

                        while(old_st->head->autonomia<old_st->distanza-old_st->prev->distanza){
                            old_st=old_st->next;
                            new_st=new_st->next;
                            if(old_st->distanza==tail)tail = rimuoviUltimaTappa(&temp);
                        }

                        if(old_st!=NULL && actual!=NULL && old_st->distanza<actual->distanza)break;
                        aggiungiTappa(&temp, old_st);
                        tmp_auto=old_st->next->head->autonomia;
                    }
                }
                new_st = new_st->prev;
            }
        }
        actual=actual->prev;
    }
    if(migliore.n_tappe<3||migliore.n_tappe==10000)migliore.n_tappe=DELETED;
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
    int autonomia = start->head->autonomia;

    while (start->distanza + autonomia >= actual->distanza&&actual->next!=NULL) {
        Percorso temp;
        temp.n_tappe = 0;
        temp.coeff_dist = 0;
        temp.tappe = NULL;

        if (actual != NULL && actual->next != NULL && actual->head->autonomia > 0 && (actual->head->autonomia >= actual->next->distanza - actual->distanza ||
                                            actual->distanza == end->distanza)) {
            Stazione *new_st = actual->next;
            Stazione *old_st = actual;
            if(temp.tappe != NULL) {
                temp.n_tappe=0;
                temp.coeff_dist=0;
                deallocaTappa(&temp.tappe);
            }
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
        if(printf("nessun percorso\n")<0) return;
        return;
    }
    int var = d_end-d_start;
    if(var == 0){
        if(printf("%d\n", d_start)<0) return;
        return;
    }
    Percorso ottimale;
    if(var>0)ottimale = ricercaPercorsoInAvanti(stazionePartenza, stazioneArrivo);
    else ottimale = ricercaPercorsoIndietro(stazionePartenza, stazioneArrivo);
    if(ottimale.n_tappe==DELETED)if(printf("nessun percorso\n")>0)return;
    stampaPercorso(ottimale.tappe);
}


int main() {
    Stazione *head = NULL;
    char c;
    char input[20];
    int indice = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n' || c == ' ') {
            input[indice] = '\0';
            indice = 0;

            if (strcmp(input, "aggiungi-stazione") == 0) {
                unsigned int dist, n_a;
                if (scanf("%d %d", &dist, &n_a) != EOF) {
                    unsigned int a[512];
                    for (int i = 0; i < n_a; i++) {
                        if (scanf(" %d", &a[i]) != EOF) continue;
                    }
                    aggiungiStazione(&head, dist, n_a, a);

                    for(int x =0; x<20; x++)input[x]=0;

                }
            } else if (strcmp(input, "aggiungi-auto") == 0) {
                unsigned int dist, a;
                if (scanf("%d %d", &dist, &a) != EOF) {
                    aggiungiAuto(&head, dist, a);
                }
                for(int x =0; x<20; x++)input[x]=0;

            } else if (strcmp(input, "rottama-auto") == 0) {
                unsigned int dist, a;
                if (scanf("%d %d", &dist, &a) != EOF) {
                    rottamaAuto(&head, dist, a);
                }
                for(int x =0; x<20; x++)input[x]=0;

            } else if (strcmp(input, "demolisci-stazione") == 0) {
                unsigned int dist;
                if (scanf("%d", &dist) != EOF) {
                    demolisciStazione(&head, dist);
                }
                for(int x =0; x<20; x++)input[x]=0;

            } else if (strcmp(input, "pianifica-percorso") == 0) {
                unsigned int start, end;
                if (scanf("%d %d", &start, &end) != EOF) {
                    pianificaPercorso(&head, start, end);
                }
                for(int x =0; x<20; x++)input[x]=0;
            }
        } else {
            input[indice++] = c;
        }
    }
    deallocaStazioni(&head);
    return 0;
}
