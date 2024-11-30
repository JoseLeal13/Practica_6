#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QDoubleValidator>
#include <QTimer>
#include "cuerpo.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer* timer;
    Cuerpo* cuerpo;

    // Campos de entrada
    QLineEdit *posXEdit;
    QLineEdit *posYEdit;
    QLineEdit *mEdit;
    QLineEdit *rEdit;
    QLineEdit *speedXEdit;
    QLineEdit *speedYEdit;

    // Botones
    QPushButton *comenzarButton;
    QPushButton *pausarButton;
    QPushButton *continuarButton;
    QPushButton *reiniciarButton;
    QPushButton *guardarButton;
    QPushButton *cargarButton;
    QPushButton *agregarButton;

    // Configuración de UI
    void setupUI();

    void inicializarGraficos();
    void actualizarInterfazGrafica();
    void actualizarSimulacion();

    // Funciones relacionadas con los botones
    void onComenzarClicked();
    void onPausarClicked();
    void onContinuarClicked();
    void onReiniciarClicked();
    void onGuardarClicked();
    void onCargarClicked();
    void onAgregarClicked();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
