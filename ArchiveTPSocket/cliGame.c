#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

#include "fon.h"   		/* primitives de la boite a outils */

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
  for(int i=0; i<100; i++) buffer[i] = 0;
  buffer[0] = vie;
  /* Message envoyé */
  h_writes(sock_id,buffer,100);
  h_reads(sock_id,buffer,100);
  printf("%s\n",buffer);



  free(buffer);
}