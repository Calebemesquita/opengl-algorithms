#include <iostream>

using namespace std;

#include <gui.h>
#include <vector>

#include "Transform.h"
#include <Model3D.h>

Model3D* esqueleto = new Model3D("3ds/skeleton.3ds");

bool coresArquivo = true;
bool mostrarCasa = true;
bool mostrarEsqueleto = false;
bool mostrarEsferaCortada = false;

// float px = 2.0;
// float py = 1.0;
// float raio = 0.2;

Transform transf;
int idText = 3;

void casa() {
    //frontal
    glBegin(GL_POLYGON);
        glNormal3f( 0.0, 0.0, 1.0 );
        glTexCoord2f(0.5,0); glVertex3f( 0.0, 0.0, 1.0 );
        glTexCoord2f(1,0); glVertex3f( 1.0, 0.0, 1.0 );
        glTexCoord2f(1,1); glVertex3f( 1.0, 1.0, 1.0 );
        glTexCoord2f(0.5,1); glVertex3f( 0.0, 1.0, 1.0 );
    glEnd();
    //traseira
    glBegin(GL_POLYGON);
        glNormal3f( 0.0, 0.0,-1.0 );
        glVertex3f( 0.0, 1.0, 0.0 );
        glVertex3f( 1.0, 1.0, 0.0 );
        glVertex3f( 1.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
    glEnd();
    //lateral esquerda
    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 0.0, 0.0 );
        glTexCoord2f(0,0); glVertex3f( 0.0, 0.0, 0.0 );
        glTexCoord2f(0.5,0); glVertex3f( 0.0, 0.0, 1.0 );
        glTexCoord2f(0.5,1); glVertex3f( 0.0, 1.0, 1.0 );
        glTexCoord2f(0,1); glVertex3f( 0.0, 1.0, 0.0 );
    glEnd();
    //lateral direita
    glBegin(GL_POLYGON);
        glNormal3f( 1.0, 0.0, 0.0 );
        glVertex3f( 1.0, 0.0, 0.0 );
        glVertex3f( 1.0, 1.0, 0.0 );
        glVertex3f( 1.0, 1.0, 1.0 );
        glVertex3f( 1.0, 0.0, 1.0 );
    glEnd();
    //teto esquerdo (exemplo de face inclinada)
    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 1.0, 0.0 );
        glVertex3f( 0.0, 1.0, 0.0 );
        glVertex3f( 0.0, 1.0, 1.0 );
        glVertex3f( 0.5, 1.5, 1.0 );
        glVertex3f( 0.5, 1.5, 0.0 );
    glEnd();
}

void trianguloColorido() {
    //triangulo colorido
    glBegin(GL_POLYGON);
        glNormal3f( 0.0, 0.0, 1.0 );
        GUI::setColor(1,0,0);
        glVertex3f( 0.0, 0.0, 0.0 );
        GUI::setColor(0,1,0);
        glVertex3f( 1.0, 0.0, 0.0 );
        GUI::setColor(0,0,1);
        glVertex3f( 1.0, 1.0, 0.0 );
    glEnd();
}

void casaTransformada() {
    glPushMatrix();
        transf.apply();
        // GUI::setColor(0.5,0.7,0.7);
        GUI::setColor(1.0,1.0,1.0);
        casa();
    glPopMatrix();
}

void esqueletoTransformado() {
    glPushMatrix();
        //transformacoes do objeto ja desenhado de acordo com o seu sistema local
        transf.apply();
        GUI::setColor(0.2,0.8,0.2); //GUI::setColor(1.0,1.0,1.0);

        //desfazendo as transformações usadas na hora da modelagem
        //trazendo para a origem, alinhando com os eixos locais e ajustando para um tamanho adequado (Sl.Rl.Tl)
        // glScalef(1,1,1);
        glRotatef(90, 1,0,0); // glRotatef(-90, 1,0,0);
        glTranslatef(0,0,-0.85); // glTranslatef(0,0,0);
        //primeiro aplica uma escala para que o objeto fique visível adequadamente no cenário
        float s = 0.02; // s = 0.02; glTranslatef(0,0.85,0); glRotatef(90, 1,0,0);
        glScalef(s,s,s);
        if (mostrarEsqueleto) esqueleto->draw( coresArquivo );
    glPopMatrix();
}

void esferaCortadaTransformada() {
    static float raio = 1.0;
    raio += 0.02*glutGUI::dlx;

    glPushMatrix();
        transf.apply();
        // GUI::setColor(0.5,0.7,0.7);
        GUI::setColor(1.0,1.0,1.0);
        // GUI::drawSphereInsideBox222(transf.pos.x,transf.pos.y,transf.pos.z, raio);
        GUI::drawSphereInsideBox222(0.0,0.0,0.0, raio);
    glPopMatrix();
}

void desenha() {
    GUI::displayInit();

    GUI::setLight(0, 3,2,5, true, false);


    GUI::drawOrigin(0.5);

    GUI::setColor(0.5,0.5,0.7);
    GUI::drawFloor(5,5);

    // glShadeModel(GL_SMOOTH); //GL_FLAT



    if (mostrarEsqueleto) esqueletoTransformado();



    GUI::habilitaTextura(true,false,0);
    GUI::selecionaTextura(idText); GUI::setColor(1.0,1.0,1.0);

    // GUI::drawSphere(px,py,0, raio);
    // px -= 0.001;

    if (mostrarCasa) casaTransformada();

    if (mostrarEsferaCortada) esferaCortadaTransformada();

    GUI::desabilitaTextura(true,false);



    //interacao com o mouse
        // cout << glutGUI::drx << endl;
        // px += 0.5*glutGUI::drx;
        // py += 0.5*glutGUI::dry;
        // raio += 0.01*glutGUI::dlx;
        transf.updateByMouse();

    GUI::displayEnd();
}

void teclado(unsigned char tecla, int xMouse, int yMouse) {
    GUI::keyInit(tecla,xMouse,yMouse);

    switch (tecla)
    {
    // case 'a':
    //     px -= 0.1;
    //     break;
    // case 'd':
    //     px += 0.1;
    //     break;
    // case 's':
    //     py -= 0.1;
    //     break;
    // case 'w':
    //     py += 0.1;
    //     break;

    case 't':
        idText ++;
        idText %= 10;
        break;

    case '1':
        mostrarCasa = !mostrarCasa;
        break;
    case '2':
        mostrarEsqueleto = !mostrarEsqueleto;
        break;
    case '3':
        mostrarEsferaCortada = !mostrarEsferaCortada;
        break;

    case 'A':
        coresArquivo = !coresArquivo;
        break;

    case ' ':
        glutGUI::trans_obj = !glutGUI::trans_obj; //variaveis globais (static) passam a ser atualizadas, de acordo com o mapeamento do mouse
        break;
    case 'l':
        glutGUI::trans_luz = !glutGUI::trans_luz; //posicao da luz passa a ser atualizada, de acordo com o mapeamento do mouse ao segurar o botao direito
        break;

    case 'i':
        transf.reset();
        break;

    default:
        break;
    }
}

// int main(int argc, char *argv[])
int main()
{
    cout << "Hello World!" << endl;

    GUI gui = GUI(800,600,desenha,teclado); //= GUI(800,600,desenha,teclado);
}



//-----------------------
//inicializando OpenGL
//while(true) {
//    desenha();
//    interacaoUsuario();
//    //if () {
//    //    break;
//    //}
//}



//-----------------------
//Usando texturas

    //GUI::habilitaTextura(true,false,0);
    //GUI::selecionaTextura(id); // GUI::setColor(1.0,1.0,1.0);

    //objeto a ser desenhado

    //GUI::desabilitaTextura(true,false);



//------------------------------------------------------------
//personagem
    // glPushMatrix();
    //     transformTronco.apply();
    //     tronco();
    //     glPushMatrix();
    //         transformBracoEsq.apply();
    //         bracoesq();
    //     glPopMatrix();
    //     glPushMatrix();
    //         transformBracoDir.apply();
    //         bracodir();
    //     glPopMatrix();
    // glPopMatrix();



//------------------------------------------------------------
//transformacoes sem usar matrizes

// Vetor3D t(0,0,0);
// Vetor3D s(1,1,1);
// Vetor3D r(0,0,0);

// Vetor3D rotacaoZ( Vetor3D v, float theta ) {
//    Vetor3D vt;
//    float thetaRad = theta*PI/180;
//    vt.x = cos(thetaRad)*v.x - sin(thetaRad)*v.y;
//    vt.y = sin(thetaRad)*v.x + cos(thetaRad)*v.y;
//    vt.z = v.z;
//    return vt;
// }

// Vetor3D translacao( Vetor3D v, Vetor3D desl ) {
//    Vetor3D vt;
//    vt.x = v.x + desl.x;
//    vt.y = v.y + desl.y;
//    vt.z = v.z + desl.z;
//    return vt;
// }

// void desenha() {
//     ...
    
//     Vetor3D v1(0,0,0);
//     Vetor3D v2(1,0,0);
//     Vetor3D v3(1,1,0);
//     Vetor3D v4(0,1,0);

//     v1 = rotacaoZ(v1,r.z);
//     v2 = rotacaoZ(v2,r.z);
//     v3 = rotacaoZ(v3,r.z);
//     v4 = rotacaoZ(v4,r.z);

//     v1 = translacao(v1,t);
//     v2 = translacao(v2,t);
//     v3 = translacao(v3,t);
//     v4 = translacao(v4,t);


//     // transform.apply();
//     GUI::setColor(1.0,1.0,1.0);
//     glBegin(GL_POLYGON);
//         glNormal3f( 0.0, 0.0, 1.0 );
//         glTexCoord2f(0,0); glVertex3f( v1.x, v1.y, v1.z );
//         glTexCoord2f(0.5,0); glVertex3f( v2.x, v2.y, v2.z );
//         glTexCoord2f(0.5,1); glVertex3f( v3.x, v3.y, v3.z );
//         glTexCoord2f(0,1); glVertex3f( v4.x, v4.y, v4.z );
//     glEnd();

//     // transform.apply();
//     GUI::setColor(1.0,1.0,1.0);
//     glBegin(GL_POLYGON);
//         glNormal3f( 0.0, 0.0, 1.0 );
//         glTexCoord2f(0,0); glVertex3f( 0.0+t.x, 0.0+t.y, 0.0 );
//         glTexCoord2f(0.5,0); glVertex3f( 1.0+t.x, 0.0+t.y, 0.0 );
//         glTexCoord2f(0.5,1); glVertex3f( 1.0+t.x, 1.0+t.y, 0.0 );
//         glTexCoord2f(0,1); glVertex3f( 0.0+t.x, 1.0+t.y, 0.0 );
//     glEnd();

//     GUI::setColor(1.0,1.0,1.0);
//     glBegin(GL_POLYGON);
//         glNormal3f( 0.0, 0.0, 1.0 );
//         glTexCoord2f(0,0); glVertex3f( 0.0*s.x, 0.0*s.y, 0.0 );
//         glTexCoord2f(0.5,0); glVertex3f( 1.0*s.x, 0.0*s.y, 0.0 );
//         glTexCoord2f(0.5,1); glVertex3f( 1.0*s.x, 1.0*s.y, 0.0 );
//         glTexCoord2f(0,1); glVertex3f( 0.0*s.x, 1.0*s.y, 0.0 );
//     glEnd();

//     float theta = r.z*PI/180.0;
//     GUI::setColor(1.0,1.0,1.0);
//     glBegin(GL_POLYGON);
//         glNormal3f( 0.0, 0.0, 1.0 );
//         glTexCoord2f(0,0); glVertex3f( 0.0*cos(theta) - 0.0*sin(theta), 0.0*sin(theta) + 0.0*cos(theta), 0.0 );
//         glTexCoord2f(0.5,0); glVertex3f( 1.0*cos(theta) - 0.0*sin(theta), 1.0*sin(theta) + 0.0*cos(theta), 0.0 );
//         glTexCoord2f(0.5,1); glVertex3f( 1.0*cos(theta) - 1.0*sin(theta), 1.0*sin(theta) + 1.0*cos(theta), 0.0 );
//         glTexCoord2f(0,1); glVertex3f( 0.0*cos(theta) - 1.0*sin(theta), 0.0*sin(theta) + 1.0*cos(theta), 0.0 );
//     glEnd();

//     // interacao com o mouse
//         // cout << glutGUI::drx << endl;
//         // px += 0.5*glutGUI::drx;
//         // py += 0.5*glutGUI::dry;
//         // raio += 0.01*glutGUI::dlx;

//         t.x += 0.25*glutGUI::drx;
//         t.y += 0.25*glutGUI::dry;
//         s.x += 0.25*glutGUI::dmx;
//         s.y += 0.25*glutGUI::dmy;
//         r.z += 0.25*glutGUI::dlrx;

//     ...
// }



//------------------------------------------------------------
//Efeito global vs efeito local (interpretacoes diferentes)

    // static float ang = 0;
    // static float vAng = 0.2;
    // ang += vAng;
    // if (ang > 45) vAng *= -1; 
    // if (ang <  0) vAng *= -1; 

    // glPushMatrix();
    //     glRotatef(ang,0,0,1);  //2a transf rot eixoZ  /\   efeito    ||  efeito
    //     glRotatef(angx,1,0,0); //1a transf rot eixoX  || ref global  \/ ref local
    //     GUI::drawOrigin(0.5);
    //     GUI::drawHalfSphere(0,0,0, 1.0);
    // glPopMatrix();


//------------------------------------------------------------
//Usando plano de corte

//void cortandoObjetos() {
//    //glRotatef(90,1,0,0);

//    //clipping
//    //GLdouble plane[4] = { 0.0, 1.0, 0.0, 1}; //deixa a parte acima (y) do plano aparecendo (plano posicionado no y=-1, d desloca o plano no sentido oposto à normal) (corta o y<-1)
//    //GLdouble plane[4] = { 0.0, -1.0, 0.0, 1}; //deixa a parte abaixo (y) do plano aparecendo (plano posicionado no y=1, d desloca o plano no sentido oposto à normal) (corta o y>1)
//    GLdouble plane[4] = { -1.0, 0.0, 1.0, 0.0 }; //deixa a parte à frente do plano (z) do plano aparecendo (plano posicionado no z=0, d desloca o plano no sentido oposto à normal) (corta o z<0)
//    glClipPlane(GL_CLIP_PLANE0, plane);
//    glEnable(GL_CLIP_PLANE0);

//    //    ax + by + cz + d = 0
//    //            n = (a,b,c)
//    //            d = deslocamento a partir da origem

//    //glRotatef(90,1,0,0);

//    //objeto a ser cortado
//    //GUI::drawSphere(0,0,0,raioEsfera);
//    casa();

//    //clipping
//    glDisable(GL_CLIP_PLANE0);
//}






//-----------------------
//Pac man

    //controle da boca do pac man
    // static float ang = 0;
    // static float vAng = 0.2;
    // ang += vAng;
    // if (ang > 45) vAng *= -1; 
    // if (ang <  0) vAng *= -1; 

    //modelagem do pac man (mais enxuta, usando primitiva halfSphere já recortada)
    // glPushMatrix();
    //     glRotatef(ang,0,0,1);  //2a transf rot eixoZ  /\   efeito    ||  efeito
    //     glRotatef(-90,1,0,0);  //1a transf rot eixoX  || ref global  \/ ref local
    //     GUI::drawHalfSphere(0,0,0, 1.0);
    // glPopMatrix();

    // glPushMatrix();
    //     glRotatef(-ang,0,0,1);  //2a transf rot eixoZ  /\   efeito    ||  efeito
    //     glRotatef(  90,1,0,0);  //1a transf rot eixoX  || ref global  \/ ref local
    //     GUI::drawHalfSphere(0,0,0, 1.0);
    // glPopMatrix();


    //modelagem do pac man (mostrando como recortar)
    // glPushMatrix();
    //     glRotatef(ang,0,0,1);

    //     //clipping
    //     GLdouble plane[4] = { 0.0, 1.0, 0.0, 0.0 }; //deixa a parte à frente do plano (z) do plano aparecendo (plano posicionado no z=0, d desloca o plano no sentido oposto à normal) (corta o z<0)
    //     glClipPlane(GL_CLIP_PLANE0, plane);
    //     glEnable(GL_CLIP_PLANE0);

    //     //    ax + by + cz + d = 0
    //     //            n = (a,b,c)
    //     //            d = deslocamento a partir da origem


    //     //objeto a ser cortado
    //     GUI::drawSphere(0,0,0,1.0);
    //     // casa();

    //     //clipping
    //     glDisable(GL_CLIP_PLANE0);
    // glPopMatrix();


    // glPushMatrix();
    //     glRotatef(-ang,0,0,1);

    //     //clipping
    //     GLdouble plane2[4] = { 0.0, -1.0, 0.0, 0.0 }; //deixa a parte à frente do plano (z) do plano aparecendo (plano posicionado no z=0, d desloca o plano no sentido oposto à normal) (corta o z<0)
    //     glClipPlane(GL_CLIP_PLANE0, plane2);
    //     glEnable(GL_CLIP_PLANE0);

    //     //    ax + by + cz + d = 0
    //     //            n = (a,b,c)
    //     //            d = deslocamento a partir da origem


    //     //objeto a ser cortado
    //     GUI::drawSphere(0,0,0,1.0);
    //     // casa();

    //     //clipping
    //     glDisable(GL_CLIP_PLANE0);
    // glPopMatrix();


//-----------------------
//Usando vários planos de corte

// if (drawCubo2x2x2) {
//     //clipping
//     GLdouble planes[6][4] = { { 1.0, 0.0, 0.0, 1},
//                               {-1.0, 0.0, 0.0, 1},
//           //comentario errado { 0.0, 1.0, 0.0, 1}, //deixa o y positivo aparecendo (a partir do y=1) (corta o y<1)
//            /*(corta o y<-1)*/ { 0.0, 1.0, 0.0, 1}, //deixa a parte acima (y) do plano aparecendo (plano posicionado no y=-1, d desloca o plano no sentido oposto à normal)
//                               { 0.0,-1.0, 0.0, 1},
//                               { 0.0, 0.0, 1.0, 1},
//                               { 0.0, 0.0,-1.0, 1} };
//     for (int pl=0;pl<6;pl++) {
//         glClipPlane(GL_CLIP_PLANE0+pl, planes[pl]);
//         glEnable(GL_CLIP_PLANE0+pl);
//     }

//     //objeto a ser desenhado

//     //clipping
//     for (int pl=0;pl<6;pl++) {
//         glDisable(GL_CLIP_PLANE0+pl);
//     }
// }

//     //GUI::drawSphere(posEsfera.x,posEsfera.y,posEsfera.z, raioEsfera);
//     GUI::drawHalfSphere(posEsfera.x,posEsfera.y,posEsfera.z, raioEsfera);
//     GUI::drawSphereInsideBox222(posEsfera.x,posEsfera.y,posEsfera.z, raioEsfera);



//-----------------------
