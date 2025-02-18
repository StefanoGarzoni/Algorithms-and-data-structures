#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLACK 0
#define RED 1

typedef struct Ingrediente{
    struct NodeIng* ing;
    int qt; 
    struct Ingrediente* succ;
} Ingrediente;

typedef struct Lotto{
    int scadenza;
    int qt;         
    struct Lotto* succ;
} Lotto;

typedef struct NodeIng{
    char* nome;   
    struct Lotto* lotti;
    int color;   
    struct NodeIng* p;
    struct NodeIng* left;
    struct NodeIng* right;                       
} NodeIng;

typedef struct Ricetta{
    char* nome;
    struct Ingrediente* ingredienti;
    int peso;
    int link;
    int color;
    struct Ricetta* left;
    struct Ricetta* right;
    struct Ricetta* p;
} Ricetta;

typedef struct NodeOrd{
    struct Ricetta* ricetta;
    int qtOrd;       
    int tempo;      
    struct NodeOrd* p;    
    struct NodeOrd* left;      
    struct NodeOrd* right;
    int stato;     
    int color;             
} NodeOrd;

typedef struct box{
    struct NodeOrd* ordine;
    struct box* succ;
}box;

NodeIng* NILM = NULL;
Ricetta* NILR = NULL;
NodeOrd* NILO = NULL;


//Albero RB INGREDIENTI

void left_rotate_mag(NodeIng** T, NodeIng* x){
    NodeIng* y = x->right; 
    x->right=y->left; 
    
    if(y->left!=NILM){
        y->left->p=x; 
    }
    y->p=x->p; 
    
    if(x->p==NILM){
        *T=y; 
    }else if(x==x->p->left){
        x->p->left=y; 
    }else{
        x->p->right=y; 
    }
    y->left=x; 
    x->p=y;
}

void right_rotate_mag(NodeIng** T, NodeIng* x){
    NodeIng* y=x->left; 
    x->left=y->right;

    if(y->right!=NILM){
        y->right->p=x; 
    }
    y->p=x->p; 
    if(x->p==NILM){
        *T=y; 
    }else if(x==x->p->right){
        x->p->right=y; 
    }else{
        x->p->left=y; 
    }
    y->right=x; 
    x->p=y; 
}

NodeIng* tree_successor_mag(NodeIng* x){
    if(x->right!=NILM){
        x=x->right;
        while(x->left!=NILM){
            x=x->left;
        }
        return x;
    }
    NodeIng* y = x->p;
    while(y!=NILM && x==y->right){
        x=y;
        y=y->p;
    }
    return y;
}

void rbMag_insert_fixup(NodeIng** T, NodeIng* z){
    NodeIng* x;
    NodeIng* y;

    while(z!=*T && z->p->color==RED){ 
         x=z->p;
        if (x==x->p->left){
            y=x->p->right; 

            if(y->color==RED){ 
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->right){ 
                    z=x;
                    left_rotate_mag(T,z);
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                right_rotate_mag(T,x->p);
            }
        }else{ 
            y=x->p->left; 

            if(y->color==RED){ 
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->left){ 
                    z=x;
                    right_rotate_mag(T,z); //left_rotate
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                left_rotate_mag(T,x->p); //right_rotate
            }
        }
    }
    (*T)->color=BLACK; 
}

void rbMag_insert(NodeIng** T, NodeIng* z){
    
    NodeIng* y=NILM;
    NodeIng* x=*T;

    while(x!=NILM){
        y=x;
        if(strcmp(z->nome,y->nome)<0){
            x=x->left;
        }else{
            x=x->right;
        }
    }

    z->p=y;

    if(y==NILM){
        *T=z; //entra qui quando l'albero è vuoto
    }else if(strcmp(z->nome,y->nome)<0){
        y->left=z;
    }else{
        y->right=z;
    }

    z->left=NILM;
    z->right=NILM;
    z->color=RED;

    rbMag_insert_fixup(T,z);
}

int search_ing(NodeIng* x, int qt, int t){ 

    if(x!=NILM){
        Lotto*l=x->lotti;
        while(l!=NULL){
            if(t>=l->scadenza){
                //free(x->lotti);
                x->lotti=l->succ;
                l=x->lotti;
            }else{
                if(qt<=l->qt){
                    return 1;
                }else{
                    qt=qt-(l->qt);
                    l=l->succ;
                }
            }
        }
        return 0;
    }else{
        return 0;
    }
    
}

NodeIng* search_ing_in_tree(NodeIng* testa, char* nome){
    int cmp;
     while(testa!=NILM){
        cmp=strcmp(nome,testa->nome);
        if(cmp==0){
            return testa;
        }else if(cmp<0){
            testa=testa->left;
        }else{
            testa=testa->right;
        }
    }
    return testa;
}

void clean_mag(NodeIng* x, int qt){
    
    Lotto*l=x->lotti;
    while(l!=NULL && qt>0){
        if(l->qt<=qt){
            qt=qt-(l->qt);
            x->lotti=l->succ;
            //free(l);
            l=x->lotti;
            
        }else{
            l->qt=l->qt-qt;
            qt=0;
        }
    }
}


//Albero RB RICETTE

void left_rotate_ric(Ricetta** T, Ricetta* x){
    Ricetta*y=x->right; 
    x->right=y->left; 
    
    if(y->left!=NILR){
        y->left->p=x; 
    }
    y->p=x->p; 
    
    if(x->p==NILR){
        *T=y; 
    }else if(x==x->p->left){
        x->p->left=y; 
    }else{
        x->p->right=y; 
    }
    y->left=x; 
    x->p=y;
}

void right_rotate_ric(Ricetta** T, Ricetta* x){
    Ricetta* y=x->left; 
    x->left=y->right;

    if (y->right!=NILR){
        y->right->p=x; 
    }
    y->p=x->p; 
    if(x->p==NILR){
        *T=y; 
    }else if(x==x->p->right){
        x->p->right=y; 
    }else{
        x->p->left=y; 
    }

    y->right=x; 
    x->p=y; 
}

Ricetta* tree_successor_ric(Ricetta* x){
    if(x->right!=NILR){
        x=x->right;
        while(x->left!=NILR){
            x=x->left;
        }
        return x;
    }
    Ricetta* y = x->p;
    while(y!=NILR && x==y->right){
        x=y;
        y=y->p;
    }
    return y;
}

void rbRic_insert_fixup(Ricetta** T, Ricetta*z){
    Ricetta* x;
    Ricetta* y;
    while (z->p!=NULL && z->p->color==RED){ 
        x=z->p;

        if(x==x->p->left){
            y=x->p->right;

            if(y!=NULL && y->color==RED){  
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->right){
                    z=x;
                    left_rotate_ric(T,z);
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                right_rotate_ric(T,x->p);
            }
        }else{
            y=x->p->left;
            if(y!=NULL && y->color==RED){
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->left){
                    z=x;
                    right_rotate_ric(T,z); //left_rotate
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                left_rotate_ric(T,x->p); //right_rotate
            }
        }
    }
    (*T)->color = BLACK;
}

void rbRic_insert(Ricetta** T, Ricetta* z){
    
    Ricetta* y=NILR;
    Ricetta* x=*T;

    while(x!=NILR){
        y=x;
        if(strcmp(z->nome,x->nome)<0){
            x=x->left;
        }else{
            x=x->right;
        }
    }
    z->p=y;

    if(y==NILR){
        *T=z; 
    }else if(strcmp(z->nome,y->nome)<0){
        y->left=z;
    }else{
        y->right=z;
    }
    z->left=NILR;
    z->right=NILR;
    z->color=RED;

    rbRic_insert_fixup(T,z);
}

void rb_delete_fixup_ric(Ricetta** T, Ricetta* x){
    Ricetta* ric;
    while(x!=*T && x->color==BLACK){
        if(x==x->p->left){
            ric=x->p->right;

            if(ric->color==RED){
                ric->color=BLACK;
                x->p->color=RED;
                left_rotate_ric(T,x->p);
                ric=x->p->right;
            }

            if(ric->left->color==BLACK && ric->right->color==BLACK){
                ric->color=RED;
                x=x->p;
            }else {
                if(ric->right->color==BLACK){
                    ric->left->color=BLACK;
                    ric->color=RED;
                    right_rotate_ric(T,ric);
                    ric=x->p->right;
                }
                ric->color=x->p->color;
                x->p->color=BLACK;
                ric->right->color=BLACK;
                left_rotate_ric(T,x->p);
                x=*T; 
            }
        }else{
            ric=x->p->left;

            if(ric->color==RED){
                ric->color=BLACK;
                x->p->color=RED;
                right_rotate_ric(T,x->p);
                ric=x->p->left;
            }
            if(ric->right->color==BLACK && ric->left->color==BLACK){
                ric->color=RED;
                x=x->p;
            }else{
                if(ric->left->color==BLACK){
                    ric->right->color=BLACK;
                    ric->color=RED;
                    left_rotate_ric(T,ric);
                    ric=x->p->left;
                }

                ric->color=x->p->color;
                x->p->color=BLACK;
                ric->left->color=BLACK;
                right_rotate_ric(T,x->p);
                x=*T;
            }
        }
    }
    x->color=BLACK;
}

void rb_delete_ric(Ricetta** T, Ricetta* z){
    Ricetta* y;
    Ricetta* x;
    int color;

    if(z->left==NILR || z->right==NILR){
        y=z;
    }
    else{
        y=tree_successor_ric(z);
    }

    if(y->left!=NILR){
        x=y->left;
    }else{
        x=y->right;
    }
    x->p=y->p;
    if(y->p==NILR){
        (*T)=x;
    }else if(y==y->p->left){
        y->p->left=x;
    }else{
        y->p->right=x;
    }

    color=y->color;
    if(y!=z){
        if(z==*T){
            *T=y;
        }

        if(z==z->p->left){
            z->p->left=y;
        }else if(z==z->p->right){
            z->p->right=y;
        }

        y->right=z->right;
        y->left=z->left;
        y->p=z->p;
        y->color=z->color;

        if(z->left!=NILR){
            z->left->p=y;
        }
        if(z->right!=NILR){
            z->right->p=y;
        }
        z->left->p=y;
        z->right->p=y;
        
    }
    if (color==BLACK) {
        rb_delete_fixup_ric(T,x);
    }

    free(z->nome);
    free(z);
}

Ricetta* search_ric(Ricetta* x, char* nome){
    int cmp;
    while(x!=NILR){
        cmp=strcmp(nome, x->nome);
        if(cmp==0){
            break;
        }else if(cmp<0){
            x=x->left;
        }else{
            x=x->right;
        }
    }
    return x;
}


//ALBERO RB ORDINI

void left_rotate(NodeOrd** T, NodeOrd* x){
    NodeOrd* y = x->right; 
    x->right = y->left; 
    
    if (y->left != NILO) {
        y->left->p = x; 
    }

    y->p = x->p; 
    
    if (x->p == NILO) {
        *T = y; 
    } else if (x == x->p->left) {
        x->p->left = y; 
    } else {
        x->p->right = y; 
    }
    y->left = x; 
    x->p = y;
}

void right_rotate(NodeOrd** T, NodeOrd* x){
    NodeOrd* y = x->left; 
    x->left = y->right;

    if (y->right != NILO) {
        y->right->p = x; 
    }
    y->p = x->p; 
    if (x->p == NILO) {
        *T = y; 
    } else if (x == x->p->right) {
        x->p->right = y; 
    } else {
        x->p->left = y; 
    }

    y->right = x; 
    x->p = y; 
}

void rbOrd_insert_fixup(NodeOrd** T, NodeOrd* z){
    NodeOrd* x;
    NodeOrd* y;
    while (z->p!=NULL && z->p->color==RED){ 
        x=z->p;

        if(x==x->p->left){
            y=x->p->right;

            if(y!=NULL && y->color==RED){  
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->right){
                    z=x;
                    left_rotate(T,z);
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                right_rotate(T,x->p);
            }
        }else{
            y=x->p->left;
            if(y!=NULL && y->color==RED){
                x->color=BLACK;
                y->color=BLACK;
                x->p->color=RED;
                z=x->p;
            }else{
                if(z==x->left){
                    z=x;
                    right_rotate(T,z); //left_rotate
                    x=z->p;
                }
                x->color=BLACK;
                x->p->color=RED;
                left_rotate(T,x->p); //right_rotate
            }
        }
    }
    (*T)->color = BLACK;
}

void rbOrd_insert(NodeOrd** T, NodeOrd* z){
    NodeOrd* y=NILO;
    NodeOrd* x=*T;

    while(x!=NILO){
        y=x;
        if(z->tempo<x->tempo){
            x=x->left;
        }else{
            x=x->right;
        }
    }
    z->p=y;
    if(y==NILO){
        *T=z; 
    }else if(z->tempo<y->tempo){
        y->left=z;
    }else{
        y->right=z;
    }
    z->left=NILO;
    z->right=NILO;
    z->color=RED;

    rbOrd_insert_fixup(T,z);
}

NodeOrd* tree_successor(NodeOrd* x){
    if(x->right!=NILO){
        x=x->right;
        while(x->left!=NILO){
            x=x->left;
        }
        return x;
    }
    NodeOrd* y = x->p;
    while(y!=NILO && x==y->right){
        x=y;
        y=y->p;
    }
    return y;
}

void rb_delete_fixup_ord(NodeOrd** T, NodeOrd* x){
    NodeOrd* ord;
    while(x!=*T && x->color==BLACK){
        if(x==x->p->left){
            ord=x->p->right;

            if(ord->color==RED){
                ord->color=BLACK;
                x->p->color=RED;
                left_rotate(T,x->p);
                ord=x->p->right;
            }

            if(ord->left->color==BLACK && ord->right->color==BLACK){
                ord->color=RED;
                x=x->p;
            }else {
                if(ord->right->color==BLACK){
                    ord->left->color=BLACK;
                    ord->color=RED;
                    right_rotate(T,ord);
                    ord=x->p->right;
                }
                ord->color=x->p->color;
                x->p->color=BLACK;
                ord->right->color=BLACK;
                left_rotate(T,x->p);
                x=*T; 
            }
        }else{
            ord=x->p->left;

            if(ord->color==RED){
                ord->color=BLACK;
                x->p->color=RED;
                right_rotate(T,x->p);
                ord=x->p->left;
            }
            if(ord->right->color==BLACK && ord->left->color==BLACK){
                ord->color=RED;
                x=x->p;
            }else{
                if(ord->left->color==BLACK){
                    ord->right->color=BLACK;
                    ord->color=RED;
                    left_rotate(T,ord);
                    ord=x->p->left;
                }

                ord->color=x->p->color;
                x->p->color=BLACK;
                ord->left->color=BLACK;
                right_rotate(T,x->p);
                x=*T;
            }
        }
    }
    x->color=BLACK;
}

void rb_delete_ord(NodeOrd** T, NodeOrd* z){
    NodeOrd* y;
    NodeOrd* x;
    int color;

    if(z->left==NILO || z->right==NILO){
         y=z;
    }
    else{
        y=tree_successor(z);
    }

    if(y->left!=NILO){
        x=y->left;
    }else{
        x=y->right;
    }
    x->p=y->p;
    if (y->p==NILO){
        *T=x;
    }else if(y==y->p->left){
        y->p->left=x;
    }else{
        y->p->right=x;
    }
    color=y->color;
    if(y!=z){
        if(z==*T){
            *T=y;
        }

        if(z==z->p->left){
            z->p->left=y;
        }else if(z==z->p->right){
            z->p->right=y;
        }

        y->p=z->p;
        y->left=z->left;
        y->right=z->right;
        y->color=z->color;

        if(z->left!=NILO){
            z->left->p=y;
        }
        if(z->right!=NILO){
            z->right->p=y;
        }
        z->left->p=y;
        z->right->p=y;

    }
    if(color == BLACK){
        rb_delete_fixup_ord(T,x);
    }

    free(z); 
    
}

//LISTE DI LOTTI
void add_lotto(NodeIng* ing, Lotto* l, int t){
    if(l!=NULL && ing!=NILM && ing!=NULL){
        while(ing->lotti!=NULL && ing->lotti->scadenza<=t){
            //Lotto* t = ing->lotti;
            ing->lotti=ing->lotti->succ;
            //free(l);
        }
        int ctrl=1;
        if(ing->lotti==NULL){
            ing->lotti=l;
        }
        else if(ing->lotti->scadenza>l->scadenza){
            l->succ=ing->lotti;
            ing->lotti=l;
        }else if(ing->lotti->scadenza==l->scadenza){
            ing->lotti->qt+=l->qt;
            free(l);
        }
        else{
            Lotto* lo = ing->lotti;
            
            while(lo->succ!=NULL && ctrl){
                if(lo->succ->scadenza>l->scadenza){
                    l->succ=lo->succ;
                    lo->succ=l;
                    ctrl=0;
                }else if(lo->succ->scadenza==l->scadenza){
                    lo->succ->qt+=l->qt;
                    free(l);
                    ctrl=0;
                }
                lo=lo->succ;
            }
            if(ctrl){
                lo->succ=l;
            }
        }
        
    }
}

//LISTA BOX CAMION
void start_camion(box* testa){
    if(testa==NULL){
        printf("camioncino vuoto\n");
    }else{
        while(testa!=NULL){
            printf("%d %s %d\n",testa->ordine->tempo,testa->ordine->ricetta->nome, testa->ordine->qtOrd);
            testa=testa->succ;
        }
    }
}

box* svuota_camion(box* testa, NodeOrd** testaRB){
    box* tmp;
    while(testa!=NULL){
        tmp=testa;
        testa=testa->succ;
        rb_delete_ord(testaRB,tmp->ordine);
        free(tmp);
    }
    return testa;
}

box* add_camion(box* testa, box*x){
    
    if(testa==NULL){
        return x;
    }

    if((x->ordine->qtOrd * x->ordine->ricetta->peso) > (testa->ordine->qtOrd * testa->ordine->ricetta->peso)){
        x->succ=testa;
        return x;
    }else if((x->ordine->qtOrd * x->ordine->ricetta->peso) == (testa->ordine->qtOrd * testa->ordine->ricetta->peso)){
        if(x->ordine->tempo<=testa->ordine->tempo){
            x->succ=testa;
            return x;
        }
    }

    box* b = testa;
    int ctrl=1;
    
    while(b->succ!=NULL && ctrl){ 
        if((x->ordine->qtOrd * x->ordine->ricetta->peso) > (b->succ->ordine->qtOrd * b->succ->ordine->ricetta->peso)){
            x->succ=b->succ;
            b->succ=x;
            ctrl=0;
        }else if((x->ordine->qtOrd * x->ordine->ricetta->peso) == (b->succ->ordine->qtOrd * b->succ->ordine->ricetta->peso)){
            if(x->ordine->tempo<=b->succ->ordine->tempo){
                x->succ=b->succ;
                b->succ=x;
                ctrl=0;
            }
        }

        b=b->succ;
    }
    if(ctrl){
        b->succ=x;
    }

    return testa;
}

void order_to_do(NodeOrd* testa, int time){
    if (testa==NILO)
        return;

    NodeOrd* ord=testa;
    NodeOrd* succ;
    Ingrediente* ing;
    int ctrl=1;
 
    while(ord->left!=NILO){
        ord=ord->left;
    }


    while(ord!=NILO){

        if(ord->stato!=1){
            ing=ord->ricetta->ingredienti;
            while(ctrl && ing!=NULL){
                ctrl = search_ing(ing->ing, (ing->qt) * ord->qtOrd, time);
                ing=ing->succ;
            }
            if(ctrl){
                ord->stato=1;
                ing=ord->ricetta->ingredienti;
                while(ing!=NULL){
                    clean_mag(ing->ing,(ing->qt) * (ord->qtOrd));
                    ing=ing->succ;
                }
            }else{
                ctrl=1;
            }
        }
    
        if(ord->right!=NILO){
            ord=ord->right;
            while(ord->left!=NILO){
                ord=ord->left;
            }
        }else{
            succ=ord->p;
            while(succ!=NILO && ord==succ->right){
                ord=succ;
                succ=succ->p;
            }
            ord=succ;
        }
    }
}

box* order_doit(NodeOrd* testa, box* camion, int c){
    if (testa==NILO)
        return camion;

    NodeOrd*ord = testa;
    NodeOrd* succ;
    box* b;
    while(ord->left!=NILO){
        ord=ord->left;
    }


    while(ord!=NILO){
        if(ord->stato==1){
            if(c-(ord->qtOrd * ord->ricetta->peso)>=0){
                c = c-(ord->qtOrd * ord->ricetta->peso);
                b = (box*)malloc(sizeof(box));
                b->ordine=ord;
                b->succ=NULL;

                (ord->ricetta->link)-=1;
                
                camion=add_camion(camion,b);

            }else{
                //break;
                return camion;
            }
        }

        if(ord->right != NILO){
            ord = ord->right; 
            while(ord->left!=NILO){
                ord=ord->left;
            }
        }else{
            succ=ord->p;
            while(succ!=NILO && ord==succ->right){
                ord=succ;
                succ=succ->p;
            }
            ord=succ;
        }
    }
    return camion;
}


int main(int argc, char *argv[]){

    NILM = (NodeIng*)malloc(sizeof(NodeIng));
    NILM->color = BLACK;
    NILM->left = NILM->right = NILM->p = NULL;
    NodeIng* testaRBIng = NILM;

    NILR = (Ricetta*)malloc(sizeof(Ricetta));
    NILR->color = BLACK;
    NILR->left = NILR->right = NILR->p = NULL;
    Ricetta* testaRBRic = NILR;

    NILO = (NodeOrd*)malloc(sizeof(NodeOrd));
    NILO->color = BLACK;
    NILO->left = NILO->right = NILO->p = NULL;
    NodeOrd* testaRBord = NILO;
    
    box* camion = NULL;

    int k = 0;
    int ris = 0;
    char car;
    int time = 0;
    char word[255];
    int tCorr;
    int capienza;
    int scad = 0;
    int peso = 0;
    Ricetta* ric;
    Ingrediente* ing;
    Ingrediente* tmp;
    NodeIng* m;
    Lotto* l;
    NodeOrd* o;

    ris = scanf("%d", &tCorr);
    ris = scanf("%d", &capienza);
    ris = scanf("%s", word);

    while(ris != -1 || time % tCorr == 0){

        if(time % tCorr == 0 && time != 0){
            
            camion=order_doit(testaRBord,camion,capienza);
            start_camion(camion);
            camion = svuota_camion(camion,&testaRBord);
            
        }
        
        if(ris != -1){

            if(strcmp(word, "aggiungi_ricetta") == 0){
                
                ris = scanf("%s", word);
                
                if(search_ric(testaRBRic, word) == NILR){

                    ric = (Ricetta*)malloc(sizeof(Ricetta));
                    ric->nome=(char*)malloc(strlen(word) + 1);
                    strcpy(ric->nome, word);
                    ric->link=0; 
                    peso=0;
                    ric->p = NULL;
                    ric->left = NILR;
                    ric->right = NILR;
                    ric->color = RED;

                    
                    ing = (Ingrediente*)malloc(sizeof(Ingrediente));
                    ing->succ=NULL;
                    ris = scanf("%s", word);
                    m=search_ing_in_tree(testaRBIng, word);

                    if(m==NILM){
                        m = (NodeIng*)malloc(sizeof(NodeIng)); 
                        m->nome=(char*)malloc(strlen(word) + 1); 
                        strcpy(m->nome, word);
                        m->p = NULL;
                        m->left = NILM;
                        m->right = NILM;
                        m->color = RED;
                        m->lotti=NULL;
                        
                        rbMag_insert(&testaRBIng,m);
                        
                    }
                    ing->ing=m;
                    
                    ris = scanf("%d", &ing->qt);
                    ric->ingredienti=ing;

                    tmp=ing;
                    peso+=ing->qt;
                    ris=scanf("%c",&car);

                    while (car!='\n')
                    {
                        ris = scanf("%s", word);

                        ing = (Ingrediente*)malloc(sizeof(Ingrediente));
                        ing->succ=NULL;
                        m=search_ing_in_tree(testaRBIng, word);

                        if(m==NILM){
                            m = (NodeIng*)malloc(sizeof(NodeIng)); 
                            m->nome=(char*)malloc(strlen(word) + 1); 
                            strcpy(m->nome, word);
                            m->p = NULL;
                            m->left = NILM;
                            m->right = NILM;
                            m->color = RED;
                            m->lotti=NULL;
                            
                            rbMag_insert(&testaRBIng,m);
                            
                        }

                        ing->ing=m;
                        ris = scanf("%d", &ing->qt);
                        ing->succ=NULL;
                        tmp->succ=ing;
                        tmp=ing;
                        peso+=ing->qt;
                        
                        ris=scanf("%c",&car);
                    }
                    ric->peso=peso;
                    rbRic_insert(&testaRBRic,ric);
                    
                    printf("aggiunta\n");

                    car='\0';
                    ris = scanf("%s", word);

                }else{

                    ris=scanf("%*[^\n]");  // Ignora tutta la riga fino al '\n'
                    ris=scanf("%*c");   //consuma '\n' dal buffer
                    car='\0';

                    printf("ignorato\n");
                    
                    ris = scanf("%s", word);
                }

                time++;

            }else if(strcmp(word, "rimuovi_ricetta") == 0){

                ris = scanf("%s", word);

                ric = search_ric(testaRBRic, word);

                if (ric!= NILR) {
                    if (ric->link<=0)
                    {
                        rb_delete_ric(&testaRBRic,ric);
                        printf("rimossa\n");

                    } else {
                        printf("ordini in sospeso\n");
                    }

                } else {
                    printf("non presente\n");
                }
                ris=scanf("%s",word);
                time++;

            }else if(strcmp(word, "rifornimento") == 0){ 
                
                while (car!='\n')
                {
                    ris = scanf("%s", word);

                    m = search_ing_in_tree(testaRBIng,word);

                    if(m!=NILM){ 

                        l=(Lotto*)malloc(sizeof(Lotto));
                        ris = scanf("%d", &l->qt);
                        ris = scanf("%d", &scad); //ris = scanf("%d", &l->scadenza);
                        l->succ = NULL;

                        if(scad>time){
                            l->scadenza=scad;
                            add_lotto(m,l,time);
                        }
                        else{
                            free(l);
                        }

                    }else{

                        m = (NodeIng*)malloc(sizeof(NodeIng)); 
                        m->nome=(char*)malloc(strlen(word) + 1); 
                        strcpy(m->nome, word);
                        m->p = NULL;
                        m->left = NILM;
                        m->right = NILM;
                        m->color = RED;

                        l=(Lotto*)malloc(sizeof(Lotto));
                        ris = scanf("%d", &l->qt);
                        ris = scanf("%d", &scad); //ris = scanf("%d", &l->scadenza);
                        l->succ = NULL;

                        if(scad>time){
                            l->scadenza=scad;
                            m->lotti=l;
                            rbMag_insert(&testaRBIng,m);
                        }
                        else{
                            free(l);
                            free(m->nome);
                            free(m);
                        }
                        
                    }

                    scad=0;
                    ris = scanf("%c", &car);
                }

                car='\0';
                ris = scanf("%s", word);
                printf("rifornito\n");
                
                order_to_do(testaRBord,time);

                time++;

            }else if(strcmp(word, "ordine") == 0){
                
                ris = scanf("%s", word);
                
                ric = search_ric(testaRBRic,word);

                if(ric != NILR){
                    
                    o = (NodeOrd*)malloc(sizeof(NodeOrd));
                    (ric->link)+=1;
                    o->ricetta=ric;
                    ris = scanf("%d", &o->qtOrd);
                    o->tempo = time;
                    o->p = NULL;
                    o->left = NILO;
                    o->right = NILO;
                    o->color = RED;
                    
                    k = 1;
                    ing = ric->ingredienti;
                    
                    while(k==1 && ing != NULL){
    
                        k = search_ing(ing->ing, (ing->qt) * o->qtOrd, time);
                        ing=ing->succ;
                    }
                    
                    if(k == 1){
                        o->stato=1;
                        ing = ric->ingredienti;
                        while (ing!=NULL)
                        {
                            clean_mag(ing->ing,(ing->qt) * (o->qtOrd));   
                            ing=ing->succ;
                        }
                        
                    }else{
                        o->stato=0;
                    }
                    rbOrd_insert(&testaRBord, o);
                    printf("accettato\n");

                }else{
                    ris = scanf("%d", &scad);
                    printf("rifiutato\n");
                }
                
                ris = scanf("%s", word);
                time++;
            }

        } else {
            break;
        }
    }
   
    return 0;
}

