#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>

#define WINDOW_WIDTH 564
#define WINDOW_HEIGHT 480

int fieldWidth = WINDOW_WIDTH / 8;
int fieldHeight = WINDOW_HEIGHT / 8;

int pontos = 0; tentativas = 10;
int tempA, tempB;
int evalue, mousex, mousey, row, column;

typedef struct { char name; int size; } Ship;
Ship ship[5];



char matrizReal[8][8] = { { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
						{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, };

char matrizVisivel[8][8] = { { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
							{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, };



void events(SDL_Window *window){

	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
		case SDL_WINDOWEVENT_CLOSE: tentativas = 0; break;
		case SDL_QUIT: tentativas = 0; break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_ESCAPE: tentativas = 0; break;
			
			}
		case SDL_MOUSEBUTTONDOWN: 
			switch (event.button.button){
			case SDL_BUTTON_LEFT:
				SDL_GetMouseState(&mousex, &mousey);
				row = mousex / fieldWidth;
				column = mousey / fieldHeight;
				printf("Botao esquerdo do mouse: %i | %i\n", row, column);
				if (matrizReal[row][column] == ' '){
					matrizVisivel[row][column] = 'e';
					pontos += 53;
				}
				else if (matrizReal[row][column] != 'e' && matrizReal[row][column] != ' '){
					matrizVisivel[row][column] = 'r';
					tentativas--;
				}
			}
		}
	}



}


void ocuppyField(){
	
	for (int i = 0; i < 5; i++){
		tempA = rand() % 7; 
		tempB = rand() % 7;

		if(matrizReal[tempA][tempB] == ' ' && tempB+ship[i].size < 7){
			for (int k = tempB; k < tempB + ship[i].size; k++)
				matrizReal[tempA][k] = ship[i].name;
		}
		else{
			i--;
		}
	}
	
}


void renderScreen(SDL_Renderer *renderer){

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Rect field = { x*fieldWidth, y*fieldHeight, fieldWidth, fieldHeight };
			SDL_RenderFillRect(renderer, &field);
			
		}

	}
	
}


void revealField(SDL_Renderer *renderer){


	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++){
			if (matrizVisivel[x][y] == 'e'){
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect field = { x*fieldWidth, y*fieldHeight, fieldWidth, fieldHeight };
				SDL_RenderFillRect(renderer, &field);
			}
			else if (matrizVisivel[x][y] == 'r'){
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_Rect field = { x*fieldWidth, y*fieldHeight, fieldWidth, fieldHeight };
				SDL_RenderFillRect(renderer, &field);
			}
			
			
			
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_Rect border = { x*fieldWidth, y*fieldHeight, fieldWidth, fieldHeight };
			SDL_RenderDrawRect(renderer, &border);
		}

	}
	
}



int main(int argc, char *args[]){

	srand((unsigned) time(NULL));

	ship[0].name = 'P';
	ship[0].size = 1;
	ship[1].name = 'O';
	ship[1].size = 2;
	ship[2].name = 'I';
	ship[2].size = 3;
	ship[3].name = 'U';
	ship[3].size = 3;
	ship[4].name = 'Y';
	ship[4].size = 4;

	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Init(SDL_WINDOW_SHOWN);

	window = SDL_CreateWindow("Batalha Naval", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	ocuppyField();

	while (tentativas){
		events(window);
		renderScreen(renderer);
		revealField(renderer);
		SDL_RenderPresent(renderer);
	}

	printf("You scored %i points\n\n", pontos);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}