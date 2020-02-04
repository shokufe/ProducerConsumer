/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: sh.simab
 *
 * Created on February 4, 2020, 12:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define nCons 10;
#define mProduc 10;
#define tapisCapacite 20;

pthread_mutex_t mutex = PTHREAD_COND_INITIALIZER;
pthread_cond_t	notEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	notFull = PTHREAD_COND_INITIALIZER;

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

/*
 * 
 */
typedef struct Paquet Paquet;
struct Paquet{
    char  nom; //data
    Paquet *next; //pointer to next paquet in list
};
typedef struct Tapis Tapis;
struct Tapis{
    int capacity ;
    int actualsize ;
    Paquet *first;
};
void enfiler(Tapis *tp, char *newnom){  //push
    //bool result;
    
    Paquet *newpaquet =malloc(sizeof(*newpaquet));
    if(tp ==NULL || newpaquet==NULL){
        exit(EXIT_FAILURE);
    }  
    newpaquet->nom = newnom;
    newpaquet->next = NULL;
    
    pthread_mutex_lock (&mutex);
    pthread_cond_wait(&notFull, &mutex);
    if(tp->first != NULL) // la file n'est pas vide
    {
        Paquet *actuelPaquet = tp->first;
        while(actuelPaquet->next !=NULL)
        {
            actuelPaquet = actuelPaquet->next;
        }
        actuelPaquet->next =newpaquet;
        tp->actualsize++;
         /*  char ch = 'A';
          printf("Character is %c \n", ch);*/
    }else{
        tp->first = newpaquet;
        tp->actualsize++;
         char ch = 'B';
          printf("Character is %c \n", ch);
    }
    pthread_cond_signal (&notEmpty);
    pthread_mutex_unlock (&mutex);
    
}
/*POP*/
char defiler(Tapis *tp){
    if(tp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char nomdefiler;
    pthread_mutex_lock (&mutex);
    while(tp->first == NULL){ //queue is empty
         pthread_cond_wait(&notEmpty, &mutex);
        //wait until producer produce new paquet
         char ch = 'S';
          printf("Character is %c \n", ch);
    }
    if(tp->first !=NULL)  //queue is not empty
    {
        char ch = 'N';
        printf("Character is %c \n", ch);
        Paquet *pq = tp->first;
        nomdefiler = pq->nom;
        tp->first = pq->next;
        free(pq);
        tp->actualsize--;
    }
      pthread_cond_signal (&notFull و&mutex);
     pthread_mutex_unlock (&mutex);
    return nomdefiler;
}
void productor(Tapis *tp,int cible){
   
    char newnom = 'A';
    int i=1;
    while(i<cible){
        //create a nom from compteur of loop 
        newnom = strcat(newnom, "number");
        char str[33];
        sprintf(str, "%d", i);
        newnom = strcat(newnom, str);
       //push newnom to our Tapis
        
        printf(newnom);
        enfiler(tp,newnom);
        i++;    
    }
  //  condition.notifyAll();
}
void *consumator(Tapis *tp, int compteur , int thread_id){
    char pqContent;
    
    while(compteur>0){ //it will consume until compteur.control empty or full will hold in defiler
       pqContent=defiler(tp);
       if(pqContent==NULL)
       {
          printf("the paquet was empty");   
       }
       else{
          printf("thread number%d", thread_id);
          printf("consume%c\n",pqContent); 
           
       }
       compteur--;
    }
   // condition.notifyAll();
   // pthread_exit(NULL);
}

int main(int argc, char** argv) {

    Tapis *tapis = malloc(sizeof(*tapis));
    pthread_t ThProducer;
    pthread_t ThConsumator;
    int compteur= 10;
    int cible =5;
    int i;
    //creat n producer
    for (i = 0; i < 10 ; i++) {
       pthread_create(&ThProducer[i],NULL,productor(tapis,cible),(void *)NULL);
    }
    //create n consomator
    for(int j= 0; j<10 ;j++){
       pthread_create(&ThConsumator[i],NULL,consumator(tapis,&compteur,i),(void*)NULL);
    }
     //join threads
    for(int i=0;i<10 ;i++){
        pthread-join(&ThProducer[i],Null);
    }
    for(int i=0 ;i<10;i++){
        pthread_join(&ThConsumator[i],Null);
    }

    /* tapis->first = NULL;
  char n ='D';
  char m= 'F';

   enfiler(tapis,n);
   enfiler(tapis,m);
   Paquet *element = tapis->first;
     while (element != NULL)
    {
        printf("%c ", element->nom);
        element = element->next;
    }
  defiler(tapis);
    
  
    printf("\nEtat de la file :\n");
    Paquet *element = tapis->first;

    while (element != NULL)
    {
        printf("%s ", element->nom);
        element = element->next;
    }
*/

    
    return (EXIT_SUCCESS);
}

