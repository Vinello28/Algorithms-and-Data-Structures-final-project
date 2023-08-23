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
    struct Tappa* next;
}Tappa;

typedef struct Percorso{
    unsigned int n_tappe;
    unsigned int coeff_dist;
    Tappa* tappe;
}Percorso;


Tappa* aggiungiTappa(Percorso* percorso, unsigned int distanza) {
    Tappa* nuovaTappa = (Tappa*)malloc(sizeof(Tappa));
    if (nuovaTappa != NULL) {
        nuovaTappa->distanza = distanza;
        nuovaTappa->next = NULL;
    }
    if (percorso->tappe == NULL) percorso->tappe = nuovaTappa;
    else {
        Tappa* corrente = percorso->tappe;
        while (corrente->next != NULL) corrente = corrente->next;
        corrente->next = nuovaTappa;
    }
    percorso->n_tappe++;
    percorso->coeff_dist+=nuovaTappa->distanza;
    return nuovaTappa;
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
void rimuoviUltimaTappa(Percorso* percorso) {
    percorso->n_tappe--;
    Tappa* tmp = percorso->tappe;
    while (percorso->tappe->next!= NULL) tmp = tmp->next;
    percorso->coeff_dist-=tmp->distanza;
    free(tmp);
}


/**
 * Funzione che aggiunge un auto scorrendo le stazioni fino a cercare (se esiste) quella desiderata,
 * poi la passa a metodo che si preoccupa di inserire l'auto mantenendo l'ordinamento decrescente
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param autonomia dell'auto da inserire
 */
void aggiungiAuto(Stazione** head, unsigned int dist, unsigned int autonomia){
    Stazione* st = *head;

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
 * @param head testa della lista passata da main
 * @param dist distanza della stazione cercata
 * @param a autonomia dell'auto da eliminare.
 */
void rottamaAuto(Stazione** head, unsigned int dist, unsigned int a){
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

    //Aggiunta delle autonomie delle auto alla lista delle auto
    for (int i = 0; i < numeroAuto; i++) {
        Auto* nuovaAuto = malloc(sizeof(Auto));
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
    int autonomia = start->head->autonomia;

    int tmp_auto = autonomia;

    while (start->distanza-autonomia <= actual->distanza){
        Percorso temp;
        temp.n_tappe=0;
        temp.coeff_dist=0;
        temp.tappe=NULL;

        if (actual->head->autonomia>0 && (actual->head->autonomia>=actual->distanza - actual->prev->distanza || actual->distanza==end->distanza)) {
            Stazione *new_st = actual->prev;
            Stazione *old_st = actual;
            Tappa *last = aggiungiTappa(&temp, start->distanza);
            last = aggiungiTappa(&temp, old_st->distanza);

            tmp_auto = old_st->head->autonomia;

            while (new_st != NULL && new_st->distanza >= end->distanza &&
                   tmp_auto >= new_st->next->distanza - new_st->distanza) {

                if (temp.n_tappe > migliore.n_tappe || migliore.coeff_dist < temp.coeff_dist) break; //condizioni di uscita
                tmp_auto -= new_st->next->distanza - new_st->distanza;

                if (tmp_auto >= 0 && (new_st == end || new_st->distanza - new_st->head->autonomia <= end->distanza)) {
                    last = aggiungiTappa(&temp, new_st->distanza);
                    if (new_st->distanza - new_st->head->autonomia <= end->distanza)
                        last = aggiungiTappa(&temp, end->distanza);
                    if (migliore.n_tappe > temp.n_tappe || (migliore.n_tappe==temp.n_tappe&&migliore.coeff_dist>temp.coeff_dist)) {
                        deallocaTappa(&migliore.tappe);
                        migliore = temp;
                        break;
                    }
                    break;
                }
                if (tmp_auto <= 0 || new_st->distanza - new_st->prev->distanza > tmp_auto) {
                    if (new_st->prev != NULL && old_st->head->autonomia <= new_st->prev->distanza - old_st->distanza) {
                        last = aggiungiTappa(&temp, new_st->distanza);
                        tmp_auto = new_st->head->autonomia;
                        old_st = new_st;
                    } else {
                        tmp_auto += old_st->distanza - new_st->distanza;
                        new_st = old_st;
                        old_st = old_st->prev;
                        do rimuoviUltimaTappa(&temp);
                        while (last != NULL && old_st != NULL && last->distanza < old_st->distanza);

                        if (old_st != NULL && actual != NULL && old_st->distanza > actual->distanza)break;
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

    Stazione* actual = start;
    int autonomia = start->head->autonomia;

    int tmp_auto = autonomia;

    while (start->distanza+autonomia >= actual->distanza){
        Percorso temp;
        temp.n_tappe=0;
        temp.coeff_dist=0;
        temp.tappe=NULL;

        if (actual->head->autonomia>0&& (actual->head->autonomia>=actual->next->distanza-actual->distanza || actual->distanza==end->distanza)){
            Stazione* new_st = actual->next;
            Stazione* old_st = actual;
            Tappa* last = aggiungiTappa(&temp, start->distanza);
            last = aggiungiTappa(&temp, old_st->distanza);

            tmp_auto = old_st->head->autonomia;

            while (new_st != NULL && new_st->distanza <= end->distanza && tmp_auto >= new_st->distanza-new_st->prev->distanza){

                if(temp.n_tappe>migliore.n_tappe || migliore.coeff_dist<temp.coeff_dist) break; //condizioni di uscita
                tmp_auto -= new_st->distanza - new_st->prev->distanza;

                if(tmp_auto >= 0 && (new_st == end || new_st->head->autonomia+new_st->distanza>=end->distanza)){
                    last = aggiungiTappa(&temp, new_st->distanza);
                    if(new_st->head->autonomia+new_st->distanza>=end->distanza) last = aggiungiTappa(&temp, end->distanza);
                    if(migliore.n_tappe>temp.n_tappe|| (migliore.n_tappe==temp.n_tappe&&migliore.coeff_dist>temp.coeff_dist)) {
                        deallocaTappa(&migliore.tappe);
                        migliore=temp;
                        break;
                    }
                }
                if(tmp_auto <= 0 || new_st->next->distanza-new_st->distanza > tmp_auto){
                    if(new_st->next!=NULL && old_st->head->autonomia <= new_st->next->distanza-old_st->distanza){
                        last = aggiungiTappa(&temp, new_st->distanza);
                        tmp_auto=new_st->head->autonomia;
                        old_st=new_st;
                    }else{
                        tmp_auto+=new_st->distanza - old_st->distanza;
                        new_st = old_st;
                        old_st = old_st->prev;
                        do rimuoviUltimaTappa(&temp);
                        while (last != NULL && old_st!=NULL && last->distanza>old_st->distanza);

                        if(old_st!=NULL && actual!=NULL && old_st->distanza<actual->distanza)break;
                    }
                }
                new_st=new_st->next;
            }
        }
        actual=actual->next;
    }
    if(migliore.n_tappe<3||migliore.n_tappe==10000)migliore.n_tappe=DELETED;
    return migliore;
}

/**
 * Stampa in stdout il percorso ottimale, cioè quello con minor numero
 * di tappe e con quest'ultime più vicine all'inizio dell'autostrada.
 * @param percorso lista mono-direzionale di tappe
 */
void stampaPercorso(Tappa* percorso) {
    while (percorso != NULL) {
        printf("%d ", percorso->distanza);
        percorso = percorso->next;
    }
    printf("\n");
    deallocaTappa(&percorso);
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
    if (stazionePartenza == NULL || stazioneArrivo == NULL) {
        if(printf("nessun percorso\n")<0) return;
        return;
    }
    int var = d_end-d_start;
    if(var == 0){
        if(printf("%d \n", d_start)<0) return;
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
    FILE* source = stdin;
    char input[20];
    while(fscanf(source, "%s", input) != EOF){
        if(strncmp(input, "aggiungi-stazione", strlen("aggiungi-stazione"))==0) {
            unsigned int dist, n_a;
            if(fscanf(source, "%d %d", &dist, &n_a)!=EOF){
                unsigned int a[512];
                for(int i = 0; i < n_a; i++)if(fscanf(source, " %d", &a[i])!=EOF)continue;
                aggiungiStazione(&head, dist, n_a, a);
            }
        }
        if(strncmp(input, "aggiungi-auto", strlen("aggiungi-auto"))==0) {
            unsigned int dist, a;
            if(fscanf(source, "%d %d", &dist, &a) != EOF) aggiungiAuto(&head, dist, a);
        }
        if(strncmp(input, "rottama-auto", strlen("rottama-auto"))==0) {
            unsigned int dist, a;
            if(fscanf(source, "%d %d", &dist, &a) != EOF) rottamaAuto(&head, dist, a);
        }
        if(strncmp(input, "demolisci-stazione", strlen("demolisci-stazione"))==0) {
            unsigned int dist;
            if(fscanf(source, "%d", &dist)!=EOF) demolisciStazione(&head, dist);
        }
        if(strncmp(input, "pianifica-percorso", strlen("pianifica-percorso"))==0) {
            unsigned int start, end;
            if(fscanf(source, "%d %d", &start, &end)!=EOF) pianificaPercorso(&head, start, end);

        }
    }
    deallocaStazioni(&head);
    return 0;
}
