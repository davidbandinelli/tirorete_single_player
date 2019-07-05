// TR personal trainer 
// Author D. Bandinelli
// (thanks to F. Filatrella)
// Created in 1999, rebooted in 2015
// TODO: differenza reti (OK)
// TODO: correggere vincitore coppa europa fine stagione (DA PROVARE)

#include "tr.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

/* Standard error macro for reporting API errors */
#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s \
        on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

int leggi_squadre(void);
void gioca_coppa(int, int);
void gestione_squadra(int, char, int);
//void tasto(void);
campionato *camp=NULL;
int flg_fc = 0;
int flg_dur = 0;

int main()
{
	/* DEBUG MODULO */
	/*
	int a = 12;
	int b = 9;
	int c = a % b;
	*/
	/* DEBUG MODULO */

   int esito = 0;
   int i = 0; 
   camp = new campionato;
   camp->initialize(0);
   srand( (unsigned)time( NULL ) );

   ofstream outfile;
   outfile.open("tr.log", ios::app); //apro il log in append
   if (!outfile) 
   {
      printf( "Output file cannot be opened.\n");
      return 1;
   }

   //lettura squadre dal file dati.tr
   esito = leggi_squadre();

   //determinazione calendario in base al numero di squadre lette
   camp->determina_calendario();
   tasto();
   cls();
   camp->display(2);
   tasto();
   cls();
   // scelta squadra controllata dal giocatore umano
   camp->display(8);
   printf ("\nScegli una squadra (0 - %d):", (camp->num_squadre - 1));
   scanf ("%d",&camp->squadra_scelta);
   camp->squadre[camp->squadra_scelta]->flg_ai = 1;
   cls();

   for (i=0;i<camp->num_squadre;i++)
   {
/*      
      camp->squadre[i]->display(1);
	  tasto();
	  cls();
      camp->squadre[i]->display(2);
      tasto();
	  cls();
*/
      esito = camp->squadre[i]->determina_valori_area();
      camp->squadre[i]->display(3);
      tasto();
	  cls();
   }

	//CAMPIONATO
   for (i = 0; i < camp->num_partite; i++)
   {
	   if (i == (camp->num_partite / 2))
		      gioca_coppa(16,1);   //coppa europa

	   partita *part;
	   part = new partita;
	   part->initialize(camp->squadre[camp->calendario[i][0]], camp->squadre[camp->calendario[i][1]]);

	   //calcola valori base di area
       tasto();
       cls();
	   printf ("\n---INFORTUNI/SQUALIFICHE---\n\n");
       esito = camp->squadre[camp->calendario[i][0]]->determina_valori_area();
       esito = camp->squadre[camp->calendario[i][1]]->determina_valori_area();
       tasto();
       cls();
	   printf ("\n---VALORI AREA BASE---------\n\n");
       camp->squadre[camp->calendario[i][0]]->display(3);
       camp->squadre[camp->calendario[i][1]]->display(3);

	   //se una delle due squadre è controllata da un giocatore umano richiama la
	   //apposita routine di gestione
	   flg_fc = 0; //posso assegnare il fc
	   flg_dur = 0; //posso assegnare la durezza
	   if (camp->squadre[camp->calendario[i][0]]->flg_ai == 1)
		   gestione_squadra(camp->calendario[i][0], 'h',camp->calendario[i][1]); //home
	   if (camp->squadre[camp->calendario[i][1]]->flg_ai == 1)
		   gestione_squadra(camp->calendario[i][1], 'a',camp->calendario[i][0]); //away

	   //squadra di casa se gestita dal computer
	   if (camp->squadre[camp->calendario[i][0]]->flg_ai == 0)
	   {
			printf ("\n---VALORI AREA MODIFICATI---\n\n");
			camp->squadre[camp->calendario[i][0]]->assegna_bonus(camp->squadre[camp->calendario[i][0]],0);
	   }
	   //squadra ospite se gestita dal computer
	   if (camp->squadre[camp->calendario[i][1]]->flg_ai == 0)
	   {
			printf ("\n---VALORI AREA MODIFICATI---\n\n");
			camp->squadre[camp->calendario[i][1]]->assegna_bonus(camp->squadre[camp->calendario[i][1]],1);
	   }

       camp->squadre[camp->calendario[i][0]]->display(3);
       camp->squadre[camp->calendario[i][1]]->display(3);
       tasto();
       cls();

	   printf ("\n---TIRI EFFETTUATI----------\n\n");
	   esito = part->controlla_triplo();
	   part->calcola_tiri();
	   tasto();
	   cls();
	   printf ("\n---INFORTUNI----------------\n\n");
	   part->determina_infortuni(camp->squadre[camp->calendario[i][0]], camp->squadre[camp->calendario[i][1]],1);
	   part->determina_infortuni(camp->squadre[camp->calendario[i][1]], camp->squadre[camp->calendario[i][0]],2);
	   printf ("\n---AMMONIZIONI/ESPULSIONI---\n\n");
	   part->determina_sanzioni(camp->squadre[camp->calendario[i][0]],1);
	   part->determina_sanzioni(camp->squadre[camp->calendario[i][1]],2);
	   tasto();
	   cls();
	   printf ("\n---PARTITA %3d/%3d----------\n\n", camp->progr_partita + 1, camp->num_partite);
	   part->gioca_partita(0);
	   tasto();
	   cls();

	   //aggiornamento storia risultati
	   strcpy(camp->storia[camp->progr_partita], camp->squadre[camp->calendario[i][0]]->get_nome());
	   strcat(camp->storia[camp->progr_partita]," - ");
	   strcat(camp->storia[camp->progr_partita], camp->squadre[camp->calendario[i][1]]->get_nome());
	   strcat(camp->storia[camp->progr_partita],"  ");
	   char appo[10];
	   sprintf(appo,"%d - ",part->res1);
	   strcat(camp->storia[camp->progr_partita],appo);
	   sprintf(appo,"%d",part->res2);
	   strcat(camp->storia[camp->progr_partita],appo);
	   camp->progr_partita ++;

	   printf ("\n---RISULTATI CAMPIONATO--\n\n");
	   camp->display(3);
	   tasto();
	   cls();
	   camp->determina_classifica();
	   camp->determina_classifica_marcatori();
	   if (camp->progr_partita % camp->partite_per_giornata == 0) {
		   printf("\n---CLASSIFICA CAMPIONATO :%3d/%3d GIORNATA %d\n\n", camp->progr_partita, camp->num_partite, camp->num_giornata - 1);
		   camp->display(4);
		   tasto();
		   cls();
		   printf("\n---CLASSIFICA MARCATORI--\n\n");
		   camp->display(5);
		   tasto();
		   cls();
	   }
   }
   printf ("\n---CLASSIFICA FINALE CAMPIONATO :%3d/%3d\n\n",camp->progr_partita,camp->num_partite);
   camp->display(4);
   tasto();
   cls();
   printf ("\n---CLASSIFICA FINALE MARCATORI--\n\n");
   camp->display(5);
   tasto();
   cls();
   gioca_coppa(8,0);
	printf ("\n---SOMMARIO RISULTATI STAGIONE--\n\n");
	printf ("Campionato vinto da: %s \n",camp->squadre[camp->classifica[0][1]]->get_nome());
	int cl = 0;
	for (i = 0; i < camp->num_squadre; i++)
	{
		if (camp->squadre[i]->tcl == 99 || camp->squadre[i]->tcl == 100)
		{
			cl = i;
		}
	}
	printf ("Coppa di lega vinta da: %s \n",camp->squadre[cl]->get_nome());

	int ce = 0;
	for (i = 0; i < camp->num_squadre; i++)
	{
		if (camp->squadre[i]->tcl == 98 || camp->squadre[i]->tcl == 100)
		{
			ce = i;
		}
	}
	printf ("Coppa Europa vinta da: %s \n",camp->squadre[ce]->get_nome());
	
	printf ("Squadre retrocesse %s %s %s %s \n", camp->squadre[camp->classifica[17][1]]->get_nome(),
												 camp->squadre[camp->classifica[16][1]]->get_nome(),
												 camp->squadre[camp->classifica[15][1]]->get_nome(),
												 camp->squadre[camp->classifica[14][1]]->get_nome());
	printf ("Capocannoniere campionato e coppa %20s (%s)\n",camp->get_nome_id(camp->classifica_marcatori[0][1]), camp->get_squadra_id(camp->classifica_marcatori[0][1]));
    
	//aggiornamento log storia
	char appo_log[60];
	outfile.write("RISULTATI STAGIONE\n", 19);
	sprintf (appo_log,"Campionato vinto da: %s \n",camp->squadre[camp->classifica[0][1]]->get_nome());
	outfile.write(appo_log,29);
	sprintf (appo_log,"Coppa di lega vinta da: %s \n",camp->squadre[cl]->get_nome());
	outfile.write(appo_log,29);
	sprintf (appo_log,"Coppa Europa vinta da: %s \n",camp->squadre[ce]->get_nome());
	outfile.write(appo_log,29);
/*	sprintf (appo_log, "Squadre retrocesse   : %s %s %s %s \n", 
			camp->squadre[camp->classifica[17][1]]->get_nome(),
			camp->squadre[camp->classifica[16][1]]->get_nome(),
			camp->squadre[camp->classifica[15][1]]->get_nome(),
			camp->squadre[camp->classifica[14][1]]->get_nome());
	outfile.write(appo_log,40); */
	sprintf (appo_log, "Capocannoniere stagione: %20s (%s)\n",
			camp->get_nome_id(camp->classifica_marcatori[0][1]),
			camp->get_squadra_id(camp->classifica_marcatori[0][1]));
	outfile.write(appo_log,53);
	outfile.close();

   tasto();
   cls();
   return 0;
}

int leggi_squadre(void)
{
   int appo_id_giocatore = 0, num_squadre=0, num_giocatori=0, num_non_giocatori=0;
   ifstream infile;
   char appo[50];

   infile.open("dati.tr", std::ifstream::in);
   if (!infile) 
   {
      printf( "Input file cannot be opened.\n");
      return 1;
   }

   while (!infile.eof())
   {
      infile.getline(appo ,50,'\n');
      if (!strcmp(appo,"SQUADRA"))
      {
		  //trovata nuova squadra
          camp->squadre[num_squadre] = new squadra;
		  camp->squadre[num_squadre]->initialize("XXX",0,0,0,0,"000",'N','N',num_squadre);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->set_nome(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->pa = atoi(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->ps = atoi(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->pgp = atoi(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->ml = atoi(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->set_tat(appo);
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->lib = (char)appo[0];
          infile.getline(appo ,50,'\n');
		  camp->squadre[num_squadre]->fuo = (char)appo[0];
          infile.getline(appo ,50,'\n');
          if (strcmp(appo,"FINE-SQUADRA"))
		  {
             printf( "Errore di formato nel file dati (SQUADRA).\n");
             return 1;
		  }
      }
      infile.getline(appo ,50,'\n');
      if (!strcmp(appo,"GIOCATORI"))
      {
		  //trovato nuovo giocatore
          while (strcmp(appo,"FINE-GIOCATORI"))
          {
             infile.getline(appo ,50,'\n');
			 if (!strcmp(appo,"FINE-GIOCATORI"))
				 break;
			 else
                 camp->squadre[num_squadre]->giocatori[num_giocatori] = new giocatore;
             
			 camp->squadre[num_squadre]->giocatori[num_giocatori]->initialize(appo_id_giocatore,
				                                                              "XXX",0,0,0,'X','N');
			 appo_id_giocatore++;
			 camp->squadre[num_squadre]->giocatori[num_giocatori]->set_nome(appo);
             infile.getline(appo ,50,'\n');
			 camp->squadre[num_squadre]->giocatori[num_giocatori]->livello = atoi(appo);
             infile.getline(appo ,50,'\n');
			 camp->squadre[num_squadre]->giocatori[num_giocatori]->forma = atoi(appo);
             infile.getline(appo ,50,'\n');
			 camp->squadre[num_squadre]->giocatori[num_giocatori]->eta = atoi(appo);
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->giocatori[num_giocatori]->ruolo = (char)appo[0];
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->giocatori[num_giocatori]->speciale = (char)appo[0];
        	 num_giocatori++;
             //printf( "Read:" << appo << "\n";
		  }
      }
      infile.getline(appo ,50,'\n');
      if (!strcmp(appo,"NON-GIOCATORI"))
      {
		  //trovato nuovo giocatore
          while (strcmp(appo,"FINE-NON-GIOCATORI"))
          {
             infile.getline(appo ,50,'\n');
			 if (!strcmp(appo,"FINE-NON-GIOCATORI"))
				 break;
			 else
                camp->squadre[num_squadre]->non_giocatori[num_non_giocatori] = new non_giocatore;
			 
			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->initialize("XXX",0,'X','N',"XXX",'N','N',0);
			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->set_nome(appo);
             infile.getline(appo ,50,'\n');
			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->eta = atoi(appo);
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->ruolo = (char)appo[0];
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->speciale = (char)appo[0];
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->set_tat(appo);
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->lib = (char)appo[0];
             infile.getline(appo ,50,'\n');
 			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->fuo = (char)appo[0];
             infile.getline(appo ,50,'\n');
			 camp->squadre[num_squadre]->non_giocatori[num_non_giocatori]->bonus = atoi(appo);
        	 num_non_giocatori++;
		  }
      }
	  camp->squadre[num_squadre]->ngr = num_giocatori;
	  camp->squadre[num_squadre]->nng = num_non_giocatori;
	  num_giocatori = 0;
	  num_non_giocatori = 0;
      num_squadre++;
   }
   infile.close();
   camp->set_num_squadre(num_squadre);
   return 0;
}

void tasto(void)
{
	_getch();
}

void cls( void )
{
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */    
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in the current buffer */
    HANDLE hConsole;

    hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	/* get the number of character cells in the current buffer */
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	PERR( bSuccess, "GetConsoleScreenBufferInfo" );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	/* fill the entire screen with blanks */
	bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
	PERR( bSuccess, "FillConsoleOutputCharacter" );
	/* get the current text attribute */
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	PERR( bSuccess, "ConsoleScreenBufferInfo" );
	/* now set the buffer's attributes accordingly */
	bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
	PERR( bSuccess, "FillConsoleOutputAttribute" );
	/* put the cursor at (0, 0) */
	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	PERR( bSuccess, "SetConsoleCursorPosition" );  
	return;
}

void gioca_coppa(int num_squadre, int tipo_coppa)
{
	int i=0, esito = 0;

	camp->num_squadre_coppa = num_squadre;
	for (i=0; i<num_squadre; i++)
	{
		camp->squadre[camp->classifica[i][1]]->tcl = 0;
	}
   camp->determina_calendario_coppa(0);
   camp->determina_turni_coppa();
   if (tipo_coppa == 0)
		printf ("\n---CALENDARIO PRIMO TURNO COPPA LEGA---\n\n");
   if (tipo_coppa == 1)
		printf ("\n---CALENDARIO PRIMO TURNO COPPA EUROPA---\n\n");
   camp->display(6);
   tasto();
   cls();

   for (i=0;i<camp->num_turni_coppa;i++)
   {
	    int ritorno = 0;
		camp->progr_partita_coppa = 0;
		for (int h=0;h<camp->num_partite_coppa;h++)
		{
			if (h >= (camp->num_partite_coppa / 2))
				ritorno = 1;

			partita *part;
			part = new partita;
			part->initialize(camp->squadre[camp->calendario_coppa[h][0]], camp->squadre[camp->calendario_coppa[h][1]]);
			//calcola valori base di area
			esito = camp->squadre[camp->calendario_coppa[h][0]]->determina_valori_area();
			esito = camp->squadre[camp->calendario_coppa[h][1]]->determina_valori_area();
			printf ("\n---VALORI AREA BASE---------\n\n");
			camp->squadre[camp->calendario_coppa[h][0]]->display(3);
			camp->squadre[camp->calendario_coppa[h][1]]->display(3);
			tasto();
		    cls();
			//squadra di casa
			printf ("\n---VALORI AREA MODIFICATI---\n\n");
			camp->squadre[camp->calendario_coppa[h][0]]->assegna_bonus(camp->squadre[camp->calendario_coppa[h][0]],0);
			//squadra ospite
			camp->squadre[camp->calendario_coppa[h][1]]->assegna_bonus(camp->squadre[camp->calendario_coppa[h][1]],1);

			camp->squadre[camp->calendario_coppa[h][0]]->display(3);
			camp->squadre[camp->calendario_coppa[h][1]]->display(3);
			tasto();
			cls();
			printf ("\n---TIRI EFFETTUATI----------\n\n");
			esito = part->controlla_triplo();
			part->calcola_tiri();
			tasto();
			cls();
			printf ("\n---INFORTUNI----------------\n\n");
			part->determina_infortuni(camp->squadre[camp->calendario_coppa[h][0]], camp->squadre[camp->calendario_coppa[h][1]],1);
			part->determina_infortuni(camp->squadre[camp->calendario_coppa[h][1]], camp->squadre[camp->calendario_coppa[h][0]],2);
			printf ("\n---AMMONIZIONI/ESPULSIONI---\n\n");
			part->determina_sanzioni(camp->squadre[camp->calendario_coppa[h][0]],1);
			part->determina_sanzioni(camp->squadre[camp->calendario_coppa[h][1]],2);
			tasto();
			cls();
			if (tipo_coppa == 0)
				printf ("\n---PARTITA COPPA LEGA--------\n\n");
			if (tipo_coppa == 1)
				printf ("\n---PARTITA COPPA EUROPA------\n\n");
			part->gioca_partita(1);
			tasto();
			cls();

		   //aggiornamento storia risultati coppa
			strcpy(camp->storia_coppa[camp->progr_partita_coppa], camp->squadre[camp->calendario_coppa[h][0]]->get_nome());
		   strcat(camp->storia_coppa[camp->progr_partita_coppa]," - ");
		   strcat(camp->storia_coppa[camp->progr_partita_coppa], camp->squadre[camp->calendario_coppa[h][1]]->get_nome());
		   strcat(camp->storia_coppa[camp->progr_partita_coppa],"  ");
		   char appo[50];
		   sprintf(appo,"%d - ",part->res1);
		   strcat(camp->storia_coppa[camp->progr_partita_coppa],appo);
		   sprintf(appo,"%d",part->res2);
		   strcat(camp->storia_coppa[camp->progr_partita_coppa],appo);
			sprintf (appo, " AGG. %d - %d", camp->squadre[camp->calendario_coppa[h][0]]->agghf +
									        camp->squadre[camp->calendario_coppa[h][0]]->aggaf,
											camp->squadre[camp->calendario_coppa[h][1]]->agghf +
											camp->squadre[camp->calendario_coppa[h][1]]->aggas);
		   strcat(camp->storia_coppa[camp->progr_partita_coppa],appo);

		   camp->progr_partita_coppa ++;

			if (tipo_coppa == 0)
				printf ("\n---RISULTATI COPPA LEGA------\n\n");
			if (tipo_coppa == 1)
				printf ("\n---RISULTATI COPPA EUROPA----\n\n");
		   camp->display(7);
		   // determinazione passaggio turno
		   if (ritorno == 1)
		   {
				if ((camp->squadre[camp->calendario_coppa[h][0]]->agghf +
					camp->squadre[camp->calendario_coppa[h][0]]->aggaf) >
					(camp->squadre[camp->calendario_coppa[h][1]]->agghf +
					camp->squadre[camp->calendario_coppa[h][1]]->aggas))
				{
					printf ("Passa il turno %s \n",camp->squadre[camp->calendario_coppa[h][0]]->get_nome());
					camp->squadre[camp->calendario_coppa[h][0]]->tcl++;
				}
				else
				{
					if ((camp->squadre[camp->calendario_coppa[h][1]]->agghf +
						camp->squadre[camp->calendario_coppa[h][1]]->aggaf) >
						(camp->squadre[camp->calendario_coppa[h][0]]->agghf +
						camp->squadre[camp->calendario_coppa[h][0]]->aggas))
					{
						printf ("Passa il turno %s \n",camp->squadre[camp->calendario_coppa[h][1]]->get_nome());
						camp->squadre[camp->calendario_coppa[h][1]]->tcl++;
					}
					else
					{
						//si contano i gol in trasferta
						if (camp->squadre[camp->calendario_coppa[h][0]]->aggaf >
							camp->squadre[camp->calendario_coppa[h][1]]->aggaf )
						{
							printf ("Passa il turno %s \n",camp->squadre[camp->calendario_coppa[h][0]]->get_nome());
							camp->squadre[camp->calendario_coppa[h][0]]->tcl++;
						}
						else
						{
							if (camp->squadre[camp->calendario_coppa[h][1]]->aggaf >
								camp->squadre[camp->calendario_coppa[h][0]]->aggaf )
							{
								printf ("Passa il turno %s \n",camp->squadre[camp->calendario_coppa[h][1]]->get_nome());
								camp->squadre[camp->calendario_coppa[h][1]]->tcl++;
							}
							else
							{
								printf ("Tempi supplementari \n");
								printf ("Passa il turno %s \n",camp->squadre[camp->calendario_coppa[h][1]]->get_nome());
								camp->squadre[camp->calendario_coppa[h][1]]->tcl++;
							}
						}
					}
				}
				camp->squadre[camp->calendario_coppa[h][0]]->agghf = 0;
				camp->squadre[camp->calendario_coppa[h][0]]->aggaf = 0;
				camp->squadre[camp->calendario_coppa[h][1]]->agghf = 0;
				camp->squadre[camp->calendario_coppa[h][1]]->aggaf = 0;
				camp->squadre[camp->calendario_coppa[h][0]]->agghs = 0;
				camp->squadre[camp->calendario_coppa[h][0]]->aggas = 0;
				camp->squadre[camp->calendario_coppa[h][1]]->agghs = 0;
				camp->squadre[camp->calendario_coppa[h][1]]->aggas = 0;
		   }
		   tasto();
		   cls();
		   printf ("\n---CLASSIFICA MARCATORI (CAMP. + COPPA)--\n\n");
		   camp->determina_classifica_marcatori();
		   camp->display(5);
		   tasto();
		   cls();
		}
	   //determinazione calendario turno successivo
	   if (i + 1 < camp->num_turni_coppa)
	   {
		   camp->determina_calendario_coppa(i + 1);
			if (tipo_coppa == 0)
			   printf ("\n---CALENDARIO %d TURNO COPPA LEGA---\n\n",i + 2);
			if (tipo_coppa == 1)
			   printf ("\n---CALENDARIO %d TURNO COPPA EUROPA---\n\n",i + 2);
		   camp->display(6);
		   tasto();
		   cls();
	   }
	   else
	   {
		   //una squadra ha vinto la coppa
		   for (int uu = 0; uu < camp->num_squadre; uu++)
		   {
			   if (camp->squadre[uu]->tcl == camp->num_turni_coppa)
			   {
					if (tipo_coppa == 0)
					{
						printf ("\n--- %s ha vinto la coppa di Lega !!! ---\n", camp->squadre[uu]->get_nome());
						camp->squadre[uu]->ml += 500;
						camp->squadre[uu]->tcl = 99;
					}
					if (tipo_coppa == 1 && camp->squadre[uu]->tcl != 99)
					{
						printf ("\n--- %s ha vinto la coppa Europa !!! ---\n", camp->squadre[uu]->get_nome());
						camp->squadre[uu]->tcl = 98;
						camp->squadre[uu]->ml += 800;
					}
					// vinto entrambe le coppe
					if (tipo_coppa == 1 && camp->squadre[uu]->tcl == 99)
					{
						printf ("\n--- %s ha vinto la coppa Europa !!! ---\n", camp->squadre[uu]->get_nome());
						camp->squadre[uu]->tcl = 100;
						camp->squadre[uu]->ml += 800;
					}
			   }
			   else {
				   if (camp->squadre[uu]->tcl != 99) {
					   camp->squadre[uu]->tcl = -1;
				   }
			   }
		   }
		   tasto();
		   cls();
	   }
	}
}

void gestione_squadra(int indice, char flg_casafuori, int avvers)
{
	int opzione = -99, id = 0, pa = 0;
	char sn;
	
	// impostazione punti durezza usabili
	camp->squadre[indice]->dur = 10;
	while (opzione != 0)
	{
		cls();
		if (flg_casafuori == 'h')
			printf("--- Gestione squadra (Partita in casa contro %s) ---\n\n",camp->squadre[avvers]->get_nome());
		if (flg_casafuori == 'a')
			printf("--- Gestione squadra (Partita fuori casa contro %s) ---\n\n",camp->squadre[avvers]->get_nome());

		camp->squadre[indice]->display(3);
		printf ("\n");
		camp->squadre[indice]->display(1);
		printf ("\n");
		camp->squadre[indice]->display(2);
		printf ("\n");
		printf ("(1): Ab. (2): Fo (3): FC (4): PGP (5): Dur (6): Tat (7): Form. (0 per finire)");
		scanf ("%d",&opzione);

		switch (opzione)
		{
			case 1: // allenamento abilita'
				cls();
				printf ("Id. giocatore da allenare per l'abilita' :");
				scanf ("%d",&id);
				
				printf ("\nGiocatore: %s per allenarlo da ab. %d a %d occorrono %d PA, ne hai %d PA\n",
					    camp->squadre[indice]->giocatori[id]->get_nome(),
						camp->squadre[indice]->giocatori[id]->livello,
						camp->squadre[indice]->giocatori[id]->livello + 1,
						camp->squadre[indice]->giocatori[id]->livello,
						camp->squadre[indice]->pa);
				printf ("Confermi s/n :");
				sn = _getch();
				//scanf ("%c",&sn);
				if (sn == 's' && camp->squadre[indice]->pa >= camp->squadre[indice]->giocatori[id]->livello)
				{
					camp->squadre[indice]->pa -= camp->squadre[indice]->giocatori[id]->livello;
					camp->squadre[indice]->giocatori[id]->livello++;
				}
				break;

			case 2: // allenamento forma
				cls();
				printf ("Id. giocatore da allenare per la forma :");
				scanf ("%d",&id);
				
				printf ("\nGiocatore: %s per allenarlo da FO %d a %d occorrono %d PA, ne hai %d PA\n",
					    camp->squadre[indice]->giocatori[id]->get_nome(),
						camp->squadre[indice]->giocatori[id]->forma,
						camp->squadre[indice]->giocatori[id]->forma + 1,
						camp->squadre[indice]->giocatori[id]->eta,
						camp->squadre[indice]->pa);
				printf ("Confermi s/n :");
				sn = _getch();
				//scanf ("%c",&sn);
				if (sn == 's' && camp->squadre[indice]->pa >= camp->squadre[indice]->giocatori[id]->eta &&
					camp->squadre[indice]->giocatori[id]->forma < 2 )
				{
					camp->squadre[indice]->pa -= camp->squadre[indice]->giocatori[id]->eta;
					camp->squadre[indice]->giocatori[id]->forma++;
				}
				break;

			case 3: //assegnazione fattore campo
				cls();
				if (flg_casafuori == 'h')
				{
					if (flg_fc == 0) //assegno il FC una volta sola
					{
						camp->squadre[indice]->fc = 7;
						flg_fc = 1;
					}
					int opz=-99;
					int app_as = 0;
					while (opz != 0 && camp->squadre[indice]->fc > 0)
					{
						printf ("Assegna FC, ora ne hai %d ? 1: DI, 2: CE, 3: AT (0 uscita)", 
							    camp->squadre[indice]->fc);
						scanf ("%d",&opz);
						if (opz != 0)
						{
							printf ("Quanti ne assegni ? (MAX 5)", app_as);
							scanf ("%d",&app_as);
						}
						switch (opz)
						{
							case 1:
								camp->squadre[indice]->pdi += app_as;
								camp->squadre[indice]->fc -= app_as;
								break;
							case 2:
								camp->squadre[indice]->pce += app_as;
								camp->squadre[indice]->fc -= app_as;
								break;
							case 3:
								camp->squadre[indice]->pat += app_as;
								camp->squadre[indice]->fc -= app_as;
								break;
						}
					}
				}
				break;

			case 4: //assegnazione PGP
			{
				int opz=-99;
				int app_as = 0;
				cls();
				while (opz != 0 && camp->squadre[indice]->pgp > 0)
				{
					printf ("Assegna PGP, ora ne hai %d ? 1: DI, 2: CE, 3: AT (0 uscita)", 
							camp->squadre[indice]->pgp);
					scanf ("%d",&opz);
					if (opz != 0 )
					{
						printf ("Quanti ne assegni ? (MAX 5)", app_as);
						scanf ("%d",&app_as);
					}
					switch (opz)
					{
						case 1:
							camp->squadre[indice]->pdi += app_as;
							camp->squadre[indice]->pgp -= app_as;
							break;
						case 2:
							camp->squadre[indice]->pce += app_as;
							camp->squadre[indice]->pgp -= app_as;
							break;
						case 3:
							camp->squadre[indice]->pat += app_as;
							camp->squadre[indice]->pgp -= app_as;
							break;
					}
				}
			}
			break;

			case 5: //assegnazione durezza
			{
				int opz=-99;
				int app_as = 0;
				cls();
				if (flg_dur == 0) //assegno la durezza una volta sola
				{
					flg_dur = 1;
				}
				while (opz != 0 && camp->squadre[indice]->dur > 0)
				{
					printf ("Assegna DUR, ora ne hai %d ? 1: DI, 2: CE, 3: AT (0 uscita)", 
							camp->squadre[indice]->dur);
					scanf ("%d",&opz);
					if (opz != 0)
					{
						printf ("Quanti ne assegni ? (MAX 5)", app_as);
						scanf ("%d",&app_as);
					}
					switch (opz)
					{
						case 1:
							camp->squadre[indice]->pdi += app_as;
							camp->squadre[indice]->dur -= app_as;
							break;
						case 2:
							camp->squadre[indice]->pce += app_as;
							camp->squadre[indice]->dur -= app_as;
							break;
						case 3:
							camp->squadre[indice]->pat += app_as;
							camp->squadre[indice]->dur -= app_as;
							break;
					}

				}
			}
			break;

			case 0:
			{
				//aggiustamento durezza al momento dell'uscita (durezza usata = 10 - durezza rimasta)
				camp->squadre[indice]->dur = 10 - camp->squadre[indice]->dur ;
			}
		}
	}
	cls();
}
