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

#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fon.h" /* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"

void serveur_appli(char *service); /* programme serveur */

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
	/*On accepte une connexion*/
	int sock_cli = h_accept(sock_id,p_adr_cli);
	h_reads(sock_cli,diff,1);
	printf("%c ",diff);
}
