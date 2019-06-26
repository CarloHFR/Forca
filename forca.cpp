//Criado por: Carlo Henrique FR
//Data: 06/06/2019
//Versão: 1.0

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <locale.h>

using namespace std;


//Protipos.
int addPalavra();
int pegarPalavra();
int telaGanhador();
int telaMorte();
int imprimirGrafico();
int checarLetra();
int checarPalavra();
int checarGanhador();
int regras();
int entradaUsuario();


//Variaveis globais.
int vidas = 7;
int aleatorio;
int rodando = 1;
char palavra[20];
char dica[20];
char lacuna[20];
char opUsuario;
char letraUsuario = 'a';
char palavraUsuario[20];


//TXT QUE SERVE DE BANCO DE PALAVRAS DEVE SER FORMATADO DESTA FORMA: "PALAVRA" " " "DICA".

int main()
{
    setlocale(LC_ALL, "Portuguese");

    cout << "Bem Vindo a Forca" << endl;
    cout << "CHFR - 2019" << endl;

    //MENU
    while((opUsuario != '1') || (opUsuario != '2') || (opUsuario != '3') || (opUsuario != '4'))
    {
        cout << "\nDigite 1 para jogar | 2 para sair | 3 para regras | 4 para adicionar palavra: ";
        cin >> opUsuario;

        if(opUsuario == '1')
        {
            break;
        }else if(opUsuario == '2')
        {
            rodando = 0;
            break;
        }else if(opUsuario == '3')
        {
            regras();
        }else if(opUsuario == '4')
        {
            addPalavra();
        }
    }

    system("cls");

    //gerando palavra aleatoria
    pegarPalavra();

    //Loop principal do jogo.
    while(rodando == 1)
    {
        system("cls");

        imprimirGrafico();
        entradaUsuario();
        checarGanhador();
    }

    return 0;
}

int checarLetra()
{
    int letraExiste = 0; //0 - Não existe   1- Existe

    //Se a letra existir substitua no vetor "lacuna".
    for(int i = 0; i < 20; i++)
    {
        if(palavra[i] == letraUsuario)
        {
            letraExiste = 1;
            lacuna [i] = letraUsuario;
        }
    }

    //Se a letra não existir diminuir as vidas.
    if(letraExiste == 0)
    {
        vidas--;
    }
}

int checarPalavra()
{
    //SE O JOGADOR QUISER ADVINHAR A PALAVRA IRA SER TUDO OU NADA.

    int palavrasIguais = 1; // 0 - Palavras diferentes / 1 - Palavras iguais.


    //Chcando se as palavras do usuario e a escolhida sao iguais.
    for(int ic = 0; ic < 20; ic++)
    {
        if(palavra[ic] != palavraUsuario[ic])
        {
            palavrasIguais = 0;
            break;
        }
    }

    //Se as palavras forem iguais entao o jogador ganha senao chame tele de perdedor.
    if(palavrasIguais == 1)
    {
        telaGanhador();
        rodando = 0;
    }else if(palavrasIguais == 0)
    {
        telaMorte();
        rodando = 0;
    }
}

int checarGanhador()
{
    int tracosRestantes = 0; //0-Não restam traços 1-Restao traços

    //Checando se restam traços no vetor "lacuna".
    for(int i = 0; i < 20; i++)
    {
        if(lacuna[i] == '-')
        {
            tracosRestantes = 1;
        }

    }

    //Se o numero de vidas for 0 entao chame o fim do jogo.
    if(vidas == 0)
    {
        telaMorte();
        rodando = 0;
    }


    //Se nao restarem traços no vetor "lacuna" entoo o jogador ganha.
    if(tracosRestantes == 0)
    {
        telaGanhador();
        rodando = 0;
    }
}

int entradaUsuario()
{
    while((opUsuario != '1') || (opUsuario != '2') || (opUsuario != '3'))
    {
        cout << "\n Digite 1 para uma letra | 2 para a palavra | 3 para sair: ";
        cin >> opUsuario;

        if(opUsuario == '1')
        {
            cout << "\n Digite a letra: ";
            cin >> letraUsuario;
            checarLetra();
            break;
        }else if(opUsuario == '2')
        {
            //SE O JOGADOR QUISER ADVINHAR A PALAVRA IRA SER TUDO OU NADA.
            cout << "\n Digite a palavra: ";
            cin >> palavraUsuario;
            checarPalavra();
            break;
        }else if(opUsuario == '3')
        {
            rodando = 0;
            system("cls");
            break;
        }
    }
}

int pegarPalavra()
{
    FILE *bancoPalavras;
    char caracter, identificador = '\n';
    int numeroLinhas = 0;
    int contadorArquivo = 0;

    //Abrindo o arquivo -- IMPORTANTE: CHECAR SE O NOME É O MESMO NAO DUAS VEZES QUE O ARQUIVO É ABERTO --
    bancoPalavras = fopen("BDpalavras.forca","r");

    if(bancoPalavras == NULL)
    {
        cout << "Erro ao tentar abrir arquivo." << endl;
        rodando = 0;
    }else
    {
        //Contando o numero de linhas para gerar o o numero aleatorio.
        while(fread (&caracter, sizeof(char), 1, bancoPalavras))
        {
            if(caracter == identificador)
                numeroLinhas++;
        }

        //Fechabdo o arquivo
        fclose(bancoPalavras);
    }

    //Gerando aleatorio
    srand(time(NULL));
    aleatorio = (rand() % numeroLinhas);

    //Abrindo novamente o arquivo pois fazer o processo inteiro com ele aberto causa erro.
    bancoPalavras = fopen("BDpalavras.forca","r");


    //Para deixar mais simples optei por usar vetores auxiliares que sempre sao modificados com um carater especifico.
    //Assim se o numero aleatorio for diferente do numero da linha lida ele ira prencher os vetores auxiliares.
    //E se for igual ele ira copiar para os vetores principais ate que o carater do vetor auxiliar seja diferente de '$'.
    char vetAux1[20];
    char vetAux2[20];

    //Preenchendo os vetores auxiliares para que se o numero sorteado seja o primeiro ele nao tenha lixo.
    for(int iL = 0; iL < 20; iL ++)
    {
        vetAux1[iL] = '$';
        vetAux2[iL] = '$';
    }

    if(bancoPalavras == NULL)
    {
        cout << "Erro ao tentar abrir arquivo." << endl;
        rodando = 0;
    }else
    {
        //Lendo linha ate que o numero da linha lida seja o mesmo que o aleatorio.
        while(contadorArquivo <= aleatorio)
        {
            //Lendo linha do txt e slavando nos vetores auxiliares.
            fscanf(bancoPalavras, "%s" "%s", &vetAux1, &vetAux2);

            //Se o numero da linha lida for igual ao aleatorio passar para os vetores principais.
            if(contadorArquivo == aleatorio)
            {
                for(int i = 0; i < 20; i++)
                {
                    if(vetAux1[i] != '$')
                    {
                        palavra[i] = vetAux1[i];
                    }else if(vetAux1[i] == '$')
                    {
                        break;
                    }
                }

                for(int i2 = 0; i2 < 20; i2++)
                {
                    if(vetAux2[i2] != '$')
                    {
                        dica[i2] = vetAux2[i2];
                    }else if(vetAux2[i2] == '$')
                    {
                        break;
                    }
                }
            }else   //Senao for igual ao aleatorio entao resetar o vetor para '$".
            {
                for(int i3 = 0; i3 < 20; i3 ++)
                {
                    vetAux1[i3] = '$';
                    vetAux2[i3] = '$';
                }
            }

            contadorArquivo++;
        }

        //Fechando novamente o arquivo.
        fclose(bancoPalavras);
    }

    //Coloca no vetor "lacuna" o caracter "-" para indicar a quantidade de letras da palavra.
    for(int i = 0; i < 20; i++)
    {
        if(palavra[i] != '\0')
        {
            lacuna[i] = '-';
        }
    }
}

int addPalavra()
{
    system("cls");
    char entraPalavra[20];
    char entraDica[20];
    char opUsuarioEntra = '0';
    char adicionarPalavra = '1';

    FILE *bancoPalavras;

    bancoPalavras = fopen("BDpalavras.forca","a+");

    if(bancoPalavras == NULL)
    {
        cout << "Erro ao tentar abrir arquivo." << endl;
        rodando = 0;
    }else
    {
        while(adicionarPalavra == '1')
        {
            cout << "Digite uma palavra: ";
            cin >> entraPalavra;

            cout << "Digite a dica para essa palavra: ";
            cin >> entraDica;

            while((opUsuarioEntra != '1') || (opUsuarioEntra != '2'))
            {
                cout << "Tem certeza que quer adicionar essa palavra ao banco? [1-SIM | 2-NAO]: ";
                cin >> opUsuarioEntra;

                if(opUsuarioEntra == '1')
                {
                    long size;

                    //Se o banco estever vazio ele deve prencher a primeria linha senao pula e escreve na proxima.
                    fseek (bancoPalavras, 0, SEEK_END); //Posiciona no começo do arquivo
                    size = ftell (bancoPalavras);   //Diz o tamanho do arquivo

                    if(size == 0)
                    {
                        fprintf(bancoPalavras, "%s" " " "%s",entraPalavra, entraDica);
                    }else
                    {
                        fprintf(bancoPalavras, "\n%s" " " "%s",entraPalavra, entraDica);
                    }

                    cout << "Palavra adicionada." << endl;
                    break;
                }else if(opUsuarioEntra == '2')
                {
                    cout << "Ok, então vou esquece-la." << endl;
                    break;
                }
            }

            cout << endl;
            cout << "Quer adicionar mais alguma palavra? [1-SIM | 2-NAO]: ";
            cin >> adicionarPalavra;
            cout << endl;
        }

        //Fechabdo o arquivo
        fclose(bancoPalavras);
    }
    system("cls");
}

int regras()
{
    system("cls");
    cout << "------------------------------- REGRAS -----------------------------------" << endl;
    cout << "- O objetivo do jogo e adivinhar a palavra antes que voce morra." << endl;
    cout << "- Voce tera 7 vidas e a cada erro voce perdera 1 vida." << endl;
    cout << "- Se souber a palavra podera tentar advinhar mas tera somente uma chance." << endl;
    cout << "- Espero que voce se divirta." << endl;
    cout << "--------------------------------------------------------------------------" << endl;
}

int telaMorte()
{
    system("cls");
    cout << "Que pena, voce perdeu!" << endl;
}

int telaGanhador()
{
    system("cls");
    cout << "Voce ganhou, Parabens!!!" << endl;
}

int imprimirGrafico()
{
    switch(vidas)
    {
        case 7:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 6:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 5:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 4:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |           |                   \n"
                 << " |    |          /|                   \n"
                 << " |    |         / |                   \n"
                 << " |    |        /  |                   \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 3:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |           |                   \n"
                 << " |    |          /|\\                 \n"
                 << " |    |         / | \\                \n"
                 << " |    |        /  |  \\               \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 2:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |           |                   \n"
                 << " |    |          /|\\                 \n"
                 << " |    |         / | \\                \n"
                 << " |    |        /  |  \\               \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |          /                    \n"
                 << " |    |         /                     \n"
                 << " |    |        /                      \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;


        case 1:
            cout << "  _____________________________       \n"
                 << " |                             |      \n"
                 << " |     ________________________|      \n"
                 << " |    |           ||                  \n"
                 << " |    |           ||                  \n"
                 << " |    |        ___||__                \n"
                 << " |    |       | X   X |               \n"
                 << " |    |       |  ___  |               \n"
                 << " |    |       |_______|               \n"
                 << " |    |           |                   \n"
                 << " |    |          /|\\                 \n"
                 << " |    |         / | \\                \n"
                 << " |    |        /  |  \\               \n"
                 << " |    |           |                   \n"
                 << " |    |           |                   \n"
                 << " |    |          / \\                 \n"
                 << " |    |         /   \\                \n"
                 << " |    |        /     \\               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    |                               \n"
                 << " |    | Vidas: "<< vidas <<"          \n"
                 << " |    | Dica: "<< dica <<"            \n"
                 << " |    | Palavra: "<< lacuna <<"       \n";
            break;
    }
    return 0;
}
