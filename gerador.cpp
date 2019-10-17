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


    int index;
    for (i = 1; i < rows; i += 2) {
        index = rand() % cols;
        for (j = 0; j < cols; j++) {
            if (j != index) {
                lab[i][j] = '-';
            }
        }
    }

    xi = rand() % rows;
    yi = rand() % cols;
    xf = rand() % rows;
    yf = rand() % cols;

    // cout << xi << " " << yi << endl;
    // cout << xf << " " << yf << endl;

    lab[xi][yi] = '#';
    lab[xf][yf] = '$';

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            cout << lab[i][j];
        }
        cout << endl;
    }

    for (i = 0; i < rows; i++) {
        delete lab[i];
    }
    delete lab;

    return 0;
}