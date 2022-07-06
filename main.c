/*
 * ---
 * ---
 * ---
 * ---
 * IPCA - EST
 * LESI - SO
 * Programa do Trabalho Prático - Parte 1
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAX 9999

/// Função utilizada para mostrar no ecrã todo o conteúdo de um ficheiro
int mostraFicheiro(char fileName[50])
{
    int fd, len;
    char content[MAX];
    fd = open(fileName, O_RDONLY);
    if (fd == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }
    len = read(fd, content, sizeof(content));
    write(STDOUT_FILENO, content, len);
    close(fd);

    return 1;
}

/// Função utilizada para acrescentar num ficheiro o conteúdo de outro
void acrescentaFicheiro(char fileName1[50], char fileName2[50])
{
    int fd1, fd2;
    char c;
    fd1 = open(fileName1, O_RDONLY);
    fd2 = open(fileName2, O_APPEND | O_WRONLY);

    if (fd1 == -1 || fd2 == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }

    while (read(fd1, &c, sizeof(c)))
    {
        write(fd2, &c, sizeof(c));
    }

    close(fd1);
    close(fd2);
}

/// Função utilizada para contar as linhas de um ficheiro
int contaLinhas(char fileName[50])
{
    char buffer[MAX];
    int len;
    unsigned lines = 1;

    int fd1 = open(fileName, O_RDONLY);

    if (fd1 == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }
    

    while (len = read(fd1, buffer, sizeof(buffer)))
    {
        int i;
        for (i = 0; i < len; i++)
            if (buffer[i] == '\n')
                ++lines;
    }
    
    close(fd1);
    return lines;
}

/// Função utilizada para apagar um ficheiro
void apagaFicheiro(char fileName[50])
{
    int fd;
    fd = open(fileName, O_RDONLY);

    if (fd == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }

    unlink(fileName);

    close(fd);
}

/// Função utilizada para criar um ficheiro com o contéudo de outro
void copiarFicheiro(char fileName[50])
{

    char copia[100];
    
    int ficheiroI, ficheiroC, leitura;


    ficheiroC = creat("ficheiro.copia.txt", S_IRWXU);

    ficheiroI = open(fileName, O_RDONLY);

    if (ficheiroI == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }

    leitura = read(ficheiroI, copia, sizeof(copia));

    write(ficheiroC, copia, leitura);

    close(ficheiroI);
    close(ficheiroC);
}

/// Função utilizada para obter a informação sobre um ficheiro
void informaFicheiro(char fileName[50])
{
    struct stat file;
    int fd;

    stat(fileName, &file);

    printf("st_mode = %d\n", file.st_mode);

    if (fd = open(fileName, O_RDONLY) == -1)
    {
        write(STDERR_FILENO, "Ficheiro não existe", 20);
    }
    else
    {
        if (S_ISDIR(file.st_mode))
        {
            printf("\nDiretoria\n");
        }
        else if (S_ISREG(file.st_mode))
        {
            printf("\nFicheiro\n");
        }
        else if (S_ISLNK(file.st_mode))
        {
            printf("\nLink\n");
        }

        printf("\nI-node: %lo\n", file.st_ino);

        printf("Utilizador dono: %o\n", file.st_uid);

        printf("Data de criaçao: %s\n", ctime(&file.st_ctime));

        printf("Data de leitura: %s\n", ctime(&file.st_atime));

        printf("Data de modificaçao: %s\n", ctime(&file.st_mtime));
    }
}

/// Função utilizada para obter todos os ficheiros de uma determinada diretoria
void listaDiretoria(char path[100])
{
    DIR* dir;
    struct dirent *d;
    struct stat file;


    if(path=="")
    {
        dir = opendir(".");
    }
    else
    {
        dir = opendir(path);
    }

    if(dir == NULL)
    {
        write(STDERR_FILENO, "Diretoria não existe", 21);
    }

    while ((d=readdir(dir))!=NULL)
    {
        stat(d->d_name, &file);

        if (S_ISDIR(file.st_mode))
        {
            printf("Diretoria\n");
        }
        else if (S_ISREG(file.st_mode))
        {
            printf("Ficheiro\n");
        }
        printf("->%s\n", d->d_name);
    }
    
    closedir(dir);
}

int main()
{
    int linhas = 0, num;
    char escolha[10];
    char fileName[50];
    char fileNameCopy[50];
    char path[100];
    int decisao;

    write(STDOUT_FILENO, "\n1 - Mostra Ficheiro!\n", 22);
    write(STDOUT_FILENO, "2 - Copia Ficheiro!\n", 21);
    write(STDOUT_FILENO, "3 - Acrescentar ficheiro!\n", 27);
    write(STDOUT_FILENO, "4 - Contar linhas!\n", 20);
    write(STDOUT_FILENO, "5 - Apagar ficheiro!\n", 22);
    write(STDOUT_FILENO, "6 - Informação ficheiro!\n", 27);
    write(STDOUT_FILENO, "7 - Lista diretoria!\n", 22);
    write(STDOUT_FILENO, "Escolha: ", 10);
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        write(STDOUT_FILENO, "Insira o nome do ficheiro: ", 28);
        scanf(" %s", fileName);
        mostraFicheiro(fileName);
        main();
        break;
    case 2:
        write(STDOUT_FILENO, "Insira o nome do ficheiro que quer copiar: ", 44);
        scanf(" %s", fileName);
        copiarFicheiro(fileName);
        main();
        break;
    case 3:
        write(STDOUT_FILENO, "Insira o nome do ficheiro original: ", 37);
        scanf(" %s", fileName);
        write(STDOUT_FILENO, "Insira o nome do ficheiro para o qual quer copiar: ", 52);
        scanf(" %s", fileNameCopy);
        acrescentaFicheiro(fileName, fileNameCopy);
        main();
        break;
        
    case 4:
        write(STDOUT_FILENO, "Insira o nome do ficheiro: ", 28);
        scanf(" %s", fileName);
        printf("Linhas: %d\n", contaLinhas(fileName));
        main();
        break;
        
    case 5:
        write(STDOUT_FILENO, "Insira o nome do ficheiro que deseja apagar: ", 46);
        scanf(" %s", fileName);
        apagaFicheiro(fileName);
        main();
        break;
    case 6:
        write(STDOUT_FILENO, "Insira o nome do ficheiro: ", 28);
        scanf(" %s", fileName);
        informaFicheiro(fileName);
        main();
        break;
    case 7:
        write(STDOUT_FILENO, "1 - Usar diretoria do código\n2 - Usar outra diretoria\nOpção: ", 65);
        scanf(" %d", &decisao);
        if (decisao == 1)
        {
            listaDiretoria("");
        }
        else
        {
            write(STDOUT_FILENO, "Insira o caminho da diretoria: ", 31);
            scanf(" %s", path);
            listaDiretoria(path);
        }
        main();
        break;
    default:
        break;
    }
    return 0;
}
