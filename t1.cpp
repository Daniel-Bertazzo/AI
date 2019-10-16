/* Trabalho 1 de Inteligencia Artificial, 2o semestre 2019
    Buscas em labirinto.

    Grupo:
        Alexandre Norcia Medeiros    - 10295583
        Daniel Penna Chaves Bertazzo - 10349561
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

#define RAIZ_2 1.41421

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
    int tamanho;
    float peso;
    vector<Vertice> c;

    Caminho();
    void push_back(Vertice v);
    void exibe_caminho();
    void pop_back();
    Vertice back();
};

Caminho::Caminho() {
    this->tamanho = 0;
    this->peso = 0.0;
}

void Caminho::push_back(Vertice v) {
    this->c.push_back(v);
    this->tamanho++;
}

void Caminho::exibe_caminho() {
    cout << "tamanho: " << this->tamanho << ", peso: " << this->peso << endl << "[ ";
    for (int i = 0; i < this->tamanho; i++) {
        cout << "(" << this->c[i].x << ", " << this->c[i].y << ") ";
    }
    cout << "]" << endl;
}

void Caminho::pop_back() {
    this->c.pop_back();
    this->tamanho--;
}

/* Retorna o ultimo vertice do caminho
*/
Vertice Caminho::back() {
    return this->c.back();
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
    int DFS(int x, int y, Caminho& c, float passo);
    void BFS(int x, int y, Caminho& c);

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
    for (int i = 0; i < this->lin; i++) {
        for (int j = 0; j < this->col; j++) {
            cout << this->m[i][j];
        }    
        cout << endl;
    }
}

/* Busca em Profundidade recursiva.
    Parametros:
        x,y - posicao atual na busca
        caminho c - caminho atual percorrido, ira conter o caminho final caso tenha solucao
        float passo - peso do passo dado (diagonal, normal, comeco).
    Retorno:
        1 - caminho atual alcanca o destino
        0 - caminho atual nao alcanca o destino
*/
/* ****APAGAR DEPOIS
    tem que comentar no realatorio

    - algoritimo esta restrito a ordem definida de escolha do proximo passo.
        -> sempre faz na mesma ordem sem inteligencia, eficiencia depende do labirinto
           (nesse caso, o labirinto ideal seria uma reta identidade (nordeste))
    - ordem decidida, diagonais priorizadas
        -> faz com que o caminho seja menor, mas nao necessariamente menor peso
    - sentido horario

    - caso nao tenha solucao, o caminho c retorna vazio.
*/
int Labirinto::DFS(int x, int y, Caminho& c, float passo) {

    // verifica se x ou y estao fora do labirinto ou sao paredes ou ja foram percorridos.
    if ((x < 0 or x >= this->lin) or (y < 0 or y >= this->col) or (this->m[x][y] == '-' or this->m[x][y] == 'O'))
        return 0;

    // coloca (x,y) no caminho
    c.push_back(Vertice(x, y));
    c.peso += passo;
    this->m[x][y] = 'O';

    if (x == this->xf and y == this->yf) {
        this->m[x][y] = '$';
        this->m[this->xi][this->yi] = '#';
        return 1;
    }
    int r = 0;
    
    // Ordem (diagonal e horario): nordeste, sudeste, sudoeste, noroeste, cima, direita, baixo, esquerda
    r = DFS(x-1, y+1, c, RAIZ_2); // diagonal nordeste
    if (r == 1) return 1;
    r = DFS(x+1, y+1, c, RAIZ_2); // diagonal sudeste
    if (r == 1) return 1;
    r = DFS(x+1, y-1, c, RAIZ_2); // diagonal sudoeste
    if (r == 1) return 1;
    r = DFS(x-1, y-1, c, RAIZ_2); // diagonal noroeste
    if (r == 1) return 1;
    
    r = DFS(x-1, y, c, 1); // Cima
    if (r == 1) return 1;
    r = DFS(x, y+1, c, 1); // Direita
    if (r == 1) return 1;
    r = DFS(x+1, y, c, 1); // Baixo
    if (r == 1) return 1;
    r = DFS(x, y-1, c, 1); // Esquerda
    if (r == 1) return 1;

    // se chegou aqui, o caminho nao vai pra lugar nenhum, portanto deve voltar
    c.pop_back();
    c.peso -= passo;
    this->m[x][y] = '*';
    if (c.tamanho == 0) // caso onde nao existe caminho da origem ate o destino
        this->m[x][y] = '#'; // remove status de visitado da origem
    
    return 0;
}

/*
procedure BFS(G,start_v):
      let Q be a queue
      label start_v as discovered
      Q.enqueue(start_v)
      while Q is not empty
          v = Q.dequeue()
          if v is the goal:
              return v
          for all edges from v to w in G.adjacentEdges(v) do
             if w is not labeled as discovered:
                 label w as discovered
                 w.parent = v
                 Q.enqueue(w) 
*/
/* Busca em largura iterativa.
    Parametros:
        int x,y - vertice inical
        caminho c - guarda o caminho final, vazio caso nao exista.
*/            
void Labirinto::BFS(int x, int y, Caminho& c) {

    // fila: [ [(0, 3)] ]
    //       [ [(0, 3), (0, 4)], [(0, 3), (1, 3)] ]
    Caminho aux_c;
    Vertice vert;
    queue<Caminho> q;

    aux_c.push_back(Vertice(x,y));
    q.push(aux_c); // adiciona vertice inicial na fila
    this->m[x][y] = 'v'; // marca como visitado

    while (!q.empty()) {
        // remove caminho atual.
        aux_c = q.front();
        vert = aux_c.back();
        q.pop();

        // verifica se terminou
        if(vert.x == this->xf and vert.y == this->yf) {
            c = aux_c;
        }

        // gera caminhos do aux_c
        // Ordem (diagonal e horario): nordeste, sudeste, sudoeste, noroeste, cima, direita, baixo, esquerda
        // adiciona diagonal nordeste na fila
        if(vert.x-1 >= 0 and vert.y+1 < this->col and this->m[vert.x][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y+1));
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            this->m[vert.x][vert.y] = 'v'; // marca como visitado.
        }
        // adiciona diagonal sudeste na fila
        if(vert.x+1 < this->lin and vert.y+1 < this->col and this->m[vert.x][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y+1));
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            this->m[vert.x][vert.y] = 'v'; // marca como visitado.
        }
        // adiciona diagonal sudoeste na fila
        if(vert.x+1 < this->lin and vert.y-1 >= 0 and this->m[vert.x][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y-1));
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            this->m[vert.x][vert.y] = 'v'; // marca como visitado.
        }
        // adiciona diagonal noroeste na fila
        if(vert.x-1 >= 0 and vert.y-1 >= 0 and this->m[vert.x][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y-1));
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            this->m[vert.x][vert.y] = 'v'; // marca como visitado.
        }
    }
}


/* ..:: Main ::.. */
/*****************************************************************************************************/


int main(int argc, char const **argv) {
    
    Labirinto lab;
    lab.le_entrada();
    lab.exibe_labirinto();

    Caminho c;

    //lab.DFS(lab.xi, lab.yi, c, 0);
    //cout << "teminou dfs" << endl;
    
    lab.BFS(lab.xi, lab.yi, c);
    cout << "teminou bfs" << endl;
    
    c.exibe_caminho();
    lab.exibe_labirinto();

    return 0;
}