//tp3test 3.0
//修复了free的问题和main函数中supprimer une liste的问题
//新的问题出现，添加第二个元素后，第一个元素变乱码。原因不明


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 30

typedef struct Element{
    char* valeur;
    struct Element* suivant;
    struct Element* precedent;
}T_Element;

typedef struct Liste{
    int taille;
    struct Element* tete;
    struct Element* queue;
}T_Liste;


T_Element *creerElement (char *val){
    T_Element *newElement = malloc(sizeof(T_Element*));
    if (newElement == NULL) {
        return NULL ;
    } else {
        newElement->valeur = val;
        newElement->precedent = NULL;
        newElement->suivant = NULL;
    }
    return newElement;
}

T_Liste *creerListe() {
    T_Liste *newListe = malloc(sizeof(T_Liste*));
    if (newListe != NULL) {
        newListe->taille = 0;
        newListe->tete = NULL;
        newListe->queue = NULL;
    }
    return newListe;
}

T_Element *rechercherElement(T_Liste *list, char *val){
    if ((list == NULL) || (list->tete == NULL)){ return NULL; }
    T_Element* p = list->tete;
    T_Element* q = NULL;
    while(p != NULL){
        if (strcmp(val,p->valeur)==0){
            q = p;
        }
        p = p->suivant;
    }
    return q;
}

int insererElement(T_Liste *list, char *val) {
    if(list == NULL) return -1;
    T_Element* newE = creerElement(val);
    if(newE == NULL) return -1;
    //il n'y a pas d'element dans la liste, on insere l'element dans la tete
    if(list->taille == 0){
        list->taille++;
        list->tete = newE;
        list->queue = newE;
        return 0;
    }
    
    T_Element* p = list->tete;
    int i = 0;
    while(i < list->taille){
        int cmp = strcmp(val,p->valeur);
        if(cmp == 0) return -1;//on trouve le meme element, on arrete
        else{
            if(cmp < 0){
                if(i == 0){//quand l'element qu'on insere est le plus petit
                    newE->suivant = p;
                    p->precedent = newE;
                    list->tete = newE;
                }else{
                    newE->suivant = p;
                    newE->precedent = p->precedent;
                    p->precedent->suivant = newE;
                    p->precedent = newE;
                }
                list->taille++;
                return 0;
            }else{
                i++;
                if(i == list->taille){//quand l'element qu'on insere est le plus grand
                    newE->precedent = p;
                    p->suivant = newE;
                    list->queue = newE;
                    list->taille++;
                    return 0;
                }
                p = p->suivant;
            }
        }
    }
    return -1;
}



int positionElement(T_Liste *list, char *val){
    T_Element* p = list->tete;
    int count = 1;
    while (strcmp(val,p->valeur)!=0 ){
        count++;
        p=p->suivant;
    }
    return count;
}

int supprimerElement(T_Liste *list, char *val){
    if(val == NULL) return -1;
    T_Element* p = rechercherElement(list,val);
    if (p == NULL)  return -1;//l'element qu'on va suprimer n'est pas dans la liste
    //si on supprime l'element, il n'y a pas d'element
    if ((p==list->tete)&&(list->taille==1)){
        list->tete = NULL;
        list->queue = NULL;
        list->taille = 0;
        free(p);
        return 0;
    }
    //si l'element qu'on va supprimer est dans la tete
    if (p==list->tete){
        list->tete = p->suivant;
        list->tete->precedent = NULL;
        free(p);
        list->taille--;
        return 0;
    }
    // si l'element qu'on va supprimer est dans la queue
    if (p==list->queue){
        list->queue = p->precedent;
        list->queue->suivant = NULL;
        free(p);
        list->taille--;
        return 0;
    }
    //autre cas
    p->precedent->suivant = p->suivant;
    p->suivant->precedent = p->precedent;
    list->taille--;
    free(p);
    return 0;
}

int supprimerListe(T_Liste *list){
    //on supprime chaque element dans la liste, apres on supprime la liste
    if (list == NULL) return -1;
    
     T_Element *q, *p = list->tete;
     
     while (p!=NULL){
     q = p;
     p = p->suivant;
     free(q);
     }
    
    list->tete = NULL;
    list->queue = NULL;
    list->taille = 0;
    free(list);
    return 0;
}

T_Liste* ajouterElement_tete(T_Liste* list,char* val){
    //on ajoute l'element dans la tete de la liste
    //c'est pour fusionnerliste
    T_Element *newElement = creerElement(val);
    if (list->taille == MAX || list == NULL || newElement == NULL ) return NULL;
    if(list->tete == NULL){
        list->tete = newElement;
        list->queue = newElement;
        list->taille = 1;
        return list;
    }
    newElement->suivant = list->tete;
    list->tete->precedent = newElement;
    list->tete = newElement;
    list->taille++;
    return list;
}


/*T_Liste* ajouterElement_queue(T_Liste* list,char* val){
 T_Element *newElement = creerElement(val);
 if ((list->taille == MAX)||(list == NULL)||(newElement == NULL)) return NULL;
 if(list->tete == NULL){
 list->tete = newElement;
 list->queue = newElement;
 list->taille = 1;
 return list;
 }
 newElement->precedent = list->queue;
 list->queue->suivant = newElement;
 list->queue = newElement;
 list->taille++;
 return list;
 }
 */
void afficherListe(T_Liste *list){
    if (list != NULL && list->tete != NULL) {
        T_Element *p = list->tete;
        while (p != NULL) {
            printf("%s",p->valeur);
            p = p->suivant;
        }
    } else {
        printf("cet ensemble est vide");
    }
}

T_Liste *fusionnerListes(T_Liste *list1, T_Liste *list2){
    T_Element *p;
    T_Liste *q = NULL;
    if(list1 == NULL || list2 == NULL) return NULL;
    if ((list1->tete == NULL) && (list2->tete == NULL)) return NULL;
    else if (list1->tete == NULL) return list2;
    else if (list2->tete == NULL) return list1;
    else if (strcmp(list1->tete->valeur,list2->tete->valeur)==0){
        p = list1->tete;
        list1->tete = p->suivant;
        list2->tete = list2->tete->suivant;
        q = ajouterElement_tete(fusionnerListes(list1,list2),p->valeur);
        
    } else if (strcmp(list1->tete->valeur,list2->tete->valeur)<0){
        p = list1->tete;
        list1->tete = p->suivant;
        q = ajouterElement_tete(fusionnerListes(list1,list2),p->valeur);
    } else if (strcmp(list1->tete->valeur,list2->tete->valeur)>0){
        p = list2->tete;
        list2->tete = p->suivant;
        q = ajouterElement_tete(fusionnerListes(list1,list2),p->valeur);
    }
    return q;
}

/*void *fusionnerListes2(T_Liste *list1, T_Liste *list2, T_Liste *newListe){
 //T_Liste* newListe = creerListe();
 T_Element *p;
 if ((list1->tete == NULL) && (list2->tete == NULL)) return;
 else if (list1->tete == NULL){
 p = list2->tete;
 list2->tete = p->suivant;
 ajouterElement_queue(newListe,p->valeur);
 fusionnerListes(list1,list2,newListe);
 }else if (list2->tete == NULL){
 p = list1->tete;
 list1->tete = p->suivant;
 ajouterElement_queue(newListe,p->valeur);
 fusionnerListes(list1,list2,newListe);
 }else{
 if (strcmp(list1->tete->valeur,list2->tete->valeur)<0){
 p = list1->tete;
 list1->tete = p->suivant;
 ajouterElement_queue(newListe,p->valeur);
 fusionnerListes(list1,list2,newListe);
 } else if (strcmp(list1->tete->valeur,list2->tete->valeur)>0){
 p = list2->tete;
 list2->tete = p->suivant;
 ajouterElement_queue(newListe,p->valeur);
 fusionnerListes(list1,list2,newListe);
 } else{
 p = list1->tete;
 list1->tete = p->suivant;
 list2->tete = list2->tete->suivant;
 ajouterElement_queue(newListe,p->valeur);
 fusionnerListes(list1,list2,newListe);
 }
 }
 }
 */



int main(){
    /*    T_Liste* list1 = creerListe();
     T_Liste* list2 = creerListe();
     int i,res;
     int j=0;
     res = insererElement(list1,"pro");
     res = insererElement(list1,"a");
     for(i = 0;i < 5;i++){
     printf("enter a word for list1: ");
     scanf("%s",c);
     res = insererElement(list1,c);
     printf("%d\n",res);
     printf("enter a word for list2: ");
     scanf("%s",c);
     res = insererElement(list2,c);
     printf("%d\n",res);
     afficherListe(list1);
     printf("\n");
     afficherListe(list2);
     printf("\n");
     }
     */
    
    T_Liste* list[10];
    int choix, i, j, res, pos;
    int nb = 0;
    int c;
    char* cle[12] = {"hello-0 ","bonjour-1 ","ni hao-2 ", "byb-3 ","salute-4 ","zai jian-5 ","what-6 ","quoi-7 ","shen me-8 ","why-9 ","pourquoi-10 ","wei shen me-11 "};
    T_Element* p;
    T_Liste* mix;
    
    do{
        printf("\n********menu**********\n");
        printf("1. Creer une liste\n");
        printf("2. Ajouter un element dans une liste\n");
        printf("3. Supprimer un element d'une liste\n");
        printf("4. Rechercher un element dans une liste\n");
        printf("5. Afficher les elements d'une liste\n");
        printf("6. Supprimer une liste\n");
        printf("7. Fusionner deux listes\n");
        printf("8. Quitter\n");
        printf("\nSaisie un chiffre  ");
        scanf("%d",&choix);
        switch(choix){
            case 1 :
                if(nb < 10){
                    list[nb] = creerListe();
                    printf("Creer une liste, No.%d\n",nb);
                    nb ++;
                }else{
                    printf("C'est pleine.\n");
                }
                break;
            case 2 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                while(i > nb-1){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                j=rand() % 12;//créer un chiffre random entre 0 et 11
                printf("Ajouter %s",cle[j]);
                res = insererElement(list[i],cle[j]);
                if(res == 0) printf("reussi\n");
                else printf("echec");
                break;
            case 3 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                while(i > nb){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                afficherListe(list[i]);
                printf("\n");
                for(j = 0;j <12;j++) printf("%s",cle[j]);
                printf("\nSaisie une element pour supprimer No.0 -- No.11:");
                scanf("%d",&c);
                //printf("\n%d",c);
                res = supprimerElement(list[i],cle[c]);
                printf("%d\n",res);
                break;
            case 4 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                while(i > nb){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                for(j = 0;j <12;j++) printf("%s",cle[j]);
                printf("\nSaisie une element pour rechercher No.0 -- No.11:");
                scanf("%d",&c);
                p = rechercherElement(list[i],cle[c]);
                if(p == NULL) printf("Il n'y a pas ce mot\n");
                else {
                    pos = positionElement(list[i],cle[c]);
                    printf("Le mot est dans la %deme postion.\n",pos);
                }
                break;
            case 5 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                while(i > nb){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                printf("\nListe No.%d : ",i);
                afficherListe(list[i]);
                break;
            case 6 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                while(i > nb){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                //tous les listes après la liste qui va étre supprimé bougent une place avant
                for(j = i;j < nb-1; j++){
                    list[j] = list[j+1];
                }
                res = supprimerListe(list[nb-1]);
                nb --;
                printf("%d\n",res);
                break;
            case 7 :
                printf("Il y a %d liste, choisissez une liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&i);
                printf("votre choix : %d\n",i);
                while(i > nb){
                    printf("Il n'y a pas cette liste, choisissez une liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&i);
                }
                printf("\nIl y a %d liste, choisissez une autre liste (No.0 -- No.%d):",nb,nb-1);
                scanf("%d",&j);
                printf("votre choix : %d\n",j);
                while(j > nb || i == j){
                    printf("choisissez une autre liste (No.0 -- No.%d) :",nb-1);
                    scanf("%d",&j);
                }
                mix = fusionnerListes(list[i],list[j]);
                afficherListe(mix);
                break;
                
                
        }
        
    }while(choix != 8);
    
    
    system("pause");
    return 0;
}

