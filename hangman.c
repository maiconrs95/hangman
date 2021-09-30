#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "hangman.h"

char palavrasecreta[20];
char chutes[26];
int chutesdados = 0;

FILE *openwordsfile(char *modes)
{
    FILE *f;

    f = fopen("palavras.txt", modes);

    if (f == 0)
    {
        printf("Desculpe, banco de palavras não disponível.\n\n");
        exit(1);
    }

    return f;
}

void adicionapalavra()
{
    char quer;

    printf("Você deseja adicionar uma nova palavra no jogo? S/N");
    scanf(" %c", &quer);

    if (quer == 'S')
    {
        char novapalavra[20];
        scanf("Qual a palavra?");
        scanf("%s", novapalavra);

        FILE *f;

        f = openwordsfile("r+");

        int qtdpalavrasatuais;

        fscanf(f, "%d", &qtdpalavrasatuais);

        qtdpalavrasatuais++;

        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", qtdpalavrasatuais);

        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", novapalavra);

        fclose(f);
    }
}

void abertura()
{
    printf("/****************/\n");
    printf("/ Jogo de Forca */\n");
    printf("/****************/\n\n");
}

void chuta()
{

    char chute;
    scanf(" %c", &chute);

    chutes[chutesdados] = chute;
    chutesdados++;
}

int jachutou(char letra)
{
    int achou = 0;

    for (int j = 0; j < chutesdados; j++)
    {
        if (chutes[j] == letra)
        {
            achou = 1;
            break;
        }
    }

    return achou;
}

void desenhaforca()
{
    for (int i = 0; i < strlen(palavrasecreta); i++)
    {
        int achou = jachutou(palavrasecreta[i]);

        if (achou)
        {
            printf("%c ", palavrasecreta[i]);
        }
        else
        {
            printf("_ ");
        }
    }
    printf("\n");
}

void escolhepalavra()
{
    FILE *f;

    f = fopen("palavras.txt", "r");

    if (f == 0)
    {
        printf("Desculpe, banco de palavras não disponível.\n\n");
        exit(1);
    }

    int qtdpalavras;
    fscanf(f, "%d", &qtdpalavras);
    srand(time(0));

    int randomico = rand() % qtdpalavras;

    for (int i = 0; i <= randomico; i++)
    {
        fscanf(f, "%s", palavrasecreta);
    }

    fclose(f);
}

int acertou()
{
    for (int i = 0; i < strlen(palavrasecreta); i++)
    {
        if (!jachutou(palavrasecreta[i]))
        {
            return 0;
        }
    }

    return 1;
}

int enforcou()
{
    int erros = 0;

    for (int i = 0; i < chutesdados; i++)
    {
        int existe = 0;

        for (int j = 0; j < strlen(palavrasecreta); j++)
        {
            if (chutes[i] == palavrasecreta[j])
            {
                existe = 1;
                break;
            }
        }

        if (!existe)
            erros++;
    }

    return erros >= 5;
}

int main()
{
    escolhepalavra();
    abertura();

    do
    {
        desenhaforca();
        chuta();
    } while (!acertou() && !enforcou());

    adicionapalavra();
}