/**
 *    TP5 de Réseaux : Programmation d'une application client/serveur
 *    ---------------------------------------------------------------
 * 
 *		Application : Jeu du pendu 
 *    fichier : serveur.c				     
 *					      
 *		Auteurs :  Damien Wykland, Baptiste Bouvier et Ancelin Serre						      
 *		Date : 13 Avril 2018 
 *									      
 */

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fon.h" /* Primitives de la boite a outils */

#define SERVICE_DEFAUT "0"

void serveur_appli(char *service); /* programme serveur */

void startServGame(int sock_id); /* boucle de jeu */

char playRound(char* mot_temp, char* mot_juste,char lettre);

int victoire(char* mot_temp, int taille_mot);

/*---------- programme serveur ----------*/

/* Procedure principale */
int main(int argc, char *argv[])
{
  /* numero de service par defaut */
	char *service = SERVICE_DEFAUT; 

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

	/* service est le service (ou numero de port) auquel sera affecte ce serveur*/
	serveur_appli(service);
}

/**
 * Procedure correspondant au traitement du serveur l'application 
 * @param char* le service (numéro de port) utilisé
 * @return void
 */
void serveur_appli(char *service)
{
	char diff;

	/* Création de la socket d'écoute */
	struct sockaddr_in *p_adr_cli;
	adr_socket(service, NULL, SOCK_STREAM, &p_adr_cli);
	/* Socket d'écoute */
	int sock_id = h_socket(AF_INET, SOCK_STREAM);
	/* Instancie l'adresse locale dans le descripteur de socket */
	h_bind(sock_id,p_adr_cli);
	/* On met la socket en écoute */
	h_listen(sock_id,5);
  int pid;

  while(1) 
  {
    /* On accepte une connexion */
	  int sock_cli = h_accept(sock_id,p_adr_cli);

    pid = fork();

    if(pid == 0) 
    {
      /* Boucle de jeu */
      startServGame(sock_cli);
      /* Fermeture du socket */
      h_close(sock_cli);
    }
  }

  /* Fermeture du socket d'écoute */
  h_close(sock_id);
}


/**
 * Boucle de jeu coté serveur
 * @param int le descripteur d'une socket utilisée par le serveur
 * pour l'échange de données avec le processus client
 * @return void
 */
void startServGame(int sock_cli) 
{
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
  
  /* Génerer un mot aléatoirement et envoyer taille au client */
  srand(time(NULL));
  int random = (rand() % 5);
  mot_juste = mots[random];
  taille_mot = strlen(mot_juste);
  for(int i = 0; i<taille_mot; i++)
    mot_temp[i] = '*';
  
  buffer = taille_mot + '0';
  h_writes(sock_cli, &buffer,1);

  /* Boucle de jeu */
  while(!gagne && vie != '0') 
  {
    h_writes(sock_cli, mot_temp,100);
    h_reads(sock_cli, &lettre, 1);

    etat = playRound(mot_temp, mot_juste, lettre);
    if(etat == '0')
      vie--;

    h_writes(sock_cli, &etat, 1);
    h_writes(sock_cli, &vie, 1);
    gagne = victoire(mot_temp, taille_mot);
    buffer = gagne + '0';
    h_writes(sock_cli, &buffer,1);
  }

  free(mot_temp);
}

/**
 * Joue un tour de la partie
 * @param char* mot_temp, le bout de mot vu par le joueur
 * @param char* mot_juste, le mot à trouver
 * @param char* lettre, la lettre donnée lors de ce tour par le joueur
 * @return char variable d'état désignant si une lettre a été dévoilée ou non
 */
char playRound(char* mot_temp, char* mot_juste,char lettre) 
{
  int win = 0;

  /* On cherche la lettre */
  for(int i=0; mot_juste[i] != '\0'; i++) 
  {
    /* Si elle est présente, on la rend visible */
    if(lettre == mot_juste[i] && mot_temp[i] == '*') {
      win = 1;
      mot_temp[i] = lettre;
    }
  }
  if(win)
    return '1';
  else
    return '0';
}

/**
 * Détermine si la partie est gagnée ou non
 * @param char* le mot vu par le joueur
 * @param char* la longueur du mot
 * @return int vrai ou faux 
 */
int victoire(char* mot_temp, int taille_mot) 
{
  int win = 1;
  /* On regarde chaque caractère de mot_temp */
  for(int i = 0; i<taille_mot; i++)
  {
    /* Il y a une * dans le mot, la partie n'est pas gagnée */
    if(mot_temp[i] == '*')
      win = 0;
  }
  return win;
}


