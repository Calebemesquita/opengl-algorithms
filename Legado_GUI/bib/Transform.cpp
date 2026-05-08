#include "Transform.h"

void Transform::apply() {
    glTranslatef(this->pos.x, this->pos.y, this->pos.z);
    glRotatef(this->rot.x, 1,0,0);
    glRotatef(this->rot.y, 0,1,0);
    glRotatef(this->rot.z, 0,0,1);
    if (this->showLocalOrigin) GUI::drawOrigin(0.5);
    glScalef(this->esc.x, this->esc.y, this->esc.z);
}

void Transform::apply2D() {
    glTranslatef(this->pos.x, this->pos.y, 0.0);
    glRotatef(this->rot.z, 0,0,1);
    if (this->showLocalOrigin) GUI::drawOrigin(0.5);
    glScalef(this->esc.x, this->esc.y, 1.0);
}

void Transform::updateByMouse() {
    this->pos.x += 0.05*glutGUI::drx;
    this->pos.y += 0.05*glutGUI::dry;
    this->pos.z += 0.05*glutGUI::dlry;
    this->rot.x += 0.25*glutGUI::dly;
    this->rot.y += 0.25*glutGUI::dlx;
    this->rot.z += 0.25*glutGUI::dlrx;
    this->esc.x += 0.25*glutGUI::dmx;
    this->esc.y += 0.25*glutGUI::dmy;
    this->esc.z += 0.25*glutGUI::dlmy;
}

void Transform::updateByMouse2D() {
    this->pos.x += 0.05*glutGUI::drx;
    this->pos.y += 0.05*glutGUI::dry;
    this->rot.z += 0.25*glutGUI::dlrx;
    this->esc.x += 0.25*glutGUI::dmx;
    this->esc.y += 0.25*glutGUI::dmy;
}

void Transform::reset()
{
    this->pos = Vetor3D(0.0, 0.0, 0.0);
    this->rot = Vetor3D(0.0, 0.0, 0.0);
    this->esc = Vetor3D(1.0, 1.0, 1.0);
    this->showLocalOrigin = true;
}
