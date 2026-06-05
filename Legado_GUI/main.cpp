#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "gui_glut/gui.h"

// CÂMERA
float px = 0.0f, py = 0.0f, pz = 0.0f;
float angulo = 0.0f;

// DADOS
const int TAM = 10;
int vetor[TAM]        = {8, 3, 9, 1, 5, 2, 7, 4, 10, 6};
int vetor_orig[TAM]   = {8, 3, 9, 1, 5, 2, 7, 4, 10, 6};

// ESTADOS VISUAIS DOS BLOCOS
enum EstadoBloco { NORMAL, COMPARANDO, TROCANDO, FINALIZADO };
EstadoBloco estado[TAM];

void resetEstados() {
    for (int k = 0; k < TAM; k++) estado[k] = NORMAL;
}

// INTERFACE DO ALGORITMO
struct Algoritmo {
    virtual void reset()  = 0;
    virtual void passo()  = 0;
    virtual const char* nome() = 0;
    bool concluido = false;
};

// BUBBLE SORT
struct BubbleSort : Algoritmo {
    int i, j;

    void reset() override {
        i = 0; j = 0; concluido = false;
        resetEstados();
    }

    const char* nome() override { return "Bubble Sort  [SPACE=passo | R=reset | TAB=algoritmo]"; }

    void passo() override {
        if (concluido) return;
        resetEstados();

        if (i < TAM - 1) {
            if (j < TAM - i - 1) {
                estado[j]   = COMPARANDO;
                estado[j+1] = COMPARANDO;

                if (vetor[j] > vetor[j+1]) {
                    swap(vetor[j], vetor[j+1]);
                    estado[j]   = TROCANDO;
                    estado[j+1] = TROCANDO;
                }
                j++;
            } else {
                estado[TAM - 1 - i] = FINALIZADO;
                j = 0;
                i++;
            }
        } else {
            concluido = true;
            for (int k = 0; k < TAM; k++) estado[k] = FINALIZADO;
            cout << "[Bubble Sort] Ordenado!" << endl;
        }
    }
};

// SELECTION SORT
struct SelectionSort : Algoritmo {
    int i, j, idx_min;

    void reset() override {
        i = 0; j = 1; idx_min = 0; concluido = false;
        resetEstados();
    }

    const char* nome() override { return "Selection Sort  [SPACE=passo | R=reset | TAB=algoritmo]"; }

    void passo() override {
        if (concluido) return;
        resetEstados();

        for (int k = 0; k < i; k++) estado[k] = FINALIZADO;

        if (i < TAM - 1) {
            if (j < TAM) {
                estado[i]       = TROCANDO;   
                estado[idx_min] = COMPARANDO;
                estado[j]       = COMPARANDO;

                if (vetor[j] < vetor[idx_min]) idx_min = j;
                j++;
            } else {
                if (idx_min != i) {
                    swap(vetor[i], vetor[idx_min]);
                    estado[i]       = TROCANDO;
                    estado[idx_min] = TROCANDO;
                } else {
                    estado[i] = FINALIZADO;
                }
                i++;
                idx_min = i;
                j = i + 1;
            }
        } else {
            concluido = true;
            for (int k = 0; k < TAM; k++) estado[k] = FINALIZADO;
            cout << "[Selection Sort] Ordenado!" << endl;
        }
    }
};

// INSERTION SORT
struct InsertionSort : Algoritmo {
    int i, j;

    void reset() override {
        i = 1; j = 1; concluido = false;
        resetEstados();
        estado[0] = FINALIZADO;
    }

    const char* nome() override { return "Insertion Sort  [SPACE=passo | R=reset | TAB=algoritmo]"; }

    void passo() override {
        if (concluido) return;
        resetEstados();

        for (int k = 0; k < i; k++) estado[k] = FINALIZADO;

        if (i < TAM) {
            if (j > 0 && vetor[j-1] > vetor[j]) {
                estado[j]   = TROCANDO;
                estado[j-1] = TROCANDO;
                swap(vetor[j], vetor[j-1]);
                j--;
            } else {
                estado[i] = FINALIZADO;
                i++;
                j = i;
            }
        } else {
            concluido = true;
            for (int k = 0; k < TAM; k++) estado[k] = FINALIZADO;
            cout << "[Insertion Sort] Ordenado!" << endl;
        }
    }
};

// MERGE SORT  
struct MergeSort : Algoritmo {
    struct Passo { int idx_a, idx_b; bool eh_troca; };
    vector<Passo> passos;
    int cursor;

    void geraPassos(int* v, int n) {
        passos.clear();
        int tmp[TAM];
        for (int k = 0; k < n; k++) tmp[k] = v[k];

        for (int width = 1; width < n; width *= 2) {
            for (int lo = 0; lo < n; lo += 2 * width) {
                int mid = min(lo + width, n);
                int hi  = min(lo + 2 * width, n);

                int left[TAM], right[TAM];
                int llen = mid - lo, rlen = hi - mid;
                for (int x = 0; x < llen; x++) left[x]  = tmp[lo + x];
                for (int x = 0; x < rlen; x++) right[x] = tmp[mid + x];

                int li = 0, ri = 0, vi = lo;
                while (li < llen && ri < rlen) {
                    bool troca = (right[ri] < left[li]);
                    passos.push_back({lo + li, mid + ri, troca});
                    if (!troca) { tmp[vi++] = left[li++]; }
                    else        { tmp[vi++] = right[ri++]; }
                }
                while (li < llen) { tmp[vi++] = left[li++]; }
                while (ri < rlen) { tmp[vi++] = right[ri++]; }
            }
        }
    }

    void reset() override {
        concluido = false;
        cursor = 0;
        geraPassos(vetor_orig, TAM);
        for (int k = 0; k < TAM; k++) vetor[k] = vetor_orig[k];
        resetEstados();
    }

    const char* nome() override { return "Merge Sort  [SPACE=passo | R=reset | TAB=algoritmo]"; }

    void passo() override {
        if (concluido) return;
        resetEstados();

        if (cursor < (int)passos.size()) {
            auto& p = passos[cursor];
            estado[p.idx_a] = COMPARANDO;
            estado[p.idx_b] = COMPARANDO;
            if (p.eh_troca) {
                swap(vetor[p.idx_a], vetor[p.idx_b]);
                estado[p.idx_a] = TROCANDO;
                estado[p.idx_b] = TROCANDO;
            }
            cursor++;
        } else {
            concluido = true;
            for (int k = 0; k < TAM; k++) estado[k] = FINALIZADO;
            cout << "[Merge Sort] Ordenado!" << endl;
        }
    }
};


// QUICK SORT  
struct QuickSort : Algoritmo {
    struct Faixa { int lo, hi; };
    vector<Faixa> pilha;
    int lo_atual, hi_atual, pivo, i_part, j_part;
    bool particionando;

    void reset() override {
        concluido = false;
        pilha.clear();
        if (TAM > 1) pilha.push_back({0, TAM - 1});
        carregaProxima();
        resetEstados();
    }

    void carregaProxima() {
        particionando = false;
        if (!pilha.empty()) {
            auto f = pilha.back(); pilha.pop_back();
            lo_atual  = f.lo;
            hi_atual  = f.hi;
            pivo      = vetor[hi_atual];
            i_part    = lo_atual - 1;
            j_part    = lo_atual;
            particionando = true;
        }
    }

    const char* nome() override { return "Quick Sort  [SPACE=passo | R=reset | TAB=algoritmo]"; }

    void passo() override {
        if (concluido) return;
        resetEstados();

        if (!particionando) { concluido = true; for (int k=0;k<TAM;k++) estado[k]=FINALIZADO; return; }

        if (j_part < hi_atual) {
            estado[j_part]   = COMPARANDO;
            estado[hi_atual] = COMPARANDO; 

            if (vetor[j_part] <= pivo) {
                i_part++;
                swap(vetor[i_part], vetor[j_part]);
                estado[i_part] = TROCANDO;
                estado[j_part] = TROCANDO;
            }
            j_part++;
        } else {
            int pos_pivo = i_part + 1;
            swap(vetor[pos_pivo], vetor[hi_atual]);
            estado[pos_pivo] = FINALIZADO;

            if (pos_pivo - 1 > lo_atual)  pilha.push_back({lo_atual,     pos_pivo - 1});
            if (pos_pivo + 1 < hi_atual)  pilha.push_back({pos_pivo + 1, hi_atual});

            carregaProxima();
        }
    }
};

// GERENCIADOR DE ALGORITMOS
BubbleSort    algo_bubble;
SelectionSort algo_selection;
InsertionSort algo_insertion;
MergeSort     algo_merge;
QuickSort     algo_quick;

Algoritmo* algoritmos[] = { &algo_bubble, &algo_selection, &algo_insertion, &algo_merge, &algo_quick };
const int NUM_ALGOS = 5;
int algo_atual = 0;

Algoritmo* algo() { return algoritmos[algo_atual]; }

void resetTudo() {
    for (int k = 0; k < TAM; k++) vetor[k] = vetor_orig[k];
    algo()->reset();
}

void embaralha() {
    srand((unsigned)time(nullptr));
    for (int k = TAM - 1; k > 0; k--) {
        int r = rand() % (k + 1);
        swap(vetor[k], vetor[r]);
        swap(vetor_orig[k], vetor_orig[r]);
    }
    algo()->reset();
    cout << "Vetor embaralhado!" << endl;
}

void aplicaCor(int k, float altura) {
    (void)altura;
    switch (estado[k]) {
        case COMPARANDO: GUI::setColor(9, 9, 0); break; // amarelo
        case TROCANDO:   GUI::setColor(9, 1, 1); break; // vermelho
        case FINALIZADO: GUI::setColor(1, 9, 3); break; // verde
        default:         GUI::setColor(3, 5, 9); break; // azul neutro
    }
}

void desenhaBloco(int k) {
    glScalef(-1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        // Base
        aplicaCor(k, 0);
        glNormal3f(0, -1, 0);
        glVertex3f(-1,0,1); glVertex3f(-1,0,-1); glVertex3f(1,0,-1); glVertex3f(1,0,1);
        // Frontal
        aplicaCor(k, 1);
        glNormal3f(1,0,0);
        glVertex3f(1,0,1); glVertex3f(1,0,-1); glVertex3f(1,1,-1); glVertex3f(1,1,1);
        // Traseira
        glNormal3f(0,0,-1);
        glVertex3f(1,0,-1); glVertex3f(-1,0,-1); glVertex3f(-1,1,-1); glVertex3f(1,1,-1);
        // Esquerda
        glNormal3f(-1,0,0);
        glVertex3f(-1,0,-1); glVertex3f(-1,0,1); glVertex3f(-1,1,1); glVertex3f(-1,1,-1);
        // Topo
        aplicaCor(k, 2);
        glNormal3f(0,0,1);
        glVertex3f(-1,0,1); glVertex3f(1,0,1); glVertex3f(1,1,1); glVertex3f(-1,1,1);
        // Superior
        glNormal3f(0,1,0);
        glVertex3f(1,1,1); glVertex3f(1,1,-1); glVertex3f(-1,1,-1); glVertex3f(-1,1,1);
    glEnd();
}

// DESENHO PRINCIPAL
void desenha() {
    GUI::displayInit();
    GUI::drawOrigin(1.0);

    glPushMatrix();
        glTranslatef(px - 12.0f, py, pz - 20.0f);
        glRotatef(angulo, 0.0f, 1.0f, 0.0f);

        for (int k = 0; k < TAM; k++) {
            glPushMatrix();
                glTranslatef(k * 2.5f, 0.0f, 0.0f);

                if (!algo()->concluido &&
                    (estado[k] == COMPARANDO || estado[k] == TROCANDO)) {
                    glTranslatef(0.0f, 0.0f, 2.0f);
                }

                glScalef(1.0f, (float)vetor[k], 1.0f);
                desenhaBloco(k);
            glPopMatrix();
        }
    glPopMatrix();

    GUI::displayEnd();
}

void teclado(unsigned char tecla, int x, int y) {
    GUI::keyInit(tecla, x, y);
    switch (tecla) {
        // Câmera
        case 'w': pz -= 0.5f; break;
        case 's': pz += 0.5f; break;
        case 'a': px -= 0.5f; break;
        case 'd': px += 0.5f; break;
        case 'u': py += 0.5f; break;
        case 'i': py -= 0.5f; break;
        case 'k': angulo += 5.0f; break;
        case 'l': angulo -= 5.0f; break;

        // Controles de algoritmo
        case ' ':  algo()->passo(); break;           // avança um passo
        case 'r':  resetTudo(); break;               // reseta com mesmo vetor
        case 'e':  embaralha(); break;               // embaralha e reseta
        case '\t': // TAB — troca algoritmo
            algo_atual = (algo_atual + 1) % NUM_ALGOS;
            resetTudo();
            cout << "Algoritmo: " << algo()->nome() << endl;
            break;
    }
}

int main() {
    srand((unsigned)time(nullptr));

    for (int i = 0; i < NUM_ALGOS; i++) algoritmos[i]->reset();

    cout << "=== Visualizador de Ordenacao 3D ===" << endl;
    cout << "SPACE  -> passo" << endl;
    cout << "R      -> resetar" << endl;
    cout << "E      -> embaralhar" << endl;
    cout << "TAB    -> trocar algoritmo" << endl;
    cout << "WASD / U,I / K,L -> camera" << endl;
    cout << "Algoritmo atual: " << algo()->nome() << endl;

    GUI gui = GUI(1024, 768, desenha, teclado);
    return 0;
}