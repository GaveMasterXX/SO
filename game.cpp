// readfile.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>



#define MAX_PLAYER_NAME 20
#define MAX_GMPLAYER_LIFE 10000
#define MAX_GMPLAYER_DAMAGE 1000
#define MAX_GMPLATEER_CRITIC 50

#define MAX_MONSTER_LIFE 1000
#define MAX_MONSTER_DAMAGE 100
#define MAX_MONSTER_CRITIC 10

#define MAX_CELLS 200
#define MAX_DESCRIPTION_CELL 1000
#define NCELLS 20
#define MAX_LINE 100



/*
Estruturas begin
*/

struct Player {
	char namePlayer[MAX_PLAYER_NAME];
	int energyPlayer;
	int cellPlayer;
	int itemPlayer;
	int treasurePlayer;
};

struct Monster {
	char nameMosnter;
	int lifeMonster;
	int damageMonster;
	int criticMonster;
	int cellMonster;
	int itemMonster;
	int treasureMonster;
};

struct Item {
	int CodItem;
	char NameItem;
	int DamageItem;
	int CriticItem;
	int PositionItem;
};

struct Tresure {
	int CodTresure;
	char NameTreasure;
	int Damagetresure;
	int CriticTreasure;
	int PositionTresure;
};

struct Cell {
	int north;
	int south;
	int east;
	int west;
	int up;
	int down;

	int treasureCell;
	int itemCell;
	char descriptionCell[MAX_DESCRIPTION_CELL];
};

struct Map
{
	struct Cell cell[MAX_CELLS];
	int nCells = NCELLS;
};
/*
Struct END
*/

void InsertPlayer(Player *pPlayer);/*Método que permite o utilizador inserir o seu avatar*/
void PrintPlayer(Player *pPlayer);/*mostra os status do avatar no ecrâ*/

void InicializedMonster(Monster *pMonster);
void PrintMonster(Monster *pMonster);/*mostra os status do monstro no ecrâ*/

int InitMap(Cell cells[]);
void PrintMap(Cell cells[]);/*mostra qual a sala onde se encontra o jogador*/

void LoadMapFromFile(struct Map *pMap);
void PrintMapFromFile(struct Map *pMap);
void MapWalk();/*método que cria o mapa do jogo*/

void MovePlayer();/*método que permite o avatar do utilizador se mover*/
void MoveMonster();/*Método que permite o monstro se mover pelo mapa sozinho*/

void GrabItems();/*Método que permite o jogador apanhar itens nas salas/ atualizar os status do seu avatar*/
void FightWhithMonster();/*método que aciona a batalha entre o jogador e o monstro*/
void EndGame();/*Método que determina quando o jogo acaba*/

void CalculateCriticItis();
void UpdateMosnter();

void ChooseMode(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster);/*O utilizador seleciona o modo de Jogo*/
void DificultyMode();/*Diferentes niveis de dificuldade*/
void MasterPlayer();/*Modo de Administrador*/
void LoadTrackMusic();
void RunTrackMusic();

/*
---------- Hunter Hallow --------
O main é onde os métodos principais são chamados
*/
int main()
{
	
	struct Player player;
	struct Monster monster;
	struct Cell cells[MAX_CELLS];
	struct Map map;
	int nCells;

	printf("--------------------------------\n");
	printf("		HUNTER HALLOW			\n");
	printf("		Frist Episode			\n");
	printf("--------------------------------\n");
	printf("\n");

	InsertPlayer(&player);
	PrintPlayer(&player);
	LoadMapFromFile(&map);
	PrintMapFromFile(&map);
	//nCells = InitMap(cells);
	//PrintMap(cells);
	return 0;
}


void ChooseMode(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster) {
	struct Player player;
	struct Monster monster;
	struct Map map;

	printf(" ---------------------------------------------------------------\n");
	printf("	   ESCOLHE O QUE PERTENDES FAZER AVENTUREIRO				\n");
	printf("  1: ENTRAR NO CASTELO   2: CONTINUAR JOGO 	 	\n");
	printf("        3:SAIR DO CASTELO 	\n");
	printf(" ---------------------------------------------------------------\n");
	int option;

	switch (option)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}

}

/* Método de iniciação do jogador
Este método inicializa o avatar do jogador no jogo,
pondendo tambem definir o modo de jogo e a dificuldade do jogo
*/
void InsertPlayer(Player *pPlayer) {
	printf("Bro insere o teu nome! \n");
	scanf("%s", pPlayer->namePlayer);
	// selecionar modo de jogo / dificuldade
	pPlayer->energyPlayer = 100;
	pPlayer->itemPlayer = 0;
	pPlayer->cellPlayer = 0;
	pPlayer->treasurePlayer = 0;

}

/* Mostrar Jogador
Este Método tem como função principal mostrar os status do jogador no ecrã
*/
void PrintPlayer(Player *pPlayer) {
	printf("Bem vindo Hunter %s \n", pPlayer->namePlayer);
	printf("HP:  %d \n", pPlayer->energyPlayer);
	printf("Items:  %d \n", pPlayer->itemPlayer);
	printf("Room:  %d \n", pPlayer->cellPlayer);
	printf("Treasure:  %d \n", pPlayer->treasurePlayer);
	printf("Are you ready to Fight Bro!!");

}

int InitMap(Cell cells[]) {

	//cell 0 - Ponto inicial do jogo
	cells[0].north = 2;
	cells[0].south = -1;
	cells[0].west = -1;
	cells[0].east = 1;
	cells[0].up = -1;
	cells[0].down = -1;
	strcpy(cells[0].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[0].treasureCell = -1;
	cells[0].itemCell = -1;

	//cell 1 - Avenida Principal
	cells[1].north = -1;
	cells[1].south = 2;
	cells[1].west = 0;
	cells[1].east = 3;
	cells[1].up = -1;
	cells[1].down = -1;
	strcpy(cells[1].descriptionCell, "Please came back!!!!");
	cells[1].treasureCell = -1;
	cells[1].itemCell = -1;

	/*Continue
	.
	.
	.
	.
	*/
	return 2;
}

void PrintMap(Cell cells[]) {

}

void LoadMapFromFile(struct Map *pMap) {
	FILE *f;
	char l[MAX_LINE];
	int line = 1;
	int i = 0;
	int count = 0;

	int north;
	int south;
	int west;
	int east;
	int up;
	int down;
	int item;
	int treasure;
	char description;

	f = fopen("map.txt", "r");
	
	
		while (fgets(l, MAX_LINE, f) != NULL) {
			if (line == 1) {
				sscanf(l, "%d %d %d %d %d %d %d %d", &north, &south, &west,
					&east, &up, &down, &item, &treasure);

				pMap->cell[i].north = north;
				pMap->cell[i].south = south;
				pMap->cell[i].west = west;
				pMap->cell[i].east = east;
				pMap->cell[i].up = up;
				pMap->cell[i].down = down;
				pMap->cell[i].itemCell = item;
				pMap->cell[i].treasureCell = treasure;
				
				line++;
				
			}
			else if(l != '\0'){
				i--;
				sscanf(l, "%s", &description);
				strcpy(pMap->cell[i].descriptionCell, &description);
				line = 1;
				
			}
			i++;
		}	
	
	fclose(f);
}

void PrintMapFromFile(struct Map *pMap) {
	for (int i = 0; i <= 2; i++) {
		printf("\n");
		printf("%d \n", pMap->cell[i].north);
		printf("%d \n", pMap->cell[i].south);
		printf("%d \n", pMap->cell[i].west);
		printf("%d \n", pMap->cell[i].east);
		printf("%d \n", pMap->cell[i].up);
		printf("%d \n", pMap->cell[i].down);
		printf("%d \n", pMap->cell[i].itemCell);
		printf("%d \n", pMap->cell[i].treasureCell);

		printf("%s \n", pMap->cell[i].descriptionCell);
	}
}