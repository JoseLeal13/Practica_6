#ifndef CUERPO_H
#define CUERPO_H

#include <QObject>

class Cuerpo : public QObject
{
    Q_OBJECT
public:
    explicit Cuerpo(QObject *parent = nullptr);

    Cuerpo(int nombre,
           float posX,
           float posY,
           float m,
           float r,
           float speedX,
           float speedY);

    // Getters
    int getNombre() const;
    float getPosX() const;
    float getPosY() const;
    float getM() const;
    float getR() const;
    float getSpeedX() const;
    float getSpeedY() const;

private:

    int nombre;
    float posX;
    float posY;
    float m;
    float r;
    float speedX;
    float speedY;

signals:
};

#endif // CUERPO_H
