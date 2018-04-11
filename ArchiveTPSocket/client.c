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
#include<stdlib.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "16010"
#define SERVEUR_DEFAUT "200.0.0.1"

void client_appli (char *serveur, char *service);


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

	char level;
	// Faire un h_read 
	printf("Entrez la difficulté : ");
	scanf("%c",&level);
	if(level == 0)
	{
		// Faire un h_read 
		printf("Caractère incorrect !\n");
		exit(0);
	}

	// On envoi la réponse au serveur
	h_writes(sock_id,&level,1);



 }

/*****************************************************************************/

