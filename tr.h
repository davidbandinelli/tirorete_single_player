// definizione delle classi

#ifndef TR_H
#define TR_H
#define MAX_GIOCATORI       50
#define MAX_NON_GIOCATORI    5
#define MAX_SQUADRE         18
#define MAX_PARTITE        306 
#include <windows.h>

void tasto(void);
void cls( void );

class giocatore 
{
protected:
   char *nome; 
   int  id_giocatore; //progressivo giocatore (a livello di tutto il campionato)

public:
   int  pdisp; //partite disputate
   int  reti;
   int  pd;    //punti disciplina
   int  livello; 
   int  forma;
   int  eta;
   int  valore;
   char ruolo;
   char speciale;
   char gioca;
   void initialize(int in_id_giocatore, char *in_nome, int in_livello, int in_forma, 
	               int in_eta, char in_ruolo, char in_speciale);
   int determina_valore(void);
   void display_dati(void);
   void set_nome(char *in_nome);
   char *get_nome(void);
   int get_id(void);
};


class non_giocatore 
{
protected:
   char *nome; 
public:
   int  eta;
   char ruolo;
   char speciale;
   char *tat;        //tattica per cui si ha il bonus
   char lib;         //libero S/N
   char fuo;         //fuorigioco S/N
   int bonus;        //bonus punti per il tattico
   void initialize(char *in_nome, int in_eta, 
	               char in_ruolo, char in_speciale, char *in_tat, char in_lib,
				   char in_fuo, int in_bonus);
   int determina_valore(void);
   void display_dati(void);
   void set_nome(char *in_nome);
   char *get_nome(void);
   void set_tat(char *in_tat);
   char *get_tat(void);
};


class squadra 
{
protected:
   char *nome;

   int poc;          //posizione in campionato
   int tce;          //numero turno coppa europea
public:
   int id_squadra;   //progressivo squadra
   int flg_ai;       //0=computer, 1=giocatore umano
   char *tat;        //tattica impiegata
   int dur;          //durezza (0-10)
   int ppo;          //valore portiere
   int pli;          //valore libero
   int pdi;          //punti difesa
   int pce;          //punti centrocampo
   int pat;          //punti attacco
   int fc;           //fattore campo
   char cat;         //catenaccio S/N
   int pa;           //punti allenamento
   int ps;           //pacche spalle
   int pgp;          //punti grande prestazione
   int ml;           //milioni
   char lib;         //libero S/N
   char fuo;         //fuorigioco S/N
   int ngr;          //numero giocatori in rosa
   int nng;          //numero non giocatori in rosa
   int pts;          //punti in campionato
   int rf;           //reti fatte
   int rs;           //reti subite
   int pv;			 //partite vinte
   int pn;           //partite pareggiate
   int pp;           //partite perse
   int ppg;          //partite disputate in una giornata
   int agghs;		//gol coppa in casa per turno
   int agghf;		//gol coppa in casa per turno
   int aggas;		//gol trasferta coppa per turno
   int aggaf;		//gol trasferta coppa per turno
   int tcl;          //numero turno coppa di lega (-1 non partecipa, 0 primo turno, 1 secondo ecc.)
   giocatore *giocatori[MAX_GIOCATORI];
   non_giocatore *non_giocatori[MAX_NON_GIOCATORI];
   void initialize(char *in_nome, int in_pa, int in_ps, int in_pgp, int in_ml, char *in_tat,
	               char in_lib, char in_fuo, int in_id_squadra);
   void set_nome(char *in_nome);
   char *get_nome(void);
   void set_tat(char *in_tat);
   char *get_tat(void);
   void display(int modo);
   int determina_valori_area(void);
   void assegna_bonus(squadra *,int);
};


class campionato
{
public:
    int giornate[(MAX_SQUADRE - 1) * 2][MAX_SQUADRE / 2][2];
	int classifica[MAX_SQUADRE][2];
	int classifica_marcatori[MAX_GIOCATORI*MAX_SQUADRE][2];
	int progr_partita;
	char storia[MAX_PARTITE][20];
	int calendario[MAX_PARTITE][2];
	int num_partite;
	int num_squadre;
	int squadra_scelta;
	int partite_per_giornata;
	int num_giornata;
	int num_giornate;
	//dati coppa
	int num_partite_coppa;
	int num_squadre_coppa;
	int num_turni_coppa;
	int progr_partita_coppa;
	int calendario_coppa[MAX_PARTITE][2];
	char storia_coppa[MAX_PARTITE][30];
	//dati coppa
	int num_giocatori_totali;
	squadra *squadre[MAX_SQUADRE];

	void initialize(int num_squadre);
	void display(int modo);
	void set_num_squadre(int in_num_squadre);
	void determina_calendario(void);
	void determina_calendarioV2(void);
	void determina_calendario_coppa(int turno);
	void determina_turni_coppa(void);
	void determina_classifica(void);
	void determina_classifica_marcatori(void);
    char *get_nome_id(int id);
    char *get_squadra_id(int id);
};

/*
class coppa:public campionato
{
public:
	int turno;
	void determina_calendario(void);
};
*/

class partita
{
protected:
public:
	squadra *squadra1;
	squadra *squadra2;
	int tiri1;
	int tiri2;
	int res1;
	int res2;
	int controlla_triplo(void); //0 OK, 1 sq. 1, 2 sq. 2, 3 entrambi
	void calcola_tiri(void);
	void gioca_partita(int flg_tipo_partita); //0 campionato, 1 coppa
	void initialize(squadra *, squadra *);
	void assegna_gol(squadra *);
	void determina_infortuni(squadra *, squadra *, int );
	void determina_sanzioni(squadra *, int);
};


#endif
