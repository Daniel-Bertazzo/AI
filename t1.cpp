/* Trabalho 1 de Inteligencia Artificial, 2o semestre 2019
    Buscas em labirinto.

    Grupo:
        Alexandre Norcia Medeiros    - 10295583
        Daniel Penna Chaves Bertazzo - 10349561
        Miguel de Mattos Gardini     - 10295728
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>

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
    void exibe();
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

void Vertice::exibe() {
    cout << "x: " << this->x << ", y: " << this->y << endl;
}


/* ..:: Classe Caminho ::.. */
/*****************************************************************************************************/

class Caminho {
public:
    float h; // valor da funcao heuristica (a*)
    int tamanho;
    float peso;
    vector<Vertice> c;

    Caminho();
    void push_back(Vertice v);
    void exibe();
    void pop_back();
    Vertice back() const;
    void calcula_h(int x, int y);

    // sobrecarga de operadores
    bool operator< (const Caminho& c2) const;
    bool operator> (const Caminho& c2) const;
};

Caminho::Caminho() {
    this->tamanho = 0;
    this->peso = 0.0;
    this->h = 0.0;
}

void Caminho::push_back(Vertice v) {
    this->c.push_back(v);
    this->tamanho++;
}

void Caminho::exibe() {
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
Vertice Caminho::back() const{
    return this->c.back();
}

/* Funcao que calcula a heuristica h para o a*
    Distancia euclidiana -> sqrt((x-xf)^2 + (y-yf)^2)
*/
void Caminho::calcula_h(int x, int y) {
    Vertice aux = this->back();
    this->h = sqrt((aux.x - x)*(aux.x - x) + (aux.y - y)*(aux.y - y));
}

 /* Sobrecarga da comparacao '<'
 */
bool Caminho::operator< (const Caminho& c2) const {
    return this->peso < c2.peso;
}

 /* Sobrecarga da comparacao '>'
 */
bool Caminho::operator> (const Caminho& c2) const {
    return this->peso > c2.peso;
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
    void exibe();
    int DFS(int x, int y, Caminho& c, float passo, char ordem);
    void BFS(int x, int y, Caminho& c);
    void Best_first_search(int x, int y, Caminho& c);
    void a_estrela(int x, int y, Caminho& c);

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

/*
    **** APAGAR DEPOIS -> COLOCAR NO RELATORIO ****
    Impressao do labirinto:
    Foi usado o ANSI color code para imprimir cores diferentes para cada componente
    do labirinto (parede, origem, objetivo, caminho final, pontos visitados)
*/
void Labirinto::exibe() {
    for (int i = 0; i < this->lin; i++) {
        for (int j = 0; j < this->col; j++) {
            if (this->m[i][j] == '-')
                cout << "\033[1;41m-\033[0m";
            else if (this->m[i][j] == '#')
                cout << "\033[1;46m#\033[0m";
            else if (this->m[i][j] == '$')
                cout << "\033[1;42m$\033[0m";
            else if (this->m[i][j] == 'O')
                cout << "\033[1;33mO\033[0m";
            else if (this->m[i][j] == '+')
                cout << "\033[21;33m+\033[0m";
            else if (this->m[i][j] >= 'a' and this->m[i][j] <= 'z')
                printf("\033[1;33m%c\033[0m", this->m[i][j]);
            else
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
        int ordem - ordem no caminho
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
int Labirinto::DFS(int x, int y, Caminho& c, float passo, char ordem) {

    if (ordem > 'z')
        ordem = 'a';

    // verifica se x ou y estao fora do labirinto ou sao paredes ou ja foram percorridos.
    // if ((x < 0 or x >= this->lin) or (y < 0 or y >= this->col) or (this->m[x][y] == '-' or this->m[x][y] == 'O'))
    if ((x < 0 or x >= this->lin) or (y < 0 or y >= this->col) or (this->m[x][y] != '*' and this->m[x][y] != '$' and this->m[x][y] != '#'))
        return 0;

    // coloca (x,y) no caminho
    c.push_back(Vertice(x, y));
    c.peso += passo;
    this->m[x][y] = ordem;

    if (x == this->xf and y == this->yf) {
        this->m[x][y] = '$';
        this->m[this->xi][this->yi] = '#';
        return 1;
    }
    int r = 0;
    
    // Ordem (horario e diagonal):cima, direita, baixo, esquerda, nordeste, sudeste, sudoeste, noroeste
    r = DFS(x-1, y, c, 1, ordem+1); // Cima
    if (r == 1) return 1;
    r = DFS(x, y+1, c, 1, ordem+1); // Direita
    if (r == 1) return 1;
    r = DFS(x+1, y, c, 1, ordem+1); // Baixo
    if (r == 1) return 1;
    r = DFS(x, y-1, c, 1, ordem+1); // Esquerda
    if (r == 1) return 1;
    
    r = DFS(x-1, y+1, c, RAIZ_2, ordem+1); // diagonal nordeste
    if (r == 1) return 1;
    r = DFS(x+1, y+1, c, RAIZ_2, ordem+1); // diagonal sudeste
    if (r == 1) return 1;
    r = DFS(x+1, y-1, c, RAIZ_2, ordem+1); // diagonal sudoeste
    if (r == 1) return 1;
    r = DFS(x-1, y-1, c, RAIZ_2, ordem+1); // diagonal noroeste
    if (r == 1) return 1;
   

    // se chegou aqui, o caminho nao vai pra lugar nenhum, portanto deve voltar
    c.pop_back();
    c.peso -= passo;
    this->m[x][y] = '*';
    if (c.tamanho == 0) // caso onde nao existe caminho da origem ate o destino
        this->m[x][y] = '#'; // remove status de visitado da origem
    
    return 0;
}

/* Busca em largura iterativa.
    Parametros:
        int x,y - vertice inical
        caminho c - guarda o caminho final, vazio caso nao exista.

    COLOCAR NO RELATORIO: (*** APAGAR DEPOIS ***)
    na BFS, todos os vertices visitados durante a execucao sao marcados no labirinto com a letra '+',
    porem apenas o caminho final eh marcado com a letra 'O', assim como na DFS.
*/            
void Labirinto::BFS(int x, int y, Caminho& c) {

    Caminho aux_c;
    Vertice vert;
    queue<Caminho> q;

    aux_c.push_back(Vertice(x,y));
    q.push(aux_c); // adiciona vertice inicial na fila
    this->m[x][y] = '+'; // marca como visitado
    this->m[this->xf][this->yf] = '*'; // marca objetivo com nao visitado.

    while(!q.empty()) {
        // remove caminho atual.
        aux_c = q.front();
        vert = aux_c.back();
        q.pop();

        // verifica se terminou
        if(vert.x == this->xf and vert.y == this->yf) {
            this->m[vert.x][vert.y] = '$';
            c = aux_c;
            break;
        }

        // gera caminhos do aux_c
        // Ordem (diagonal e horario): nordeste, sudeste, sudoeste, noroeste, cima, direita, baixo, esquerda
        // adiciona diagonal nordeste na fila
        if(vert.x-1 >= 0 and vert.y+1 < this->col and this->m[vert.x-1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y+1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudeste na fila
        if(vert.x+1 < this->lin and vert.y+1 < this->col and this->m[vert.x+1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y+1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudoeste na fila
        if(vert.x+1 < this->lin and vert.y-1 >= 0 and this->m[vert.x+1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y-1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y-1] = '+'; // marca como visitado.
        }
        // adiciona diagonal noroeste na fila
        if(vert.x-1 >= 0 and vert.y-1 >= 0 and this->m[vert.x-1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y-1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y-1] = '+'; // marca como visitado.
        }
        
        // Cima 
        if(vert.x-1 >= 0 and this->m[vert.x-1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.peso -= 1.0;
            aux_c.pop_back();
            this->m[vert.x-1][vert.y] = '+'; // marca como visitado.
        }
        // Direita
        if(vert.y+1 < this->col and this->m[vert.x][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y+1));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y+1] = '+'; // marca como visitado.
        }
        // Baixo 
        if(vert.x+1 < this->lin and this->m[vert.x+1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x+1][vert.y] = '+'; // marca como visitado.
        }
        // Esquerda
        if(vert.y-1 >= 0 and this->m[vert.x][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y-1));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y-1] = '+'; // marca como visitado.
        }
    }
    
    // marca caminho percorrido no labiritno.
    this->m[this->xi][this->yi] = '#';
    char ordem = 'a';
    for(int i = 1; i < c.tamanho -1; i++) {
        vert = c.c[i];
        this->m[vert.x][vert.y] = ordem;
        (ordem == 'z') ? ordem = 'a' : ordem++;
    }
}


/* Busca best first.
    Parametros:
        int x,y - vertice inical
        caminho c - guarda o caminho final, vazio caso nao exista.

    COLOCAR NO RELATORIO: (apagar depois)
    nessa busca acha o menor por explorar o menor.
    porem ele sempre explora o menor caminho atual, idependente de quao perto esse caminho esta do destino
    assim ele visita muito mais vertices que a busca a*
*/            
void Labirinto::Best_first_search(int x, int y, Caminho& c) {

    Caminho aux_c;
    Vertice vert;

    // fila de prioridade com ordem crescente (menor fica no topo)
    priority_queue<Caminho, vector<Caminho>, greater<Caminho> > q;

    aux_c.push_back(Vertice(x,y));
    q.push(aux_c); // adiciona vertice inicial na fila
    this->m[x][y] = '+'; // marca como visitado
    this->m[this->xf][this->yf] = '*'; // marca objetivo com nao visitado.

    while(!q.empty()) {
        // remove caminho atual.
        aux_c = q.top();
        vert = aux_c.back();
        q.pop();

        // verifica se terminou
        if(vert.x == this->xf and vert.y == this->yf) {
            this->m[vert.x][vert.y] = '$';
            c = aux_c;
            break;
        }

        // gera caminhos do aux_c
        // Ordem (diagonal e horario): nordeste, sudeste, sudoeste, noroeste, cima, direita, baixo, esquerda
        // adiciona diagonal nordeste na fila
        if(vert.x-1 >= 0 and vert.y+1 < this->col and this->m[vert.x-1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y+1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudeste na fila
        if(vert.x+1 < this->lin and vert.y+1 < this->col and this->m[vert.x+1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y+1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudoeste na fila
        if(vert.x+1 < this->lin and vert.y-1 >= 0 and this->m[vert.x+1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y-1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y-1] = '+'; // marca como visitado.
        }
        // adiciona diagonal noroeste na fila
        if(vert.x-1 >= 0 and vert.y-1 >= 0 and this->m[vert.x-1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y-1));
            aux_c.peso += RAIZ_2;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y-1] = '+'; // marca como visitado.
        }
        
        // Cima 
        if(vert.x-1 >= 0 and this->m[vert.x-1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.peso -= 1.0;
            aux_c.pop_back();
            this->m[vert.x-1][vert.y] = '+'; // marca como visitado.
        }
        // Direita
        if(vert.y+1 < this->col and this->m[vert.x][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y+1));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y+1] = '+'; // marca como visitado.
        }
        // Baixo 
        if(vert.x+1 < this->lin and this->m[vert.x+1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x+1][vert.y] = '+'; // marca como visitado.
        }
        // Esquerda
        if(vert.y-1 >= 0 and this->m[vert.x][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y-1));
            aux_c.peso += 1.0;
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y-1] = '+'; // marca como visitado.
        }
    }
    
    // marca caminho percorrido no labiritno.
    this->m[this->xi][this->yi] = '#';
    char ordem = 'a';
    for(int i = 1; i < c.tamanho -1; i++) {
        vert = c.c[i];
        this->m[vert.x][vert.y] = ordem;
        (ordem == 'z') ? ordem = 'a' : ordem++;
    }
}

/* Busca A*.
    Parametros:
        int x,y - vertice inical
        caminho c - guarda o caminho final, vazio caso nao exista.

    COLOCAR NO RELATORIO: (apagar depois)
    ele busca pelo melhor caminho definido pelo menor valor f(x)
    f(x) = g(x) + h(x)
    onde g(x) = tamanho atual do caminho
    e h(x) = { sqrt((x-xf)^2 + (y-yf)^2) } distancia euclidiana entre o vertice e o destino

*/            
void Labirinto::a_estrela(int x, int y, Caminho& c) {

    Caminho aux_c;
    Vertice vert;

    // declara f(x)
    auto f_comparacao = [](const Caminho& c1, const Caminho& c2) 
        {
            return ((c1.peso + c1.h) > (c2.peso + c2.h));
        };

    // fila de prioridade com ordem crescente (menor fica no topo)
    priority_queue<Caminho, vector<Caminho>, decltype(f_comparacao)> q(f_comparacao);

    aux_c.push_back(Vertice(x,y));
    aux_c.calcula_h(this->xf, this->yf);
    q.push(aux_c); // adiciona vertice inicial na fila
    this->m[x][y] = '+'; // marca como visitado
    this->m[this->xf][this->yf] = '*'; // marca objetivo com nao visitado.

    while(!q.empty()) {
        // remove caminho atual.
        aux_c = q.top();
        vert = aux_c.back();
        q.pop();

        // verifica se terminou
        if(vert.x == this->xf and vert.y == this->yf) {
            this->m[vert.x][vert.y] = '$';
            c = aux_c;
            break;
        }

        // gera caminhos do aux_c
        // Ordem (diagonal e horario): nordeste, sudeste, sudoeste, noroeste, cima, direita, baixo, esquerda
        // adiciona diagonal nordeste na fila
        if(vert.x-1 >= 0 and vert.y+1 < this->col and this->m[vert.x-1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y+1));
            aux_c.peso += RAIZ_2;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudeste na fila
        if(vert.x+1 < this->lin and vert.y+1 < this->col and this->m[vert.x+1][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y+1));
            aux_c.peso += RAIZ_2;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y+1] = '+'; // marca como visitado.
        }
        // adiciona diagonal sudoeste na fila
        if(vert.x+1 < this->lin and vert.y-1 >= 0 and this->m[vert.x+1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y-1));
            aux_c.peso += RAIZ_2;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x+1][vert.y-1] = '+'; // marca como visitado.
        }
        // adiciona diagonal noroeste na fila
        if(vert.x-1 >= 0 and vert.y-1 >= 0 and this->m[vert.x-1][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y-1));
            aux_c.peso += RAIZ_2;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= RAIZ_2;
            this->m[vert.x-1][vert.y-1] = '+'; // marca como visitado.
        }
        
        // Cima 
        if(vert.x-1 >= 0 and this->m[vert.x-1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x-1, vert.y));
            aux_c.peso += 1.0;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.peso -= 1.0;
            aux_c.pop_back();
            this->m[vert.x-1][vert.y] = '+'; // marca como visitado.
        }
        // Direita
        if(vert.y+1 < this->col and this->m[vert.x][vert.y+1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y+1));
            aux_c.peso += 1.0;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y+1] = '+'; // marca como visitado.
        }
        // Baixo 
        if(vert.x+1 < this->lin and this->m[vert.x+1][vert.y] == '*' ) {
            aux_c.push_back(Vertice(vert.x+1, vert.y));
            aux_c.peso += 1.0;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x+1][vert.y] = '+'; // marca como visitado.
        }
        // Esquerda
        if(vert.y-1 >= 0 and this->m[vert.x][vert.y-1] == '*' ) {
            aux_c.push_back(Vertice(vert.x, vert.y-1));
            aux_c.peso += 1.0;
            aux_c.calcula_h(this->xf, this->yf);
            q.push(aux_c); // cria uma copia de aux em q
            aux_c.pop_back();
            aux_c.peso -= 1.0;
            this->m[vert.x][vert.y-1] = '+'; // marca como visitado.
        }
    }
    
    // marca caminho percorrido no labiritno.
    this->m[this->xi][this->yi] = '#';
    char ordem = 'a';
    for(int i = 1; i < c.tamanho -1; i++) {
        vert = c.c[i];
        this->m[vert.x][vert.y] = ordem;
        (ordem == 'z') ? ordem = 'a' : ordem++;
    }
}

/* ..:: Main ::.. */
/*****************************************************************************************************/

int main(int argc, char const **argv) {
    
    Labirinto lab;
    lab.le_entrada();
    cout << "Labirinto a ser explorado:" << endl << endl;
    lab.exibe();
    cout << endl << "-------------------------------------------------------------------------";
    cout << endl << endl;

    Caminho c;

    // lab.DFS(lab.xi, lab.yi, c, 0, 'a'-1);
    // cout << "teminou dfs" << endl;
    
    // lab.BFS(lab.xi, lab.yi, c);
    // cout << "teminou bfs" << endl;
    
    lab.Best_first_search(lab.xi, lab.yi, c);
    cout << "terminou best first" << endl;

    // lab.a_estrela(lab.xi, lab.yi, c);
    // cout << "terminou A*" << endl;

    cout << "Caminho final:" << endl << endl;
    c.exibe();
    cout << endl << "-------------------------------------------------------------------------";
    cout << endl << endl << endl;

    cout << "Labirinto apos execucao:" << endl << endl;
    lab.exibe();
    cout << endl << "-------------------------------------------------------------------------";
    cout << endl << endl;

    return 0;
}