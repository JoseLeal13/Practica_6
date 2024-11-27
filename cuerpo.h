#ifndef CUERPO_H
#define CUERPO_H
#include <cmath> // Para sqrt, pow, atan2
#include <vector> // Para usar std::vector
#include <QObject>
using namespace std;

#define G 6.6738e-11 // Constante gravitacional

class Cuerpo : public QObject {
    Q_OBJECT

public:
    explicit Cuerpo(QObject *parent = nullptr);

    Cuerpo(float posX,
           float posY,
           float m,
           float r,
           float speedX,
           float speedY);

    // Getters
    float getPosX() const;
    float getPosY() const;
    float getM() const;
    float getR() const;
    float getSpeedX() const;
    float getSpeedY() const;

    void calcularAceleracion(const Cuerpo& otro, float& ax, float& ay) const;
    void actualizar(float dt, float ax, float ay);

    // Gestión estática de cuerpos
    static vector<Cuerpo*> cuerpos;
    static void agregarCuerpo(Cuerpo* cuerpo);
    static const vector<Cuerpo*>& obtenerCuerpos();
    static void limpiarCuerpos(); // Limpia memoria de los cuerpos almacenados



private:
    float posX;
    float posY;
    float m;
    float r;
    float speedX;
    float speedY;

signals:
    void posicionActualizada(float nuevaPosX, float nuevaPosY);
};

#endif // CUERPO_H
