#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;


/* ..:: Classe Vertice ::.. */
/*****************************************************************************************************/


class Vertice {
public:
    int x, y;

    Vertice();
    Vertice(int x, int y);
    void set_x(int x);
    void set_y(int y);
};

Vertice::Vertice() {
    this->x = 0;
    this->y = 0;
}

Vertice::Vertice(int x, int y) {
    this->x = x;
    this->y = y;
}

void Vertice::set_x(int x) {
    this->x = x;
}

void Vertice::set_y(int y) {
    this->y = y;
}


/* ..:: Classe Caminho ::.. */
/*****************************************************************************************************/


class Caminho {
public:
    int tamanho, peso;
    vector<Vertice> c;

    Caminho();
    void add_vertice(Vertice v);
    void exibe_caminho();
    void pop_back();
};

Caminho::Caminho() {
    this->tamanho = 0;
    this->peso = 0;
}

void Caminho::add_vertice(Vertice v) {
    this->c.push_back(v);
    this->tamanho++;
}

void Caminho::exibe_caminho() {
    cout << "tamanho: " << this->tamanho << endl << "[ ";
    for(int i = 0; i < this->tamanho; i++) {
        cout << "(" << this->c[i].x << ", " << this->c[i].y << ") ";
    }
    cout << "]" << endl;
}

void Caminho::pop_back() {
    this->c.pop_back();
    this->tamanho--;
}


/* ..:: Classe Labirinto ::.. */
/*****************************************************************************************************/


class Labirinto {
public:
    int lin, col; // quantidade de linhas e colunas do labirinto
    int xi, yi, xf, yf; // posicoes inicial e do objetivo
    vector< vector<char> > m; // Matriz que representa o labirinto
    
    Labirinto();

    void le_entrada();
    void exibe_labirinto();
    int DFS(int x, int y, Caminho& c);

};

Labirinto::Labirinto() {
    this->lin = 0;
    this->col = 0;
    this->xi = 0;
    this->yi = 0;
    this->xf = 0;
    this->yf = 0;
}

void Labirinto::le_entrada() {
    char entrada;
    vector<char> aux;
    
    cin >> this->lin >> this->col;

    for(int i = 0; i < this->lin; i++) {
        aux.clear();
        for(int j = 0; j < this->col; j++) {
            cin >> entrada;
            aux.push_back(entrada);

            // trata comeco e fim do labirinto
            if(entrada == '#') {
                this->xi = i;
                this->yi = j;
            }
            if(entrada == '$') {
                this->xf = i;
                this->yf = j;
            }
        }
        this->m.push_back(aux);
    }
}

void Labirinto::exibe_labirinto() {
    for(int i = 0; i < this->lin; i++) {
        for (int j = 0; j < this->col; j++) {
            cout << this->m[i][j];
        }    
        cout << endl;
    }
}

int Labirinto::DFS(int x, int y, Caminho& c) {

    // verifica se x ou y estao fora do labirinto ou sao paredes ou ja foram percorridos.
    if((x < 0 or x >= this->lin) or (y < 0 or y >= this->col) or (this->m[x][y] == '-' or this->m[x][y] == 'O'))
        return 0;

    // coloca (x,y) no caminho
    //cout << x << " " << y << endl;
    c.add_vertice(Vertice(x, y));
    this->m[x][y] = 'O';
    //cout << "adicionou" << endl;

    if(x == this->xf and y == this->yf){
        //cout << "CHEGOU!!!!!!!!!" << endl;
        this->m[x][y] = '$';
        return 1;
    }
    int r = 0;
    
    // ordem (horario): cima dir baixo esq
    r = DFS(x-1, y, c); // Cima
    if(r == 1) { return 1; }
    r = DFS(x, y+1, c); // Direita
    if(r == 1) { return 1; }
    r = DFS(x+1, y, c); // Baixo
    if(r == 1) { return 1; }
    r = DFS(x, y-1, c); // Esquerda
    if(r == 1) { return 1; }

    // se chegou aqui, o caminho nao vai pra lugar nenhum, portanto deve voltar
    c.pop_back();
    this->m[x][y] = '*';
    //cout << "voltando : " << x << " " << y << endl;
    return 0;
}


/* ..:: Main ::.. */
/*****************************************************************************************************/


int main(int argc, char const **argv) {
    
    Labirinto lab;
    lab.le_entrada();
    lab.exibe_labirinto();

    Caminho c;

    lab.DFS(lab.xi, lab.yi, c);
    cout << "teminou dfs" << endl;
    c.exibe_caminho();
    lab.exibe_labirinto();

    return 0;
}