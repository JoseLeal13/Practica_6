#include "Cuerpo.h"

Cuerpo::Cuerpo(QObject *parent)
    : QObject(parent), nombre(0), posX(0), posY(0), m(0), r(0), speedX(0), speedY(0)
{
}

Cuerpo::Cuerpo(int nombre, float posX, float posY, float m, float r, float speedX, float speedY)
    : QObject(nullptr), nombre(nombre), posX(posX), posY(posY), m(m), r(r), speedX(speedX), speedY(speedY)
{
}

int Cuerpo::getNombre() const {
    return nombre;
}

float Cuerpo::getPosX() const {
    return posX;
}

float Cuerpo::getPosY() const {
    return posY;
}

float Cuerpo::getM() const {
    return m;
}

float Cuerpo::getR() const {
    return r;
}

float Cuerpo::getSpeedX() const {
    return speedX;
}

float Cuerpo::getSpeedY() const {
    return speedY;
}
