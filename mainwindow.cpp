#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Crear el widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout principal (vertical)
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // **Panel superior** con botones
    QVBoxLayout *buttonLayout = new QVBoxLayout;

    comenzarButton = new QPushButton("Comenzar");
    pausarButton = new QPushButton("Pausar");
    continuarButton = new QPushButton("Continuar");
    reiniciarButton = new QPushButton("Reiniciar");
    guardarButton = new QPushButton("Guardar en Archivo");
    cargarButton = new QPushButton("Cargar desde Archivo");

    // Ajustar tamaño de los botones
    QSize buttonSize(150, 40);  // Ancho x Alto
    comenzarButton->setFixedSize(buttonSize);
    pausarButton->setFixedSize(buttonSize);
    continuarButton->setFixedSize(buttonSize);
    reiniciarButton->setFixedSize(buttonSize);
    guardarButton->setFixedSize(buttonSize);
    cargarButton->setFixedSize(buttonSize);

    // Añadir botones al layout vertical
    buttonLayout->addWidget(comenzarButton);
    buttonLayout->addWidget(pausarButton);
    buttonLayout->addWidget(continuarButton);
    buttonLayout->addWidget(reiniciarButton);
    buttonLayout->addWidget(guardarButton);
    buttonLayout->addWidget(cargarButton);

    // Espaciador para centrar los botones en la parte superior
    buttonLayout->addStretch();

    // **Panel inferior** con campos de entrada
    QHBoxLayout *inputLayout = new QHBoxLayout;

    posXEdit = new QLineEdit("0");
    posYEdit = new QLineEdit("0");
    mEdit = new QLineEdit("0");
    rEdit = new QLineEdit("0");
    speedXEdit = new QLineEdit("0");
    speedYEdit = new QLineEdit("0");

    // Validación de entrada (solo números)
    QDoubleValidator *validator = new QDoubleValidator(this);
    posXEdit->setValidator(validator);
    posYEdit->setValidator(validator);
    mEdit->setValidator(validator);
    rEdit->setValidator(validator);
    speedXEdit->setValidator(validator);
    speedYEdit->setValidator(validator);

    // Ajustar tamaño de los campos de entrada
    QSize fieldSize(80, 30);  // Ancho x Alto
    posXEdit->setFixedSize(fieldSize);
    posYEdit->setFixedSize(fieldSize);
    mEdit->setFixedSize(fieldSize);
    rEdit->setFixedSize(fieldSize);
    speedXEdit->setFixedSize(fieldSize);
    speedYEdit->setFixedSize(fieldSize);

    inputLayout->addWidget(new QLabel("posX:"));
    inputLayout->addWidget(posXEdit);
    inputLayout->addWidget(new QLabel("posY:"));
    inputLayout->addWidget(posYEdit);
    inputLayout->addWidget(new QLabel("m:"));
    inputLayout->addWidget(mEdit);
    inputLayout->addWidget(new QLabel("r:"));
    inputLayout->addWidget(rEdit);
    inputLayout->addWidget(new QLabel("speedX:"));
    inputLayout->addWidget(speedXEdit);
    inputLayout->addWidget(new QLabel("speedY:"));
    inputLayout->addWidget(speedYEdit);

    // Espaciador para centrar los cuadros de entrada en la parte inferior
    inputLayout->addStretch();

    // Añadir los layouts al principal
    mainLayout->addLayout(buttonLayout);  // Panel superior (botones)
    mainLayout->addStretch();            // Espaciador entre botones y campos
    mainLayout->addLayout(inputLayout);  // Panel inferior (campos de entrada)

    // Ajustar tamaño de la ventana para pantalla completa
    showMaximized();

    // Conexión de botones
    connect(comenzarButton, &QPushButton::clicked, this, &MainWindow::onComenzarClicked);
    connect(pausarButton, &QPushButton::clicked, this, &MainWindow::onPausarClicked);
    connect(continuarButton, &QPushButton::clicked, this, &MainWindow::onContinuarClicked);
    connect(reiniciarButton, &QPushButton::clicked, this, &MainWindow::onReiniciarClicked);
    connect(guardarButton, &QPushButton::clicked, this, &MainWindow::onGuardarClicked);
    connect(cargarButton, &QPushButton::clicked, this, &MainWindow::onCargarClicked);
}


void MainWindow::onComenzarClicked() {
    qDebug() << "Botón Comenzar presionado";
    // Código para iniciar algún proceso
}

void MainWindow::onPausarClicked() {
    qDebug() << "Botón Pausar presionado";
    // Código para pausar el proceso
}

void MainWindow::onContinuarClicked() {
    qDebug() << "Botón Continuar presionado";
    // Código para reanudar el proceso
}

void MainWindow::onReiniciarClicked() {
    qDebug() << "Botón Reiniciar presionado";
    // Código para reiniciar el proceso
}

void MainWindow::onGuardarClicked() {
    qDebug() << "Botón Guardar presionado";
    // Código para guardar en un archivo
}

void MainWindow::onCargarClicked() {
    qDebug() << "Botón Cargar presionado";
    // Código para cargar desde un archivo
}
