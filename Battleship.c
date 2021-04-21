#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>


#define TAB_COL 15  /*Variável que define o número máximo de linhas que o tabuleiro pode ter (15)*/
#define TAB_ROW 24  /*Variável que define o número máximo de colunas que o tabuleiro pode ter (24)*/



/*Variáveis Globais*/
int g_lines; /*Variável do tipo inteiro que irá corresponder ao número de linhas que o utilizador introduzirá*/
int g_columns; /*Variável do tipo inteiro que irá corresponder ao número de colunas que o utilizador introduzirá*/
int Tab[TAB_COL][TAB_ROW][2]; /*Variável do tipo inteiro que representa o nosso tabuleiro em 3 dimensões, sendo a primeira a representante das colunas, a segunda a representante das linhas e a terceira um auxiliar para quando queremos esconder ou mostrar o tabuleiro*/
int Modo_Jogo; /*Variável do tipo inteiro que vai servir para distinguir os modos de jogo --> 0,1 e 2*/
int Modo_Disparo; /*Variável do tipo inteiro que vai servir para distinguir os modos de disparo --> 1,2 e 3*/
int Modo_Posicionamento; /*Variável do tipo inteiro que que vai servir para distinguir os modos de posicionamento --> 1 e 2*/
int n_pecas=0; /*Variável do tipo inteiro que servirá para fazer a contagem do número de peças geradas no tabuleiro*/
int Tipo_Peca[9]; /*Variável do tipo inteiro que servirá para distinguir os diferentes tipos de pecas --> 8 tipos diferentes + 1 (matriz vazia)*/
int Default[9]; /*Variável do tipo inteiro que servirá para salvaguardar os diferentes tipos de peças inseridos pelo utilizador aquando um "reset" no tabuleiro*/

/*Funções*/
void Tabuleiro(void); /*Função que inicializa o tabuleiro*/
void Conta_Tipo_Peca(int ID,int decide);
void Matriz(void); /*Função que divide o tabuleiro em matrizes 3x3*/
void delay(int number_of_seconds); /*Função que permite dar delay do tabuleiro ao fim de um tempo, afim de criar um aspeto visual mais agradável do jogo*/
int Desenha_Pecas(int index,int ID); /*Função que gera todas as combinações de peças possíveis*/
int Restricao1(void); /*Função para a restrição 1 --> na qual não pode haver peças imediatamente a seguir a outras*/
int Modo_Disparo1(int number_of_squares); /*Função para o modo de disparo 1*/
void Confirma_Disparo(int linhas,int colunas);
void Regiao_restringida(int linhas, int colunas); /*Função que irá ser usada para o modo de disparo 3 -> conceito semelhante à restrição 1*/
void Modo_Disparo_2_ou_3(int shoot_line, int shoot_column, int selciona_modo); /*Função para o modo de disparo 2 ou 3*/
int Verfica_Numero_Pecas(int linhas,int colunas,int confirma_numero);
void CommandLineHelp(); /*Função que dá print da linha de comandos auxiliar ao utilizador do programa*/
int Modo_P2(int AUX_P2[9]); /*Função para o modo de Posicionamento 2*/



/*Função que irá ser executada pelo compilador*/
int main(int argc, char *argv[]){

    srand(time(NULL)); /*Nomenclatura necessária para se gerarem as pecas de forma aleatória mais à frente no programa*/
    int opt; /*Variável do tipo inteiro utilizada na sintaxe do getopt*/
    double Aux; /*Variável auxiliar do tipo double que permite a execução da restriçao 3*/

    while((opt=getopt(argc,argv,"ht:j:p:d:1:2:3:4:5:6:7:8:"))!=-1){

        switch(opt){

            /*Indica que o "h" da interface com o utilizador serve para informar este relativamente à funçaõ de cada letra da linha de comandos*/
            case 'h':
                CommandLineHelp();
                break;


            /*Indica que o "t" da interface com o utilizador diz respeito à escolha das dimensões do tabuleiro */
            case 't':
                sscanf(optarg,"%d x%d",&g_lines,&g_columns); /*Introdução por parte do utilizador do número de linhas e colunas*/
                Aux = ((g_lines / 3) * (g_columns / 3)) / 2; /*Fórmula para pôr em prática a restriçao 3*/
                if ((g_lines % 3!= 0 || g_columns % 3 != 0 || g_lines <= 8 || g_lines >= 16) || (g_columns <= 8 || g_columns >= 25)){
                    printf("Dimensoes Invalidas");
                    exit(0);
                    /*O utilizador terá sempre de introzuir um número de linhas entre 9 e 15 que seja múltiplo de 3, caso contrário aparecerá a mensagem "Dimensões inválidas"*/
                    /*O utilizador terá sempre de introduzir um número de colunas entre 9 e 24 que seja múltiplo de 3, caso contrário aparecerá a mensagem "Dimensões inválidas"*/
                }
                break;


            /*Indica que o "j" da interface com o utilizador diz respeito à escolha do modo de jogo --> 0,1 ou 2*/
            case 'j':
                Modo_Jogo = atoi(optarg);
                if(Modo_Jogo < 0 || Modo_Jogo > 2){
                    printf("Modo de Jogo invalido");
                    exit(0);
                    /*Caso o utilizador insira um modo de jogo que não os permitidos (0,1 e 2) aparecerá a mensagem de "Modo de jogo inválido"*/
                }
                break;


            /*Indica que o "p" da interface com o utilizador diz respeito à escolha do modo de posicionamento --> 1 ou 2*/
            case 'p':
                Modo_Posicionamento = atoi(optarg);
                break;


            /*Indica que o "d" da interface com o utilizador diz respeito à escolha do modo de disparo --> 1,2 ou 3*/
            case 'd':
                Modo_Disparo = atoi(optarg);

                if (Modo_Disparo < 1 || Modo_Disparo > 3){
                    printf("Modo de Disparo invalido");
                    exit(0);
                    /*Caso o utilizador insira um modo de disparo que não os permitidos (1,2 ou 3) aparecerá a mensagem de "Modo de disparo inválido"*/
                }
                break;


            /*Indica que o "1" da interface com o utilizador diz respeito à escolha do número de peças do tipo 1 que o utilizador pretende*/
            case '1':
                printf("1");
                Tipo_Peca[1] = atoi(optarg);
                n_pecas += Tipo_Peca[1]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[1] = Tipo_Peca[1]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 1 é salvaguardado*/
                break;


            /*Indica que o "2" da interface com o utilizador diz respeito à escolha do número de peças do tipo 2 que o utilizador pretende*/
            case '2':
                printf("2");
                Tipo_Peca[2] = atoi(optarg);
                n_pecas += Tipo_Peca[2]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[2] = Tipo_Peca[2]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 2 é salvaguardado*/
                break;


            /*Indica que o "3" da interface com o utilizador diz respeito à escolha do número de peças do tipo 3 que o utilizador pretende*/
            case '3':
                printf("3");
                Tipo_Peca[3] = atoi(optarg);
                n_pecas += Tipo_Peca[3]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[3] = Tipo_Peca[3]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 3 é salvaguardado*/
                break;


            /*Indica que o "4" da interface com o utilizador diz respeito à escolha do número de peças do tipo 4 que o utilizador pretende*/
            case '4':
                Tipo_Peca[4] = atoi(optarg);
                n_pecas += Tipo_Peca[4]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[4] = Tipo_Peca[4]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 4 é salvaguardado*/
                break;


            /*Indica que o "5" da interface com o utilizador diz respeito à escolha do número de peças do tipo 5 que o utilizador pretende*/
            case '5':
                Tipo_Peca[5] = atoi(optarg);
                n_pecas += Tipo_Peca[5]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[5] = Tipo_Peca[5]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 5 é salvaguardado*/
                break;


            /*Indica que o "6" da interface com o utilizador diz respeito à escolha do número de peças do tipo 6 que o utilizador pretende*/
            case '6':
                Tipo_Peca[6] = atoi(optarg);
                n_pecas += Tipo_Peca[6]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[6] = Tipo_Peca[6]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 6 é salvaguardado*/
                break;


            /*Indica que o "7" da interface com o utilizador diz respeito à escolha do número de peças do tipo 7 que o utilizador pretende*/
            case '7':
                Tipo_Peca[7] = atoi(optarg);
                n_pecas += Tipo_Peca[7]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[7] = Tipo_Peca[7]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 7 é salvaguardado*/
                break;


            /*Indica que o "8" da interface com o utilizador diz respeito à escolha do número de peças do tipo 8 que o utilizador pretende*/
            case '8':
                Tipo_Peca[8] = atoi(optarg);
                n_pecas += Tipo_Peca[8]; /*Sempre que uma peca é gerada no tabuleiro o numero de pecas totais irá incrementar*/
                Default[8] = Tipo_Peca[8]; /*Sempre o tabuleiro sofre "reset" o número de peças do tipo 8 é salvaguardado*/
                break;


            /*Caso o utilizador insira alguma letra que não as permitidas na linha de comandos, aparecerá a mensagem "Comando inválido"*/
            default:
                printf("Comando Invalido");
                break;
        }


        /*Conjunto de condições que implementam a restrição 3, ou seja, não pode haver mais pecas de indíce superior em relação às de índice inferior*/
        if(Tipo_Peca[8] > Tipo_Peca[7] ||
                Tipo_Peca[7] > Tipo_Peca[6] ||
                Tipo_Peca[6] > Tipo_Peca[5] ||
                Tipo_Peca[5] > Tipo_Peca[4] ||
                Tipo_Peca[4] > Tipo_Peca[3] ||
                Tipo_Peca[3] > Tipo_Peca[2] ||
                Tipo_Peca[2] > Tipo_Peca[1])
        {
            printf("não está de acordo com a Restricao 3\n");
            exit(0);
        }


        /*Conjunto de condições que implementam a restrição 4, ou seja, não pode haver mais peças do que 50% das matrizes 3x3 presentes no tabuleiro*/
        if(Modo_Posicionamento==2 && n_pecas>(int) Aux){
            printf("%lf\n",Aux);
            exit(0);
        }


        /*Conjunto de condições que implementam a restrição que nos diz que quando o utilizador seleciona o modo de posicionamento 1,
        não pode escolher o número de peças de cada tipo que pretende*/
        if(Modo_Posicionamento==1 &&
                (Tipo_Peca[8]!=0 ||
                 Tipo_Peca[7]!=0 ||
                 Tipo_Peca[6]!=0 ||
                 Tipo_Peca[5]!=0 ||
                 Tipo_Peca[4]!=0 ||
                 Tipo_Peca[3]!=0 ||
                 Tipo_Peca[2]!=0 ||
                 Tipo_Peca[1]!=0))
        {
            printf("o comando -[1 a 8] apenas é valido para o modo p 2\n");
            exit(0);
        }

    }


    int number_of_squares = g_lines * g_columns; /*Variável do tipo inteiro que nos diz o número total de quadrículas que o nosso tabuleiro tem*/

    int total_of_pieces = Tipo_Peca[1] * 1 + Tipo_Peca[2] * 2 + Tipo_Peca[3] * 3 + Tipo_Peca[4] * 4 +
	Tipo_Peca[5] * 5 + Tipo_Peca[6] * 6 + Tipo_Peca[7] * 7 * Tipo_Peca[8] * 8; /*Variável do tipo Inteiro que permite
	calcular a quantidade de quadrados diferentes de 0; isto vai ser útil para o modo de disparo 1*/

    Tipo_Peca[0] = (g_lines * g_columns / 9) - n_pecas; /*Indica o número de matrizes vazias que estão presentes no
     tabuleiro sempre que o modo de posionamento selecionado é o 2*/

    Default[0] = Tipo_Peca[0]; /*Sempre que o tabuleiro sofre "reset" o número de peças do tipo 0 (matrizes 3x3 vazias) é salvaguardado*/

    /*Modo de jogo 0*/
    if(Modo_Jogo == 0){

    	int a;
    	for(a = 0; a <= g_lines-1; a++){
    	    int b;
    		for(b = 0; b <= g_columns-1; b++){
    			Tab[a][b][0] = 1;
    		}
    	}

    	Matriz();
    	printf(" %dx%d %d %d %d %d %d",g_lines, g_columns, Tipo_Peca[1], Tipo_Peca[2], Tipo_Peca[3], Tipo_Peca[4], Tipo_Peca[5]);
    	printf(" %d %d %d\n",Tipo_Peca[6], Tipo_Peca[7], Tipo_Peca[8]);
    	Tabuleiro();
    }


    /*Modo de jogo 1*/
    if(Modo_Jogo == 1){
    	printf("* ================================\n");
    	printf("* Modo de Jogo 1\n");
    	printf("* Insira as Coordenadas de Disparo\n");
    	printf("* ================================\n");


    	Matriz();
    	int numero_total_quadriculas = 0; /*Variável do tipo inteiro que corresponderá ao número total de quadrículas*/

    	/*Os dois ciclos for que se seguem permitem contar o número total de quadrículas do tabuleiro escolhido pelo utilizador*/
        int a;
    	for(a = 0; a <= g_lines-1;a++){
            int b;
    		for(b = 0;b <= g_columns-1;b++){
    			if(Tab[a][b][1] != 0){
    				numero_total_quadriculas++;

    			}
    		}
    	}

    	char letra; /*Variável correspondente ao caractér que corresponderá à coordenda da coluna*/
    	int coluna;
    	int linha;
    	int k; /*Variável do tipo inteiro correspondente à coordenda da linha*/

    	do{
    		scanf("%c %d",&letra, &k); /*Utilizador insere as coordenadas de disparo*/
    		linha = g_lines - k; /*A linha 0 corresponde à linha máxima introudzida, portanto fazemos isto para passarmos as coordenadas corretas para o nosso array*/
    		coluna =  letra - 65; /*Fazer a conversão da letra maiúscula introduzida pelo utilizador para inteiro para ser lida corretamente pelo programa*/
    		if(k>=0  &&  k <= g_lines  &&  coluna >=0  &&  coluna <= g_columns){
    			printf("%d\n",Tab[linha][coluna][1]);
    			Tab[linha][coluna][0] = 1; /*Print do tabuleiro com o disparo efetuado*/
    		}

    		if(Tab[linha][coluna][1] != 0 && linha>=0 && linha <= g_lines && coluna >=0 && coluna <= g_columns){
    			numero_total_quadriculas--; /*Sempre que ocorre um disparo, o número de quadrículas sujeitas a disparos é decrementado*/
    		}

    	} while(numero_total_quadriculas > 0); /*Quando o número de quadriculas for igual a 0, significa que já não existem mais coordenadas que o utilizador possa inserir*/

    	printf(" %dx%d %d %d %d %d %d", g_lines, g_columns ,Tipo_Peca[1] ,Tipo_Peca[2] ,Tipo_Peca[3] ,Tipo_Peca[4] ,Tipo_Peca[5]);
    	printf(" %d %d %d\n", Tipo_Peca[6], Tipo_Peca[7], Tipo_Peca[8]);
    	Tabuleiro();
    }


     /*Modo de jogo 2*/
     if(Modo_Jogo == 2){

    	printf("* ================================\n");
    	printf("* Modo de Jogo 2\n");
    	printf("* Crie um tabuleiro com as caracteríticas indicadas\n");
    	printf("* Reponda aos disparos do programa\n");
    	printf("* ================================\n");
    	printf(" %dx%d %d %d %d %d %d", g_lines, g_columns, Tipo_Peca[1], Tipo_Peca[2] ,Tipo_Peca[3], Tipo_Peca[4], Tipo_Peca[5]);
    	printf(" %d %d %d\n", Tipo_Peca[6], Tipo_Peca[7], Tipo_Peca[8]);

    	/*Modo de disparo 1*/
        if(Modo_Disparo == 1){

    		do{
    			if(Modo_Disparo1(number_of_squares) == 1){
    				total_of_pieces--;
    			}

    			number_of_squares--;
    			Tabuleiro();
    		} while(number_of_squares >= 0 && total_of_pieces > 0);
    	}
         else{ /*Modo de disparo 2 ou 3*/
    			int shoot_line;
                for(shoot_line = 1; shoot_line <= g_lines - 2; shoot_line += 3){
                    int shoot_column;
                    for(shoot_column=1; shoot_column <= g_columns - 2; shoot_column += 3){
                            if(n_pecas == 0){
                            break;
                            }

                            Modo_Disparo_2_ou_3(shoot_line,shoot_column,Modo_Disparo);
                    }
                }

    		Tabuleiro();
    	}
	}
}



/*Função para gerar o tabuleiro*/
void Tabuleiro(void){

    int number = g_lines; /*Inicialização da variável do tipo inteiro number com o número de linhas introduzido pelo utilizador, que servirá para indicar depois as coordenadas das linhas*/
    char Letra = 'A'; /*Inicialização do caractér letra com o caractér "A" que servirá para indicar depois as coordenadas das colunas*/

    /*Tudo o que se segue para baixo irá servir para atribuir de forma ordenada um número a cada uma das linhas do tabuleiro e uma letra a cada uma das colunas*/
    int linha;
    for(linha = 0; linha <= g_lines-1; linha++){
        int coluna;
        for(coluna = -1; coluna <= g_columns-1; coluna++){
            if(coluna == -1){
                /*Este if/else que se segue é apenas para o tabuleiro não ficar desconfigurado quando a coordenada da linha tem 2 algarismos*/
                if(number >= 10){
                    printf("%d ",number);
                }
                else{
                    printf(" %d ", number);
                }
            }
            else{
                /*Isto serve para escolher se as nossas coordenadas do tabuleiro vão ficar visíveis ou não e, por isso, é que usamos um 3º array*/
                printf("%c ",Tab[linha][coluna][0] == 0 ? ' ' : (Tab[linha][coluna][1] == 0 ? '-' : '0' + Tab[linha][coluna][1]));
            }

            /*Este ciclo if e este ciclo for que se seguem indicam a coordenada de cada uma das colunas*/
            if(linha==g_lines-1   &&   coluna==g_columns-1){
                printf("\n  ");
                int a;
                for(a = 1; a <= g_columns; a++, Letra++){
                    printf("%c ", Letra);
                }
            }
        }
        printf("\n"); /*Passamos para linha de baixo*/
        number--; /*O número da coordenada da linha vai decrementar à medidade que vamos descendo no tabuleiro*/
    }
}



/*Função que conta o número total de peças de um determinado tipo*/
void Conta_Tipo_Peca(int ID,int decide){

	if(ID == 0){ /*ID=0 corresponde à peça do tipo 0, matriz vazia*/
		if(decide == 1){ /*Quando temos decide=1, o número total de peças deste tipo vai incrementar, e assim sucessivamente para as outras todas*/
			Tipo_Peca[0]++;
		}
		 else if(decide == -1){/*Quando temos decide=1, o número total de peças deste tipo vai decrementar, e assim sucessivamente para as outras todas*/
			Tipo_Peca[0]--;
		 }
	}

	else if(ID >= 1 && ID <= 9){ /*Entre estes ID's as peças são do tipo 1*/
		if(decide == 1){
			Tipo_Peca[1]++;
		}
		 else if(decide == -1){
			Tipo_Peca[1]--;
		 }

	}

	else if(ID >= 10 && ID <= 21){ /*Entre estes ID's as peças são do tipo 2*/
		if(decide == 1){
			Tipo_Peca[2]++;
		}
		 else if(decide == -1){
			Tipo_Peca[2]--;
		 }
	}

	else if(ID >= 22 && ID <= 27){ /*Entre estes ID's as peças são do tipo 3*/
		if(decide == 1){
			Tipo_Peca[3]++;
		}
		 else if(decide == -1){
			Tipo_Peca[3]--;
		 }
	}

	else if(ID >= 28 && ID <= 31){ /*Entre estes ID's as peças são do tipo 4*/
		if(decide == 1){
			Tipo_Peca[4]++;
		}
		 else if(decide == -1){
			Tipo_Peca[4]--;
		 }
	}

	else if(ID >= 32 && ID <= 35){ /*Entre estes ID's as peças são do tipo 5*/
		if(decide == 1){
			Tipo_Peca[5]++;
		}
		 else if(decide == -1){
			Tipo_Peca[5]--;
		 }
	}

	else if(ID >= 36 && ID <= 39){ /*Entre estes ID's as peças são do tipo 6*/
		if(decide == 1){
			Tipo_Peca[6]++;
		}
		 else if(decide == -1){
			Tipo_Peca[6]--;
		 }
	}

	else if(ID >= 40 && ID <= 42){ /*Entre estes ID's as peças são do tipo 7*/
		if(decide == 1){
			Tipo_Peca[7]++;
		}
		 else if(decide == -1){
			Tipo_Peca[7]--;
		 }
	}

	else{
		if(decide == 1){ /*Entre estes ID's as peças são do tipo 8*/
			Tipo_Peca[8]++;
		}
		 else if(decide == -1){
			Tipo_Peca[8]--;
		 }
	}
}



/*Função que divide o tabuleiro em matrizes 3x3*/
void Matriz(void){

    int ID; /*Variável do tipo inteiro que distingue todas as combinações de peças existentes, isto é, cada ID diferente corresponde a uma combinação diferente de uma peça, sendo o ID=0, a matriz vazia ou peça zero*/
    int index; /*Variável do tipo inteiro que permite dar print de todos os numeros que constituem uma peca para as matrizes 3x3 do tabuleiro*/
    int AUX_P2[9]; /**/

    int linhas;
    for (linhas = 0; linhas <= g_lines - 1; linhas += 3){
        int colunas;
        for (colunas = 0; colunas <= g_columns - 1 ; colunas += 3){

            /*Quando se seleciona o modo de posicionamento 1, são escolhidas de forma aleatória as pecas que constituem o tabuleiro*/
            if(Modo_Posicionamento==1){
                ID = rand()%42 + 1;
                index = 0;
            }
             /**/
             else if(Modo_Posicionamento == 2){
                ID=Modo_P2(AUX_P2);
                index=0;
            }

            /*Estes dois ciclos for que se seguem, permitem colocar as pecas nas matrizes 3x3 do tabuleiro*/
            int i; /*Variável do tipo inteiro equivalente às linhas do nosso tabuleiro*/
            for (i = 0; i < 3; i++){
                int j;/*Variável do tipo inteiro equivalente às colunas do nosso tabuleiro*/
                for (j = 0; j < 3; j++){
                    Tab[linhas+i][colunas+j][1] = Desenha_Pecas(index, ID);
                    index++;
                }
            }/*Chegando aqui temos uma matriz finalizada*/

            int AUX_P1 = 0; /*Variável auxiliar do tipo inteiro que permite realizar a restrição que diz que ao fim
            de 3 tentativas de colocação de uma peça, se nenhuma der, então coloca-se a peça com ID=5;
            Basicamente esta variável vai contabilizar o número de tentativas*/

            /*Modo de posicionamento 1*/
            while(Restricao1() == 0 && Modo_Posicionamento == 1){
                printf("%d-",ID);/*Debug para verificar restrição 1*/
                ID = rand()%42+1; /*Geram-se peças aleatórias*/
                index = 0; /*Inicializa-se a variável que permite dar print de todos os numeros que constituem uma peca para as matrizes 3x3 do tabuleiro*/
                AUX_P1++; /*Incrementa o número de tentativas em colocar uma peça numa das matrizes do tabuleiro*/

                /*Como disse, sempre que ocorrem 3 tentativas, a peça gerada é a peça com ID=5*/
                if(AUX_P1==3){
                    ID = 5;
                }

                /*Estes dois ciclos for que se seguem, permitem colocar as pecas nas matrizes 3x3 do tabuleiro*/
                int i;
                for (i = 0; i < 3; i++){
                    int j;
                    for (j = 0; j < 3; j++){
                        Tab[linhas+i][colunas+j][1] = Desenha_Pecas(index, ID);
                        index++;
                    }
                }
            }

            if(Modo_Posicionamento == 1){
            	Conta_Tipo_Peca(ID,1);
            }

            AUX_P2[0] = 0;
            AUX_P2[1] = 0;
            AUX_P2[2] = 0;
            AUX_P2[3] = 0;
            AUX_P2[4] = 0;
            AUX_P2[5] = 0;
            AUX_P2[6] = 0;
            AUX_P2[7] = 0;
            AUX_P2[8] = 0;

            int Var = 0; /*Variável auxiliar do tipo inteiro utilizada para verificar, quando trocar de tipo de peça dentro da função Modo_P2 e essa peça não couber, voltar à variante inicial desse tipo de peça*/
            int tentativas;
            while(Restricao1() == 0 && Modo_Posicionamento == 2) /*Modo P2*/{

                /*Sempre que não for possível colocar uma peça, o programa irá percorrer todas as variantes de pecas pela ordem indicada
                no enunciado do projeto e irá dar print assim que encontrar uma peça que esteja de acordo com as restrições*/
                if((ID >= 1 && ID <= 9) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 1;
                    printf("(%d)-",ID);
                }

                if((ID >= 10 && ID <= 21) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 10;
                    printf("(%d)-",ID);
                }

                if((ID >= 22 && ID <= 27) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 22;
                    printf("(%d)-",ID);
                }

                if((ID >= 28 && ID <= 31) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 28;
                    printf("(%d)-",ID);
                }

                if((ID >= 32 && ID <= 35) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 32;
                    printf("(%d)-",ID);
                }

                if((ID >= 36 && ID <= 39) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 36;
                    printf("(%d)-",ID);
                }

                if((ID >= 40 && ID <= 41) && Var == 0 /*&& Restricao1(Tab)==0*/){
                    ID = 40;
                    printf("(%d)-",ID);
                }
                Var++;


                /*Verificar se a ultima variante da peça do tipo 1 cabe*/
                if(ID == 9 &&/* Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[1] = 1;
                    Tipo_Peca[1]++;
                    ID=Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                }

                 /*Verificar se a ultima variante da peça do tipo 2 cabe*/
                 else if(ID == 21 &&/* Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[2] = 1;
                    Tipo_Peca[2]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 3 cabe*/
                 else if(ID == 27 &&/* Restricao1()==0 &&*/ Var != 0){
                    AUX_P2[3] = 1;
                    Tipo_Peca[3]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 4 cabe*/
                 else if(ID == 31 && /*Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[4] = 1;
                    Tipo_Peca[4]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 5 cabe*/
                 else if(ID == 35 &&/* Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[5] = 1;
                    Tipo_Peca[5]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 6 cabe*/
                 else if(ID == 39 &&/* Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[6] = 1;
                    Tipo_Peca[6]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 7 cabe*/
                 else if(ID == 41 &&/* Restricao1()==0 &&*/ Var != 0){
                    printf("%d-",ID);
                    AUX_P2[7]=1;
                    Tipo_Peca[7]++;
                    ID = Modo_P2(AUX_P2);

                    Var = 0;
                    tentativas++;
                 }

                 /*Verificar se a ultima variante da peça do tipo 8 cabe*/
                 else if(ID == 42 /*&& Restricao1()==0*/){
                    printf("%d-",ID);
                    AUX_P2[8] = 1;
                    Tipo_Peca[8]++;
                    ID=Modo_P2(AUX_P2);
                    Var = 0;
                    tentativas++;
                 }
                 else{
                    ID++;
                 }


                /*Falta ver os casos em que quando passa de uma variante para a outra e no caso da peça que for sorteada couber*/
                index = 0;
                int i;
                for (i = 0; i < 3; i++){
                    int j;
                    for (j = 0; j < 3; j++){
                        Tab[linhas + i][colunas + j][1] = Desenha_Pecas(index,ID);
                        index++;
                    }
                }
                printf("%d-",ID);

            }
            printf("%d certa\n",ID);

        }
    }
}



/*Função para gerar todas as combinações possíveis das peças*/
int Desenha_Pecas(int index, int ID){

    if(ID==0){
        int Peca[]= {0,0,0,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
        /*Retorna, para ID=0, todo o conteúdo que está dentro do array int Peca*/
        /*Assim sucessivamente para todos os ID's a seguir*/
    }

    else if(ID==1){
        int Peca[]= {1,0,0,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==2){
        int Peca[]= {0,1,0,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==3){
        int Peca[]= {0,0,1,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==4){
        int Peca[]= {0,0,0,
                     1,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==5){
        int Peca[]= {0,0,0,
                     0,1,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==6){
        int Peca[]= {0,0,0,
                     0,0,1,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==7){
        int Peca[]= {0,0,0,
                     0,0,0,
                     1,0,0
                    };
        return Peca[index];
    }

    else if(ID==8){
        int Peca[]= {0,0,0,
                     0,0,0,
                     0,1,0
                    };
        return Peca[index];
    }

    else if(ID==9){
        int Peca[]= {0,0,0,
                     0,0,0,
                     0,0,1
                    };
        return Peca[index];
    }

    else if(ID==10){
        int Peca[]= {2,2,0,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==11){
        int Peca[]= {0,2,2,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==12){
        int Peca[]= {0,0,0,
                     2,2,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==13){
        int Peca[]= {0,0,0,
                     0,2,2,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==14){
        int Peca[]= {0,0,0,
                     0,0,0,
                     2,2,0
                    };
        return Peca[index];
    }

    else if(ID==15){
        int Peca[]= {0,0,0,
                     0,0,0,
                     0,2,2
                    };
        return Peca[index];
    }

    else if(ID==16){
        int Peca[]= {2,0,0,
                     2,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==17){
        int Peca[]= {0,0,0,
                     2,0,0,
                     2,0,0
                    };
        return Peca[index];
    }

    else if(ID==18){
        int Peca[]= {0,2,0,
                     0,2,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==19){
        int Peca[]= {0,0,0,
                     0,2,0,
                     0,2,0
                    };
        return Peca[index];
    }

    else if(ID==20){
        int Peca[]= {0,0,2,
                     0,0,2,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==21){
        int Peca[]= {0,0,0,
                     0,0,2,
                     0,0,2
                    };
        return Peca[index];
    }

    else if(ID==22){
        int Peca[]= {3,3,3,
                     0,0,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==23){
        int Peca[]= {0,0,0,
                     3,3,3,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==24){
        int Peca[]= {0,0,0,
                     0,0,0,
                     3,3,3
                    };
        return Peca[index];
    }

    else if(ID==25){
        int Peca[]= {3,0,0,
                     3,0,0,
                     3,0,0
                    };
        return Peca[index];
    }

    else if(ID==26){
        int Peca[]= {0,3,0,
                     0,3,0,
                     0,3,0
                    };
        return Peca[index];
    }

    else if(ID==27){
        int Peca[]= {0,0,3,
                     0,0,3,
                     0,0,3
                    };
        return Peca[index];
    }

    else if(ID==28){
        int Peca[]= {4,4,0,
                     4,4,0,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==29){
        int Peca[]= {0,4,4,
                     0,4,4,
                     0,0,0
                    };
        return Peca[index];
    }

    else if(ID==30){
        int Peca[]= {0,0,0,
                     4,4,0,
                     4,4,0
                    };
        return Peca[index];
    }

    else if(ID==31){
        int Peca[]= {0,0,0,
                     0,4,4,
                     0,4,4
                    };
        return Peca[index];
    }

    else if(ID==32){
        int Peca[]= {5,5,5,
                     0,5,0,
                     0,5,0
                    };
        return Peca[index];
    }

    else if(ID==33){
        int Peca[]= {5,0,0,
                     5,5,5,
                     5,0,0
                    };
        return Peca[index];
    }

    else if(ID==34){
        int Peca[]= {0,5,0,
                     0,5,0,
                     5,5,5
                    };
        return Peca[index];
    }

    else if(ID==35){
        int Peca[]= {0,0,5,
                     5,5,5,
                     0,0,5
                    };
        return Peca[index];
    }

    else if(ID==36){
        int Peca[]= {0,6,0,
                     6,0,6,
                     6,6,6
                    };
        return Peca[index];
    }

    else if(ID==37){
        int Peca[]= {0,6,6,
                     6,0,6,
                     0,6,6
                    };
        return Peca[index];
    }

    else if(ID==38){
        int Peca[]= {6,6,6,
                     6,0,6,
                     0,6,0
                    };
        return Peca[index];
    }

    else if(ID==39){
        int Peca[]= {6,6,0,
                     6,0,6,
                     6,6,0
                    };
        return Peca[index];
    }

    else if(ID==40){
        int Peca[]= {7,0,7,
                     7,7,7,
                     7,0,7
                    };
        return Peca[index];
    }

    else if(ID==41){
        int Peca[]= {7,7,7,
                     0,7,0,
                     7,7,7
                    };
        return Peca[index];
    }

    else if(ID==42){
        int Peca[]= {8,8,8,
                     8,0,8,
                     8,8,8
                    };
        return Peca[index];
    }

    return 0;
}



/*Função que serve de apoio ao utlizador caso este queira saber para que serve cada parâmetro da Linhas de Comandos*/
void CommandLineHelp(){

    /*Na linha de comandos, sempre que o utilizador premir a tecla "h", aparecerão todas estas mensagens que se seguem, de modo a
    ajudar o utilizador do programa a compreender melhor a função de cada parâmetro da linha de comandos*/
    printf("-t dimensões do tabuleiro (linha x coluna).\n");
    printf("-j modo de jogo (0 a 2)\n");
    printf("-p modo de posicionamento de peças pelo computador (1 a 2)\n");
    printf("-d modo de disparo de peças pelo computador (1 a 3)\n");
    printf("-1 número de peças tipo 1 (mínimo 1)\n");
    printf("-2 número de peças tipo 2\n");
    printf("-3 número de peças tipo 3\n");
    printf("-4 número de peças tipo 4\n");
    printf("-5 número de peças tipo 5\n");
    printf("-6 número de peças tipo 6\n");
    printf("-7 número de peças tipo 7\n");
    printf("-8 número de peças tipo 8\n");
}



/*Função para a Restrição 1*/
/*Se a Restrição=1 significa que esta está a ser executada, ou seja, tudo foi cumprido*/
int Restricao1() {

    int Verifica_Matriz; /*Variável do tipo inteiro que permitirá verificar em que matriz nos encontramos*/

    int linhas;
    for (linhas = 0; linhas <= g_lines-1; linhas++){
        int colunas;
        for (colunas = 0; colunas <= g_columns-1; colunas++){
            Verifica_Matriz = (colunas / 3) * (g_columns / 3) + (linhas / 3) + 1; /*Fórmula de verificação da matriz*/
            if(Tab[linhas][colunas][1] != 0){
                int j;
                for(j = -1; j <= 1; j++){
                    int k;
                    for(k = -1; k <= 1; k++){
                        if(linhas + j < 0 || linhas + j >= g_lines){
                            continue;
                        }

                        if(colunas + k < 0 || colunas + k >= g_columns){
                            continue;
                        }

                        if(Tab[linhas + j][colunas + k][1] != 0 && Verifica_Matriz != ((((colunas + k) / 3) * (g_columns / 3) + ( linhas + j ) / 3 ) + 1))
                            return 0; /*Este ciclo if verifica se existe algum número fora da matriz e que seja diferente de 0; se isso
                                        se verificar, então iremos retornar o valor 0, ou seja, o programa não poderá colocar pecas nessas
                                        coordenadas*/
                    }
                }
            }
        }
    }
    return 1;
}



/*Função para o modo de disparo 1*/
int Modo_Disparo1(int numero_quadriculas){

    int rand_coluna; /*Variável do tipo inteiro que corresponderá a uma coordenada de uma coluna, sendo este gerada aleatoriamente mais abaixo*/
    int rand_linha; /*Variável do tipo inteiro que corresponderá a uma coordenada de uma linha, sendo este gerada aleatoriamente mais abaixo*/
    rand_linha = rand() % (g_lines);
    rand_coluna = rand() % (g_columns);

    /*Enquanto o nosso programa continuar a acertar em peças e enquanto o número de quadriculas do nosso tabuleiro for maior que zero,
    o nosso programa continuará a gerar novas coordenadas, isto é mais disparos, só parando, portanto, assim que todas as coordenadas
    do tabuleiro sofreram disparo*/
    while(Tab[rand_linha][rand_coluna][0]==1 && numero_quadriculas>0){
        rand_linha = rand() % (g_lines);
        rand_coluna = rand() % (g_columns);
    }

    Tab[rand_linha][rand_coluna][0] = 1;
    Confirma_Disparo(rand_linha,rand_coluna);

	if(Tab[rand_linha][rand_coluna][1] == 0){
		return 0;
	}
	 else{
		return 1;
	 }
}



/*Função que permite criar uma pequena "conversa" com o utilizador, na medida em que sempre que o programa nos gerar uma coordenada de disparo,
o utilizador irá dizer se o computador acertou em alguma peça ou não e, por sua vez, reagirá a isso --> esta função será usada para para alguns
modos de disparo que serão usados em algusn modos de jogo*/
void Confirma_Disparo(int linhas,int colunas){

	char posicao;
    int posicao_transformada;

    char coluna = colunas + 65;
    int linha = g_lines - linhas;

    printf("coordenadas de disparo:%c%d\n",coluna,linha);
    printf("confirmacao:\n");

    do{

        scanf("%c",&posicao);/* Posição é a variavel que indica qual é que foi a peça em que o disparo acertou*/
        if(posicao == '-'){
            posicao_transformada = posicao - 45;
        }
         else{
            posicao_transformada = posicao - 48;
         }

        Tab[linhas][colunas][1] = posicao_transformada;

	} while(posicao_transformada < 0 || posicao_transformada >= 9);
}



/*Funçao utilizada para fazer um delay nos modos de disparo, para os tiros irem aparecendo sequencialmente ao longo do tempo*/
void delay(int number_of_seconds){

    int mili_seconds = 1000*number_of_seconds;

    clock_t start_time =clock();

    while(clock()<start_time+mili_seconds);
}



/*Função muito semelhante à da Restrição 1, mas aqui está a ser usada para o modo de disparo 3, pois neste modo,
sempre que o computador acerta numa peca, automaticamente tem de por um tracinho em todas as posições adjacentes
ao numero no qual acertou*/
void Regiao_restringida(int linhas, int colunas){

    int Verifica_Matriz;

    Verifica_Matriz = (colunas / 3) * (g_columns / 3) + (linhas / 3) + 1; /*Fórmula de verificação da matriz*/

    if(Tab[linhas][colunas][1] != 0){
        int j;
        for(j = -1; j <= 1; j++){
            int k;
            for(k = -1; k <= 1; k++){
                if(linhas + j < 0 || linhas + j >= g_lines){
                    continue;
                }

                if(colunas + k < 0 || colunas + k >= g_columns){
                    continue;
                }

                if(Tab[linhas + j][colunas + k][1] == 0 && Verifica_Matriz != ((((colunas + k) / 3) * (g_columns / 3) + ( linhas + j ) / 3 ) + 1))
                            Tab[linhas+j][colunas+k][0] = 1;
                }
            }
        }
}



/*Função para o modo de disparo 2*/
void Modo_Disparo_2_ou_3(int linha_disparo,int coluna_disparo, int seleciona_modo){

    /*Disparo sobre a quadricula do meio*/
    Tab[linha_disparo][coluna_disparo][0] = 1;
    Confirma_Disparo(linha_disparo,coluna_disparo);

    if(Tab[linha_disparo][coluna_disparo][1]!=0){
        n_pecas--;

    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
                Regiao_restringida (linha_disparo, coluna_disparo);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo, coluna_disparo);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula central de baixo*/
    if (seleciona_modo==2){
        Tab[linha_disparo-1][coluna_disparo][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo-1][coluna_disparo][0]==1){
      }
      else{
        Tab[linha_disparo-1][coluna_disparo][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo);
      }

    if(Tab[linha_disparo -1 ][coluna_disparo][1] != 0 ){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo-1, coluna_disparo);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo-1, coluna_disparo);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula central de cima*/
    if (seleciona_modo==2){
        Tab[linha_disparo+1][coluna_disparo][0] = 1;
         Confirma_Disparo(linha_disparo+1,coluna_disparo);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo+1][coluna_disparo][0]==1){
      }
      else{
        Tab[linha_disparo+1][coluna_disparo][0] = 1;
        Confirma_Disparo(linha_disparo+1,coluna_disparo);
      }

    if(Tab[linha_disparo + 1][coluna_disparo][1] != 0){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo+1, coluna_disparo);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo+1, coluna_disparo);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula à esquerda do meio*/
     if (seleciona_modo==2){
        Tab[linha_disparo][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo,coluna_disparo-1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo][coluna_disparo-1][0]==1){
      }
      else{
        Tab[linha_disparo][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo,coluna_disparo-1);
      }

    if(Tab[linha_disparo][coluna_disparo - 1][1] != 0 ){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo, coluna_disparo-1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo, coluna_disparo-1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula à direita do meio*/
    if (seleciona_modo==2){
        Tab[linha_disparo][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo,coluna_disparo+1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo][coluna_disparo+1][0]==1){
      }
      else{
        Tab[linha_disparo][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo,coluna_disparo+1);
      };

    if(Tab[linha_disparo][coluna_disparo + 1][1] != 0 ){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo, coluna_disparo+1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo, coluna_disparo+1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula do canto superior esquerdo*/
    if (seleciona_modo==2){
        Tab[linha_disparo-1][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo-1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo-1][coluna_disparo-1][0]==1){
      }
      else{
        Tab[linha_disparo-1][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo-1);
      };

    if(Tab[linha_disparo - 1][coluna_disparo - 1][1] != 0 ){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo-1, coluna_disparo-1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo-1, coluna_disparo-1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula do canto inferior direito*/
    if (seleciona_modo==2){
        Tab[linha_disparo+1][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo+1,coluna_disparo+1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo+1][coluna_disparo+1][0]==1){
      }
      else{
        Tab[linha_disparo+1][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo+1,coluna_disparo+1);
      };

    if(Tab[linha_disparo + 1][coluna_disparo + 1][1] != 0){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo+1, coluna_disparo+1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo+1, coluna_disparo+1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula do canto superior direito*/
    if (seleciona_modo==2){
        Tab[linha_disparo-1][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo+1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo-1][coluna_disparo+1][0]==1){
      }
      else{
        Tab[linha_disparo-1][coluna_disparo+1][0] = 1;
        Confirma_Disparo(linha_disparo-1,coluna_disparo+1);
      };

    if(Tab[linha_disparo - 1][coluna_disparo + 1][1] !=0){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo-1, coluna_disparo+1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo-1, coluna_disparo+1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");



    /*Disparo sobre a quadricula do canto inferior esquerdo*/
    if (seleciona_modo==2){
        Tab[linha_disparo+1][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo+1,coluna_disparo-1);
    } else if (seleciona_modo==3  &&   Tab[linha_disparo+1][coluna_disparo-1][0]==1){
      }
      else{
        Tab[linha_disparo+1][coluna_disparo-1][0] = 1;
        Confirma_Disparo(linha_disparo+1,coluna_disparo-1);
      };

    if(Tab[linha_disparo + 1][coluna_disparo - 1][1]!=0){
        n_pecas--;
    }

    if(n_pecas==0){
        if(seleciona_modo == 3){
            Regiao_restringida (linha_disparo+1, coluna_disparo-1);
        }
        Tabuleiro();
        delay(300);
        system("clear");
        return;
    }

    if(seleciona_modo == 3){
        Regiao_restringida (linha_disparo+1, coluna_disparo-1);
    }
    Tabuleiro();
    delay(300);
    system("clear");
    printf("\n");
}



/*Função para o modo de posicionamento 2*/
int Modo_P2(int AUX_P2[9]){

    int ID;
    int K = 0;
    int Rand;
    int tentativas=0;
    while(K == 0){

        Rand = rand() % 9;
        if(Rand == 0 && Tipo_Peca[0] > 0){
            ID = 0;
            Tipo_Peca[0]--;
            K = 1;
        }

        else if(Rand == 1 && Tipo_Peca[1] > 0 && AUX_P2[1] != 1){
            ID= 1 + rand() % 9;
            K = 1;
            Tipo_Peca[1]--;
        }

        else if(Rand == 2 && Tipo_Peca[2] > 0 && AUX_P2[2] != 1){
            ID = 10 + rand() % 12;
            K = 1;
            Tipo_Peca[2]--;
        }

        else if(Rand == 3 && Tipo_Peca[3] > 0 && AUX_P2[3] != 1){
            ID = 22 + rand() % 6;
            K = 1;
            Tipo_Peca[3]--;
        }

        else if(Rand == 4 && Tipo_Peca[4] > 0 && AUX_P2[4] != 1){
            ID = 28 + rand() % 4;
            K=1;
            Tipo_Peca[4]--;
        }

        else if(Rand == 5 && Tipo_Peca[5] > 0 && AUX_P2[5] != 1){
            ID = 32 + rand() % 4;
            K = 1;
            Tipo_Peca[5]--;
        }

        else if(Rand == 6 && Tipo_Peca[6] > 0 && AUX_P2[6] != 1){
            ID = 36 + rand() % 4;
            K = 1;
            Tipo_Peca[6]--;
        }

        else if(Rand == 7 && Tipo_Peca[7] > 0 && AUX_P2[7] != 1){
            ID = 40 + rand() % 2;
            K = 1;
            Tipo_Peca[7]--;
        }

        else if(Rand == 8 && Tipo_Peca[8] > 0 && AUX_P2[8] != 1){
            ID = 42;
            K = 1;
            Tipo_Peca[8]--;
        }

        else if(Tipo_Peca[0] == 0 &&
                Tipo_Peca[1] == 0 &&
                Tipo_Peca[2] == 0 &&
                Tipo_Peca[3] == 0 &&
                Tipo_Peca[4] == 0 &&
                Tipo_Peca[5] == 0 &&
                Tipo_Peca[6] == 0 &&
                Tipo_Peca[7] == 0 &&
                Tipo_Peca[8] == 0)
        {
            K=1;
            printf("todas a pecas utilizadas\n\n");
            printf("-0-%d %d\n",Tipo_Peca[0],AUX_P2[0]);
            printf("-1-%d %d\n",Tipo_Peca[1],AUX_P2[1]);
            printf("-2-%d %d\n",Tipo_Peca[2],AUX_P2[2]);
            printf("-3-%d %d\n",Tipo_Peca[3],AUX_P2[3]);
            printf("-4-%d %d\n",Tipo_Peca[4],AUX_P2[4]);
            printf("-5-%d %d\n",Tipo_Peca[5],AUX_P2[5]);
            printf("-6-%d %d\n",Tipo_Peca[6],AUX_P2[6]);
            printf("-7-%d %d\n",Tipo_Peca[7],AUX_P2[7]);
            printf("-8-%d %d\n",Tipo_Peca[8],AUX_P2[8]);
            printf("-%d\n",ID);
        }

        else{
            K=0;
            if(AUX_P2[1]==1 &&
                    AUX_P2[2]==1 &&
                    AUX_P2[3]==1 &&
                    AUX_P2[4]==1 &&
                    AUX_P2[5]==1 &&
                    AUX_P2[6]==1 &&
                    AUX_P2[7]==1 &&
                    AUX_P2[8]==1 )
            {
                return -1;
            }

            printf("tentativas:%d\n",tentativas);
            tentativas++;
            /*if(tentativas==10){
            break;
            }*/
            printf("rand:%d\n",Rand);
            printf("0-%d %d\n",Tipo_Peca[0],AUX_P2[0]);
            printf("1-%d %d\n",Tipo_Peca[1],AUX_P2[1]);
            printf("2-%d %d\n",Tipo_Peca[2],AUX_P2[2]);
            printf("3-%d %d\n",Tipo_Peca[3],AUX_P2[3]);
            printf("4-%d %d\n",Tipo_Peca[4],AUX_P2[4]);
            printf("5-%d %d\n",Tipo_Peca[5],AUX_P2[5]);
            printf("6-%d %d\n",Tipo_Peca[6],AUX_P2[6]);
            printf("7-%d %d\n",Tipo_Peca[7],AUX_P2[7]);
            printf("8-%d %d\n",Tipo_Peca[8],AUX_P2[8]);
            printf("%d\n",ID);

        }
    }
    return ID;
}
