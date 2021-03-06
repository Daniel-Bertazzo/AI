/*
    Estrutura do arquivo a ser gerado:
    [linhas] [colunas]
    [matriz]

    compilacao:
    g++ gerador.cpp -o gerador
*/

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        cout << "Modo de uso:" << endl << "./gerador [Mode]" << endl;
        cout << "Mode = 1 => paredes nas linhas" << endl;
        cout << "Mode = 2 => paredes nas colunas" << endl;
        cout << "Mode = 3 => labirinto vazio, pontos de origem e destino opostos" << endl;
        cout << "Mode = 4 => labirinto aleatorio" << endl;
        cout << "Colocar '> out.txt' ao chamar o programa, para colocar o labirinto gerado em um arquivo texto," << endl;
        cout << "podendo ser usado como entrada das buscas posteriormente."
        exit(0);
    }

    int rows, cols;
    int xi, yi, xf, yf;
    int i, j;
    char **lab;

    int door;
  
    /* ..:: Modo 1 (paredes nas linhas) ::.. */
    // Gera labirintos do tipo:
    //
    //       * * # * * * * * * *
    //       - - - - * - - - - -
    //       * * * * * * * * * *
    //       - * - - - - - - - -
    //       * * * * * * * * * *
    //       - - - - - - - - * -
    //       * * * * * $ * * * *
    //
    if (atoi(argv[1]) == 1) {
        rows = (rand() % 50) + 15;
        cols = (rand() % 50) + 15;
        cout << rows << " " << cols << endl;

        lab = new char*[rows];
        for (i = 0; i < rows; i++) {
            lab[i] = new char[cols];
            for (j = 0; j < cols; j++) {
                lab[i][j] = '*';
            }
        }

        for (i = 1; i < rows; i += 2) {
            door = rand() % cols;
            for (j = 0; j < cols; j++) {
                if (j != door) {
                    lab[i][j] = '-';
                }
            }
        }
        yi = rand() % cols;
        yf = rand() % cols;

        lab[0][yi] = '#';
        lab[rows-1][yf] = '$';
    }

    /* ..:: Modo 2 (paredes nas colunas) ::.. */
    // Gera labirintos do tipo:
    // 
    //     * - * - * - * - * - *
    //     * - * - * - * * * - *
    //     * - * - * - * - * - $
    //     * - * * * - * - * - *
    //     # - * - * * * - * - *
    //     * * * - * - * - * * *
    //     * - * - * - * - * - *
    // 
    else if (atoi(argv[1]) == 2) {
        rows = (rand() % 50) + 15;
        cols = (rand() % 50) + 15;
        cout << rows << " " << cols << endl;

        lab = new char*[rows];
        for (i = 0; i < rows; i++) {
            lab[i] = new char[cols];
            for (j = 0; j < cols; j++) {
                lab[i][j] = '*';
            }
        }

        for (j = 0; j < cols; j++) {
            door = rand() % rows;
            for (i = 0; i < rows; i++) {
                if (i != door)
                    lab[i][j] = '-';
            }
        }
        yi = rand() % cols;
        yf = rand() % cols;

        lab[0][yi] = '#';
        lab[rows-1][yf] = '$';
    }

    /* ..:: Modo 3 (labirinto vazio) ::.. */
    // Gera labirintos do tipo:
    // 
    //     # * * * * * * * * *
    //     * * * * * * * * * *
    //     * * * * * * * * * *
    //     * * * * * * * * * *
    //     * * * * * * * * * *
    //     * * * * * * * * * *
    //     * * * * * * * * * $
    //
    else if (atoi(argv[1]) == 3) {
        cin >> rows >> cols;

        lab = new char*[rows];
        for (i = 0; i < rows; i++) {
            lab[i] = new char[cols];
            for (j = 0; j < cols; j++) {
                lab[i][j] = '*';
            }
        }
        lab[0][0] = '#';
        lab[rows-1][cols-1] = '$';
    }

    /* ..:: Modo 4 (labirinto aleatorio) ::.. */
    // Gera labirintos do tipo:
    // 
    //     * * * # * * * * * *
    //     * * - - - - - - - *
    //     - * - * * - * - * *
    //     * * - * - * - * * *
    //     - * - * * * - - - *
    //     - * * * * * * * * *
    //     - * * - * * * * * $
    //
    else if (atoi(argv[1]) == 4) {
        // Valores das linhas e colunas gereados aleatoriamente
        rows = (rand() % 50) + 15;
        cols = (rand() % 50) + 15;

        // Dimensoes limite da DFS recursiva
        // rows = 20;
        // cols = 21;

        // Valor maximo testado para as outras buscas (todas menos DFS)
        // rows = 1000;
        // cols = 1000;
        cout << rows << " " << cols << endl;

        lab = new char*[rows];
        for (i = 0; i < rows; i++) {
            lab[i] = new char[cols];
            for (j = 0; j < cols; j++) {
                lab[i][j] = '*';
            }
        }

        // Chance de ser caminho = threshold
        // Chance de ser parede =  100 - threshold
        int cell, threshold = 60;

        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                cell = rand() % 100;
                if (cell >= threshold) {
                    lab[i][j] = '-';
                } else {
                    lab[i][j] = '*';
                }
            }
        }

        // Inicio na primeira linha, fim na ultima linha
        yi = rand() % cols;
        yf = rand() % cols;
        lab[0][yi] = '#';
        lab[rows-1][yf] = '$';

        // Inicio na primeira coluna, fim na ultima coluna
        // xi = rand() % (rows/2);
        // xf = rand() % (rows/2) + (rows/2);
        // lab[xi][0] = '#';
        // lab[xf][cols-1] = '$';

    }

    // Imprime o labirinto
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            cout << lab[i][j];
        }
        cout << endl;
    }

    // Libera memoria
    for (i = 0; i < rows; i++) {
        delete lab[i];
    }
    delete lab;
    return 0;
}