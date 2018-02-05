// sh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "windows.h"



#define MAX_NAME 12
#define MAX_MOSNTERS 100
#define MAX_ITEMS 1000
#define MAX_TREASURE 1000
#define MAX_CELLS 200
#define MAX_DESCRIPTION_CELL 1000
#define NCELLS 200
#define MAX_LINE 1000
#define MAX_FILENAME 30

#define EMPTY _T( ' ' )
#define MAX_THREADS 20

HANDLE hMutexMonster;
HANDLE hMutexEcran;


/*
Estruturas begin
*/

struct Player {
	char namePlayer[MAX_NAME];
	int energyPlayer;
int damage;
int critic;
int cellPlayer;
int itemPlayer;
int treasurePlayer;
};

struct Monster {
	char nameMosnter[MAX_NAME];
	int lifeMonster;
	int damageMonster;
	int criticMonster;
	int cellMonster;
	int itemMonster;
	int treasureMonster;
	int nMonsters;
};

struct Item {
	int CodItem;
	char NameItem[MAX_NAME];
	int DamageItem;
	int CriticItem;
	int PositionItem;
	int LifeItem;
};

struct Tresure {
	int CodTresure;
	char NameTreasure[MAX_NAME];
	int Gold;
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
	struct Item item[MAX_ITEMS];
	struct Tresure treasure[MAX_TREASURE];

	int nCells;
};

struct SaveGame {
	struct Monster saveMonster;
};

struct Threads {
	struct Monster monsters[MAX_MOSNTERS];
	struct Player Player;
	struct Map map;
};
/*
Struct END
*/

void FunctionClear(); // esta fun��o limpa o ecr�
void PrintToConsole(char text[]);
void InsertPlayer(Player *pPlayer);/*Fun�a que permite o utilizador inserir o seu avatar*/
void PrintPlayer(Player *pPlayer);/*mostra os status do avatar no ecr�*/

void InicializeMonster(Monster monster[]);// inicializa varios monstros no jogo
void PrintMonster(Monster monster[]);/*mostra os status do monstro no ecr�*/

void LoadMapFromFile(Map *pMap); // carrega o mapa do jogo de um ficheiro txt
void PrintMapFromFile(Map *pMap); // faz o print de todas as salas do jogo
void InitObejectItem(Map *pMap); // carrega os objectos de um ficheiro txt e transforma os em ficheiro bin
void InitObejectItemBin(Map *pMap); // carrega os objectos de um ficheiro bin
void InitObejectTreasure(Map *pMap); //carrega os tesouros de ficheiro em txt

void PlayerWalk(Player *pPlayer, Map *pMap, Monster monster[]);/*~Fun��o que cria o mapa do jogo*/
void MonstersWalk(Player *pPlayer, Map *pMap, Monster monster[]);/*Fun��o que permite o monstro se mover pelo mapa sozinho*/
void Battle(Player *pPlayer, Map *pMap, Monster monster[]);// Batalha entre os monstros e o jogador
void EndGame(Player *pPlayer, Monster monster[], Map *pMap);/*Fun��o que determina quando o jogo acaba*/

void SaveGame(Player *pPlayer, Monster monster[]); // garda o jogo num ficheiro em bin�rio
void LoadGame(Player *pPlayer, Monster monster[]); // carrega o jogo de um ficheiro em bin�rio
void UpdateThreads(struct Player *pPlayer, struct Monster monster[], struct Map *pMap, struct Threads *pThreads, int controller);


DWORD WINAPI ThreadMovePlayer(LPVOID lpParam)
{
	do
	{
		PlayerWalk(&((struct Threads *) lpParam)->Player, &((struct Threads *) lpParam)->map, ((struct Threads *) lpParam)->monsters);
	} while (true);

	return 0;
}


DWORD WINAPI ThreadMoveMonsters(LPVOID lpParam)
{
	time_t t;
	srand((unsigned)time_t(&t));		// inicializa o random number generator

	do
	{
		MonstersWalk(&((struct Threads *) lpParam)->Player, &((struct Threads *) lpParam)->map, ((struct Threads *) lpParam)->monsters);
		Sleep(1000 + (rand() % 20000));	} while (true);
	return 0;
}

/*THREADS END*/
/*
---------- Hunter Hallow --------
O main � onde as fun��es principais s�o chamados
*/
int main()
{
	int i;
	int countThreads = 0;

	hMutexMonster = CreateMutex(
		NULL,                       // default security attributes
		FALSE,                      // initially not owned
		NULL);                      // unnamed mutex

	hMutexEcran = CreateMutex(
		NULL,                       // default security attributes
		FALSE,                      // initially not owned
		NULL);                      // unnamed mutex


	HANDLE hThreadPlayer;
	HANDLE hThreadMonster;

	struct Player player;
	struct Monster monster[MAX_MOSNTERS];
	struct Cell cells[MAX_CELLS];
	struct Map map;
	struct Threads threads;
	int nCells;

	printf("--------------------------------\n");
	printf("		HUNTER HALLOW			\n");
	printf("		First Episode			\n");
	printf("--------------------------------\n");
	printf("\n");


	int option = 0;
	printf("Pretende iniciar um novo desafio soldado? \n");
	printf(" 1 - Novo Jogo     2 - Continuar Jogo     3 - Sair do Jogo\n");
	scanf("%d", &option);

	switch (option) {
	case 1:
		InsertPlayer(&player);
		PrintPlayer(&player);
		InicializeMonster(monster);
		//PrintMonster(monster);

		break;

	case 2:
		LoadGame(&player, monster);
		PrintPlayer(&player);
		break;

	case 3:
		exit(0);
		break;
	default:
		printf("Soldado estas a dormir e n�o percebeste as instru��es vou explicar de uma maneira mais facil se for possivel\n apenas pode iserir numeros de 1 a 3\n");
		break;
	}

	LoadMapFromFile(&map);
	//PrintMapFromFile(&map);
	InitObejectItemBin(&map);
	InitObejectTreasure(&map);

	UpdateThreads(&player, monster, &map, &threads, 0);

	hThreadPlayer = CreateThread(
		NULL,              // default security attributes
		0,                 // use default stack size
		ThreadMovePlayer,        // thread function
		&threads,             // argument to thread function
		0,                 // use default creation flags
		NULL);   // returns the thread identifier

	hThreadMonster = CreateThread(
		NULL,              // default security attributes
		0,                 // use default stack size  
		ThreadMoveMonsters,        // thread function 
		&threads,             // argument to thread function 
		0,                 // use default creation flags 
		NULL);   // returns the thread identifier


	while (true) {				//player.cellPlayer <= (map.nCells + 1
		//PlayerWalk(&player, &map, monster);
		//MonstersWalk(&player, &map, monster);

		//WaitForSingleObject(hThreadPlayer, INFINITE);
		//WaitForSingleObject(hThreadMonster, INFINITE);
		UpdateThreads(&player, monster, &map, &threads, 1);
		Battle(&player, &map, monster);
		EndGame(&player, monster, &map);
		//UpdateThreads(&player, monster, &map, &threads, 0);


			if (monster[5].lifeMonster <= 0 && monster[6].lifeMonster <= 0 && monster[7].lifeMonster <= 0 && monster[8].lifeMonster <= 0) {
				//CloseHandle(hThreadPlayer);
				WaitForSingleObject(hThreadMonster, INFINITE);
				CloseHandle(hThreadMonster);
				//CloseHandle(hMutexMonster);
				//CloseHandle(hMutex);
				//CloseHandle(hMutexEcran);
			}

		}


		CloseHandle(hThreadPlayer);
		CloseHandle(hThreadMonster);

		CloseHandle(hMutexMonster);
		CloseHandle(hMutexEcran);

		return 0;
	}

	/*
	Fun��o que tem como objetivo limpar a consola com se fosse um "System("cls")"
	esta fun��o foi retirado de um dos documentos de apoio do Professor Luis Garcia
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
		/*limpa a formata��o*/
		FillConsoleOutputAttribute(hStdout, strConsoleInfo.wAttributes,
			strConsoleInfo.dwSize.X * strConsoleInfo.dwSize.Y, Home, &hWrittenChars);

		SetConsoleCursorPosition(hStdout, Home);

	}


	void PrintToConsole(char text[]) {
		WaitForSingleObject(hMutexEcran, INFINITE);
		printf("%s", text);
		fflush(stdout);
		ReleaseMutex(hMutexEcran);
	}

	/*
	Esta fun��o inicializa o avatar do jogador no jogo,
	pondendo tambem definir o modo de jogo e a dificuldade do jogo
	*/
	void InsertPlayer(struct Player *pPlayer) {
		printf("\n");
		printf("Soldado insere o teu nome! \n");
		scanf("%s", pPlayer->namePlayer);

		if ((strcmp(pPlayer->namePlayer, "SU") == 0) || (strcmp(pPlayer->namePlayer, "su") == 0) ||
			(strcmp(pPlayer->namePlayer, "super user") == 0) || (strcmp(pPlayer->namePlayer, "Super User") == 0)) {

			// selecionar modo de jogo / dificuldade
			pPlayer->energyPlayer = 1000000;
			pPlayer->damage = 10000;
			pPlayer->critic = 200;
			pPlayer->itemPlayer = 5;
			pPlayer->cellPlayer = 0;
			pPlayer->treasurePlayer = 5;
		}
		else {

			// selecionar modo de jogo / dificuldade
			pPlayer->energyPlayer = 200;
			pPlayer->damage = 60;
			pPlayer->critic = 20;
			pPlayer->itemPlayer = 0;
			pPlayer->cellPlayer = 0;
			pPlayer->treasurePlayer = 0;
		}
	}

	/* Mostrar Jogador
	Esta fun�ao tem como fun��o principal mostrar os status do jogador no ecr�
	*/
	void PrintPlayer(struct Player *pPlayer) {
		printf("\n");
		printf("Bem vindo Hunter: %s \n", pPlayer->namePlayer);
		printf("A tua Vida:  %d \n", pPlayer->energyPlayer);
		printf("Os teus Items:  %d \n", pPlayer->itemPlayer);
		printf("A tua localisa��o:  %d \n", pPlayer->cellPlayer);
		printf("Dinheiro:  %d \n", pPlayer->treasurePlayer);
		printf("Estas pronto para o desafio Soldado !!!\n");

	}

	/*
	Nesta fun��o s�o inicializados os monstros do jogo
	*/
	void InicializeMonster(struct Monster monster[]) {
		int count = 0;
		//Monstro Principal - s� depois de ser derrotado � que o jogador ganha
		monster[0].cellMonster = 13;
		strcpy(monster[0].nameMosnter, "LUCIFER");
		monster[0].damageMonster = 60;
		monster[0].lifeMonster = 1000;
		monster[0].criticMonster = 70;
		monster[0].itemMonster = 5;
		monster[0].treasureMonster = 5;
		count++;

		//Monstro da cell 2
		monster[1].cellMonster = 2;
		strcpy(monster[1].nameMosnter, "AZREL");
		monster[1].damageMonster = 10;
		monster[1].lifeMonster = 100;
		monster[1].criticMonster = 10;
		monster[1].itemMonster = 1;
		monster[1].treasureMonster = 1;
		count++;

		//Monstro da cell 3
		monster[2].cellMonster = 3;
		strcpy(monster[2].nameMosnter, "HERCULES");
		monster[2].damageMonster = 20;
		monster[2].lifeMonster = 200;
		monster[2].criticMonster = 20;
		monster[2].itemMonster = 2;
		monster[2].treasureMonster = 2;
		count++;

		//Monstro da cell 8
		monster[3].cellMonster = 8;
		strcpy(monster[3].nameMosnter, "JACLINE");
		monster[3].damageMonster = 30;
		monster[3].lifeMonster = 300;
		monster[3].criticMonster = 30;
		monster[3].itemMonster = 3;
		monster[3].treasureMonster = 3;
		count++;

		//Monstro da cell 9
		monster[4].cellMonster = 9;
		strcpy(monster[4].nameMosnter, "HADES");
		monster[4].damageMonster = 40;
		monster[4].lifeMonster = 500;
		monster[4].criticMonster = 4;
		monster[4].itemMonster = 4;
		monster[4].treasureMonster = 4;
		count++;

		//Monstros lv1
		monster[5].cellMonster = 10;
		strcpy(monster[5].nameMosnter, "ESCLETO 1");
		monster[5].damageMonster = 10;
		monster[5].lifeMonster = 100;
		monster[5].criticMonster = 10;
		monster[5].itemMonster = 0;
		monster[5].treasureMonster = 1;
		count++;

		//Monstros lv1
		monster[6].cellMonster = 10;
		strcpy(monster[6].nameMosnter, "ESCLETO 2");
		monster[6].damageMonster = 10;
		monster[6].lifeMonster = 100;
		monster[6].criticMonster = 10;
		monster[6].itemMonster = 0;
		monster[6].treasureMonster = 1;
		count++;


		//Monstros lv1
		monster[7].cellMonster = 10;
		strcpy(monster[7].nameMosnter, "ESCLETO 3");
		monster[7].damageMonster = 10;
		monster[7].lifeMonster = 100;
		monster[7].criticMonster = 10;
		monster[7].itemMonster = 0;
		monster[7].treasureMonster = 1;
		count++;


		//Monstros lv1
		monster[8].cellMonster = 10;
		strcpy(monster[8].nameMosnter, "ESCLETO 4");
		monster[8].damageMonster = 10;
		monster[8].lifeMonster = 100;
		monster[8].criticMonster = 10;
		monster[8].itemMonster = 0;
		monster[8].treasureMonster = 1;
		count++;

		monster[0].nMonsters = count;
	}

	/*
	Este fun��o vai fazer o print de todos os monstros no jogo
	*/
	void PrintMonster(struct Monster monster[]) {
		for (int i = 0; i < monster[0].nMonsters; i++) {
			printf("\n");
			printf("Monstro %s\n", monster[i].nameMosnter);
			printf("Monstro HP: %d\n", monster[i].lifeMonster);
			printf("Monstro ATK: %d\n", monster[i].damageMonster);
			printf("Monstro CTRITIC: %d\n", monster[i].criticMonster);
			printf("Monstro ROOM: %d\n", monster[i].cellMonster);
			printf("Monstro ITEM: %d\n", monster[i].itemMonster);
			printf("Monstro TREASURE: %d\n", monster[i].treasureMonster);
		}
	}


	/*
	Nesta fun��o inicializa os objectos do ficheiro txt e transforma os num ficheiro bin
	*/
	void InitObejectItem(struct Map *pMap) {
		FILE *f, *f2;
		char l[MAX_LINE];
		int line = 1;
		int i = 0;
		int count = 0;

		int id, position, damage, critic, hp;

		f = fopen("items.txt", "r");
		f2 = fopen("objectos.dat", "w");

		while (fgets(l, MAX_LINE, f) != NULL) {
			if (line == 1 && strcmp(l, "\n") != 0) {
				sscanf(l, "%d %d %d %d %d", &id, &position, &damage, &critic, &hp);

				pMap->item[i].CodItem = id;
				pMap->item[i].PositionItem = position;
				pMap->item[i].DamageItem = damage;
				pMap->item[i].CriticItem = critic;
				pMap->item[i].LifeItem = hp;

				line++;

			}
			else if (strcmp(l, "\n") != 0 && l != " ") {
				strcpy(pMap->item[i].NameItem, l);
				line = 1;
				i++;
				count++;
			}
			else {
				//do Nothing
			}

		}


		for (int i = 0; i < count; i++) { // escrever fich bin

			fwrite(&pMap->item[i], sizeof(struct Item), 1, f2);
		}

		fclose(f);
		fclose(f2);
	}

	/*
	Nesta fun��o incializa-se os itens de um ficheiro binario
	*/
	void InitObejectItemBin(struct Map *pMap) {
		struct Item item;
		FILE *f;
		char l[MAX_LINE];
		int i = 0;
		f = fopen("objectos.dat", "r");

		while (((fread(&item, sizeof(struct Item), 1, f)) > 0) != NULL) {
			pMap->item[i].CodItem = item.CodItem;
			pMap->item[i].PositionItem = item.PositionItem;
			pMap->item[i].DamageItem = item.DamageItem;
			pMap->item[i].CriticItem = item.CriticItem;
			pMap->item[i].LifeItem = item.LifeItem;
			strcpy(pMap->item[i].NameItem, item.NameItem);
			i++;
		}

		fclose(f);
	}

	/*
	Esta fun��o l� os tesouros existentes no mapa do ficheiro txt
	*/
	void InitObejectTreasure(struct Map *pMap) {
		FILE *f;
		char l[MAX_LINE];
		int line = 1;
		int i = 0;
		int count = 0;

		int id, position, gold;

		f = fopen("treasures.txt", "r");


		while (fgets(l, MAX_LINE, f) != NULL) {
			if (line == 1 && strcmp(l, "\n") != 0) {
				sscanf(l, "%d %d %d", &id, &position, &gold);

				pMap->treasure[i].CodTresure = id;
				pMap->treasure[i].PositionTresure = position;
				pMap->treasure[i].Gold = gold;


				line++;

			}
			else if (strcmp(l, "\n") != 0 && l != " ") {
				strcpy(pMap->treasure[i].NameTreasure, l);
				line = 1;
				i++;
				count++;
			}
			else {
				//do Nothing
			}

		}

		fclose(f);
	}

	/*
	Esta fun��o carrega o mapa do jogo de um ficheiro com o nome "map.txt"
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

				line++;

			}
			else if (strcmp(l, "\n") != 0 && l != " ") {
				strcpy(pMap->cell[i].descriptionCell, l);
				line = 1;
				i++;
				count++;
			}
			else {
				//do Nothing
			}

		}
		pMap->nCells = count;
		fclose(f);
	}

	/*
	Esta fun��o � o que permite o jogador navegar no mapa e usar algumas das fun�oes implementadas
	*/
	void PlayerWalk(struct Player *pPlayer, struct Map *pMap, struct Monster monster[]) {

		int option;
		//FunctionClear();
		WaitForSingleObject(hMutexEcran, INFINITE);
		printf("\n");
		printf(" --------------------------------------------------------------------\n");
		printf(" Para andar na aldeia seleciona uma das op��es abaixo Soldado\n");
		printf("1 - Norte  2 - Sul  3 - Oeste  4 - Este  5 - Subir 6 - Descer 7 - Save  8 - Menu Principal\n");
		printf(" --------------------------------------------------------------------\n");
		printf("\n");
		printf("Posi�ao atual: %d\n", pPlayer->cellPlayer);
		printf("\n");
		printf("%s", pMap->cell[pPlayer->cellPlayer].descriptionCell);
		printf("\n");
		printf("Escolhe a dire��o que queres soldado: \n");
		ReleaseMutex(hMutexEcran);
		scanf("%d", &option);
		
		//FunctionClear();
		switch (option)
		{
		case 1:
			if (pMap->cell[pPlayer->cellPlayer].north == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else {
				pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].north;
			}
			break;
		case 2:
			if (pMap->cell[pPlayer->cellPlayer].south == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].south; }
			break;
		case 3:
			if (pMap->cell[pPlayer->cellPlayer].west == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].west; }
			break;
		case 4:
			if (pMap->cell[pPlayer->cellPlayer].east == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].east; }
			break;
		case 5:
			if (pMap->cell[pPlayer->cellPlayer].up == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].up; }
			break;
		case 6:
			if (pMap->cell[pPlayer->cellPlayer].down == -1) {
				PrintToConsole("Lamento mas nao podes atravesar paredes !!!");
			}
			else { pPlayer->cellPlayer = pMap->cell[pPlayer->cellPlayer].down; }
			break;
		case 7:
			SaveGame(pPlayer, monster);
			printf("-----------------------------------\n");
			printf("O Jogo foi guardado com sucesso\n");

			break;

		case 8:
			main();
			break;
		default:
			printf("O valor introduzido � invalido!!! \n Insira novamente um numero de 1 a 8 \n");
			break;
		}

	}

	/*
	Esta Fun��o faz com que os monstros do indice 5 a 7 andem pelo mapa de forma aleat�ria e sendo estes escolhidos de forma aleat�ria
	*/
	void MonstersWalk(struct Player *pPlayer, struct Map *pMap, struct Monster monster[]) {
		time_t t;
		srand((unsigned)time_t(&t));
		int randMoveMonster = rand() % 6;
		int randMonster =  rand() % 8;
		while ((randMonster < 5) && (randMonster > 8)) {
			randMonster++;
			randMonster = rand() % 8;
		}


		if ((strcmp(pPlayer->namePlayer, "SU") == 0) || (strcmp(pPlayer->namePlayer, "su") == 0) ||
			(strcmp(pPlayer->namePlayer, "super user") == 0) || (strcmp(pPlayer->namePlayer, "Super User") == 0)) {
			switch (randMoveMonster)
			{
			case 1:
				if (pMap->cell[monster[randMonster].cellMonster].north == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].north;
					WaitForSingleObject(hMutexEcran, INFINITE);
					printf("\n");
					printf("Monstro: %s\n", monster[randMonster].nameMosnter);
					printf("Monstro foi para a sala: %d\n", monster[randMonster].cellMonster);
					ReleaseMutex(hMutexEcran);
				}
				break;
			case 2:
				if (pMap->cell[monster[randMonster].cellMonster].south == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].south;
					WaitForSingleObject(hMutexEcran, INFINITE);
					printf("\n");
					printf("Monstro: %s\n", monster[randMonster].nameMosnter);
					printf("Monstro foi para a sala: %d\n", monster[randMonster].cellMonster);
					ReleaseMutex(hMutexEcran);
				}
				break;
			case 3:
				if (pMap->cell[monster[randMonster].cellMonster].west == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].west;
					WaitForSingleObject(hMutexEcran, INFINITE);
					printf("\n");
					printf("Monstro: %s\n", monster[randMonster].nameMosnter);
					printf("Monstro foi para a sala: %d\n", monster[randMonster].cellMonster);
					ReleaseMutex(hMutexEcran);
				}
				break;
			case 4:
				if (pMap->cell[monster[randMonster].cellMonster].east == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].east;
					WaitForSingleObject(hMutexEcran, INFINITE);
					printf("\n");
					printf("Monstro: %s\n", monster[randMonster].nameMosnter);
					printf("Monstro foi para a sala: %d\n", monster[randMonster].cellMonster);
					ReleaseMutex(hMutexEcran);
				}
				break;
			case 5:
				if (pMap->cell[monster[randMonster].cellMonster].up == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				break;
			case 6:
				if (pMap->cell[monster[randMonster].cellMonster].down == -1) {
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				else
				{
					PrintToConsole("Monstro n�o se moveu!\n");
				}
				break;
			default:
				break;
			}
		}
		else {
			switch (randMoveMonster)
			{
			case 1:
				if (pMap->cell[monster[randMonster].cellMonster].north == -1) {

				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].north;

				}
				break;
			case 2:
				if (pMap->cell[monster[randMonster].cellMonster].south == -1) {

				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].south;

				}
				break;
			case 3:
				if (pMap->cell[monster[randMonster].cellMonster].west == -1) {

				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].west;

				}
				break;
			case 4:
				if (pMap->cell[monster[randMonster].cellMonster].east == -1) {

				}
				else {
					monster[randMonster].cellMonster = pMap->cell[monster[randMonster].cellMonster].east;

				}
				break;
			case 5:
				if (pMap->cell[monster[randMonster].cellMonster].up == -1) {

				}
				else {

				}
				break;
			case 6:
				if (pMap->cell[monster[randMonster].cellMonster].down == -1) {
				}
				else
				{

				}
				break;
			default:
				break;
			}
		}
		randMoveMonster++;
		randMonster++;
	}

	/*
	Nesta Fun��o o jogador batalha com um monstro ou varios caso exista algum monstro na sua cela
	*/
	void Battle(struct Player *pPlayer, struct Map *pMap, struct Monster monster[]) {
		time_t t;
		srand((unsigned)time_t(&t));

		int randCriticPlayer = 0; // � sorteado um valor de ataque critico
		int randAtkPlayer = 0; //O Player acerta o ataque ou falha
		int newatkPlayer = 0; //Novo valor de ataque do jogador

		int randCriticMonster = 0; // � sorteado um valor de ataque critico
		int randAtkMonster = 0; //O Monster acerta o ataque ou falha
		int newatkMonster = 0; //Novo valor de ataque do monstro

		for (int i = 0; i < monster[0].nMonsters; i++) {// enquanto houver monstros na sala o jodador vai lutando contra eles

			while (pPlayer->cellPlayer == monster[i].cellMonster && pPlayer->energyPlayer > 0 && monster[i].lifeMonster > 0 && monster[i].cellMonster == pPlayer->cellPlayer) {
				randAtkPlayer = rand() % 6;
				randCriticPlayer = rand() % pPlayer->critic;
				newatkPlayer = ((pPlayer->damage * randCriticPlayer) / 100);
				newatkPlayer = newatkPlayer + pPlayer->damage;


				if (randAtkPlayer > 3 && pPlayer->energyPlayer > 0) {// o jogador ataca
					monster[i].lifeMonster = monster[i].lifeMonster - newatkPlayer;
					printf("\n");
					printf("Monstro: %s\n", monster[i].nameMosnter);//o jogador falha o ataque
					printf("Dano tirado ao Monstro: %d\n", newatkPlayer);
					printf("HP atual do Monstro: %d\n", monster[i].lifeMonster);
				}
				else {
					if (pPlayer->energyPlayer > 0) {
						printf("Parabens acabaste de falar completamente o ATAQUE!!!\n");
					}
					else {
						//do nothing
					}
				}

				randAtkMonster = rand() % 6;
				randCriticMonster = rand() % monster[i].criticMonster;
				newatkMonster = ((monster[i].damageMonster * randCriticMonster) / 100);
				newatkMonster += monster[i].damageMonster;


				if (randAtkMonster > 3 && monster[i].lifeMonster > 0) { // o mosntro ataca
					pPlayer->energyPlayer -= newatkMonster;
					printf("\n");
					printf("Soldado: %s\n", pPlayer->namePlayer);
					printf("Dano tirado pelo Monstro: %d\n", newatkMonster);
					printf("HP: %d\n", pPlayer->energyPlayer);
				}
				else {
					if (monster[i].lifeMonster > 0) {// o monstro falhou o ataque
						printf("Para tua sorte Soldado o Monstro %s", monster[i].nameMosnter);
						printf(" acabou de falhar o ataque\n");
					}
					else { //Quando a vida do monstro for menor ou igual 0 o jogador podera receber os items que est�o na sala
						int itemSelect = pMap->cell[pPlayer->cellPlayer].itemCell; // esta variavel guarda o valor do item que se encontra na mesma sala do jogador
						int treasureSelected = pMap->cell[pPlayer->cellPlayer].treasureCell; //esta variaver guarda o valor do tesouro que se encontra na mesma sala do jogador


						if (itemSelect != -1) { // jogador apanha item
							pPlayer->damage = pPlayer->damage + pMap->item[itemSelect].DamageItem;
							pPlayer->critic = pPlayer->critic + pMap->item[itemSelect].CriticItem;
							pPlayer->energyPlayer = pPlayer->energyPlayer + pMap->item[itemSelect].LifeItem;

							printf("Item Adicionado: %s", pMap->item[itemSelect].NameItem);
							printf("Dano Adicionado: %d", pMap->item[itemSelect].DamageItem);
							printf("Dano critico Adicionado: %d", pMap->item[itemSelect].CriticItem);
							printf("HP Adicionado: %d", pMap->item[itemSelect].LifeItem);
						}
						if (treasureSelected != -1) {// jogador apanha tesouro
							pPlayer->treasurePlayer = pPlayer->treasurePlayer + pMap->treasure[treasureSelected].Gold;
							printf("\n");
							printf("Tesouro encontrado: %s\n", pMap->treasure[treasureSelected].NameTreasure);
							printf("Gold Adicionado: %d\n", pMap->treasure[treasureSelected].Gold);
						}
					}
				}
			} // end while
		} // end for
	}

	/*
	Esta fun��o guarda o jogo num ficheiro bin�rio em que o nume � atribuido pelo utilizador
	*/
	void SaveGame(struct Player *pPlayer, struct Monster monster[]) {
		struct SaveGame save;
		FILE *f;
		char fileName[MAX_FILENAME];
		printf("Insere um nome para o ficheiro: \n");
		scanf("%s", fileName);
		//fgets(fileName, MAX_NAME, stdin);
		fileName[strlen(fileName) - 1] = '\0';
		strcat(fileName, ".bin");

		f = fopen(fileName, "wb");

		fwrite(pPlayer, sizeof(struct Player), 1, f);
		for (int i = 0; i < monster[0].nMonsters; i++) {
			save.saveMonster.cellMonster = monster[i].cellMonster;
			save.saveMonster.criticMonster = monster[i].criticMonster;
			save.saveMonster.damageMonster = monster[i].damageMonster;
			save.saveMonster.itemMonster = monster[i].itemMonster;
			save.saveMonster.lifeMonster = monster[i].lifeMonster;
			strcpy(save.saveMonster.nameMosnter, monster[i].nameMosnter);
			save.saveMonster.treasureMonster = monster[i].treasureMonster;
			fwrite(&save, sizeof(struct SaveGame), 1, f);
		}
		fclose(f);
	}

	/*
	Esta fun��o carrega o jogo de um ficheiro bin�rio, o utilizador pode escolher qual � o save que quer carregar
	*/
	void LoadGame(struct Player *pPlayer, struct Monster monster[]) {
		struct SaveGame save;
		FILE *f;
		char fileName[MAX_FILENAME];
		printf("Insere o nome do savefile que pretendes continuar: \n");
		//fgets(fileName, MAX_NAME, stdin);
		scanf("%s", fileName);
		strcat(fileName, ".bin");

		f = fopen(fileName, "rb");

		if (f == NULL) {
			printf("N�o existe nenhum jogo com esse nome!");
			LoadGame(pPlayer, monster);
		}
		else {
			fread(pPlayer, sizeof(struct Player), 1, f);
			for (int i = 0; i < monster[0].nMonsters; i++) {
				fread(&save, sizeof(struct SaveGame), 1, f);
				monster[i].cellMonster = save.saveMonster.cellMonster;
				monster[i].criticMonster = save.saveMonster.criticMonster;
				monster[i].damageMonster = save.saveMonster.damageMonster;
				monster[i].itemMonster = save.saveMonster.itemMonster;
				monster[i].lifeMonster = save.saveMonster.lifeMonster;
				strcpy(monster[i].nameMosnter, save.saveMonster.nameMosnter);
				monster[i].treasureMonster = save.saveMonster.treasureMonster;
			}
			fclose(f);
		}
	}

	/*
	Esta fun��o verifica se o jogador ganha o jogo o se o jodador morre e mostra as mensagens correspondentes
	*/
	void EndGame(struct Player *pPlayer, struct Monster monster[], struct Map *pMap) {
		for (int i = 0; i < monster[0].nMonsters; i++) {
			if (strcmp(monster[i].nameMosnter, "LUCIFER") == 0 && pPlayer->energyPlayer > 0 && monster[i].lifeMonster <= 0) {
				FunctionClear();
				printf("Muitos Parabens soldado acabaste de derrotar rei dos monstros e salvaste a vila da destrui��o\n");
				printf("Agora que recuperaste a lendaria armadura mais a lendaria espada das maos do %s\n", monster[i].nameMosnter);
				printf("Por este feito decidimos-te prover a Class B soldado %s\n", pPlayer->namePlayer);
				printf("Aconselho-te a n�o tirar muito tempo de ferias, estou com sentimento que em breve vamos precisar de ti ... \n");
				printf("\n");
				printf("---------------------------------------------------------------------------------------------------------------------\n");
				printf("\n");
				printf("Jogo desenvolvido por: \n");
				printf("Rute Figeiredo\n");
				printf("Tiago Campos\n");
				printf("\n");
			}

			if (pPlayer->energyPlayer < 0) {
				FunctionClear();
				printf("O Soldado %s", pPlayer->namePlayer);
				printf(" foi um soldado exemplar, mas ficou demasiado convencido e acabou por se descuidar\n");
				printf("e isso foi o seu fim -_-\n");
				printf("R.I.P : &s", pPlayer->namePlayer);
				main();
			}
		}
	}


	/*
		Esta fun��o passa os dados da estrutura das threads para as estruturas do player, do monstro e do mapa
		e vice ver�a
	*/
	void UpdateThreads(struct Player *pPlayer, struct Monster monster[], struct Map *pMap, struct Threads *pThreads, int controller) {

		if (controller == 0) {
			for (int i = 0; i < monster[0].nMonsters; i++) {

				strcpy(pThreads->monsters[i].nameMosnter, monster[i].nameMosnter);
				pThreads->monsters[i].cellMonster = monster[i].cellMonster;
				pThreads->monsters[i].criticMonster = monster[i].criticMonster;
				pThreads->monsters[i].damageMonster = monster[i].damageMonster;
				pThreads->monsters[i].itemMonster = monster[i].itemMonster;
				pThreads->monsters[i].lifeMonster = monster[i].lifeMonster;
				pThreads->monsters[i].treasureMonster = monster[i].treasureMonster;
				pThreads->monsters[i].nMonsters = monster[0].nMonsters;
			}

			for (int i = 0; i < pMap->nCells; i++) { // para passar os dados do mapa para a estrutura de estruturas
				pThreads->map.cell[i].north = pMap->cell[i].north;
				pThreads->map.cell[i].south = pMap->cell[i].south;
				pThreads->map.cell[i].west = pMap->cell[i].west;
				pThreads->map.cell[i].east = pMap->cell[i].east;
				pThreads->map.cell[i].up = pMap->cell[i].up;
				pThreads->map.cell[i].down = pMap->cell[i].down;
				strcpy(pThreads->map.cell[i].descriptionCell, pMap->cell[i].descriptionCell);
			}

			//  add data from player
			pThreads->Player = *pPlayer;
		}
		else if (controller == 1) {
			for (int i = 0; i < monster[0].nMonsters; i++) {

				strcpy(monster[i].nameMosnter, pThreads->monsters[i].nameMosnter);
				monster[i].cellMonster = pThreads->monsters[i].cellMonster;
				monster[i].criticMonster = pThreads->monsters[i].criticMonster;
				monster[i].damageMonster = pThreads->monsters[i].damageMonster;
				monster[i].itemMonster = pThreads->monsters[i].itemMonster;
				monster[i].lifeMonster = pThreads->monsters[i].lifeMonster;
				monster[i].treasureMonster = pThreads->monsters[i].treasureMonster;
				monster[0].nMonsters = pThreads->monsters[i].nMonsters;
			}

			for (int i = 0; i < pMap->nCells; i++) { // para passar os dados do mapa para a estrutura de estruturas
				pMap->cell[i].north = pThreads->map.cell[i].north;
				pMap->cell[i].south = pThreads->map.cell[i].south;
				pMap->cell[i].west = pThreads->map.cell[i].west;
				pMap->cell[i].east = pThreads->map.cell[i].east;
				pMap->cell[i].up = pThreads->map.cell[i].up;
				pMap->cell[i].down = pThreads->map.cell[i].down;
				strcpy(pMap->cell[i].descriptionCell, pThreads->map.cell[i].descriptionCell);
			}

			//  add data from player
			*pPlayer = pThreads->Player;
		}

		//  add data from player
		pThreads->Player = *pPlayer;
	}

