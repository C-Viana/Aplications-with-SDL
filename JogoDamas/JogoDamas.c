/* NOTA 1: Não consegui fazer de modo a seguir a lógica proposta para a atividade
Por isso tentei fazer o mais funcional quanto pude com meu próprio raciocínio  

NOTA 2: O jogo não implementa todas as regras. Não há jogadas múltiplas (iterativas) nem forma-se dama ao chegar no extremo oposto */


#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#define T 8
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int COMPRIMENTO = SCREEN_WIDTH / T;
const int ALTURA = SCREEN_HEIGHT / T;

int x = 0;
int y = 0;
int linhaOrigem = 0, colunaOrigem = 0;
int linhaDestino = 0, colunaDestino = 0;
int clickOn = 0;
int valido = 0;
int inimigo = 0;
int resposta = 0;

typedef struct { int linha, coluna; } Pos;

int tabuleiro[T][T] = { { 0, 1, 0, 1, 0, 1, 0, 1 },
						{ 1, 0, 1, 0, 1, 0, 1, 0 },
						{ 0, 1, 0, 1, 0, 1, 0, 1 },
						{ 0, 0, 0, 0, 0, 0, 0, 0 },
						{ 0, 0, 0, 0, 0, 0, 0, 0 },
						{ 2, 0, 2, 0, 2, 0, 2, 0 },
						{ 0, 2, 0, 2, 0, 2, 0, 2 },
						{ 2, 0, 2, 0, 2, 0, 2, 0 } };


int capturaEventos(SDL_Window *window, Pos *pos){
	int valor = 0;
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type){
		case SDL_WINDOWEVENT_CLOSE:
			if (window){ SDL_DestroyWindow(window); window = NULL; valor = 1; } break;
		case SDL_QUIT: valor = 1; break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_ESCAPE: valor = 1; break;
			}//fim do SWITCH KEYDOWN
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button){
			case SDL_BUTTON_LEFT:
				if (clickOn == 0){
					SDL_GetMouseState(&x, &y);
					linhaOrigem = x / COMPRIMENTO;
					colunaOrigem = y / ALTURA;
					printf("Botao esquerdo do mouse: %i | %i\n", linhaOrigem, colunaOrigem);
					if (tabuleiro[colunaOrigem][linhaOrigem] > 0)
						clickOn++;
					if (haPecaInimiga(linhaOrigem, colunaOrigem, linhaDestino, colunaDestino)){
						inimigo++;
						printf("INIMIGO PROXIMO!\n");
					}
				}
				else if (clickOn == 1){
					SDL_GetMouseState(&x, &y);
					linhaDestino = x / COMPRIMENTO;
					colunaDestino = y / ALTURA;
					printf("Botao direito do mouse: %i | %i\n", linhaDestino, colunaDestino);
					clickOn++;
				}
				break;
			case SDL_BUTTON_RIGHT: printf("Botao direito do mouse\n"); break;
			}//fim do SWITCH MOUSEBUTTONDOWN
		}//fim do SWITCH 1
	}//fim do WHILE

	const Uint8 *teclado = SDL_GetKeyboardState(NULL);
	if (teclado[SDL_SCANCODE_W]) printf("Tecla 'W' acionada!\n");

	return valor;
}






void pintaPecas(SDL_Renderer *renderer){
	//------------------------------------

	for (int i = 0; i < T; i++){
		for (int j = 0; j < T; j++){

			if (tabuleiro[i][j] == 1){
				SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
				SDL_Rect casaA = { j*COMPRIMENTO + 10, i*ALTURA + 10, COMPRIMENTO - 20, ALTURA - 20 };
				SDL_RenderFillRect(renderer, &casaA);
			}
			else if (tabuleiro[i][j] == 2){
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect casaB = { (j)*COMPRIMENTO + 10, (i)*ALTURA + 10, COMPRIMENTO - 20, ALTURA - 20 };
				SDL_RenderFillRect(renderer, &casaB);
			}


		}//fim do laço for j
	}//fim do laço for i

	if (clickOn){
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect borda = { linhaOrigem*COMPRIMENTO, colunaOrigem*ALTURA, COMPRIMENTO, ALTURA };
		SDL_RenderDrawRect(renderer, &borda);
	}


	//------------------------------------
	
}



int calculaDistancia(int linhaOrigem, int colunaOrigem, int linhaDestino, int colunaDestino){
	resposta = 0;
	int difColuna = abs(colunaOrigem - colunaDestino);
	int difLinha = abs(linhaOrigem - linhaDestino);

	if (difColuna == difLinha && difColuna == 1){
		resposta = 1;
	}
	else if (inimigo && (difColuna == 2 && difColuna == difLinha)){
		resposta = 2;
		inimigo = 0;
	}

	return resposta;
}



int testaMovimentacaoPeca(int linhaOrigem, int colunaOrigem, int linhaDestino, int colunaDestino){
	valido = 0;

	if ((linhaDestino != linhaOrigem) && (colunaDestino != colunaOrigem) && (tabuleiro[colunaDestino][linhaDestino] == 0)){
		if (tabuleiro[colunaOrigem][linhaOrigem] == 1){
			if (!inimigo && colunaDestino > colunaOrigem)
				valido = 1;
			else if (inimigo)
				valido = 1;
		}
		else if (tabuleiro[colunaOrigem][linhaOrigem] == 2){
			if (!inimigo && colunaDestino < colunaOrigem)
				valido = 1;
			else if (inimigo)
				valido = 1;
		}
	}
	else{
		printf("MOVIMENTO INVALIDO!\n");
	}

	return valido;
}




void movimentaPeca(int linhaOrigem, int colunaOrigem, int linhaDestino, int colunaDestino){

	if (testaMovimentacaoPeca(linhaOrigem, colunaOrigem, linhaDestino, colunaDestino)){
		tabuleiro[colunaDestino][linhaDestino] = tabuleiro[colunaOrigem][linhaOrigem];
		tabuleiro[colunaOrigem][linhaOrigem] = 0;

		if (colunaDestino < colunaOrigem && linhaDestino < linhaOrigem)
			tabuleiro[colunaDestino + 1][linhaDestino + 1] = 0;
		else if (colunaDestino < colunaOrigem && linhaDestino > linhaOrigem)
			tabuleiro[colunaDestino + 1][linhaDestino - 1] = 0;
		else if (colunaDestino > colunaOrigem && linhaDestino < linhaOrigem)
			tabuleiro[colunaDestino - 1][linhaDestino + 1] = 0;
		else if (colunaDestino > colunaOrigem && linhaDestino > linhaOrigem)
			tabuleiro[colunaDestino - 1][linhaDestino - 1] = 0;
	}

	clickOn = 0;

}




int haPecaInimiga(int colunaOrigem, int linhaOrigem, int colunaDestino, int linhaDestino){
	int haInimigo = 0;
	if (tabuleiro[linhaOrigem][colunaOrigem] == 1){
		if (tabuleiro[linhaOrigem + 1][colunaOrigem + 1] == 2 || tabuleiro[linhaOrigem + 1][colunaOrigem - 1] == 2){
			haInimigo = 1;
		}
		else if (tabuleiro[linhaOrigem - 1][colunaOrigem + 1] == 2 || tabuleiro[linhaOrigem - 1][colunaOrigem - 1] == 2){
			haInimigo = 1;
		}
	}
	else if (tabuleiro[linhaOrigem][colunaOrigem] == 2){
		if (tabuleiro[linhaOrigem - 1][colunaOrigem + 1] == 1 || tabuleiro[linhaOrigem - 1][colunaOrigem - 1] == 1){
			haInimigo = 1;
		}
		else if (tabuleiro[linhaOrigem + 1][colunaOrigem + 1] == 1 || tabuleiro[linhaOrigem + 1][colunaOrigem - 1] == 1){
			haInimigo = 1;
		}
	}
	return haInimigo;
}




void pintaTabuleiro(SDL_Renderer *renderer){
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	//------------------------------------

	for (int i = 0; i < T; i++){
		for (int j = 0; j < T; j++){
			if ((i + j) % 2 != 0){
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_Rect casa = { i*COMPRIMENTO, j*ALTURA, COMPRIMENTO, ALTURA };
				SDL_RenderFillRect(renderer, &casa);
			}

		}
	}

	//------------------------------------
	pintaPecas(renderer);
}



int main(int argc, char *args[]){
	Pos pos;
	int terminar = 0;

	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Init(SDL_WINDOW_SHOWN);

	window = SDL_CreateWindow("Jogo de Damas 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	while (!terminar){

		terminar = capturaEventos(window, &pos);
		pintaTabuleiro(renderer);


		if (clickOn == 2){
			if (calculaDistancia(linhaOrigem, colunaOrigem, linhaDestino, colunaDestino)){
				movimentaPeca(linhaOrigem, colunaOrigem, linhaDestino, colunaDestino);
				
			}
			else{
				printf("MOVIMENTO INVALIDO!\n");
				clickOn = 0;
			}
		}

		


		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}