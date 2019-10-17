/*
    Estrutura do arquivo a ser gerado:
    [linhas] [colunas]
    [matriz]
*/

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        cout << "Modo de uso:" << endl;
        cout << "./gerador [M]" << endl;
        cout << "M = 1 => paredes nas linhas" << endl << "M = 2 => paredes nas colunas" << endl;
        exit(0);
    }

    int rows, cols;
    int xi, yi, xf, yf;
    int i, j;
    char **lab;

    rows = (rand() % 50) + 5;
    cols = (rand() % 50) + 5;

    cout << rows << " " << cols << endl;

    lab = new char*[rows];
    for (i = 0; i < rows; i++) {
        lab[i] = new char[cols];
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            lab[i][j] = '*';
        }
    }


    int door;
    // Paredes nas linhas
    if (atoi(argv[1]) == 1) {
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

    // Paredes nas colunas
    else if (atoi(argv[1]) == 2) {
        for (j = 0; j < cols; j++) {
            door = rand() % rows;
            for (i = 0; i < rows; i++) {
                if (i != door)
                    lab[i][j] = '-';
            }
        }
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