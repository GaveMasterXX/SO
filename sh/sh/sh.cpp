// sh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "windows.h"



#define MAX_PLAYER_NAME 20
#define MAX_GMPLAYER_LIFE 10000
#define MAX_GMPLAYER_DAMAGE 1000
#define MAX_GMPLATEER_CRITIC 50

#define MAX_MONSTER_LIFE 1000
#define MAX_MONSTER_DAMAGE 100
#define MAX_MONSTER_CRITIC 10

#define MAX_CELLS 200
#define MAX_DESCRIPTION_CELL 1000
#define NCELLS 200
#define MAX_LINE 100

#define EMPTY _T( ' ' )



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
	int nCells;
};
/*
Struct END
*/

void FunctionClear();
void InsertPlayer(Player *pPlayer);/*Método que permite o utilizador inserir o seu avatar*/
void PrintPlayer(Player *pPlayer);/*mostra os status do avatar no ecrâ*/

void InicializedMonster(Monster *pMonster);
void PrintMonster(Monster *pMonster);/*mostra os status do monstro no ecrâ*/

int InitMap(Cell cells[]);

void LoadMapFromFile(Map *pMap);
void PrintMapFromFile(Map *pMap);
void PlayerWalk(Player *pPlayer, Map *pMap, Monster *pMonster);/*método que cria o mapa do jogo*/

void MovePlayer();/*método que permite o avatar do utilizador se mover*/
void MoveMonster();/*Método que permite o monstro se mover pelo mapa sozinho*/

void GrabItems();/*Método que permite o jogador apanhar itens nas salas/ atualizar os status do seu avatar*/
void FightWhithMonster();/*método que aciona a batalha entre o jogador e o monstro*/
void EndGame();/*Método que determina quando o jogo acaba*/

void CalculateCriticItis();

void MasterPlayer();/*Modo de Administrador*/


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
	while (player.cellPlayer != map.nCells) {
		PlayerWalk(&player, &map, &monster);
	}
	PrintMapFromFile(&map);
	//map.nCells = InitMap(cells);
	return 0;
}

/*
	Método que tem como objetivo limpar a consola com se fosse um "System("cls")"
	este método foi retirado de um dos documentos de apoio do Professor Luis Garcia
*/
void FunctionClear() {
	_tsetlocale(LC_ALL, _T("Portuguese"));

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO strConsoleInfo;

	GetConsoleScreenBufferInfo(hStdout, &strConsoleInfo);

	COORD Home = { 0, 0 };
	DWORD hWrittenChars;

	/*limpa os caracteres*/
	FillConsoleOutputCharacter(hStdout, EMPTY, strConsoleInfo.dwSize.X * strConsoleInfo.dwSize.Y,
		Home, &hWrittenChars);
	/*limpa a formatação*/
	FillConsoleOutputAttribute(hStdout, strConsoleInfo.wAttributes,
		strConsoleInfo.dwSize.X * strConsoleInfo.dwSize.Y, Home, &hWrittenChars);

	SetConsoleCursorPosition(hStdout, Home);

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

	//cell 0 
	cells[0].north = 2;
	cells[0].south = -1;
	cells[0].west = -1;
	cells[0].east = 1;
	cells[0].up = -1;
	cells[0].down = -1;
	strcpy(cells[0].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[0].treasureCell = -1;
	cells[0].itemCell = -1;

	//cell 1 
	cells[1].north = -1;
	cells[1].south = 2;
	cells[1].west = 0;
	cells[1].east = 3;
	cells[1].up = -1;
	cells[1].down = -1;
	strcpy(cells[1].descriptionCell, "Please came back!!!!");
	cells[1].treasureCell = -1;
	cells[1].itemCell = -1;

	//cell 2 
	cells[2].north = 2;
	cells[2].south = -1;
	cells[2].west = -1;
	cells[2].east = 1;
	cells[2].up = -1;
	cells[2].down = -1;
	strcpy(cells[2].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[2].treasureCell = -1;
	cells[2].itemCell = -1;

	//cell 3 
	cells[3].north = 2;
	cells[3].south = -1;
	cells[3].west = -1;
	cells[3].east = 1;
	cells[3].up = -1;
	cells[3].down = -1;
	strcpy(cells[3].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[3].treasureCell = -1;
	cells[3].itemCell = -1;

	//cell 4 
	cells[4].north = 2;
	cells[4].south = -1;
	cells[4].west = -1;
	cells[4].east = 1;
	cells[4].up = -1;
	cells[4].down = -1;
	strcpy(cells[4].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[4].treasureCell = -1;
	cells[4].itemCell = -1;

	//cell 5 
	cells[5].north = 2;
	cells[5].south = -1;
	cells[5].west = -1;
	cells[5].east = 1;
	cells[5].up = -1;
	cells[5].down = -1;
	strcpy(cells[5].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[5].treasureCell = -1;
	cells[5].itemCell = -1;

	//cell 6 
	cells[6].north = 2;
	cells[6].south = -1;
	cells[6].west = -1;
	cells[6].east = 1;
	cells[6].up = -1;
	cells[6].down = -1;
	strcpy(cells[6].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[6].treasureCell = -1;
	cells[6].itemCell = -1;

	//cell 7 
	cells[7].north = 2;
	cells[7].south = -1;
	cells[7].west = -1;
	cells[7].east = 1;
	cells[7].up = -1;
	cells[7].down = -1;
	strcpy(cells[7].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[7].treasureCell = -1;
	cells[7].itemCell = -1;

	//cell 8 
	cells[8].north = 2;
	cells[8].south = -1;
	cells[8].west = -1;
	cells[8].east = 1;
	cells[8].up = -1;
	cells[8].down = -1;
	strcpy(cells[8].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[8].treasureCell = -1;
	cells[8].itemCell = -1;

	//cell 9 
	cells[9].north = 2;
	cells[9].south = -1;
	cells[9].west = -1;
	cells[9].east = 1;
	cells[9].up = -1;
	cells[9].down = -1;
	strcpy(cells[9].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[9].treasureCell = -1;
	cells[9].itemCell = -1;

	//cell 10 
	cells[10].north = 2;
	cells[10].south = -1;
	cells[10].west = -1;
	cells[10].east = 1;
	cells[10].up = -1;
	cells[10].down = -1;
	strcpy(cells[10].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[10].treasureCell = -1;
	cells[10].itemCell = -1;

	//cell 11
	cells[11].north = 2;
	cells[11].south = -1;
	cells[11].west = -1;
	cells[11].east = 1;
	cells[11].up = -1;
	cells[11].down = -1;
	strcpy(cells[11].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[11].treasureCell = -1;
	cells[11].itemCell = -1;

	//cell 12
	cells[12].north = 2;
	cells[12].south = -1;
	cells[12].west = -1;
	cells[12].east = 1;
	cells[12].up = -1;
	cells[12].down = -1;
	strcpy(cells[12].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[12].treasureCell = -1;
	cells[12].itemCell = -1;

	//cell 13
	cells[13].north = 2;
	cells[13].south = -1;
	cells[13].west = -1;
	cells[13].east = 1;
	cells[13].up = -1;
	cells[13].down = -1;
	strcpy(cells[13].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[13].treasureCell = -1;
	cells[13].itemCell = -1;

	//cell 14
	cells[14].north = 2;
	cells[14].south = -1;
	cells[14].west = -1;
	cells[14].east = 1;
	cells[14].up = -1;
	cells[14].down = -1;
	strcpy(cells[14].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[14].treasureCell = -1;
	cells[14].itemCell = -1;

	//cell 15
	cells[15].north = 2;
	cells[15].south = -1;
	cells[15].west = -1;
	cells[15].east = 1;
	cells[15].up = -1;
	cells[15].down = -1;
	strcpy(cells[15].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[15].treasureCell = -1;
	cells[15].itemCell = -1;

	//cell 16
	cells[16].north = 2;
	cells[16].south = -1;
	cells[16].west = -1;
	cells[16].east = 1;
	cells[16].up = -1;
	cells[16].down = -1;
	strcpy(cells[16].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[16].treasureCell = -1;
	cells[16].itemCell = -1;

	//cell 17
	cells[17].north = 2;
	cells[17].south = -1;
	cells[17].west = -1;
	cells[17].east = 1;
	cells[17].up = -1;
	cells[17].down = -1;
	strcpy(cells[17].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[17].treasureCell = -1;
	cells[17].itemCell = -1;

	//cell 18
	cells[18].north = 2;
	cells[18].south = -1;
	cells[18].west = -1;
	cells[18].east = 1;
	cells[18].up = -1;
	cells[18].down = -1;
	strcpy(cells[18].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[18].treasureCell = -1;
	cells[18].itemCell = -1;

	//cell 19
	cells[19].north = 2;
	cells[19].south = -1;
	cells[19].west = -1;
	cells[19].east = 1;
	cells[19].up = -1;
	cells[19].down = -1;
	strcpy(cells[19].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[19].treasureCell = -1;
	cells[19].itemCell = -1;

	//cell 20
	cells[20].north = 2;
	cells[20].south = -1;
	cells[20].west = -1;
	cells[20].east = 1;
	cells[20].up = -1;
	cells[20].down = -1;
	strcpy(cells[20].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[20].treasureCell = -1;
	cells[20].itemCell = -1;

	//cell 21
	cells[21].north = 2;
	cells[21].south = -1;
	cells[21].west = -1;
	cells[21].east = 1;
	cells[21].up = -1;
	cells[21].down = -1;
	strcpy(cells[21].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[21].treasureCell = -1;
	cells[21].itemCell = -1;

	//cell 22
	cells[22].north = 2;
	cells[22].south = -1;
	cells[22].west = -1;
	cells[22].east = 1;
	cells[22].up = -1;
	cells[22].down = -1;
	strcpy(cells[22].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[22].treasureCell = -1;
	cells[22].itemCell = -1;

	//cell 23
	cells[23].north = 2;
	cells[23].south = -1;
	cells[23].west = -1;
	cells[23].east = 1;
	cells[23].up = -1;
	cells[23].down = -1;
	strcpy(cells[23].descriptionCell, "Welcome and Good Luck Boy!!!");
	cells[23].treasureCell = -1;
	cells[23].itemCell = -1;

	

	return 23;
}

/*
	
*/
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
	char description[MAX_DESCRIPTION_CELL];

	f = fopen("map.txt", "r");


	while (fgets(l, MAX_LINE, f) != NULL) {
		if (line == 1 && strcmp(l, "\n") != 0) {
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

			/*printf("\n");
			printf("Norte: %d \n", pMap->cell[i].north);
			printf("Sul: %d \n", pMap->cell[i].south);
			printf("Oeste: %d \n", pMap->cell[i].west);
			printf("Este: %d \n", pMap->cell[i].east);
			printf("Cima: %d \n", pMap->cell[i].up);
			printf("Baixo: %d \n", pMap->cell[i].down);
			printf("Item: %d \n", pMap->cell[i].itemCell);
			printf("Tesouro: %d \n", pMap->cell[i].treasureCell);*/
			line++;
			pMap->nCells++;

		}
		else if (strcmp(l, "\n") != 0 && l != " ") {
			i--;
			sscanf(l, "%s", &description);
			strcpy(pMap->cell[i].descriptionCell, description);
			line = 1;
			//printf("Descricao: %s \n", pMap->cell[i].descriptionCell);
		}
		else {
			//do Nothing
			//printf("hello\n");
		}
		i++;
		
	}

	fclose(f);
}

void PrintMapFromFile(Map *pMap) {
	for (int i = 0; i <= pMap->nCells; i++) {
		printf("\n");
		printf("Norte: %d \n", pMap->cell[i].north);
		printf("Sul: %d \n", pMap->cell[i].south);
		printf("Oeste: %d \n", pMap->cell[i].west);
		printf("Este: %d \n", pMap->cell[i].east);
		printf("Cima: %d \n", pMap->cell[i].up);
		printf("Baixo: %d \n", pMap->cell[i].down);
		printf("Item: %d \n", pMap->cell[i].itemCell);
		printf("Tesouro: %d \n", pMap->cell[i].treasureCell);
		printf("Descrição: %s \n", pMap->cell[i].descriptionCell);
	}
}

void PlayerWalk(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster) {
	int option;
	//FunctionClear();
	printf(" --------------------------------------------------------------------\n");
	printf(" Para andar a traves das catacumbas selecione uma das opções abaixo \n");
	printf("1 - Norte  2 - Sul  3 - Oeste  4 - Este  5 - Subir 6 - Descer 7 - menu  8 - Save\n");
	printf(" --------------------------------------------------------------------\n");
	printf("\n Porta de destino: ");
	
	scanf("%d", &option);

	switch (option)
	{
	case 1:
		if (pMap->cell[pPlayer->cellPlayer].north == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].north; }
		break;
	case 2:
		if (pMap->cell[pPlayer->cellPlayer].south == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].south; }
		break;
	case 3:
		if (pMap->cell[pPlayer->cellPlayer].west == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].west; }
		break;
	case 4:
		if (pMap->cell[pPlayer->cellPlayer].east == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].east; }
		break;
	case 5:
		if (pMap->cell[pPlayer->cellPlayer].up == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].up; }
		break;
	case 6:
		if (pMap->cell[pPlayer->cellPlayer].down == -1) {
			printf("Lamento mas as catacunbas não tem portas secretas !!!");
		}
		else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].down; }
		break;
	case 7:
		//gameMenu();
		break;

	case 8:
		//SaveGame(pPlayer, pMonster);
		printf("-----------------------------------\n");
		printf("O Jogo foi guardado \n");

		break;
	default:
		printf("O valor introdusido é invalido!!! \n Insira novamente um numero de 1 a 7 \n");
		break;
	}
}