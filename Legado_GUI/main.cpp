#include <iostream>
using namespace std;

#include "gui.h"

const int TAM_VECTOR = 10;
int vet[TAM_VECTOR] = {9, 8, 6, 3, 6, 1, 4, 7, 2, 4};



// Variável global para controlar a posição no eixo X
float px = 0.0; 
float pz = 0.0;
float py = 0.0;
float angulo = 0.0;


void desenhaBloco(){
    
    glBegin(GL_QUADS);
        // Cacular norma:
        // vetor U = B - A
        // vetor V = C - A
        // normal  = U × V  (produto vetorial)
        //
        // ponto A = (-1.0, 0.0, 1.0)
        // ponto B = (-1.0, 0.0, -1.0)
        // ponto C = (1.0, 0.0, -1.0)
        // B - A = (-2.0, 0.0, -2.0)
        // C - A =(2.0, 0.0, -2.0)
        //
        //  i  j  k  i  j
        // -0  0 -2  0  0
        //  2  0 -2  2  0
        // 
        // -4j
        // (0 -4, 0)
        // agora normal
        // (sqrt(0² + 4² + 0²))
        //  sqrt(16)
        // (0, 4, 0)
        //
        // normalizando:
        // normal normalizada
        // = (0/4, -4/4, 0/4) = (0, -1, 0)
        //


        // base do bloco
        GUI::setColor(0.0, 10, 10);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(-1.0, 0.0, 1.0);
        glVertex3f(-1.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, 1.0);

         // Face Traseira
        GUI::setColor(5, 1, 9);
        glNormal3f(0, 0, -1);
        glVertex3f(1, 0, -1);
        glVertex3f(-1, 0, -1);
        glVertex3f(-1, 1, -1);
        glVertex3f(1, 1, -1);

        //Face Frontal
        GUI::setColor(0, 9, 1);
        glNormal3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 0, -1);
        glVertex3f(1, 1, -1);
        glVertex3f(1, 1, 1);


        // Face Esquerda
        GUI::setColor(5, 5, 5);
        glNormal3f(-1.0, 0, 0);
        glVertex3f(-1.0, 0, -1);
        glVertex3f(-1, 0, 1);
        glVertex3f(-1, 1, 1);
        glVertex3f(-1, 1, -1);

        // Topo
        GUI::setColor(9, 9, 9);
        glNormal3f(0, 0, 1);
        glVertex3f(-1, 0, 1);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(-1, 1, 1);

        // Face Direita
        GUI::setColor(5, 9, 1);
        glNormal3f(0, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, -1);
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, 1, 1);
    glEnd();
}



void desenha() {
    GUI::displayInit();
    GUI::drawOrigin(1.0); 

    glPushMatrix();
        glTranslatef(px - 12.0f, py, pz - 20.0f);
        glRotatef(angulo, 0.0, 1.0, 0.0);

        for(int i = 0; i < TAM_VECTOR; i++){
            glPushMatrix();
                glTranslatef(i * 2.5f, 0.0, 0.0);
                glScalef(1.0f, (float)vet[i], 1.0f);
                desenhaBloco();
            glPopMatrix();
        }
    glPopMatrix();

    // Joga pra tela
    GUI::displayEnd();
}


void teclado(unsigned char tecla, int x, int y) {
    GUI::keyInit(tecla, x, y);

     switch(tecla) {
        // Movimento da câmera (WASD + UI)
        case 'w': pz -= 0.5; break; 
        case 's': pz += 0.5; break; 
        case 'a': px -= 0.5; break; 
        case 'd': px += 0.5; break; 
        case 'i': py -= 0.5; break;
        case 'u': py += 0.5; break;
        
        // Rotação da câmera
        case 'k': angulo += 5.0; break; 
        case 'l': angulo -= 5.0; break; 
    }
}

int main() {
    cout << "Fase 1: Geometria Base e Estrutura 3D." << endl;
    cout << "Controles: WASD, I/U (Cima/Baixo), K/L (Girar)." << endl;
    
    GUI gui = GUI(1024, 768, desenha, teclado);
    return 0;
}



// void desenhaQuadrado() {
//     //glTranslatef(1.0f, 2.0f, 0.0f); transladar o objeto 2d em (1, 2)
//     //glDisable(GL_CULL_FACE);
//     // 5a: Quadrado com glNormal3f.
//     glTranslatef(0.5f, 0.5f, 0.0f);
//     glBegin(GL_QUADS);
//         GUI::setColor(9, 4, 1);
//         // O vetor normal (0, 0, 1) aponta para "fora" da tela, direto pro seu olho.
//         // Declaramos ele UMA vez, e os 4 vértices assumem essa propriedade.
//         glNormal3f(0.0, 0.0, 1.0); 

//         glVertex3f(-0.5, -0.5, 0.0); // Inferior Esquerdo
//         GUI::setColor(2, 4, 1);
//         glVertex3f( 0.5, -0.5, 0.0); // Inferior Direito
//         GUI::setColor(10, 2, 2);
//         glVertex3f( 0.5,  0.5, 0.0); // Superior Direito
//         GUI::setColor(0, 2, 10);
//         glVertex3f(-0.5,  0.5, 0.0); // Superior Esquerdo
//     glEnd();
// }

// void desenhaPiramide() {
//     //glDisable(GL_CULL_FACE);

//     // Passo 1: Base (Quadrado no chão)
//     glBegin(GL_QUADS);
//         GUI::setColor(0.5, 0.5, 0.5); 
//         glNormal3f(0.0, -1.0, 0.0); // Aponta para baixo (-Y)
        
//         glVertex3f(-1.0, 0.0,  1.0); 
//         glVertex3f( -1.0, 0.0,  -1.0); 
//         glVertex3f( 1.0, 0.0, -1.0); 
//         glVertex3f(1.0, 0.0, 1.0); 
//     glEnd();

//     // Passo 2: Laterais (Triângulos ancorados no Ápice)
//     glBegin(GL_TRIANGLES);
        
//         // Face Frontal (+Z, +Y)
//         GUI::setColor(1.0, 0.0, 0.0); 
//         glNormal3f(0.0, 0.707, 0.707);
//         glVertex3f( 0.0, 1.0,  0.0); // Topo
//         glVertex3f(-1.0, 0.0,  1.0); // Esq
//         glVertex3f( 1.0, 0.0,  1.0); // Dir

//         // Face Direita (+X, +Y)
//         GUI::setColor(0.0, 1.0, 0.0); 
//         glNormal3f(0.707, 0.707, 0.0);
//         glVertex3f( 0.0, 1.0,  0.0);
//         glVertex3f( 1.0, 0.0,  1.0);
//         glVertex3f( 1.0, 0.0, -1.0);

//         // Face Traseira (-Z, +Y)
//         GUI::setColor(0.0, 0.0, 1.0); 
//         glNormal3f(0.0, 0.707, -0.707);
//         glVertex3f( 0.0, 1.0,  0.0);
//         glVertex3f( 1.0, 0.0, -1.0);
//         glVertex3f(-1.0, 0.0, -1.0);

//         // Face Esquerda (-X, +Y)
//         GUI::setColor(1.0, 1.0, 0.0); 
//         glNormal3f(-0.707, 0.707, 0.0);
//         glVertex3f( 0.0, 1.0,  0.0);
//         glVertex3f(-1.0, 0.0, -1.0);
//         glVertex3f(-1.0, 0.0,  1.0);
        
//     glEnd();
// }


// void triangulo(){
//     //glTranslated(1.0f, 2.0f, 0.0f);
//     //glScaled(2.0f, 2.0f, 0.0f);

//     glBegin(GL_TRIANGLES);
//         // (1.0, -1.0, 0) - (-1, -1, 0) 
//         //  (2, 0, 0)
//         //
//         //
//         // (0, 1.0, 0) - (-1, -1, 0) 
//         // (1, 2, 0)
//         // 
//         //
//         // normalização pelo produto vetorial ()
//         // i  j  k  i  j
//         // 2  0  0  2  0 
//         // 1  2  0  1  2
//         //
//         //(0, 0, 4) 
//         // agora divide pelo modulo
//         // sqrt (4^2) = 4
//         // 
//         //0/4 0/4 4/4
//         // (0, 0, 1)
        
//         GUI::setColor(9, 5, 1);
//         glNormal3f(0, 0, 1);
//         glVertex3f(-1.0, -1.0, 0); 
//         glVertex3f(1.0, -1.0, 0);
//         glVertex3f(0.0, 1.0, 0.0);
//     glEnd();
// }


// void desenhaBloco(){
//     //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
//     //glScalef(1.0f, 2.0f, 1.0f);
//     //glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
//     //glTranslatef(3.0f, 0.0f, 0.0f);
//     //glScalef(2.0f, 1.0f, 1.0f);
//     // Depois das transformações originais, aplique:
//     // glScalef(0.5f, 1.0f, 1.0f);          // inverso do scale (1/2)
//     // glTranslatef(-3.0f, 0.0f, 0.0f);      // inverso da translação
//     // glRotatef(-90.0f, 0.0f, 0.0f, 1.0f); // inverso da rotação (-90° em torno de +Z)
    

//     glScalef(-1.0f, 1.0f, 1.0f);
//     glBegin(GL_QUADS);
//         //normal
//         //(-1.0, 0.0, -1.0) - (-1.0, 0.0, 1.0) = u = (0, 0, -2)
//         // (1.0, 0.0, -1.0) - (-1.0, 0.0, 1.0) =  v = (-1, 0, -2)
//         //
//         // i  j  k  i  j
//         // 0  0 -2  0  0
//         //-1  0 -2 -1  0
//         //
//         //(0, -2, 0)
//         //
//         // (tira a normal)
//         // sqrt(4) = 2
//         //0/2 -2/2 0/2
//         //(0, -1, 0)
//         GUI::setColor(9, 5, 1);
//         glNormal3f(0, -1, 0);
//         glVertex3f(-1.0, 0.0, 1.0);
//         glVertex3f(-1.0, 0.0, -1.0);
//         glVertex3f(1.0, 0.0, -1.0);
//         glVertex3f(1.0, 0.0, 1.0);


//         GUI::setColor(0, 9, 1);
//         glNormal3f(1, 0, 0);
//         glVertex3f(1, 0, 1);
//         glVertex3f(1, 0, -1);
//         glVertex3f(1,1, -1);
//         glVertex3f(1, 1, 1);

//         GUI::setColor(5, 1, 9);
//         glNormal3f(0, 0, -1);
//         glVertex3f(1, 0, -1);
//         glVertex3f(-1, 0, -1);
//         glVertex3f(-1, 1, -1);
//         glVertex3f(1, 1, -1);

//         GUI::setColor(5, 5, 5);
//         glNormal3f(-1.0, 0, 0 );
//         glVertex3f(-1.0, 0, -1);
//         glVertex3f(-1, 0, 1);
//         glVertex3f(-1, 1, 1);
//         glVertex3f(-1, 1, -1);

//         GUI::setColor(9,9,9);
//         glNormal3f(0,0,1);
//         glVertex3f(-1, 0, 1);
//         glVertex3f(1, 0, 1);
//         glVertex3f(1, 1, 1);
//         glVertex3f(-1, 1, 1);

//         GUI::setColor(5, 9, 1);
//         glNormal3f(0, 1, 0);
//         glVertex3f(1,1,1);
//         glVertex3f(1,1,-1);
//         glVertex3f(-1, 1, -1);
//         glVertex3f(-1, 1, 1);

//     glEnd();

// }



