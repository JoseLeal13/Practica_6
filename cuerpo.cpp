#include "Cuerpo.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QPixmap>
#include <cmath>

vector<Cuerpo*> Cuerpo::cuerpos;

void Cuerpo::agregarCuerpo(Cuerpo* cuerpo) {
    cuerpos.push_back(cuerpo);
}

vector<Cuerpo*>& Cuerpo::obtenerCuerpos() {
    return cuerpos;
}

Cuerpo::Cuerpo(QObject *parent)
    : QObject(parent), posX(0), posY(0), m(0), r(0), speedX(0), speedY(0), ax(0), ay(0)
{
}

Cuerpo::Cuerpo(float posX, float posY, float m, float r, float speedX, float speedY)
    : QObject(nullptr), posX(posX), posY(posY), m(m), r(r), speedX(speedX), speedY(speedY), ax(0), ay(0) {
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

void Cuerpo::setPosicion(float x, float y) {
    posX = x;
    posY = y;
}

void Cuerpo::setVelocidad(float vx, float vy) {
    speedX = vx;
    speedY = vy;
}

void Cuerpo::limpiarCuerpos() {
    for (Cuerpo* cuerpo : cuerpos) {
        delete cuerpo;
    }
    cuerpos.clear();
}

void Cuerpo::calcularAceleracion(const Cuerpo& otro, float& ax, float& ay) const {
    // Distancia en los ejes
    float dx = otro.getPosX() - posX;
    float dy = otro.getPosY() - posY;

    // Calcular distancia al cuadrado
    float distancia2 = dx * dx + dy * dy;

    // Evitar cálculos con distancias muy pequeñas para prevenir errores numéricos
    if (distancia2 < 1e-10) return;

    // Calcular distancia y fuerza gravitacional
    float distancia = sqrt(distancia2);
    float fuerza = G * m * otro.getM() / distancia2;

    // Ángulo de dirección
    float theta = atan2(dy, dx);

    // Descomponer la fuerza en componentes y actualizar la aceleración
    ax += fuerza * cos(theta) / m;
    ay += fuerza * sin(theta) / m;
}


void Cuerpo::aplicarFuerza(float fx, float fy) {
    ax = fx / m;
    ay = fy / m;
}

void Cuerpo::actualizar(float dt) {
    // Actualizar velocidad con la aceleración
    speedX += ax * dt;
    speedY += ay * dt;

    // Actualizar posición con la velocidad
    posX += speedX * dt;
    posY += speedY * dt;

    // Emitir señal para la interfaz gráfica (si aplica)
    emit posicionActualizada(posX, posY);
}


void Cuerpo::paint(QPainter *painter) {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);
    painter->drawEllipse(QRect(posX, posY, r, r));
}

void Cuerpo::actualizarFuerzas() {
    // Para cada cuerpo en el sistema
    for (size_t i = 0; i < cuerpos.size(); i++) {
        float ax_total = 0;
        float ay_total = 0;

        // Calcular la aceleración causada por todos los demás cuerpos
        for (size_t j = 0; j < cuerpos.size(); j++) {
            if (i != j) {
                cuerpos[i]->calcularAceleracion(*cuerpos[j], ax_total, ay_total);
            }
        }

        // Aplicar la fuerza total calculada
        cuerpos[i]->aplicarFuerza(ax_total * cuerpos[i]->getM(), ay_total * cuerpos[i]->getM());
    }

    // Actualizar posiciones y velocidades
    for (size_t i = 0; i < cuerpos.size(); i++) {
        cuerpos[i]->actualizar(0.01f); // Usar un pequeño delta t
    }
}


