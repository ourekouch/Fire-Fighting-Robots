#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define COLOMN 7
#define LIGN 7
#define MAX_CASE LIGN*COLOMN
#define NR 3
#define Malloc(type,VAL)   (type*)malloc(VAL*sizeof(type))
#define Mallop(type,VAL)   (type**)malloc(VAL*sizeof(type*))
typedef struct maps {
	int **mapcoor;
}Maps;
typedef struct robot{
	int xr;
	int yr;

 } ROBOT;
typedef struct feu{
	int xf;
	int yf;
}FIRE;
typedef struct obtacle{
	int xo;
	int yo;
}WALL;
typedef struct _road{

	int distance;
	int placex,placey;
	struct _road *up;
	struct _road *down;
	struct _road *right;
	struct _road *left;
}Road;
void afficher(int **map){// l'affichage de la carte
	int i=0,j=0;
	printf("\n");
    time_t timer,timeref;
    time(&timeref);
    do{
    time(&timer);
    }
    while(timer-timeref<0.01);//une duree qui ralenti les affichages pour qu'ils serait visibles
    system("cls");// pour que le nouveau affichage s'affiche sur le premier
	for (;i<LIGN;i++){
		//printf("\t\t");
		for (j=0;j<COLOMN;j++){
			if (*(*(map+i)+j)==0)  printf("...\t");//le vide
			if (*(*(map+i)+j)==3)  printf("],[\t");//symbole de l'obstacle dans la carte
			if (*(*(map+i)+j)==5)  printf("|R=\t");//sumbole du robot dans la carte
			if (*(*(map+i)+j)==2)  printf(".#.\t");//symbole du feu dans la carte
			if (*(*(map+i)+j)==6)  printf("-|-\t");//symbole de drone dans la carte
		}
		printf("\n");
	}
}
void dronesearsh(int **knowntosystemmap,int **mapofstructures){//le parcours du drone dans la carte
        int i=0,j=0;
        *(*(knowntosystemmap+i)+j)=6;// drone in first position
        afficher(knowntosystemmap);
        for(i=0;i<LIGN;i++){
            if(j==0) {
                for(j;j<COLOMN;j++){
                    *(*(knowntosystemmap+i)+j)=*(*(mapofstructures+i)+j);
                     if(j+1<COLOMN)   *(*(knowntosystemmap+i)+j+1)=6;
                     else if (i+1<LIGN) *(*(knowntosystemmap+i+1)+j)=6;
                     afficher(knowntosystemmap);
                }
                j=COLOMN;
            }
            else {
                if(j==COLOMN)
                    for(j=COLOMN-1;j>=0;j--){
                    *(*(knowntosystemmap+i)+j)=*(*(mapofstructures+i)+j);
                     if(j-1>=0)   *(*(knowntosystemmap+i)+j-1)=6;
                     else if (i+1<LIGN) *(*(knowntosystemmap+i+1)+j)=6;
                     afficher(knowntosystemmap);
                }
                j=0;
            }
        }

}
int	**initial(int **map,int valin){//initialisation du carte
		int i=0,j;
		for (;i<LIGN;i++){
			for (j=0;j<COLOMN;j++){
				*(*(map+i)+j)=valin;
			}
	}
	return map;
}
int	depel(int p){
	if (p<0)	return ++p;
	return --p;
}
int abs(int x){
if (x<0) return -x;
return x;
}
int min(int a,int b){
	if(a<=b) return a;
	return b;
}
int  minchm(Road *ch1,Road *ch2,Road *ch3){//le chemin de distance minimale
	if (ch1!=NULL){
		if(ch2!=NULL){
			if(ch3!=NULL){
				return min(ch1->distance,min(ch2->distance,ch3->distance));
			}
			else{
				return min(ch1->distance,ch2->distance);
			}
		}
		else{
			if(ch3!=NULL){
				return min(ch1->distance,ch3->distance);
			}
			else{
				return ch1->distance;
			}
		}
		}
	else{
		if(ch2!=NULL){
			if(ch3!=NULL){
				return min(ch2->distance,ch3->distance);
			}
			else{
				return ch2->distance;
			}
		}
		else{
			if(ch3!=NULL){
				return ch3->distance;
			}
			else{
				return -9999;
			}
		}
	}
	}
Road  *freeroad(Road *che){
    if (che==NULL) return che;   // c'est le cas de null
    else{
	if (che->up!=NULL) che->up=freeroad(che->up);
    if (che->down!=NULL) che->down=freeroad(che->down);
    if (che->left!=NULL) che->left=freeroad(che->left);
    if (che->right!=NULL) che->right=freeroad(che->right);
    }
    free(che);
    return NULL;
}
void freemap(int **map){//léberation du mémoire
    int i,j;
    for(i=0;i<LIGN;i++){
		for(j=0;j<COLOMN;j++){
			free(*((map)+i)+j);
		}
	}
}
void freerobo(ROBOT **ro){//léberation du mémoire
    int i;
    for(i=0;i<NR;i++){
        free(*(ro+i));
        free(ro+i);
    }
    return;
}
void freefire(FIRE **feu,int *pnf){//léberation du mémoire
    int i;
    for(i=0;i<*pnf;i++){
        free((*feu)+i);
    }
    return;
}
void freewall(WALL **obstacle,int *pno){//léberation du mémoire
    int i;
    for(i=0;i<*pno;i++){
        free((*obstacle)+i);
    }
    return;
}
int **copymap(int **mapcoor){//generation d'une copie du carte
    int i,j;// coordonée
    int **mapcopy;
    mapcopy=Mallop(int,LIGN);
    for (i=0;i<LIGN;i++){
		*(mapcopy+i)=Malloc(int,COLOMN);
	}
	for (i=0;i<LIGN;i++){
        for(j=0;j<COLOMN;j++){
            *(*((mapcopy)+i)+j)=*(*((mapcoor)+i)+j);
        }
	}
	return mapcopy;
	}
Road *quickroadof2(Road *r1,Road *r2){
 /*   if(r1!=NULL){
        if (r2!=NULL){
            if (r1->distance<=r2->distance) return r1;
            else return r2;
        }
        else return r1;
    }
    else {
        if (r2!=NULL){
            return r2;
        }
        else return NULL;*/
if ((r1!=NULL) && (r2==NULL)) {
     return r1;}
if ((r2!=NULL) && (r1==NULL)) {
     return r2;}
if ((r2==NULL) && (r1==NULL)) {
     return NULL;}
return ((r1->distance<=r2->distance)? r1:r2);
}
int maxpermited(int x,int y,FIRE *feu,int no){
     int op=(abs((feu->xf)-x)+abs((feu->yf)-y));             //approximation du route optimal
     //int op=LIGN+COLOMN-2;
     if (op==1) return op;
    // if (testofdecreasingdegree) return op+(*degree);
     else return op+2*no;
}
int nearobstacles(int x,int y,int **map){
if ((x+1<LIGN)&&(*(*((map)+x+1)+y)==3)) return 1;
if ((y+1<COLOMN)&&(*(*((map)+x)+y+1)==3)) return 1;
if ((x-1>0)&&(*(*((map)+x-1)+y)==3)) return 1;
if ((y-1>0)&&(*(*((map)+x)+y-1)==3)) return 1;
return 0;
}
int maxpermitedwithdecreasingeffect(int x,int y,FIRE *feu,int no,int *degreeofdetor,int *testofdecreasingdegree,int **map){
     int op=(abs((feu->xf)-x)+abs((feu->yf)-y));             //approximation du route optimal
     //int op=LIGN+COLOMN-2;
     if (op==1) return op;
     if(!(*testofdecreasingdegree))   (*testofdecreasingdegree)=nearobstacles(x,y,map);
     if((*degreeofdetor)-1==0) return op;
     if ((*testofdecreasingdegree))
        {     if ((*degreeofdetor)%2==1)
            {
                    (*testofdecreasingdegree)=0;
                    (*testofdecreasingdegree)=nearobstacles(x,y,map);
                    if ((*testofdecreasingdegree)) {

                            --(*degreeofdetor);}
            }
            (*testofdecreasingdegree)=1;
            return op+(--(*degreeofdetor));
     }
     else return op+2*no;
}
Road *quickroadfromright(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int* testofdecreasingdegree);
Road *quickroadfromup(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree);
Road *quickroadfromleft(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree);
Road *quickroadfromdown(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree){
                //printf("up \n ");
                if ((chemin->placex)-1<0)                                           return NULL;                    //endofroad
				if(*(*((mapcoor)+(chemin->placex)-1)+(chemin->placey))==3)	return NULL;
                int *test1=Malloc(int,1);
                int *test2=Malloc(int,1);
                *test1=*degreeofdetor;
                *test2=*testofdecreasingdegree;

                if ((chemin->distance+1)+maxpermitedwithdecreasingeffect(chemin->placex-1,chemin->placey,feu,no,test1,test2,mapcoor)/*estimation to arrive*/>maxpermited(ro->xr,ro->yr,feu,no))  return NULL; // pour éviter des cas non obtimal  programme non parfaite pour éviter des calcul non aprécialble
                //if (optimumroadtofire(chemin->placex-1,chemin->placey,feu,no)>optimumroadtofire(chemin->placex,chemin->placey,feu,no))
                                 //finding an obstacle // on peut de plus raisonner sur les autre feu ou les autre robot mais pour l'instant on dit que les robot peut tous pénetrer.
                int **mapcopy;
                mapcopy=copymap(mapcoor);
				int mindistance;                                                                                    // qui contien la court des distance
				Road *Qchemin=Malloc(Road,1);                                                                       // notre nouveau route
				Qchemin->placex=chemin->placex-1;                                                                   //
				Qchemin->placey=chemin->placey;                                                                     // les coordonée
				Qchemin->down=NULL;                                                                                 // naicessaire pour ne rien repeter
				Qchemin->distance=chemin->distance+1;                                                               // l'organisme de calcule des distances
				Qchemin->up=NULL;                                                                                   //
				Qchemin->left=NULL;                                                                                 //
				Qchemin->right=NULL;                                                                                //----------->>on va les changer selon la situation
				*(*((mapcopy)+(chemin->placex))+(chemin->placey))=3;                                                // pour éviter la répétition lors des appelles
				if(((chemin->placex)-1!=feu->xf)||((chemin->placey)!=feu->yf)){                                     // cette pas on n'a pas trouver notre feu

					Qchemin->up=quickroadfromdown(mapcopy,feu,ro,Qchemin,no,test1,test2);                                       //
					Qchemin->left=quickroadfromright(mapcopy,feu,ro,Qchemin,no,test1,test2);                                    //
					Qchemin->right=quickroadfromleft(mapcopy,feu,ro,Qchemin,no,test1,test2);                                    //---->chercher le court chemin
					free(test1);
					free(test2);

					mindistance=minchm(Qchemin->right,Qchemin->left,Qchemin->up);                                   // récupérer la distance minimal
					Qchemin->distance=mindistance;                                                                  //la vrai distance ver le feu
					if(mindistance<0){
						Qchemin->left=freeroad(Qchemin->left);
						Qchemin->right=freeroad(Qchemin->right);
						Qchemin->up=freeroad(Qchemin->up);
						free(Qchemin);
						freemap(mapcopy);
						return NULL;                                                                              /// c'est le cas ou je suis dans une fin route
					}
					else{

                        if ((Qchemin->right!=NULL)&&(mindistance==Qchemin->right->distance)){
						Qchemin->left=freeroad(Qchemin->left);
						Qchemin->up=freeroad(Qchemin->up);
                        }
                        else{
                            if ((Qchemin->left!=NULL)&&(mindistance==Qchemin->left->distance)){
                            Qchemin->right=freeroad(Qchemin->right);
                            Qchemin->up=freeroad(Qchemin->up);
                            }
                            else{
                                if ((Qchemin->up!=NULL)&&(mindistance==Qchemin->up->distance)){
                                Qchemin->left=freeroad(Qchemin->left);
                                Qchemin->right=freeroad(Qchemin->right);
                                }
                                }
                            }
                        }///   ici je prend seulement le court chemin
				}

					freemap(mapcopy);
					return Qchemin;


}
Road *quickroadfromright(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree){
                //printf("left \n ");
                if ((chemin->placey)-1<0)                                           return NULL;//endofroad
				if(*(*((mapcoor)+(chemin->placex))+(chemin->placey)-1)==3)	return NULL;
                int *test1=Malloc(int,1);
                int *test2=Malloc(int,1);
                *test1=*degreeofdetor;
                *test2=*testofdecreasingdegree;
               if ((chemin->distance+1)+maxpermitedwithdecreasingeffect(chemin->placex,chemin->placey-1,feu,no,test1,test2,mapcoor)/*estimation to arrive*/>maxpermited(ro->xr,ro->yr,feu,no)) return NULL; // pour éviter des cas non obtimal  programme non parfaite pour éviter des calcul non aprécialble
                //finding an obstacle // on peut de plus raisonner sur les autre feu ou les autre robot mais pour l'instant on dit que les robot peut tous pénetrer.
				int **mapcopy;
				mapcopy=copymap(mapcoor);
				int mindistance;   // qui contien la court des distance

				Road *Qchemin=Malloc(Road,1); // notre nouveau route
				Qchemin->placex=chemin->placex;  //
				Qchemin->placey=chemin->placey-1;  // les coordonée
				Qchemin->right=NULL;       // naicessaire pour ne rien repeter
				Qchemin->distance=(chemin->distance)+1; // l'organisme de calcule des distances
				Qchemin->up=NULL;    //
				Qchemin->left=NULL;  //
				Qchemin->down=NULL; //----------->>on va les changer selon la situation
				*(*((mapcopy)+(chemin->placex))+(chemin->placey))=3;// pour éviter la répétition lors des appelles
				if(((chemin->placex)!=feu->xf)||((chemin->placey)-1!=feu->yf)){ // cette pas on n'a pas trouver notre feu
					Qchemin->up=quickroadfromdown(mapcopy,feu,ro,Qchemin,no,test1,test2);          //
					Qchemin->left=quickroadfromright(mapcopy,feu,ro,Qchemin,no,test1,test2);       //
					Qchemin->down=quickroadfromup(mapcopy,feu,ro,Qchemin,no,test1,test2);       //---->chercher le court chemin
					free(test1);
					free(test2);
					mindistance=minchm(Qchemin->down,Qchemin->left,Qchemin->up);// récupérer la distance minimal
					Qchemin->distance=mindistance;//la vrai distance ver le feu
					if(mindistance<0){
						Qchemin->left=freeroad(Qchemin->left);
						Qchemin->down=freeroad(Qchemin->down);
						Qchemin->up=freeroad(Qchemin->up);
						freemap(mapcopy);
						free(Qchemin);
						return NULL;                                /// c'est le cas ou je suis dans une fin route
					}
					else{

                        if ((Qchemin->down!=NULL)&&(mindistance==Qchemin->down->distance)){
						Qchemin->left=freeroad(Qchemin->left);
						Qchemin->up=freeroad(Qchemin->up);
                        }
                        else{
                            if ((Qchemin->left!=NULL)&&(mindistance==Qchemin->left->distance)){
                            Qchemin->down=freeroad(Qchemin->down);
                            Qchemin->up=freeroad(Qchemin->up);
                            }
                            else{
                                if ((Qchemin->up!=NULL)&&(mindistance==Qchemin->up->distance)){
                                Qchemin->left=freeroad(Qchemin->left);
                                Qchemin->down=freeroad(Qchemin->down);
                                }
                                }
                            }
                        }///   ici je prend seulement le court chemin
				}

					freemap(mapcopy);
					return Qchemin;


}
Road *quickroadfromup(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree){
                if ((chemin->placex)+1>=LIGN)                      return NULL;//endofroad
				if(*(*((mapcoor)+(chemin->placex)+1)+(chemin->placey))==3)	return NULL; //finding an obstacle // on peut de plus raisonner sur les autre feu ou les autre robot mais pour l'instant on dit que les robot peut tous pénetrer.
                int *test1=Malloc(int,1);
                int *test2=Malloc(int,1);
                *test1=*degreeofdetor;
                *test2=*testofdecreasingdegree;
                if ((chemin->distance+1)+maxpermitedwithdecreasingeffect(chemin->placex+1,chemin->placey,feu,no,test1,test2,mapcoor)>maxpermited(ro->xr,ro->yr,feu,no)){       /*estimation to arrive*/
                return NULL;}// pour éviter des cas non obtimal  programme non parfaite pour éviter des calcul non aprécialble
                int **mapcopy;
                mapcopy=copymap(mapcoor);
				int mindistance;   // qui contien la court des distance
				Road *Qchemin=Malloc(Road,1); // notre nouveau route
				Qchemin->placex=chemin->placex+1;  //
				Qchemin->placey=chemin->placey;  // les coordonée
				Qchemin->up=NULL;       // naicessaire pour ne rien repeter
				Qchemin->distance=chemin->distance+1; // l'organisme de calcule des distances
				Qchemin->down=NULL;    //
				Qchemin->left=NULL;  //
				Qchemin->right=NULL; //----------->>on va les changer selon la situation
				*(*((mapcopy)+(chemin->placex))+(chemin->placey))=3;// pour éviter la répétition lors des appelles
				if(((chemin->placex)+1!=feu->xf)||((chemin->placey)!=feu->yf)){ // cette pas on n'a pas trouver notre feu
					Qchemin->down=quickroadfromup(mapcopy,feu,ro,Qchemin,no,test1,test2);          //
					Qchemin->left=quickroadfromright(mapcopy,feu,ro,Qchemin,no,test1,test2);       //
					Qchemin->right=quickroadfromleft(mapcopy,feu,ro,Qchemin,no,test1,test2);       //---->chercher le court chemin
                    free(test1);
                    free(test2);
					mindistance=minchm(Qchemin->right,Qchemin->left,Qchemin->down);// récupérer la distance minimal
                    Qchemin->distance=mindistance;//la vrai distance ver le feu
					if(mindistance<0){
						Qchemin->left=freeroad(Qchemin->left);
						Qchemin->right=freeroad(Qchemin->right);
						Qchemin->down=freeroad(Qchemin->down);
						freemap(mapcopy);
						free(Qchemin);
						return NULL;                                /// c'est le cas ou je suis dans une fin route
					}
					else{

                        if ((Qchemin->right!=NULL)&&(mindistance==Qchemin->right->distance)){

                            Qchemin->left=freeroad(Qchemin->left);
                            Qchemin->down=freeroad(Qchemin->down);
                        }
                        else{
                            if ((Qchemin->left!=NULL)&&(mindistance==Qchemin->left->distance)){
                            Qchemin->right=freeroad(Qchemin->right);
                            Qchemin->down=freeroad(Qchemin->down);
                            }
                            else{
                                if ((Qchemin->down!=NULL)&&(mindistance==Qchemin->down->distance)){
                                Qchemin->left=freeroad(Qchemin->left);
                                Qchemin->right=freeroad(Qchemin->right);
                                }
                                }
                            }
                        }///   ici je prend seulement le court chemin
				}

					freemap(mapcopy);
					return Qchemin;


}
Road *quickroadfromleft(int **mapcoor,FIRE *feu,ROBOT *ro,Road *chemin,int no,int *degreeofdetor,int *testofdecreasingdegree){

                //printf("right \n ");

                if(*(*((mapcoor)+(chemin->placex))+(chemin->placey)+1)==3)  	return NULL;//finding an obstacle // on peut de plus raisonner sur les autre feu ou les autre robot mais pour l'instant on dit que les robot peut tous pénetrer.
                if ((chemin->placey)+1>=COLOMN)  return NULL;
                int *test1=Malloc(int,1);
                int *test2=Malloc(int,1);
                *test1=*degreeofdetor;
                *test2=*testofdecreasingdegree;
                if ((chemin->distance+1)+maxpermitedwithdecreasingeffect(chemin->placex,chemin->placey+1,feu,no,test1,test2,mapcoor)/*estimation to arrive*/>maxpermited(ro->xr,ro->yr,feu,no))
                {

                return NULL;
                } // pour éviter des cas non obtimal  programme non parfaite pour éviter des calcul non aprécialble

                //endofroad
				int **mapcopy;
				mapcopy=copymap(mapcoor);
				int mindistance;   // qui contien la court des distance
				Road *Qchemin=Malloc(Road,1); // notre nouveau route
				Qchemin->placex=chemin->placex;  //
				Qchemin->placey=chemin->placey+1;  // les coordoné
				Qchemin->left=NULL;       // naicessaire pour ne rien repeter
				Qchemin->distance=chemin->distance+1; // l'organisme de calcule des distances
				Qchemin->up=NULL;    //
				Qchemin->down=NULL;  //
				Qchemin->right=NULL; //----------->>on va les changer selon la situation
				*(*((mapcopy)+(chemin->placex))+(chemin->placey))=3;// pour éviter la répétition lors des appelles
				if(((chemin->placex)!=feu->xf)||((chemin->placey)+1!=feu->yf)){ // cette pas on n'a pas trouver notre feu
					Qchemin->up=quickroadfromdown(mapcopy,feu,ro,Qchemin,no,test1,test2);          //
					Qchemin->down=quickroadfromup(mapcopy,feu,ro,Qchemin,no,test1,test2);       //
					Qchemin->right=quickroadfromleft(mapcopy,feu,ro,Qchemin,no,test1,test2);       //---->chercher le court chemin
					free(test1);
					free(test2);
					mindistance=minchm(Qchemin->right,Qchemin->down,Qchemin->up);// récupérer la distance minimal
					Qchemin->distance=mindistance;//la vrai distance ver le feu
					if(mindistance<0){
						Qchemin->down=freeroad(Qchemin->down);
						Qchemin->right=freeroad(Qchemin->right);
						Qchemin->up=freeroad(Qchemin->up);
						freemap(mapcopy);
						free(Qchemin);
						return NULL;                                /// c'est le cas ou je suis dans une fin route
					}
					else{

                        if ((Qchemin->right!=NULL)&&(mindistance==Qchemin->right->distance)){
						Qchemin->down=freeroad(Qchemin->down);
						Qchemin->up=freeroad(Qchemin->up);
                        }
                        else{
                            if ((Qchemin->down!=NULL)&&(mindistance==Qchemin->down->distance)){
                            Qchemin->right=freeroad(Qchemin->right);
                            Qchemin->up=freeroad(Qchemin->up);
                            }
                            else{
                                if ((Qchemin->up!=NULL)&&(mindistance==Qchemin->up->distance)){
                                Qchemin->down=freeroad(Qchemin->down);
                                Qchemin->right=freeroad(Qchemin->right);
                                }
                                }
                            }
                        }///   ici je prend seulement le court chemin
				}

                    freemap(mapcopy);
					return Qchemin;



}
Road *findquickpath(int **mapcopy,ROBOT *ro,FIRE *feu,int no){
	Road *chemin=Malloc(Road,1);
	int degreeofdetor=2*no,testofdecreasingdegree=0;     //the maximum steps needed to detour all the obstacles that exist!
	chemin->placex=ro->xr;
	chemin->placey=ro->yr;
	chemin->distance=0;
    (testofdecreasingdegree)=nearobstacles(chemin->placex,chemin->placex,mapcopy);
    if ((testofdecreasingdegree)) --(degreeofdetor);
    //printf("up\n");
	chemin->up=quickroadfromdown(mapcopy,feu,ro,chemin,no,&degreeofdetor,&testofdecreasingdegree);
	//if (!chemin->up) printf("no road !!");
    //printf("down\n");
    chemin->down=quickroadfromup(mapcopy,feu,ro,chemin,no,&degreeofdetor,&testofdecreasingdegree);
    //if (!chemin->down) printf("no road !!");
    //printf("right\n");
	chemin->right=quickroadfromleft(mapcopy,feu,ro,chemin,no,&degreeofdetor,&testofdecreasingdegree);
    //if (!chemin->right) printf("no road !!");
    //printf("left\n");
	chemin->left=quickroadfromright(mapcopy,feu,ro,chemin,no,&degreeofdetor,&testofdecreasingdegree);// dans chaque chemin on a le plus court ou bien NULL signifiant qu'il n'y a pas de route
    //if (!chemin->left) printf("no road !!");
    chemin->distance=minchm(quickroadof2(chemin->right,chemin->up),chemin->down,chemin->left);// la distance minimale
    if (chemin->distance<0) {
        chemin->right=freeroad(chemin->right);
        chemin->up=freeroad(chemin->up);
        chemin->down=freeroad(chemin->down);
        chemin->left=freeroad(chemin->left);
        free(chemin);
        return NULL;  // NULL implique pas de chemin
    }
    else {
        if ((chemin->right!=NULL)&&(chemin->right->distance==chemin->distance)){
        chemin->up=freeroad(chemin->up);
        chemin->down=freeroad(chemin->down);
        chemin->left=freeroad(chemin->left);
        //return chemin->right;
        }
        else{
            if ((chemin->up!=NULL)&&(chemin->up->distance==chemin->distance)){
                chemin->right=freeroad(chemin->right);
                chemin->down=freeroad(chemin->down);
                chemin->left=freeroad(chemin->left);
                //return chemin->up;
            }
            else{
                if ((chemin->down!=NULL)&&(chemin->down->distance==chemin->distance)){
                    chemin->up=freeroad(chemin->up);
                    chemin->right=freeroad(chemin->right);
                    chemin->left=freeroad(chemin->left);
                   // return chemin->down;
                }
                else{
                    if ((chemin->left!=NULL)&&(chemin->left->distance==chemin->distance)){
                        chemin->up=freeroad(chemin->up);
                        chemin->down=freeroad(chemin->down);
                        chemin->right=freeroad(chemin->right);
                       // return chemin->left;
                    }
                }
            }
        return chemin;          //pour le supprimer apprés
        }
    }
}
Maps *creemap(){
	int i=0;
	int **map=Mallop(int,LIGN);//nembre de lign dans la map
	Maps *strmap;
	strmap=Malloc(Maps,1);
	for (;i<LIGN;i++){
		*(map+i)=Malloc(int,COLOMN);
	}
	//	int j=0;
	map=initial(map,0);
	afficher(map);
	strmap->mapcoor=map;
	return strmap;

}
Road *nextstep(Road *che){
    if(!che) return NULL;
    if(che->up) return che->up;
    if(che->down) return che->down;
    if(che->left) return che->left;
    if(che->right) return che->right;
}
void followpath(int **map,Road *che,FIRE *feu,ROBOT **ro){
    if (che==NULL) return;
    else {
        Road *next=nextstep(che);
        if ((next->placex==feu->xf)&&(next->placey==feu->yf)){
            *(*((map)+next->placex)+next->placey)=0;
            afficher(map);
            return;
        }                       //printf("step x=%d  y=%d\n",che->placex,che->placey);
        if(*(*((map)+che->placex)+che->placey)!=2) *(*((map)+che->placex)+che->placey)=0; //le cas de deux feu  au moins sucessive  // le cas ou on a 2 robot sucessive ou un robot puis une feu est impossible a les traiter dans followpath d'où l'interet de refresh
        if((*(*((map)+next->placex)+next->placey)==5)||(*(*((map)+next->placex)+next->placey)==2)) {
            (*ro)->xr=next->placex; // le robot et un autre(ou feu ) sont dans le meme emplacement
            (*ro)->yr=next->placey;
            afficher(map);
            Road *next2=nextstep(next);
            if ((next2->placex==feu->xf)&&(next2->placey==feu->yf)){      //deux pas est fait dans ce cas
                        *(*((map)+next2->placex)+next2->placey)=0;
                        afficher(map);
                        return;
            }
            if((*(*((map)+next2->placex)+next2->placey)==5)||(*(*((map)+next2->placex)+next2->placey)==2))
            {
                followpath(map,next,feu,ro); //en refait le calcule pour le next2 du next par les conditions qui le traite en haut
            }
            else{
            *(*((map)+next2->placex)+next2->placey)=5;
            (*ro)->xr=next2->placex;
            (*ro)->yr=next2->placey;
            afficher(map);
            followpath(map,next2,feu,ro);
            }
        }
        else{
                *(*((map)+next->placex)+next->placey)=5;
                (*ro)->xr=next->placex;
                (*ro)->yr=next->placey;
                afficher(map);
                followpath(map,next,feu,ro);
        }
    }
}
int distance (int **map,ROBOT *ro ,FIRE *feu,int no){
    Road *chemin;
    int distance;
    chemin=findquickpath(map,ro,feu,no);
    if (!chemin)
    {
        freeroad(chemin);
        return 9999;
    }
    distance=chemin->distance;
    freeroad(chemin);
    return distance;
}
int not_in(int element,int *tableau , int nb_elt){ ///fonction qui verifie si un element est dans une liste
    int j=0;
    while(j<=nb_elt){
            if(element==*(tableau+j)) return 0;
            j++;
    }
    return 1;
}
int neerrobot(int **map,int nr, ROBOT **ro , int *robot_proches , FIRE *feu , int currentfire,int no){
    int i , neerrob ;
    int proche_robot=0;                                                     // initialisation par le robot 0
    for (i=1;i<nr;i++){                                         // chercher pour cette feu le robot plus proche parmis les robots disponibles
            if ((not_in(i,robot_proches,currentfire))&&(distance(map,*(ro+i),feu+currentfire,no)<=distance(map,*(ro+proche_robot),feu+currentfire,no))) proche_robot=i;
    }
    return proche_robot;
}
int exitroad(int  **map,FIRE *feu,ROBOT **ro,int nr,int no){
        int i;
        Road *testroad;
        for(i=0;i<nr;i++) {
            testroad=findquickpath(map,*(ro+i),feu,no);
            if (testroad) {
                    freeroad(testroad);
                    return 1;
            }
            freeroad(testroad);
        }
        return 0;
}
int **eleminatfire(int **map,int nr,int *nf,ROBOT **ro,FIRE *feu,int no){
		int i=0,j=0,numberofuneliminatedfire=0,totalfire=*nf;
		Road *roadtogivenfire;
		int *robot_proches=Malloc(int,*nf);              ///contient les indices des robotes proches pour chaque feu
		FIRE *remainedfire;
		for(i=0;i<*nf;i++)robot_proches[i]=-1;          ///initialiser les indices des  robot proches de chaque feu par -1
		//printf("did you pass this  \n");
		for(i=0;i<*nf;i++){
            robot_proches[i]=neerrobot(map,nr,ro,robot_proches,feu,i,no);
            //printf("did you pass this  %d \n",i);
            //printf("robot plus proche a feu %d est robot %d \n",i,robot_proches[i]);

		}

		if(*nf>nr) numberofuneliminatedfire=((*nf)-nr);
        {
            remainedfire=Malloc(FIRE,numberofuneliminatedfire);
            for (i=0;i<numberofuneliminatedfire;i++)    // registrer les feu qui ne sont pas encore eleminer !
                {
            (remainedfire+i)->xf=(feu+nr+i)->xf  ;
            (remainedfire+i)->yf=(feu+nr+i)->yf  ;
            }
        }
        i=0;
		do {

            if (distance(map,*(ro+robot_proches[i]),feu+i,no)==9999)   // pas de route vers cette le feu i/ ici dans le cas ou les position des robots est pris en compte  // car seul autre cas cette fonction est appler c'est dans le cas où aucun robot ne peut éteindre cette feu
                {
                    if (!exitroad(map,feu+i,ro,nr,no)) { // ici pas de route vers cette feu
                        printf("\n\nla feu dans la position x=%d;y=%d le system n'arrive pas a l'étaindre !, il faut chercher une solution ultérieur\n\n",(feu+i)->xf,(feu+i)->yf);
                        getchar();
                    (*nf)--;
                    }
                    else     //ce cas est appeller si on prit on conte les positions des robots
                    {
                    printf("\n\nla feu dans la position x=%d;y=%d le system n'arrive pas a l'éteindre pour le mement !\n\n",(feu+i)->xf,(feu+i)->yf);
                    getchar();
                    numberofuneliminatedfire++;
                    remainedfire=(FIRE*)realloc(remainedfire,numberofuneliminatedfire*sizeof(FIRE));
                    (remainedfire+numberofuneliminatedfire-1)->xf=(feu+i)->xf; //  coordoner de nouveau feu
                    (remainedfire+numberofuneliminatedfire-1)->yf=(feu+i)->yf; //
                    }
                }
			else {
                    //printf("this controlle consile fire number %d\n",i);
                    //printf("finding good path");
                    roadtogivenfire=findquickpath(map,*(ro+robot_proches[i]),feu+i,no);
                    //if(!roadtogivenfire)    printf("no path found");
                   // printf("oh there is a path :)");
                    followpath(map,roadtogivenfire,feu+i,ro+robot_proches[i]);
                    freeroad(roadtogivenfire);

			}
            (*nf)--;
           // printf("fire number %d",(*nf));
            i++;

		}while(*nf>numberofuneliminatedfire);
		//printf("finshed with this");
		if(numberofuneliminatedfire!=0)  // le cas des feu restant
        {
        *nf=numberofuneliminatedfire;
		freefire(&feu,&totalfire);
		feu=remainedfire;
		free(robot_proches);
		map=eleminatfire(map,nr,nf,ro,feu,no); // eleminer les feu qui restent
        }
       // printf("end of function");
		return map;
}
void generer_rapport(int* no, int nf ,double duree){
    int surface;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf(asctime(timeinfo));
    FILE* fichier = NULL;
    fichier = fopen("rapport_simulation.txt", "w");
    if (fichier != NULL)
    {
        surface=LIGN*COLOMN;
        fputs("\n\n\t############################### RAPPORT FIGHTFIRE ROBOTS ###################################\n\n", fichier);
        fputs("\n\nDATE AND TIME :\t\t ", fichier);
        fputs(asctime (timeinfo), fichier);
        fprintf(fichier,"\n\nMAPS INFORMATIONS :\n\n\t#LONGEUR:\t%i\tKM\n\t#LARGEUR:\t%i\tKM\n\t#SURFACE:\t%i\tKM²",LIGN,COLOMN,surface);
        fputs("\n\nSIMULATION INFORMATIONS :\n\n\t#DUREE DE SIMULATION:\t", fichier);
        fprintf(fichier,"%f\t second",duree);
        fprintf(fichier,"\n\t#NOMBRE DES ROBOTS:\t%i\tROBOT\n\t#NOMBRE DES OBSTACLES:\t%i\tOBSTACLE",NR,*no);
        fprintf(fichier,"\n\t#NOMBRE DES FEUX:\t%i\tFEU",nf);
        fputs("\n\n\t#####################################   FIN RAPPORT ########################################\n\n", fichier);

        fclose(fichier);
    }
}
int **construire(int **newmap,ROBOT **ro,WALL **obstacle,FIRE **feu, int *nf , int *no ){
    int i,xf,yf,xo,yo;
    int erreur;
    for (i=0;i<NR;i++) *(ro+i)=Malloc(ROBOT,1);                            /// placer les robots dans leurs emplecements
	for (i=0;i<NR;i++){
        (*(ro+i))->xr=i;
        (*(ro+i))->yr=0;
	}
	for (i=0;i<NR;i++){
		*(*((newmap)+((*(ro+i))->xr))+((*(ro+i))->yr))=5;   ///placer les robots dans la maps
	}
	//afficher(newmap);                      ///affichage du maps avec les robots
    printf("\n\n");
	printf("donnez le nombre des feu!");
	do{
        scanf("%d",nf);
	}while((*nf<0)||(*nf>MAX_CASE-NR));			        /// concernant le nombre des feu tant que     -1<x<26
	(*feu)=Malloc(FIRE,*nf);
	for (i=0;i<*nf;i++){
        erreur=0;
	    do{
        if(erreur)printf("\nERROR:ces coordonnes sont invalides SVP entrez des autres coordonnees !!\n");
        printf("donnez les coordonnees du feu %d : ",i+1);		///l'emplacement des feu ici sans conditions
        printf("\nxf= ");
        scanf("%d",&xf);
        printf("\nyf= ");
        scanf("%d",&yf);
        ((*feu)+i)->xf=xf;
        ((*feu)+i)->yf=yf;
        erreur=1;
        }while((xf>=LIGN)||(xf<0)||(yf>=COLOMN)||(yf<0)||(*(*((newmap)+xf)+yf)!=0));
	}
	for (i=0;i<*nf;i++){
		*(*((newmap)+((*feu)+i)->xf)+((*feu)+i)->yf)=2;  ///placer les feu dans la maps
	}
	printf("\n\n");
	printf("donnez le nombre des obstacles!");
	do{
        scanf("%d",no);
	}while((*no<0)||(*no>MAX_CASE-*nf-NR));			        /// concernant le nombre des obstacles tant que     -1<x<26
	(*obstacle)=Malloc(WALL,*no);
	for (i=0;i<*no;i++){
        erreur=0;
	    do{
        if(erreur)printf("\nERROR:ces coordonnes sont invalides SVP entrez des autres coordonnees !!\n");
        printf("donnez les coordonnees de l'obstacle %d : ",i+1);		///l'emplacement des obstacles ici sans conditions
        printf("\nxo= ");
        scanf("%d",&xo);
        printf("\nyo= ");
        scanf("%d",&yo);
        ((*obstacle)+i)->xo=xo;
        ((*obstacle)+i)->yo=yo;
        erreur=1;
        }while((xo>=LIGN)||(xo<0)||(yo>=COLOMN)||(yo<0)||(*(*((newmap)+xo)+yo)!=0));
	}
	for (i=0;i<*no;i++){
		*(*((newmap)+((*obstacle)+i)->xo)+((*obstacle)+i)->yo)=3;  ///placer les obstacles dans la maps
	}
	return newmap;
}
void refreshmap(int **map,ROBOT **ro,int nr){
    int i;
    for(i=0;i<nr;i++){
        *(*((map)+(*(ro+i))->xr)+((*(ro+i))->yr))=5;
    }
    afficher(map);
}
void main(){
	Maps *systemmap;
	int **newmap=Mallop(int,LIGN);;
	ROBOT **ro=Mallop(ROBOT,NR); // nessecaire pour l'echange des valeurs des robot  if faut trouvé les adresse des pointeur de chaque robot
	FIRE **feu=Mallop(FIRE,1);
	WALL **obstacle=Mallop(WALL,1);
	Road *myroad;
	double duree;

    time_t depart,arrivee;
	int i,j,nf,nf_r,no,xf,yf,*pnf,*pno;
	for (i=0;i<LIGN;i++){
		*(newmap+i)=Malloc(int,COLOMN);
	}
	int  degreeofdetor,testofdecreasingdegree=0;
	time(&depart);
	//(*testofdecreasingdegree)=0;//related to finding the quickest path
	pnf=&nf;
	pno=&no;
	getchar();
	printf("######### WELCOME TO \"FIGHTFIRE ROBOTS SIMULATE\" ##########\n\n");
	systemmap=creemap();
	newmap=initial(newmap,0);
	printf("\n\n");
	newmap=construire(newmap,ro,obstacle,feu,pnf,pno);
    dronesearsh(systemmap->mapcoor,newmap);
    nf_r=*pnf;  // garder le nombres des feu initiales pour la creation du rapport
	afficher(systemmap->mapcoor);
	printf("\n\n");
    systemmap->mapcoor=eleminatfire(systemmap->mapcoor,NR,pnf,ro,*feu,*pno);
    refreshmap(systemmap->mapcoor,ro,NR);
    /*myroad=findquickpath(strmap->mapcoor,*ro,*feu,*pno);
	afficher(strmap->mapcoor);
	if (myroad==NULL)       printf("pas de chemin");
	else {
            printf("quick distance %d",myroad->distance);
            followpath(strmap->mapcoor,myroad,*feu,ro);
        }
    printf("les coordonée du robo sont :x=%d y=%d",(*ro)->xr,(*ro)->yr);   */                                                                           //strmap=gererfeu(strmap,NR,pnf,ro,feu);
	getchar();
	time(&arrivee);
	duree=difftime(arrivee,depart);
	generer_rapport(pno,nf_r,duree);
	freemap(systemmap->mapcoor);
	freerobo(ro);
	freewall(obstacle,pno);
	free(obstacle);
	freefire(feu,pnf);
	free(feu);
    printf("################## THE END OF SIMULATION #####################\n\n");

	}


