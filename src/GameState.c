#include "../head/GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
GameState state = {.map = NULL, .size = 0};

int** P1;
int** P2;
int nP1case;
int nP2case;

void create_empty_game_state (GameState* state, int size){
	state->size=size;
	state->map=malloc(size*size*sizeof(Color));
	P1=malloc(size*size*sizeof(int*));
	P2=malloc(size*size*sizeof(int*));
	nP1case=0;
	nP2case=0;

}
void set_map_value (GameState* state, int x, int y, Color value){
	if (state -> map == NULL || x > state -> size || y > state -> size || x < 0 || y < 0)
	{
		//printf("[ERROR SET] map not big enough or not initialized %p %i access (%i %i)\n", state -> map, state -> size, x, y);
		printf("e");
		return;
	}
	if (value==PLAYER_1)
	{
		int* coordonne=malloc(2*sizeof(int));
		coordonne[0]=x;
		coordonne[1]=y;
		P1[nP1case]=coordonne;
		nP1case+=1;
	}
	if (value==PLAYER_2)
	{
		int* coordonne=malloc(2*sizeof(int));
		coordonne[0]=x;
		coordonne[1]=y;
		P2[nP2case]=coordonne;
		nP2case+=1;
	}
	
	state -> map[y * state -> size + x]=value;
}
Color get_map_value (GameState* state, int x, int y){
	if (state -> map == NULL || x >= state -> size || y >= state -> size || x < 0 || y < 0)
	{
		//printf("[ERROR GET] map not big enough or not initialized %p %i access (%i %i)\n", state -> map, state -> size, x, y);
		printf("f");
		return ERROR;
	}
	return state -> map[y * state -> size + x];
}
void fill_map(GameState* map){
	//On parcours la map et on la remplie de couleur aléatoir
	int color_of_square;
	for(int y=0;y<map->size;y++){
		for(int x=0;x<map->size;x++){
			color_of_square=rand()%7;
			switch (color_of_square)
			{
			case 0:
				set_map_value (map, x, y, GREEN);
				break;
			case 1:
				set_map_value (map, x, y, BLUE);
				break;
			case 2:
				set_map_value (map, x, y, RED);
				break;
			case 3:
				set_map_value (map, x, y, YELLOW);
				break;
			case 4:
				set_map_value (map, x, y, MAGENTA);
				break;
			case 5:
				set_map_value (map, x, y, CYAN);
				break;
			case 6:
				set_map_value (map, x, y, WHITE);
				break;
			}
			
		}
	}
	//On met les joueurs aux position 
	set_map_value (map, 0, map->size-1, PLAYER_1);
	set_map_value (map, map->size-1, 0, PLAYER_2);
}


char color_to_char(Color color){
	char car;
	switch (color)
			{
			case GREEN:
				car='G';
				break;
			case BLUE:
				car='B';
				break;
			case PLAYER_1:
				car='1';
				break;
			case PLAYER_2:
				car='2';
				break;
			case RED:
				car='R';
				break;
			case YELLOW:
				car='Y';
				break;
			case MAGENTA:
				car='M';
				break;
			case CYAN:
				car='C';
				break;
			case WHITE:
				car='W';
				break;
			default:
				car='0';
				break;
			}
	

	return car;
}

int are_x_and_y_ok(int x, int y,int size){
	if( x >= size || y >= size || x < 0 || y < 0) return 0;
	else return 1;

}

void get_around_square(int**tab,int x,int y){
	
	for (int i=0;i<4;i++) tab[i]=malloc(2*sizeof(int));
	tab[0][0]=x+1;
	tab[0][1]=y;
	tab[1][0]=x-1;
	tab[1][1]=y;
	tab[2][0]=x;
	tab[2][1]=y+1;
	tab[3][0]=x;
	tab[3][1]=y-1;
}


void free_tab(int** tab,int size){
	for (int i=0;i<size;i++) free(tab[i]);
	free(tab);
}
void draw_game(GameState* map){
	Color element;
	for(int y=0;y<map->size;y++){
		printf("\n");
		for(int x=0;x<map->size;x++){
			element=get_map_value (map, x, y);
			switch (element)
			{
			case GREEN:
				printf("\e[0;42;1;1m  \e[0m");
				break;
			case BLUE:
				printf("\e[0;44;1;1m  \e[0m");
				break;
			case PLAYER_1:
				printf(" 1");
				break;
			case PLAYER_2:
				printf(" 2");
				break;
			case RED:
				printf("\e[0;41;1;1m  \e[0m");
				break;
			case YELLOW:
				printf("\e[0;43;1;1m  \e[0m");
				break;
			case MAGENTA:
				printf("\e[0;45;1;1m  \e[0m");
				break;
			case CYAN:
				printf("\e[0;46;1;1m  \e[0m");
				break;
			case WHITE:
				printf("\e[0;47;1;1m  \e[0m");
				break;
			default:
				printf("Attention, il y a eu un problème");
				break;
			}
			//printf("");
		}
	}
}

void turn(GameState* map,int player,char choice){
	//on regarde le choix du joueur 
	Color choiceColor;
	if (player==1){
		switch (choice)
			{
			case 'G':
				choiceColor=GREEN;
				break;
			case 'B':
				choiceColor=BLUE;
				break;
			case 'R':
				choiceColor=RED;
				break;
			case 'Y':
				choiceColor=YELLOW;
				break;
			case 'M':
				choiceColor=MAGENTA;
				break;
			case 'C':
				choiceColor=CYAN;
				break;
			case 'W':
				choiceColor=WHITE;
				break;
			default:
				choiceColor=ERROR;
				printf("il y a eu un souci avec le joueur 1");
				break;
			}
		
		//on parcours toutes les cases controlées par P1
		for (int i=0;i<nP1case;i++){
			int currentX=P1[i][0];
			int currentY=P1[i][1];
			//si la case est de la couleur séléctionnée, on la transforme en teritoire de player1 
			if (choiceColor==get_map_value(map,currentX-1,currentY)) set_map_value(map,currentX-1,currentY,PLAYER_1);
			if (choiceColor==get_map_value(map,currentX+1,currentY)) set_map_value(map,currentX+1,currentY,PLAYER_1);
			if (choiceColor==get_map_value(map,currentX,currentY-1)) set_map_value(map,currentX,currentY-1,PLAYER_1);
			if (choiceColor==get_map_value(map,currentX,currentY+1)) set_map_value(map,currentX,currentY+1,PLAYER_1);
		}

	}
	else{
		printf("%c",choice);
		switch (choice)
			{
			case 'G':
				choiceColor=GREEN;
				break;
			case 'B':
				choiceColor=BLUE;
				break;
			case 'R':
				choiceColor=RED;
				break;
			case 'Y':
				choiceColor=YELLOW;
				break;
			case 'M':
				choiceColor=MAGENTA;
				break;
			case 'C':
				choiceColor=CYAN;
				break;
			case 'W':
				choiceColor=WHITE;
				break;
			default:
				choiceColor=ERROR;
				printf("il y a eu un souci avec le joueur 2");
				break;
			}
		
		//on parcours toutes les cases controlées par P1
		for (int i=0;i<nP2case;i++){
			int currentX=P2[i][0];
			int currentY=P2[i][1];
			if (choiceColor==get_map_value(map,currentX-1,currentY)) set_map_value(map,currentX-1,currentY,PLAYER_2);
			if (choiceColor==get_map_value(map,currentX+1,currentY)) set_map_value(map,currentX+1,currentY,PLAYER_2);
			if (choiceColor==get_map_value(map,currentX,currentY-1)) set_map_value(map,currentX,currentY-1,PLAYER_2);
			if (choiceColor==get_map_value(map,currentX,currentY+1)) set_map_value(map,currentX,currentY+1,PLAYER_2);
		}

	}
}

int is_finished(GameState* map){
	if((nP1case+nP2case)==(map->size*map->size)){ 
		if (nP1case>nP2case) return 1;else return 2;
	}
	else return 0;
}

char AI_dumb(GameState* map){
	int choice=rand()%7;
	char choicetoreturn;
	switch (choice)
	{
	case 0:
		choicetoreturn='G';
		break;
	case 1:
		choicetoreturn='W';
		break;
	case 2:
		choicetoreturn='B';
		break;
	case 3:
		choicetoreturn='R';
		break;
	case 4:
		choicetoreturn='C';
		break;
	case 5:
		choicetoreturn='M';
		break;
	case 6:
		choicetoreturn='Y';
		break;
	
	default:
		break;
	}
	return choicetoreturn;
}

int is_in_table(Color* tab,int nelement,Color element){
	int in_table=0;
	for(int i=0;i<nelement;i++){
		if(tab[i]==element){
			in_table+=1;
		}
	}
	return in_table;
}

int are_coordonate_in_table(int** tab,int nelement,int x, int y){
	int ans=0;
	for (int i=0;i<nelement;i++){
		if(tab[i][0]==x && tab[i][1]==y) ans+=1;
	}
	return ans;
}

int where_in_table(Color* tab,int nelement,Color element){
	for(int i=0;i<nelement;i++){
		if(tab[i]==element){
			return i;
		}
	}
	return 0;
	
}

int is_color(Color element){
	int itisacolor=0;
	if (element==GREEN ||element==BLUE ||element==RED ||element==YELLOW ||element==MAGENTA ||element==WHITE ||element==CYAN ) {
		itisacolor=1;
	}
	return itisacolor;
}


char AI_glouton(GameState* map,int player){
	Color color_next_to_me[7]={ERROR,ERROR,ERROR,ERROR,ERROR,ERROR,ERROR};
	int nelement=0;
	Color colorchoice;
	if (player==1){
		//on parcours toutes les cases controlées par P1
		for (int i=0;i<nP1case;i++){
			int currentX=P1[i][0];
			int currentY=P1[i][1];
			//Si la case est une couleur et qu'elle n'est pas encore prise en compte
			if (is_color(get_map_value(map,currentX,currentY))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX,currentY)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX,currentY);
					nelement+=1;
				}
			}
		}
		//On fait un choix aléatoir parmis les couleurs disponible 
		if(nelement==0){
			colorchoice=BLUE;
		}
		else{
			colorchoice=color_next_to_me[rand()%nelement];
		}
		

	}
	else{
		//on parcours toutes les cases controlées par P2
		for (int i=0;i<nP2case;i++){
			int currentX=P2[i][0];
			int currentY=P2[i][1];
			//Si les cases adjacentes sont d'une couleur et qu'elle n'est pas encore prise en compte
			if (is_color(get_map_value(map,currentX+1,currentY))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX+1,currentY)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX+1,currentY);
					nelement+=1;
				}
			}
			
			if (is_color(get_map_value(map,currentX,currentY+1))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX,currentY+1)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX,currentY+1);
					nelement+=1;
				}
			}
			if (is_color(get_map_value(map,currentX-1,currentY))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX-1,currentY)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX-1,currentY);
					nelement+=1;
				}
			}
			if(!is_color(get_map_value(map,currentX,currentY-1)))
			if (is_color(get_map_value(map,currentX,currentY-1))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX,currentY-1)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX,currentY-1);
					nelement+=1;
				}
			}
			
		}
		//On fait un choix aléatoir parmis les couleurs disponible 
		if(nelement==0){
			colorchoice=BLUE;
		}
		else{
			colorchoice=color_next_to_me[rand()%nelement];
		}
		
	}
	return color_to_char(colorchoice);
}

void n_square_of_the_same_color(GameState* map, int x, int y,int** coordonee,int * nelement){
	//on créer un tableau avec toutes les coordonnées des cases de cette couleur, on initialise la première
	//int** coordonee=malloc(sizeof(int*)*map->size*map->size);
	for(int i=0;i<map->size*map->size;i++){
		coordonee[i]=malloc(sizeof(int)*2);
	}
	//int nelement=1;
	coordonee[0][0]=x;
	coordonee[0][1]=y;
	Color color=get_map_value(map,coordonee[0][0],coordonee[0][1]);

	//on parcours ce tableau
	for (int i=0;i<*nelement;i++){
		//on parcours les cases adjacentes 
		int **tab;
		tab=malloc(4*sizeof(int*));
		get_around_square(tab,coordonee[i][0],coordonee[i][1]);
		for (int j=0;j<4;j++){
			//si les coordonnées sont valides
			if(are_x_and_y_ok(tab[j][0],tab[j][1],map->size)){
				//si la couleur est la bonne
				if(color==get_map_value(map,tab[j][0],tab[j][1])){
					//si la case n'est pas déjà mise
					if(!are_coordonate_in_table(coordonee,*nelement,tab[j][0],tab[j][1])){
						coordonee[*nelement][0]=tab[j][0];
						coordonee[*nelement][1]=tab[j][1];
						*nelement+=1;
					}
				}
			}
		}
		free_tab(tab, 4);

	}	
}




char AI_glouton_max(GameState* map,int player){
	Color color_next_to_me[7]={ERROR,ERROR,ERROR,ERROR,ERROR,ERROR,ERROR};
	//int number_of_that_color[7]={0,0,0,0,0,0,0};
	int nelement=0;
	Color colorchoice;
	if (player==1){
		//on parcours toutes les cases controlées par P1
		for (int i=0;i<nP1case;i++){
			int currentX=P1[i][0];
			int currentY=P1[i][1];
			//Si la case est une couleur et qu'elle n'est pas encore prise en compte
			if (is_color(get_map_value(map,currentX,currentY))){
				if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,currentX,currentY)))){
					//on l'ajoute aux choix 
					color_next_to_me[nelement]=get_map_value(map,currentX,currentY);
					nelement+=1;
				}
			}
		}
		//On fait un choix aléatoir parmis les couleurs disponible 
		if(nelement==0){
			colorchoice=BLUE;
		}
		else{
			colorchoice=color_next_to_me[rand()%nelement];
		}
		

	}
	else{
		//on parcours toutes les cases controlées par P2
		for (int i=0;i<nP2case;i++){
			int currentX=P2[i][0];
			int currentY=P2[i][1];

			//on regarde les cases adjacentes
			int**tab;
			tab=malloc(4*sizeof(int*));
			get_around_square(tab,currentX,currentY);
			for(int j=0;j<4;j++){
				//Si les cases adjacentes sont d'une couleur et qu'elle n'est pas encore prise en compte
				if (is_color(get_map_value(map,tab[j][0],tab[j][1]))){
					if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,tab[j][0],tab[j][1])))){
						//on l'ajoute aux choix 
						color_next_to_me[nelement]=get_map_value(map,tab[j][0],tab[j][1]);
						nelement+=1;
					}

				}
			}
			free_tab(tab,4);
		}
		if(nelement==0){
			return 'G';
		}

		//On fait un choix raisoné, pour cela on stoque dans max le nombre de case auquel on a accès par couleur
		int max[]={0,0,0,0,0,0,0};
		//on boucle sur toutes les couleurs que l'on peut choisir, et on initialise les variables
		for (int i=0;i<nelement;i++){
			Color color_of_element=color_next_to_me[i];//couleur de l'élement 

			int** coordonne=malloc(map->size*map->size*sizeof(int*));//tableau de toutes les positions de l'élement
			int c_element=1;

			//on boucle sur toutes les cases de P2
			for(int j=0;j<nP2case;j++){
				int currentX=P2[j][0];
				int currentY=P2[j][1];
				//on regarde les 4 cases aux alentour
				int**tab2;
				tab2=malloc(4*sizeof(int*));
				get_around_square(tab2,currentX,currentY);
					for(int k=0;k<4;k++){
						//si la case est de la bonne couleur
						if(get_map_value(map,tab2[k][0],tab2[k][1])==color_of_element){
							//on ajoute ces coordonnées dans coordonne et le nombre d'élément (normalement, la fonction gère elle même le faite qu'un élément ne puisse pas être deux fois dans tab2)
							n_square_of_the_same_color(map,tab2[k][0],tab2[k][1],coordonne,&c_element);
						}

					}
					free_tab(tab2,4);
			}
			free_tab(coordonne,map->size*map->size);
			max[i]=c_element;
			c_element=1;
		}

		//on peut alors choisir la bonne couleur 
		int maxpos=0;
		int current_max=max[0];
		for (int i=1;i<nelement;i++){
			if(current_max<max[i]){
				maxpos=i;
				current_max=max[i];
			}
		}
		colorchoice=color_next_to_me[maxpos];

	}
	return color_to_char(colorchoice);
}


char AI_frontier(GameState* map,int player){
	Color color_next_to_me[7]={ERROR,ERROR,ERROR,ERROR,ERROR,ERROR,ERROR};
	int nelement=0;
	Color colorchoice;
	int maxX=0;
	int minY=map->size;
	if (player==1){
		//on parcours toutes les cases controlées par P2
		for (int i=0;i<nP1case;i++){
			int currentX=P1[i][0];
			int currentY=P1[i][1];
			//on cherche la valeur maximal des frontières
			if (currentX>maxX){
				maxX=currentX;
			}
			if (currentY<minY){
				minY=currentY;
			}
			//on regarde aussi les cases adjacentes
			int**tab;
			tab=malloc(4*sizeof(int*));
			get_around_square(tab,currentX,currentY);
			for(int j=0;j<4;j++){
				//Si les cases adjacentes sont d'une couleur et qu'elle n'est pas encore prise en compte
				if (is_color(get_map_value(map,tab[j][0],tab[j][1]))){
					if(!is_in_table(color_next_to_me,nelement,(get_map_value(map,tab[j][0],tab[j][1])))){
						//on l'ajoute aux choix 
						color_next_to_me[nelement]=get_map_value(map,tab[j][0],tab[j][1]);
						nelement+=1;
					}

				}
			}
			free_tab(tab,4);
		}

		if(nelement==0){
			return 'G';
		}
		//On fait un choix raisonné, pour ce faire on boucle sur toutes les couleurs
		//pour lesquels on peut faire un choix, et on stoque la valeur maxiaml en Y et en X qu'elle nous fait atteindre,
		// on stoque ensuite la valeur maxiamle des frontières gagnées
		int max[]={0,0,0,0,0,0,0};
		//on boucle sur toutes les couleurs que l'on peut choisir, et on initialise les variables
		for (int i=0;i<nelement;i++){
			Color color_of_element=color_next_to_me[i];//couleur de l'élement 

			int** coordonne=malloc(map->size*map->size*sizeof(int*));//tableau de toutes les positions de l'élement
			int c_element=1;
			int c_maxX=0;
			int c_minY=map->size;

			//on boucle sur toutes les cases de P2
			for(int j=0;j<nP1case;j++){
				int currentX=P1[j][0];
				int currentY=P1[j][1];
				//on regarde les 4 cases aux alentour
				int**tab1;
				tab1=malloc(4*sizeof(int*));
				get_around_square(tab1,currentX,currentY);
					for(int k=0;k<4;k++){
						//si la case est de la bonne couleur
						if(get_map_value(map,tab1[k][0],tab1[k][1])==color_of_element){
							//on ajoute ces coordonnées dans coordonne et le nombre d'élément (normalement, la fonction gère elle même le faite qu'un élément ne puisse pas être deux fois dans tab2)
							n_square_of_the_same_color(map,tab1[k][0],tab1[k][1],coordonne,&c_element);
						}

					}
					free_tab(tab1,4);
			}
			//on calcule les cmaxX et cminY
			for (int l=0; l<c_element;l++){
				if (coordonne[l][0]>c_maxX){
					c_maxX=coordonne[l][0];
				}
				if (coordonne[l][1]<c_minY){
					c_minY=coordonne[l][1];
				}
			}
			free_tab(coordonne,map->size*map->size);
			max[i]=0;
			if(maxX<c_maxX){
				max[i]+=c_maxX-maxX;
			}
			if(c_minY<minY){
				max[i]+=minY-c_minY;
			}
			c_element=1;
		}
		//on peut alors choisir la bonne couleur 
		int maxpos=0;
		int current_max=max[0];
		for (int i=1;i<nelement;i++){
			if(current_max<max[i]){
				maxpos=i;
				current_max=max[i];
			}
		}
		colorchoice=color_next_to_me[maxpos];
		return color_to_char(colorchoice);
	}
	

	
	else{
		return 'C';
	}
}


int main(int argc, char** argv){
	

	int win_for_player1=0;
	int win_for_player2=0;
	int n_game=100;
	int size=10;
	for (int f=0;f<n_game;f++){
	srand((unsigned int)time(NULL));  // seed basée sur le temps
	rand();
	rand();
	
	GameState state;
	create_empty_game_state(&state,size);
	fill_map(&state);


	int nturn=0;
	while(!is_finished(&state)){
		
		nturn+=1;
		if (nturn%2==0){
			printf("Tour du joueur 2");
			printf("\n");
			turn(&state,nturn%2,AI_glouton_max(&state,2));
		}
		else{
			printf("Tour du joueur 1, merci d'entrer une couleur :");
			//scanf(" %c",&choice);
			printf("\n");
			turn(&state,nturn%2,AI_frontier(&state,1));
			//turn(&state,nturn%2,choice);

		}
		draw_game(&state);
		printf("\n");
	}
	if (is_finished(&state)==1)win_for_player1+=1;
	else win_for_player2+=1;
	}
	printf("Le joueur 1 avec l'IA frontière à gagné %i parties, le joueur 2 avec l'IA glouton a gagné %i parties",win_for_player1,win_for_player2);
}