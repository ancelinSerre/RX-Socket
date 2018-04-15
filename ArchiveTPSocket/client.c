/**
 *    TP5 de Réseaux : Programmation d'une application client/serveur
 *    ---------------------------------------------------------------
 * 
 *		Application : Jeu du pendu 
 *    fichier : client.c				     
 *					      
 *		Auteurs :  Damien Wykland, Baptiste Bouvier et Ancelin Serre						      
 *		Date : 13 Avril 2018 
 *									      
 */

#include <stdio.h>
#include <curses.h> /* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

#include "fon.h" /* primitives de la boite a outils */

#define SERVICE_DEFAUT "16010"
#define SERVEUR_DEFAUT "200.0.0.1"

void client_appli(char *serveur, char *service);

void startCliGame(int sock_id);

char lireChar();

/*---------- programme client ----------*/

/* Procedure principale */
int main(int argc, char *argv[])
{

  char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
  char *service = SERVICE_DEFAUT; /* numero de service par defaut (num de port) */

  /* Permet de passer un nombre de parametre variable a l'executable */
  switch (argc)
  {
  case 1: /* arguments par defaut */
    printf("serveur par defaut: %s\n", serveur);
    printf("service par defaut: %s\n", service);
    break;
  case 2: /* serveur renseigne  */
    serveur = argv[1];
    printf("service par defaut: %s\n", service);
    break;
  case 3: /* serveur, service renseignes */
    serveur = argv[1];
    service = argv[2];
    break;
  default:
    printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
    exit(1);
  }

  /* 
    serveur est le nom (ou l'adresse IP) auquel le client va acceder 
    service le numero de port sur le serveur correspondant au  
    service desire par le client 
  */
  client_appli(serveur, service);
}

/**
 * Procedure correspondant au traitement du client de l'application
 * @param char* l'adresse du serveur
 * @param char* le service (numéro de port) utilisé 
 * @return void
 */
void client_appli(char *serveur, char *service)
{

  /* On créé un socket distant */
  struct sockaddr_in *p_adr_serv;
  /* On rempli les champs de la sock distante */
  adr_socket(service, serveur, SOCK_STREAM, &p_adr_serv);

  /* On crée la socket client et on stock son id */
  int sock_id = h_socket(AF_INET, SOCK_STREAM);
  /* On lance la connexion au serveur avec en param l'id du socket et les infos du serveur */
  h_connect(sock_id, p_adr_serv);

  /* Lancement de la boucle de jeu côté client */
  startCliGame(sock_id);

  /* On close la socket */
  h_close(sock_id);
}


/**
 * Boucle de jeu côté client
 * @param int le descripteur d'une socket utilisée par le client
 * pour l'échange de données avec le processus serveur
 * @return void
 */
void startCliGame(int sock_id)
{
  /* Variables */
  char* buffer = malloc(100);
  char temp;
  char vie;
  int gagne = 0;

  /* Demande et envoi de la difficulté (nombre de vie) */
  printf("\nBonjour, veuillez saisir la difficulté (votre nombre de vie entre 1 et 9) : ");
  vie = getchar();
  while (getchar() != '\n');
  while (!isdigit(vie))
  {
    printf("Veuillez entrer un chiffre entre 1..9 : ");
    vie = getchar();
    while (getchar() != '\n');
    printf("\n");
  }
  h_writes(sock_id, &vie, 1);

  /* Affichage de la taille du mot */
  h_reads(sock_id, &temp, 1);
  printf("Le mot est de taille %c, bonne chance !\n", temp);

  /* Boucle de jeu */
  while (!gagne && vie != '0')
  {
    /* Affichage de l'état du mot */
    h_reads(sock_id, buffer, 100);
    printf("Etat du mot : %s\n", buffer);

    /* On choisi un caractère : */
    temp = lireChar();
    h_writes(sock_id, &temp, 1);

    /* On affiche l'état du tour */
    h_reads(sock_id, &temp, 1);
    switch (temp)
    {
      case '0':
        printf("Loupé ! Vous perdez une vie.\n");
        break;
      case '1':
        printf("Bravo !\n");
        break;
    }
    /* Affichage du nombre de vie restant */
    h_reads(sock_id, &vie, 1);
    printf("Nombre de vie restante : %c\n", vie);

    /* Si le joueur n'a plus de vie, la partie est terminée */
    h_reads(sock_id, &temp, 1);
    if(temp == '1')
      gagne = 1;
  }

  /* Affichage du mot à trouver */
  if (gagne)
    printf("Bien joué ! Vous avez gagné.\n");
  else
    printf("Dommage, bien essayé !\n");

  free(buffer);
}

/** 
 * Lit un caractère sur l'entrée standard
 * @return char, le caractère lu 
 */
char lireChar()
{
  char c = 0;
  c = getchar();
  c = toupper(c);
  while (getchar() != '\n');
  return c;
}