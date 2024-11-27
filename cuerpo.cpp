#include "Cuerpo.h"

// Inicialización del vector estático
vector<Cuerpo*> Cuerpo::cuerpos;

void Cuerpo::agregarCuerpo(Cuerpo* cuerpo) {
    cuerpos.push_back(cuerpo);
}

const std::vector<Cuerpo*>& Cuerpo::obtenerCuerpos() {
    return cuerpos;
}


Cuerpo::Cuerpo(QObject *parent)
    : QObject(parent), posX(0), posY(0), m(0), r(0), speedX(0), speedY(0)
{
}

Cuerpo::Cuerpo(float posX, float posY, float m, float r, float speedX, float speedY)
    : QObject(nullptr), posX(posX), posY(posY), m(m), r(r), speedX(speedX), speedY(speedY) {
    // Agregar automáticamente el cuerpo al vector al crearlo
    cuerpos.push_back(this);
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

void Cuerpo::limpiarCuerpos() {
    for (Cuerpo* cuerpo : cuerpos) {
        delete cuerpo;
    }
    cuerpos.clear();
}

void Cuerpo::calcularAceleracion(const Cuerpo& otro, float& ax, float& ay) const {
    float dx = otro.getPosX() - posX;
    float dy = otro.getPosY() - posY;
    float distancia2 = dx * dx + dy * dy; // Usar distancia al cuadrado para evitar sqrt

    if (distancia2 < 1e-10) return; // Evitar división por cero o distancias muy pequeñas

    float fuerza = G * m * otro.getM() / distancia2; // Fuerza gravitacional
    float distancia = sqrt(distancia2);             // Calcular sqrt solo cuando sea necesario

    ax += fuerza * dx / (distancia * m);
    ay += fuerza * dy / (distancia * m);
}

void Cuerpo::actualizar(float dt, float ax, float ay) {
    // Actualizar velocidades
    speedX += ax * dt;
    speedY += ay * dt;

    // Actualizar posiciones
    posX += speedX * dt;
    posY += speedY * dt;

    // Emitir señal opcional para Qt
    emit posicionActualizada(posX, posY);
}
