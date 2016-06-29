#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	float x;
	float y;
}Ponto;

typedef struct {
	int qtd;
	Ponto soma;
}CG;

typedef struct listagen ListaGen;

struct listagen{
	void *info;
	ListaGen *prox;
};

ListaGen *insere(ListaGen *lista, void *info);
int percorre(ListaGen *lista, FILE* saida, int (*cb) (FILE*, void*, void*), void *dado);
int imprime(FILE* saida, void *info, void *dado);
int centro_geometrico(FILE* saida,void *info, void *dado);
int igualdade(FILE* saida, void *info1, void *info2);
int pertence(ListaGen *lista, FILE* saida, float x, float y);
void libera(ListaGen* lista, void* (*cb) (void*));

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Arquivo de entrada nao foi chamado\n");
		return 0;
	}
	FILE* entrada = fopen(argv[1], "r");
	if(argv[1] == NULL)
	{
		printf("Arquivo de entrada inválido ou vázio\n");
		return 0;
	}
	FILE* saida = fopen("saida.txt", "w");
	
	ListaGen *lista = NULL;
	CG cg= {0, {.0f, .0f}}; // zera os valores da estrutura de CG
	Ponto ponto[4]; // valor de acordo com o arquivo de entrada;
	int i=0;
	float x, y;
	while(fscanf(entrada, "%f %f", &x, &y) != EOF)
	{
		ponto[i].x = x;
		ponto[i].y = y;
		lista = insere(lista, &ponto[i]);
		i++;
	}

	percorre(lista, saida, imprime, NULL);
	percorre(lista, saida, centro_geometrico, &cg);
	Ponto centro;
	centro.x = cg.soma.x/cg.qtd;
	centro.y = cg.soma.y/cg.qtd;
	fprintf(saida, "O centro geometrico é (%f,%f)\n",centro.x, centro.y);
	
	if(pertence(lista, saida, 2.2f, 2.5f)){
		fprintf(saida, "O ponto está na lista\n");
	}		
	else 
		fprintf(saida, "O ponto está fora da lista\n");
		
// 	libera(lista, lista->info);
	return 0;
}

ListaGen *insere(ListaGen *lista, void *info)
{
	ListaGen *novo = (ListaGen *)  malloc(sizeof(ListaGen));
	novo->info = info;
	novo->prox = lista;
	
	return novo;
}

int percorre(ListaGen *lista, FILE* saida, int (*cb) (FILE*, void*, void*), void *dado)
{
	ListaGen *temp;
	
	for(temp = lista ; temp != NULL; temp = temp->prox)
	{
		int r = cb(saida, temp->info, dado);
		
		if(r)
			return r;
	}
	
	return 0;
}

int imprime(FILE* saida, void *info, void *dado)
{
	Ponto *ponto = (Ponto*) info;
	
	fprintf(saida, "x : %f  y: %f\n", ponto->x, ponto->y);

	return 0;
}

int centro_geometrico(FILE* saida, void *info, void *dado)
{
	Ponto *p = (Ponto *) info;
	CG *cg = (CG *) dado;
	
	cg->soma.x += p->x;
	cg->soma.y += p->y;
	cg->qtd++;	
	
	return 0;
}

int igualdade(FILE* saida, void *info1, void *info2)
{
	Ponto *ponto1 = (Ponto *) info1;
	Ponto *ponto2 = (Ponto *) info2;
	
	if(ponto1->x == ponto2->x && ponto1->y == ponto2->y){
		return 1;
	}
	return 0;
}

int pertence(ListaGen *lista, FILE* saida, float x, float y)
{
	Ponto temp;
	temp.x = x;
	temp.y = y;
	
	return percorre(lista, saida, igualdade, &temp);
}
void libera(ListaGen* lista, void* (*cb) (void*))
{
	ListaGen* h = lista;
	ListaGen* antes = NULL;
	while(h != NULL)
	{
		cb(h->info);
		antes = h;
		h = h->prox;
		free(antes);
	}
}
