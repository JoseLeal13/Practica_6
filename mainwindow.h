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

    // Configuración de UI
    void setupUI();

    // Funciones relacionadas con los botones
    void onComenzarClicked();
    void onPausarClicked();
    void onContinuarClicked();
    void onReiniciarClicked();
    void onGuardarClicked();
    void onCargarClicked();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
