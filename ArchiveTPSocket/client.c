/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/	


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "16010"
#define SERVEUR_DEFAUT "200.0.0.1"

void client_appli (char *serveur, char *service);

void cliGame(int sock_id)
{
  /* On lit le premier message */
  char *buffer = malloc(100);
  h_reads(sock_id,buffer,100);
  /* Le serveur demande de rentrer la difficulté */
  printf("%s\n",buffer);

  /* On envoi la difficulté (<=> nombre de vies) */
  char vie;
  scanf("%c",&vie);
  while(!isdigit(vie) || vie == 0)
  {
    printf("Veuillez entrer un chiffre entre 0..9 : ");
    scanf("%c",&vie);
    printf("\n");
  }
  /* Message envoyé */
  h_writes(sock_id,&vie,1);
	/* char* win ou loose */
	h_reads(sock_id,buffer,1);
	char res = buffer[0];
	switch(res)
	{	
		case 1:
			printf("Et c'est gagné, mash'allah mon frère !\n");
			break;
		case 0:
			printf("Partie en cours, attention hein !\n");
			break;
		case -1:
			printf("Aie aie aie, la partie est terminée => GAME OVER\n");
			break;
	}

	/* Vie(s) restante(s) */
	h_reads(sock_id,&vie,1);
	printf("Il vous reste %c vie(s) ! \n");
	/* Mot temporaire */
	h_reads(sock_id,buffer,100);



  free(buffer);
}

/*****************************************************************************/
void client_appli (char *serveur,char *service)

/* procedure correspondant au traitement du client de votre application */

{
  
	/* On créé un socket distant */
	struct sockaddr_in *p_adr_serv; 
	/* On rempli les champs de la sock distante */
	adr_socket(service,serveur,SOCK_STREAM,&p_adr_serv);
	
	/* On crée la socket client et on stock son id */
	int sock_id = h_socket(AF_INET,SOCK_STREAM);
  /* On lance la connexion au serveur avec en param l'id du socket et les infos du serveur */
	h_connect(sock_id,p_adr_serv);

	cliGame(sock_id);
//h_writes(sock_id,"Bonjourno Damiano",100);

	/* On close la socket */
	h_close(sock_id);
 }

/*****************************************************************************/

/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  break;
  	case 3 :		/* serveur, service renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  break;
    default:
		  printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		  exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	
	client_appli(serveur,service);
}