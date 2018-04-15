/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "servGame.c"

#include "fon.h" /* Primitives de la boite a outils */

#define SERVICE_DEFAUT "0"

void serveur_appli(char *service); /* programme serveur */

void startServGame(int sock_id); /* boucle de jeu */

char playRound(char* mot_temp, char* mot_juste,char lettre);

int victoire(char* mot_temp, int taille_mot);

/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc, char *argv[])
{

	char *service = SERVICE_DEFAUT; /* numero de service par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
	{
		case 1:
			printf("defaut service = %s\n", service);
			break;
		case 2:
			service = argv[1];
			break;

		default:
			printf("Usage:serveur service (nom ou port) \n");
			exit(1);
	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/

	serveur_appli(service);
}

/******************************************************************************/
void serveur_appli(char *service)

/* Procedure correspondant au traitemnt du serveur de votre application */

{
	char diff;

	/*création de la socket d'écoute*/
	struct sockaddr_in *p_adr_cli;
	adr_socket(service, NULL, SOCK_STREAM, &p_adr_cli);
	/* socket d'écoute */
	int sock_id = h_socket(AF_INET, SOCK_STREAM);
	/*instancie l'adresse locale dans le descripteur de socket*/
	h_bind(sock_id,p_adr_cli);
	/*On met la socket en écoute*/
	h_listen(sock_id,5);
  int pid;

  while(1) {
    /*On accepte une connexion*/
	  int sock_cli = h_accept(sock_id,p_adr_cli);

    pid = fork();

    if(pid == 0) {
      // boucle jeu
      startServGame(sock_cli);
      //fermer socket
      h_close(sock_cli);
    }
  }

  // fermer sockets
  h_close(sock_id);

}


/**
 * Boucle de jeu coté serveur
 */
void startServGame(int sock_cli) {
  /* Variables */
  char vie;
  char* mots[5] = {"TABLE","CHAISE","CAHIER","CASQUE","SERVIETTE"};  
  char* mot_juste;
  char* mot_temp = malloc(100);
  int taille_mot;
  char etat = '3';
  int gagne = 0;
  char lettre;
  char buffer;

  /* Lecture difficulté */
  h_reads(sock_cli, &vie, 1);
  
  /* Génerer mot et envoyer taille*/
  srand(time(NULL));
  int random = (rand() % 5);
  mot_juste = mots[random];
  taille_mot = strlen(mot_juste);
  for(int i = 0; i<taille_mot; i++) {
    mot_temp[i] = '*';
  }
  
  buffer = taille_mot + '0';
  h_writes(sock_cli, &buffer,1);

  /* Boucle de jeu */
  while(!gagne && vie != '0') {
    h_writes(sock_cli, mot_temp,100);
    h_reads(sock_cli, &lettre, 1);

    etat = playRound(mot_temp, mot_juste, lettre);
    if(etat == '0') {
      vie--;
    }

    h_writes(sock_cli, &etat, 1);
    h_writes(sock_cli, &vie, 1);
    gagne = victoire(mot_temp, taille_mot);
    buffer = gagne + '0';
    h_writes(sock_cli, &buffer,1);
  }

}

char playRound(char* mot_temp, char* mot_juste,char lettre) {
  int win = 0;

  for(int i=0; mot_juste[i] != '\0'; i++) {
    if(lettre == mot_juste[i] && mot_temp[i] == '*') {
      win = 1;
      mot_temp[i] = lettre;
    }
  }
  if(win) {
    return '1';
  }else {
    return '0';
  }

}

int victoire(char* mot_temp, int taille_mot) {
  int win = 1;
  for(int i = 0; i<taille_mot; i++) {
    if(mot_temp[i] == '*') {
      win = 0;
    }
  }
  return win;
}


