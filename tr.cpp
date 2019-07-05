// implementazione delle classi

#include "tr.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <math.h>

void squadra::initialize(char *in_nome, int in_pa, int in_ps, int in_pgp, int in_ml, 
						 char *in_tat, char in_lib, char in_fuo, int in_id_squadra)
{
    nome = new char[strlen(in_nome)];
	strcpy(nome,in_nome);
    tat = new char[strlen(in_tat)];
	strcpy(tat,in_tat);

	pa = in_pa;
	ps = in_ps;
	pgp = in_pgp;
	ml = in_ml;
	lib = in_lib;
	fuo = in_fuo;
	poc = 0;
    ngr = 0;
    nng = 0;
    tce = 0;
    tcl = -1; // -1 non partecipa alle coppe
    dur = 0;
    ppo = 0;
    pli = 0;
    pdi = 0;
    pce = 0;
    pat = 0;
	pts = 0;
	fc  = 0;
	cat = 'N';
	id_squadra = in_id_squadra;
	flg_ai = 0;
	rf = 0;
	rs = 0;
	pv = 0;
	pp = 0;
	pn = 0;
	aggaf = 0;
	agghs = 0;
	aggas = 0;
	agghf = 0;
	ppg = 0;
}

void squadra::set_nome(char *in_nome)
{
    nome = new char[strlen(in_nome)];
	strcpy(nome,in_nome);
}

char *squadra::get_nome(void)
{
	return nome;
}

void squadra::set_tat(char *in_tat)
{
    tat = new char[strlen(in_tat)];
	strcpy(tat,in_tat);
}

char *squadra::get_tat(void)
{
	return tat;
}

void squadra::display(int modo)
{
   switch(modo)
   {
      case 1: // elenco giocatori
	  {
		  printf("Id.R Nome                 LV  FO  PD RF DI ETA\n");
		  printf("----------------------------------------------\n");
          for(int i=0;i<ngr;i++)
	         printf( "%2d %c %20s %2d %3d %2d %2d %2d  %1d\n", i , 
			         giocatori[i]->ruolo ,
			         giocatori[i]->get_nome(), giocatori[i]->livello, giocatori[i]->forma,
					 giocatori[i]->pd, giocatori[i]->reti, giocatori[i]->pdisp, 
					 giocatori[i]->eta);
	  }
	  break;
      case 2: // elenco non giocatori
	  {
          for(int i=0;i<nng;i++)
	         printf( "Non giocatore : %d  %c %20s %5s\n" , i ,non_giocatori[i]->ruolo, 
			         non_giocatori[i]->get_nome(),non_giocatori[i]->get_tat());
	  }
	  break;
      case 3: // valori di area
	  {
		 printf ( "Nome: %s \n",get_nome());
		 printf( "TAT: %s LI: %c FU: %c \n", tat , lib , fuo);
         printf( "PO : %d LI: %d DI: %d CE: %d AT: %d \n", ppo , pli , pdi , pce , pat); 
		 printf ( "PA : %d PGP : %d PS : %d MIL : %d DUR : %d\n", pa, pgp, ps, ml, dur);
	  }
	  break;
   }
}

int squadra::determina_valori_area(void)
{
   int ndif=0,ncen=0,natt=0;
   int sdif=0,scen=0,satt=0;
   char appo[2];	

   pdi = 0;
   pli = 0;
   pdi = 0;
   pce = 0;
   pat = 0;
   ppo = 0;

   appo[0] = tat[0];
   appo[1] = '\0';
   ndif = atoi(appo);
   appo[0] = tat[1];
   appo[1] = '\0';
   ncen = atoi(appo);
   appo[0] = tat[2];
   appo[1] = '\0';
   natt = atoi(appo);
   if ((ndif + ncen + natt) != 10)
   {
	   printf( "%s Tattica incongruente ! \n",get_nome());
	   return 1;
   }
   int port = 0;
   int libero = 0;
   for (int i=0;i<ngr;i++)
   {
	   //cerco portiere schierato
	   if (giocatori[i]->ruolo == 'P' && giocatori[i]->gioca == 'S' 
		   && giocatori[i]->forma >= -2 && giocatori[i]->pd <= 10)
	   {
		   port = 1;
		   ppo = giocatori[i]->livello + giocatori[i]->forma;
	   }
	   //cerco libero schierato se tattica include libero
	   if (lib == 'S' && giocatori[i]->ruolo == 'L' && giocatori[i]->gioca == 'S'
		   && giocatori[i]->forma >= -2 && giocatori[i]->pd <= 10)
	   {
		   libero = 1;
		   pli = giocatori[i]->livello + giocatori[i]->forma;
	   }
	   //cerco difensore schierato
	   if (giocatori[i]->ruolo == 'D' && giocatori[i]->gioca == 'S' 
		   && giocatori[i]->forma >= -2 && giocatori[i]->pd <= 10)
	   {
		   sdif ++;
		   pdi += (giocatori[i]->livello + giocatori[i]->forma);
	   }
	   //cerco centrocampista schierato
	   if (giocatori[i]->ruolo == 'C' && giocatori[i]->gioca == 'S' 
		   && giocatori[i]->forma >= -2 && giocatori[i]->pd <= 10)
	   {
		   scen ++;
		   pce += (giocatori[i]->livello + giocatori[i]->forma);
	   }
	   //cerco attaccante schierato
	   if (giocatori[i]->ruolo == 'A' && giocatori[i]->gioca == 'S' 
		   && giocatori[i]->forma >= -2 && giocatori[i]->pd <= 10)
	   {
		   satt ++;
		   pat += (giocatori[i]->livello + giocatori[i]->forma);
	   }
   }

   // gestione squalifiche e visualizzazione infortuni gravi
   for (int i=0;i<ngr;i++)
   {
		if (giocatori[i]->pd >10)
		{
			printf ("%s e' squalificato per una partita \n",giocatori[i]->get_nome());
			giocatori[i]->pd -= 10;
		}
		if (giocatori[i]->forma < -2)
		{
			printf ("%s non puo' giocare perche' ha forma %d \n",giocatori[i]->get_nome(),
					giocatori[i]->forma);
			giocatori[i]->forma ++;
		}
   }

   // controllo congruenza formazione
   if (port == 0)
   {
	   printf( "%s Manca il portiere !\n",get_nome());
	   return 1;
   }
   if (libero == 0 && lib == 'S')
   {
	   printf( "%s Manca il libero (la tattica scelta lo prevede) !\n",get_nome());
	   return 2;
   }
   if (lib == 'N' && sdif != ndif)
   {
	   printf( "%s Numero difensori (%d) incongruente (senza libero) !\n",get_nome(),sdif);
	   return 3;
   }
   if (lib == 'S' && sdif != (ndif-1))
   {
	   printf( "%s Numero difensori (%d) incongruente (con libero) !\n",get_nome(),sdif);
	   return 4;
   }
   if (scen != ncen)
   {
	   printf( "%s Numero centrocampisti (%d) incongruente !\n",get_nome(),scen);
	   return 5;
   }
   if (satt != natt)
   {
	   printf( "%s Numero attaccanti (%d) incongruente !\n",get_nome(),satt);
	   return 6;
   }
   return 0;
}

void giocatore::initialize(int in_id_giocatore, char *in_nome, int in_livello, 
						   int in_forma, int in_eta, char in_ruolo, char in_speciale)
{
    nome = new char[strlen(in_nome)];
	strcpy(nome,in_nome);
	livello = in_livello;
	forma = in_forma;
	eta = in_eta;
	ruolo = in_ruolo;
	speciale = in_speciale;
	id_giocatore = in_id_giocatore;
	valore = determina_valore();
	reti = 0;
	pd = 0;
	gioca = 'S';
	pdisp = 0;
}

int giocatore::determina_valore(void)
{
	valore = ((6 - eta) * livello) * 100;
	return valore;
}

void giocatore::display_dati(void)
{
	printf( "Nome:   %s \n" , get_nome());
	printf( "Valore: %d \n" , valore);
}

void giocatore::set_nome(char *in_nome)
{
    nome = new char[strlen(in_nome)];
	strcpy(nome,in_nome);
}

char *giocatore::get_nome(void)
{
	return nome;
}

int giocatore::get_id(void)
{
	return id_giocatore;
}

void non_giocatore::initialize(char *in_nome, int in_eta, char in_ruolo, 
                               char in_speciale, char *in_tat, char in_lib,
                               char in_fuo, int in_bonus)
{
   nome = new char[strlen(in_nome)];
   strcpy(nome,in_nome);

   tat = new char[strlen(in_tat)];
   strcpy(tat,in_tat);
   nome = in_nome;
   eta = in_eta;
   ruolo = in_ruolo;
   speciale = in_speciale;
   lib = in_lib;
   fuo = in_fuo;
   bonus = in_bonus;
}

void non_giocatore::set_nome(char *in_nome)
{
    nome = new char[strlen(in_nome)];
	strcpy(nome,in_nome);
}

char *non_giocatore::get_nome(void)
{
	return nome;
}

void non_giocatore::set_tat(char *in_tat)
{
    tat = new char[strlen(in_tat)];
	strcpy(tat,in_tat);
}

char *non_giocatore::get_tat(void)
{
	return tat;
}

void non_giocatore::display_dati(void)
{
	printf( "Nome : %s \n" , get_nome());
	printf( "Ruolo: %c \n" , ruolo);
}

void campionato::initialize(int in_squadre)
{
   num_squadre = in_squadre;
   num_partite = 0;
   progr_partita = 0;
   num_partite_coppa = 0;
   num_squadre_coppa = 0;
   num_turni_coppa = 0;
   progr_partita_coppa = 0;
   partite_per_giornata = 0;
   num_giornata = 0;
   num_giornate = 0;
}

char *campionato::get_nome_id(int id)
{
	for (int j=0;j<num_squadre;j++)
	{
		for (int i=0;i<squadre[j]->ngr;i++)
		{
			if (squadre[j]->giocatori[i]->get_id() == id)
				return squadre[j]->giocatori[i]->get_nome();
		}
	}
	return NULL;
}

char *campionato::get_squadra_id(int id)
{
	for (int j=0;j<num_squadre;j++)
	{
		for (int i=0;i<squadre[j]->ngr;i++)
		{
			if (squadre[j]->giocatori[i]->get_id() == id)
				return squadre[j]->get_nome();
		}
	}
	return NULL;
}

void campionato::set_num_squadre(int in_num_squadre)
{
	num_squadre = in_num_squadre;
}

void campionato::display(int modo)
{
   switch(modo)
   {
      case 1: // squadre partecipanti
	  {
          for(int i=0;i<num_squadre;i++)
		  {
	         printf( "Squadra :%d %s \n" , i ,squadre[i]->get_nome());
			 printf( "PA: %d PS: %d PGP: %d \n" , squadre[i]->pa ,squadre[i]->ps , 
				     squadre[i]->pgp);
			 printf( "TAT: %d L: %c F: %c ML: %d \n" , squadre[i]->get_tat() ,
				     squadre[i]->lib, squadre[i]->fuo , squadre[i]->ml);
		  }
	  }
	  break;
      case 2: // calendario
	  {
          for(int i=0;i<num_partite;i++)
		  {
		     printf( "%s - %s \n",squadre[calendario[i][0]]->get_nome(), squadre[calendario[i][1]]->get_nome());
		  }
		  printf ("\n Numero partite %d di cui %d per giornata; TOT giornate %d\n",num_partite, partite_per_giornata, num_giornate);
	  }
	  break;
      case 3: // storia risultati
	  {
          for(int i=0;i<progr_partita;i++)
		     printf( "%s\n", storia[i]);
	  }
	  break;
      case 4: // classifica campionato
	  {
          for(int i=0;i<num_squadre;i++)
		     printf( "%d)\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1,
			         squadre[classifica[i][1]]->get_nome() , 
			         squadre[classifica[i][1]]->pts ,  
				     squadre[classifica[i][1]]->rf  ,
				     squadre[classifica[i][1]]->rs  ,
				     squadre[classifica[i][1]]->pv  ,
				     squadre[classifica[i][1]]->pn  ,
				     squadre[classifica[i][1]]->pp);
	  }
	  break;
      case 5: // classifica marcatori
	  {
		  int p=0;
          for(int i=0;i<num_giocatori_totali;i++)
		  {
			  if(classifica_marcatori[i][0] > 0)
			  {
				if (p < 20)
				{
					printf( "%2d\t%20s\t(%s)\n",classifica_marcatori[i][0],
						    get_nome_id(classifica_marcatori[i][1]),
							get_squadra_id(classifica_marcatori[i][1]));
					p++;
				}
			  }
		  }
	  }
	  break;
      case 6: // calendario coppa
	  {
          for(int i=0;i<num_partite_coppa;i++)
		  {
			 if (i == (num_partite_coppa/2))
				 printf ("\n");

		     printf( "%s - %s \n",squadre[calendario_coppa[i][0]]->get_nome(), squadre[calendario_coppa[i][1]]->get_nome());
		  }
		  printf ("\n Numero partite %d Numero turni %d\n",num_partite_coppa,num_turni_coppa);
	  }
	  break;
	  /*
      case 7: // storia risultati di una squadra
	  {
          for(int i=0;i<progr_partita;i++)
		     printf( "%s\n", storia[i]);
	  }
	  break;
	  */
      case 7: // storia risultati coppa
	  {
		  printf ("Andata: \n");
          for(int i=0;i<progr_partita_coppa;i++)
		  {
			 if (i == (num_partite_coppa/2))
				 printf ("\nRitorno: \n");

		     printf( "%s\n", storia_coppa[i]);
		  }
	  }
	  break;
	  case 8: //elenco squadre per scelta
	  {
          for(int i=0;i<num_squadre;i++)
		  {
	         printf( "Squadra :%d %s \n" , i ,squadre[i]->get_nome());
		  }
	  }
	  break;
     }
}

void campionato::determina_calendario(void)
{
	int h = 0;
	int pg = 0; // partite della giornata
	int ng = 0; // numero giornate
	int k = 0;

	num_partite = 0;
	campionato::partite_per_giornata = num_squadre / 2;
	campionato::num_giornata = 0;

	//andata
	for (int i=0;i<num_squadre;i++)
	{
		for (int j = i + 1;j < num_squadre;j++)
		{
			calendario[h][0] = i;
			calendario[h][1] = j;
			num_partite++;
			h++;
		}
	}

	//randomizzazione del calendario
	for (int i = 0;(i < 100000);i++)
	{
		int appo1 = 0,appo2 = 0;
		int apposwap1 = 0,apposwap2 = 0;
		appo1 = (int)((rand() % (num_partite)));
		appo2 = (int)((rand() % (num_partite)));

		apposwap1 = calendario[appo1][0];
		apposwap2 = calendario[appo1][1];

		calendario[appo1][0] = calendario[appo2][0];
		calendario[appo1][1] = calendario[appo2][1];

		calendario[appo2][0] = apposwap1;
		calendario[appo2][1] = apposwap2;
	}

	//ritorno
	for (int i = num_partite;i < (2 * num_partite);i++)
	{
		calendario[i][0] = calendario[i-num_partite][1];
		calendario[i][1] = calendario[i-num_partite][0];
	}
	num_partite = num_partite * 2;
	num_giornate = num_squadre - 1;

	//int giornate[num_giornate][partite_per_giornata][2];
	for (int I1 = 0; I1 < (num_giornate * 2); I1++) {
        for (int I2 = 0; I2 < partite_per_giornata; I2++) {
            giornate[I1][I2][0] = -1;
            giornate[I1][I2][1] = -1;
        }
    }

	// andata
	int nGiornata = 0, pGiornata = 0;
	while (nGiornata < num_giornate) {
		pGiornata = 0;
		while (pGiornata < partite_per_giornata) {
            for (k = 0; k < num_partite; k++) {
				int flgAlready = 0;
				for (int w = 0; w < num_giornate; w++) {
					for (int z = 0; z < partite_per_giornata; z++) {
						if (
							 (
							  (giornate[w][z][0] == calendario[k][0]) &&
							  (giornate[w][z][1] == calendario[k][1])
							 ) 
							) {
							flgAlready = 1;
						}
					}
				}
				if (flgAlready == 0) {
					// controllo nella giornata corrente di non aver già fatto giocare la squdra
					int flgTeam = 0;
					for (int z = 0; z < partite_per_giornata; z++) {
						if (
							(
							(giornate[nGiornata][z][0] == calendario[k][0])
							) 
							||
							(
							(giornate[nGiornata][z][0] == calendario[k][1])
							) 
							||
							(
							(giornate[nGiornata][z][1] == calendario[k][0])
							) 
							||
							(
							(giornate[nGiornata][z][1] == calendario[k][1])
							) 
							) {
							flgTeam = 1;
						}
					}
					if (flgTeam == 0) {
						giornate[nGiornata][pGiornata][0] = calendario[k][0];
						giornate[nGiornata][pGiornata][1] = calendario[k][1];
						//printf("DEBUG ADDP G=%d >%s< >%s<\n", nGiornata, squadre[giornate[nGiornata][pGiornata][0]]->get_nome(), squadre[giornate[nGiornata][pGiornata][1]]->get_nome());
						pGiornata++;
					}
				}
            }
        }
        nGiornata++;
    }
	// ritorno
	for (int w = 0; w < num_giornate; w++) {
		for (int z = 0; z < partite_per_giornata; z++) {
			giornate[w + 17][z][0] = giornate[w][z][1];
			giornate[w + 17][z][1] = giornate[w][z][0];
		}
	}

    //nGiornata = num_giornate - 1;

    //printf("DEBUG GIORNATE \n");
    //printf("NG=%d ppg=%d\n", num_giornate, partite_per_giornata);
	for (h=0; h < partite_per_giornata; h++) {
        //printf("GG1=%d, GG2=%d SQ=%s \n", giornate[0][h][0], giornate[0][h][1], squadre[14]->get_nome());
        //printf ("DEBUG G1 AND >%s< >%s<\n",squadre[giornate[0][h][0]]->get_nome(),squadre[giornate[0][h][1]]->get_nome());
    }
	for (h=0; h < partite_per_giornata; h++) {
        //printf ("DEBUG G1 RIT >%s< >%s<\n",squadre[giornate[17][h][0]]->get_nome(),squadre[giornate[17][h][1]]->get_nome());
    }
    //printf("FINE DEBUG GIORNATE \n");
  
	// riversamento calendario
	int calend = 0;
	for (int w = 0; w < (num_giornate * 2); w++) {
		for (int z = 0; z < partite_per_giornata; z++) {
			calendario[calend][0] = giornate[w][z][0];
			calendario[calend][1] = giornate[w][z][1];
			calend++;
		}
	}
}

void campionato::determina_classifica(void)
{
	num_giornata = (progr_partita / partite_per_giornata) + 1;

	int appo0 = 0;
	int appo1 = 0;

	for (int i = 0; i < num_squadre; i++)
	{
		classifica[i][0] = squadre[i]->pts;
		classifica[i][1] = i;
	}
	for (int i = 0; i < num_squadre; i++)
	{
		for (int j = i + 1; j < num_squadre; j++)
		{
			if (classifica[i][0] < classifica[j][0])
			{
				if (classifica[i][0] < classifica[j][0]) {
					appo0 = classifica[j][0];
					appo1 = classifica[j][1];
					classifica[j][0] = classifica[i][0];
					classifica[j][1] = classifica[i][1];
					classifica[i][0] = appo0;
					classifica[i][1] = appo1;
				}
			}
		}
	}

	// differenza reti
	for (int i = 0; i < num_squadre; i++) {
		for (int j = i + 1; j < num_squadre; j++) {
			if (classifica[i][0] == classifica[j][0]) {
				int diffRetA = (squadre[classifica[i][1]]->rf - squadre[classifica[i][1]]->rs);
				int diffRetB = (squadre[classifica[j][1]]->rf - squadre[classifica[j][1]]->rs);
				if (diffRetA < diffRetB) {
					appo1 = classifica[j][1];
					classifica[j][1] = classifica[i][1];
					classifica[i][1] = appo1;
				}
				// a parità di differenza reti valgono le reti segnate
				if (diffRetA == diffRetB) {
					int rfA = squadre[classifica[i][1]]->rf;
					int rfB = squadre[classifica[j][1]]->rf;
					if (rfA < rfB) {
						appo1 = classifica[j][1];
						classifica[j][1] = classifica[i][1];
						classifica[i][1] = appo1;
					}
				}
			}
		}
	}

}

void campionato::determina_classifica_marcatori(void)
{
	int n=0, appo0=0, appo1=0;
	for (int i=0;i<(MAX_GIOCATORI*MAX_SQUADRE);i++)
	{
		classifica_marcatori[i][1] = 0;
		classifica_marcatori[i][0] = 0;
	}

	//classifica marcatori
	for (int i=0;i<num_squadre;i++)
	{
		for (int j=0;j<squadre[i]->ngr;j++)
		{
			classifica_marcatori[n][0] = squadre[i]->giocatori[j]->reti;
			classifica_marcatori[n][1] = squadre[i]->giocatori[j]->get_id();
			n++;
		}
	}
	num_giocatori_totali = n;
	for (int i=0;i<n;i++)
	{
		for (int j=i+1;j<n;j++)
		{
			if (classifica_marcatori[i][0] < classifica_marcatori[j][0])
			{
				appo0 = classifica_marcatori[j][0];
				appo1 = classifica_marcatori[j][1];
				classifica_marcatori[j][0] = classifica_marcatori[i][0];
				classifica_marcatori[j][1] = classifica_marcatori[i][1];
				classifica_marcatori[i][0] = appo0;
				classifica_marcatori[i][1] = appo1;
			}
		}
	}
}

void partita::initialize(squadra *in_squadra1, squadra *in_squadra2)
{
	tiri1 = 0;
	tiri2 = 0;
	res1 = 0;
	res2 = 0;
	squadra1 = in_squadra1;
	squadra2 = in_squadra2;
}

int partita::controlla_triplo(void)
{
	int max1=0;
	if (squadra1->pdi > squadra1->pce)
		max1 = squadra1->pdi;
	else
		max1 =squadra1->pce;
	if (squadra1->pat > max1)
		max1 = squadra1->pat;
    if ((squadra1->pdi * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (DI squadra 1)\n");
		return 1;
	}
    if ((squadra1->pce * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (CE squadra 1)\n");
		return 1;
	}
    if ((squadra1->pat * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (AT squadra 1)\n");
		return 1;
	}
	if (squadra2->pdi > squadra2->pce)
		max1 = squadra2->pdi;
	else
		max1 =squadra2->pce;
	if (squadra2->pat > max1)
		max1 = squadra2->pat;
    if ((squadra2->pdi * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (DI squadra 2)\n");
		return 2;
	}
    if ((squadra2->pce * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (CE squadra 2)\n");
		return 2;
	}
    if ((squadra2->pat * 3) < max1)
	{
		printf( "Regola del triplo non rispettata (AT squadra 2)\n");
		return 2;
	}
	return 0;
}

void partita::calcola_tiri(void)
{
	int diff=0;
	//tiri difesa
	if (squadra1->pdi > squadra2->pdi)
	{
		diff = squadra1->pdi - squadra2->pdi;
		if (squadra1->lib == 'S')
			tiri1 += (int)(diff / 3);
		else
			tiri1 += (int)(diff / 5);
    }
	else
	{
		diff = squadra2->pdi - squadra1->pdi;
 		if (squadra2->lib == 'S')
			tiri2 += (int)(diff / 3);
		else
			tiri2 += (int)(diff / 5);
    }

	//tiri centrocampo
	if (squadra1->pce > squadra2->pce)
	{
		diff = squadra1->pce - squadra2->pce;
		if (squadra2->fuo == 'S')
			tiri1 += diff;
		else
			tiri1 += (int)(diff / 2);
	}
	else
	{
		diff = squadra2->pce - squadra1->pce;
		if (squadra1->fuo == 'S')
			tiri2 += diff;
		else
			tiri2 += (int)(diff / 2);
	}

	//tiri attacco
	if (squadra1->pat > squadra2->pat)
	{
		diff = squadra1->pat - squadra2->pat;
		if (squadra2->fuo == 'S')
			tiri1 += (int)(diff / 2);
		else
			tiri1 += diff;
	}
	else
	{
		diff = squadra2->pat - squadra1->pat;
		if (squadra1->fuo == 'S')
			tiri2 += (int)(diff / 2);
		else
			tiri2 += diff;
	}
	
	//gestione catenaccio
	if (squadra1->cat == 'S')
		tiri1 = int(tiri1 / 2);
	if (squadra2->cat == 'S')
		tiri2 = int(tiri2 / 2);
	printf( "Tiri totali sq. %s: %d Tiri totali sq. %s: %d \n " ,squadra1->get_nome(), tiri1 ,
		    squadra2->get_nome(), tiri2);
}

void partita::gioca_partita(int flg_tipo_partita)
{
	int appo=0;
	int appoport = 0;
	int appolib  = 0;

	//rigori, infortuni, ammonizioni, espulsioni
	
//	determina_infortuni(squadra1, squadra 2); 
//	determina_infortuni(squadra2, squadra 1); 

	for (int i=0;i<tiri1;i++)
	{
		appo = (int)((rand() % 100)+1);
		if (appo <= 65)
			printf( "Squadra 1, tiro: %d Fuori! \n" , i);
		else
		{
			//la squadra avversaria ha il libero ?
			if (squadra2->lib == 'S')
			{
				appolib = (int)(20+(squadra2->pli * 2));
				appo = (int)((rand() % 100)+1);
				if (appo <= appolib)
					printf( "Squadra 1, tiro: %d Bloccato dal libero \n" , i);
				else
				{
					//portiere
					appoport = (int)(30+(squadra2->ppo * 2.25));
					appo = (int)((rand() % 100)+1);
					if (appo <= appoport)
						printf( "Squadra 1, tiro: %d Bloccato dal portiere \n" , i);
					else
					{
						//palo o traversa
						appo = (int)((rand() % 100)+1);
						if (appo <= 5)
							printf( "Squadra 1, tiro: %d Palo/Traversa! \n", i);
						else
						{
							//gol
							printf( "Squadra 1, tiro: %d GOL!!! \n" , i);
							res1++;
							assegna_gol(squadra1);
						}
					}
				}
			}
			else
			{
				//portiere
				appoport = (int)(30+(squadra2->ppo * 2.25));
				appo = (int)((rand() % 100)+1);
				if (appo <= appoport)
					printf( "Squadra 1, tiro: %d Bloccato dal portiere \n" , i);
				else
				{
					//palo o traversa
					appo = (int)((rand() % 100)+1);
					if (appo <= 5)
						printf( "Squadra 1, tiro: %d Palo/Traversa! \n" , i);
					else
					{
						//gol
						printf( "Squadra 1, tiro: %d GOL!!! \n" , i);
						res1++;
						assegna_gol(squadra1);
					}
				}
			}
		}
	}

   tasto();
   cls();
	
	for (int i=0;i<tiri2;i++)
	{
		appo = (int)((rand() % 100)+1);
		if (appo <= 65)
			printf( "Squadra 2, tiro: %d Fuori! \n" , i);
		else
		{
			//la squadra avversaria ha il libero ?
			if (squadra1->lib == 'S')
			{
				appolib = (int)(20+(squadra1->pli * 2));
				appo = (int)((rand() % 100)+1);
				if (appo <= appolib)
					printf( "Squadra 2, tiro: %d Bloccato dal libero \n" , i);
				else
				{
					//portiere
					appoport = (int)(30+(squadra1->ppo * 2.25));
					appo = (int)((rand() % 100)+1);
					if (appo <= appoport)
						printf( "Squadra 2, tiro: %d Bloccato dal portiere \n" , i);
					else
					{
						//palo o traversa
						appo = (int)((rand() % 100)+1);
						if (appo <= 5)
							printf( "Squadra 2, tiro: %d Palo/Traversa! \n" , i);
						else
						{
							//gol
							printf( "Squadra 2, tiro: %d GOL!!! \n" , i);
							res2++;
							assegna_gol(squadra2);
						}
					}
				}
			}
			else
			{
				//portiere
				appoport = (int)(30+(squadra1->ppo * 2.25));
				appo = (int)((rand() % 100)+1);
				if (appo <= appoport)
					printf( "Squadra 2, tiro: %d Bloccato dal portiere \n" , i);
				else
				{
					//palo o traversa
					appo = (int)((rand() % 100)+1);
					if (appo <= 5)
						printf( "Squadra 2, tiro: %d Palo/Traversa! \n" , i);
					else
					{
						//gol
						printf( "Squadra 2, tiro: %d GOL!!! \n" , i);
						res2++;
						assegna_gol(squadra2);
					}
				}
			}
		}
	}
	//rigori
	int percrig = 0;
	percrig = 10 + (5 * squadra2->dur);
	appo = (int)((rand() % 100)+1);
	if (appo <= percrig)
	{
		printf( "Rigore per la squadra 1 ! \n");
		appo = (int)((rand() % 100)+1);
		if (appo <= ((int)10+(squadra2->ppo * 2.25)))
			printf( "Parato !!!\n");
		else
		{
			printf( "Trasformato !!! \n");
			res1++;
			assegna_gol(squadra1);
		}
	}
	percrig = 10 + (5 * squadra1->dur);
	appo = (int)((rand() % 100)+1);
	if (appo <= percrig)
	{
		printf( "Rigore per la squadra 2 ! \n");
		appo = (int)((rand() % 100)+1);
		if (appo <= ((int)10+(squadra1->ppo * 2.25)))
			printf( "Parato !!!\n");
		else
		{
			printf( "Trasformato !!! \n");
			res2++;
			assegna_gol(squadra2);
		}
	}
	
	if (flg_tipo_partita == 0)
	{
		// aggiornamento classifica, PA e ML per il campionato
		if (res1 > res2)
		{
			squadra1->pa += 10;
			squadra1->pts += 3;
			squadra1->pv++;
			squadra2->pp++;
			squadra1->ml += 100;
			squadra2->ml += 50;
		}
		if (res1 == res2)
		{
			squadra1->pa += 5;
			squadra2->pa += 5;
			squadra1->pts += 1;
			squadra2->pts += 1;
			squadra1->pn++;
			squadra2->pn++;
			squadra1->ml += 75;
			squadra2->ml += 75;
		}
		if (res1 < res2)
		{
			squadra2->pa += 10;
			squadra2->pts += 3;
			squadra2->pv++;
			squadra1->pp++;
			squadra1->ml += 25;
			squadra2->ml += 125;
		}
	}
	else
	{
		// aggiornamento PA e reti per la coppa
		if (res1 > res2)
		{
			squadra1->pa += 10;
			squadra1->ml += 200;
			squadra2->ml += 100;
		}
		if (res1 == res2)
		{
			squadra1->pa += 5;
			squadra2->pa += 5;
			squadra1->ml += 150;
			squadra2->ml += 150;
		}
		if (res1 < res2)
		{
			squadra2->pa += 10;
			squadra1->ml += 50;
			squadra2->ml += 250;
		}
		squadra1->agghf += res1;
		squadra2->agghs += res1;
		squadra2->aggas += res2;
		squadra2->aggaf += res2;
	}

	squadra1->rf += res1;
	squadra2->rs += res1;
	squadra1->rs += res2;
	squadra2->rf += res2;

	printf( "%s - %s %d - %d \n",squadra1->get_nome() , squadra2->get_nome(), res1 ,res2);
}

//assegnazione fc,punti tattico,pgp, cat, durezza ecc.
void squadra::assegna_bonus(squadra *sq,int flg_casa)
{
	int appo = 0,appomax = 0;

	//flg_casa 0 (in casa), 1 (fuori casa)
	//assegnazione FC
	if (flg_casa == 0)
		sq->fc = 7;
	else
		sq->fc = 0;

	if (sq->fc > 0)
	{
		//assegnazione FC
		int set = 0;
		if (sq->pdi >= sq->pce)
		{
			appomax = sq->pdi;
			set = 1;
		}
		else
		{
			appomax = sq->pce;
			set = 2;
		}
		if (appomax <= sq->pat)
		{
			appomax = sq->pat;
			set = 3;
		}
		if (set == 1)
		{
			//rinforzo attacco e centrocampo
			appo = (int)((rand() % 100)+1);
			if (appo <= 33)
			{
				sq->pce += 5;
				sq->pat += 2;
				printf ("%s FC  +5CE +2AT \n",get_nome());
			}
			if (appo > 33 && appo <= 66)
			{
				sq->pce += 3;
				sq->pat += 4;
				printf ("%s FC  +3CE +4AT \n",get_nome());
			}
			if (appo > 66)
			{
				sq->pce += 2;
				sq->pat += 5;
				printf ("%s FC  +2CE +5AT \n",get_nome());
			}
		}
		if (set == 2)
		{
			//rinforzo attacco e difesa
			appo = (int)((rand() % 100)+1);
			if (appo <= 33)
			{
				sq->pdi += 5;
				sq->pat += 2;
				printf ("%s FC  +5DI +2AT \n",get_nome());
			}
			if (appo > 33 && appo <= 66)
			{
				sq->pdi += 3;
				sq->pat += 4;
				printf ("%s FC  +3DI +4AT \n",get_nome());
			}
			if (appo > 66)
			{
				sq->pdi += 2;
				sq->pat += 5;
				printf ("%s FC  +2DI +5AT \n",get_nome());
			}
		}
		if (set == 3)
		{
			//rinforzo difesa e centrocampo
			appo = (int)((rand() % 100)+1);
			if (appo <= 33)
			{
				sq->pdi += 5;
				sq->pce += 2;
				printf ("%s FC  +5DI +2CE \n",get_nome());
			}
			if (appo > 33 && appo <= 66)
			{
				sq->pdi += 4;
				sq->pce += 3;
				printf ("%s FC  +4DI +3CE \n",get_nome());
			}
			if (appo > 66)
			{
				sq->pdi += 2;
				sq->pce += 5;
				printf ("%s FC  +2DI +5CE \n",get_nome());
			}
		}
	}
	//assegnazione punti tattico
	for (int i=0;i<sq->nng;i++)
	{
		if (sq->non_giocatori[i]->ruolo == 'T')
		{
			//trovato tattico
			//controllare se la tattica usata è uguale a quella prevista dal tattico
			if (!strcmp(sq->non_giocatori[i]->get_tat(),sq->get_tat()) && 
				sq->non_giocatori[i]->lib == sq->lib                   &&
				sq->non_giocatori[i]->fuo == sq->fuo)
			{
				//aggiungere punti tattico dato che la tattica corrisponde
				//assegnazione FC
				int set=0;
				if (sq->pdi >= sq->pce)
				{
					appomax = sq->pdi;
					set = 1;
				}
				else
				{
					appomax = sq->pce;
					set = 2;
				}
				if (appomax <= sq->pat)
				{
					appomax = sq->pat;
					set = 3;
				}
				if (set == 1)
				{
					sq->pce += 2;
					sq->pat += 3;
					printf ("%s TAT +2CE +3AT \n",get_nome());
				}
				if (set == 2)
				{
					sq->pdi += 2;
					sq->pat += 3;
					printf ("%s TAT +2DI +3AT \n",get_nome());
				}
				if (set == 3)
				{
					sq->pdi += 2;
					sq->pce += 3;
					printf ("%s TAT +2DI +3CE \n",get_nome());
				}
			}
		}
	}
	//determino se la squadra usa PGP
	appo = (int)((rand() % 100)+1);
	if (appo <= 20)
	{
		//se ho ancora PGP li uso
		if (sq->pgp > 0)
		{
			appo = (int)((rand() % 5)+1);
			sq->pgp -= appo;
			int set=0;
			if (sq->pdi >= sq->pce)
			{
				appomax = sq->pdi;
				set = 1;
			}
			else
			{
				appomax = sq->pce;
				set = 2;
			}
			if (appomax <= sq->pat)
			{
				appomax = sq->pat;
				set = 3;
			}
			if (set == 1)
			{
				sq->pat += appo;
				printf ("%s PGP +%dAT PGP rimasti: %d\n",get_nome(), appo, sq->pgp);
			}
			if (set == 2)
			{
				sq->pat += appo;
				printf ("%s PGP +%dAT PGP rimasti: %d\n",get_nome(), appo, sq->pgp);
			}
			if (set == 3)
			{
				sq->pdi += appo;
				printf ("%s PGP +%dDI PGP rimasti: %d\n",get_nome(), appo, sq->pgp);
			}
		}
	}
	//determino se la squadra usa durezza
	appo = (int)((rand() % 100)+1);
	if (appo <= 20)
	{
		//determino il grado di durezza da utilizzare
		appo = (int)((rand() % 5)+1);
		sq->dur = appo;
		int set=0;
		if (sq->pdi >= sq->pce)
		{
			appomax = sq->pdi;
			set = 1;
		}
		else
		{
			appomax = sq->pce;
			set = 2;
		}
		if (appomax <= sq->pat)
		{
			appomax = sq->pat;
			set = 3;
		}
		if (set == 1)
		{
			sq->pce += appo;
			printf ("%s DUR +%dCE\n",get_nome(), appo);
		}
		if (set == 2)
		{
			sq->pdi += appo;
			printf ("%s DUR +%dDI\n",get_nome(), appo);
		}
		if (set == 3)
		{
			sq->pce += appo;
			printf ("%s DUR +%dCE\n",get_nome(), appo);
		}
	}
	else 
		sq->dur = 0;
	//eventuale catenaccio (attenzione modifica la tattica)
}

void partita::assegna_gol(squadra *sq)
{
int appon = 0, n = 0,conta = 0;
char appo[2];	

	appon = (int)((rand() % 100)+1);
	if (appon <= 50)
	{
		//gol di un attaccante

	    appo[0] = sq->tat[2];
	    appo[1] = '\0';
	    n = atoi(appo);
		appon = (int)((rand() % n)+1);
		for (int i=0;i<sq->ngr;i++)
			if (sq->giocatori[i]->ruolo == 'A')
			{
				conta++;
				if (conta == appon)
				{
					printf( "Gol di %s\n" , sq->giocatori[i]->get_nome());
					sq->giocatori[i]->reti++;
					return;
				}
			}
	}
	else
	{
		if (appon <= 80)
		{
			//gol di un centrocampista
			appo[0] = sq->tat[1];
			appo[1] = '\0';
			n = atoi(appo);
			appon = (int)((rand() % n)+1);
			for (int i=0;i<sq->ngr;i++)
				if (sq->giocatori[i]->ruolo == 'C')
				{
					conta++;
					if (conta == appon)
					{
						printf( "Gol di %s\n" , sq->giocatori[i]->get_nome());
						sq->giocatori[i]->reti++;
						return;
					}
				}
		}
		else
		{
			//gol di un difensore o del libero
			if (sq->lib == 'S')
			{
				for (int i=0;i<sq->ngr;i++)
					if (sq->giocatori[i]->ruolo == 'L')
					{
						printf( "Gol di %s\n" , sq->giocatori[i]->get_nome());
						sq->giocatori[i]->reti++;
						return;
					}
			}
			else
			{
				//gol di un difensore
				appo[0] = sq->tat[0];
				appo[1] = '\0';
				n = atoi(appo);
				appon = (int)((rand() % n)+1);
				for (int i=0;i<sq->ngr;i++)
					if (sq->giocatori[i]->ruolo == 'D')
					{
						conta++;
						if (conta == appon)
						{
							printf( "Gol di %s\n" , sq->giocatori[i]->get_nome());
							sq->giocatori[i]->reti++;
							return;
						}
					}
			}
		}
	}
}

void partita::determina_infortuni(squadra *sq1,squadra *sq2, int fl)
{
	int appo = 0;

	for (int i=0;i<sq1->ngr;i++)
	{
		appo = (int)((rand() % 100)+1);
		if (appo < (5 + (5 * sq2->dur)))
		{
			//infortunio
			appo = (int)(rand() % 5);
			sq1->giocatori[i]->forma -= appo;
			printf( "Infortunato : %20s nuova forma : %d\n" , sq1->giocatori[i]->get_nome(), 
				    sq1->giocatori[i]->forma , "\n");
			if (appo < 3)
			{
				if (fl == 1)
				{
					if (tiri1 > 0)
						tiri1 -= 2;
				}
				else
				{
					if (tiri2 > 0)
						tiri2 -= 2;
				}
			}
			else
			{
				if (fl == 1)
				{
					if (tiri1 > 0)
						tiri1 -= 3;
				}
				else
				{
					if (tiri2 > 0)
						tiri2 -= 3;
				}
			}
		}
	}
}

void partita::determina_sanzioni(squadra *sq1,int fl)
{
	int appo = 0;

	for (int i=0;i<sq1->ngr;i++)
	{
		appo = (int)((rand() % 100)+1);
		if (appo < (10 + (5 * sq1->dur)))
		{
			//ammonizione
			sq1->giocatori[i]->pd += 4;
			printf( "Ammonito : %20s punti disciplina : %d\n" , sq1->giocatori[i]->get_nome() ,
				    sq1->giocatori[i]->pd);
		}
		appo = (int)((rand() % 100)+1);
		if (appo < (5 + (3 * sq1->dur)))
		{
			//espulsione
			sq1->giocatori[i]->pd += 8;
			printf( "Espulso  : %20s punti disciplina : %d \n", sq1->giocatori[i]->get_nome(), 
				    sq1->giocatori[i]->pd);
			if (fl == 1)
				tiri1 -= 3;
			else
				tiri2 -= 3;
		}
	}
}

void campionato::determina_calendario_coppa(int turno)
{
	int h = 0, j = 0, i = 0;
	int prima = 0;
	num_partite_coppa = 0;
	//andata
	for (j=0; j<num_squadre; j++)
	{
		if (squadre[j]->tcl == turno && prima == 0)
		{
			calendario_coppa[h][0] = j;
			prima = 1;
		}
		else
		{
			if (squadre[j]->tcl == turno && prima == 1)
			{
				calendario_coppa[h][1] = j;
				num_partite_coppa++;
				h++;
				prima = 0;
			}
		}
	}
	//randomizzazione del calendario
	for (i=0;(i<1000);i++)
	{
		int appo1=0,appo2=0;
		int apposwap1=0,apposwap2=0;
		appo1 = (int)((rand() % (num_partite_coppa)));
		appo2 = (int)((rand() % (num_partite_coppa)));

		apposwap1 = calendario_coppa[appo1][0];
		apposwap2 = calendario_coppa[appo1][1];

		calendario_coppa[appo1][0] = calendario_coppa[appo2][0];
		calendario_coppa[appo1][1] = calendario_coppa[appo2][1];

		calendario_coppa[appo2][0] = apposwap1;
		calendario_coppa[appo2][1] = apposwap2;
	}
	//ritorno
	for (i=num_partite_coppa;i<(2*num_partite_coppa);i++)
	{
		calendario_coppa[i][0] = calendario_coppa[i-num_partite_coppa][1];
		calendario_coppa[i][1] = calendario_coppa[i-num_partite_coppa][0];
	}
	num_partite_coppa = num_partite_coppa * 2;
}

void campionato::determina_turni_coppa(void)
{
	int i = 0;

	if (num_partite_coppa == 2)
		num_turni_coppa = 1;
	else
	{
		for (i=0;i<10;i++)
		{
			int appo3=0;
			appo3 = (int)pow( 2, i );
			if (appo3 == num_partite_coppa)
				num_turni_coppa = i;
		}
	}
}
