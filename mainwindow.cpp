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
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Crear la escena gráfica
    scene = new QGraphicsScene(this);  // Inicializa la escena

    // Crear la vista para la escena (es opcional dependiendo de si deseas mostrarla)
    view = new QGraphicsView(scene, this);

    // Layout principal (vertical)
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // **Panel superior** con botones
    QVBoxLayout *buttonLayout = new QVBoxLayout;

    comenzarButton = new QPushButton("Comenzar");
    pausarButton = new QPushButton("Pausar");
    continuarButton = new QPushButton("Continuar");
    reiniciarButton = new QPushButton("Reiniciar");
    guardarButton = new QPushButton("Guardar en Archivo");
    cargarButton = new QPushButton("Cargar en Archivo");

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

    agregarButton = new QPushButton("Agregar");
    agregarButton->setFixedSize(buttonSize);

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
    inputLayout->addWidget(agregarButton);

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
    connect(agregarButton, &QPushButton::clicked, this, &MainWindow::onAgregarClicked);
    connect(cuerpo, &Cuerpo::posicionActualizada, this, &MainWindow::actualizarInterfazGrafica);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);
    timer->start(16); // 16 ms = ~60 fps

    // Eliminar la segunda inicialización de scene
    // scene = new QGraphicsScene(this);
    // QGraphicsView* view = new QGraphicsView(scene, centralWidget);
    mainLayout->addWidget(view);
}


// Dentro de MainWindow

void MainWindow::inicializarGraficos() {
    const auto& cuerpos = Cuerpo::obtenerCuerpos();

    // Iterar sobre los cuerpos y crear un gráfico para cada uno
    for (Cuerpo* cuerpo : cuerpos) {
        // Crear un QGraphicsEllipseItem para cada cuerpo (un círculo)
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(
            -cuerpo->getR(), -cuerpo->getR(), cuerpo->getR() * 2, cuerpo->getR() * 2);

        // Establecer el color o estilo del gráfico (puedes personalizarlo)
        item->setBrush(Qt::blue);
        item->setPos(cuerpo->getPosX(), cuerpo->getPosY());

        // Agregarlo a la escena
        this->scene->addItem(item);  // Acceder a la escena con this->scene
    }
}


void MainWindow::actualizarSimulacion() {
    // Obtener los cuerpos directamente desde el vector estático
    auto& cuerpos = Cuerpo::obtenerCuerpos();

    for (size_t i = 0; i < cuerpos.size(); ++i) {
        float ax = 0, ay = 0;
        for (size_t j = 0; j < cuerpos.size(); ++j) {
            if (i != j) {
                float axTemp, ayTemp;
                cuerpos[i]->calcularAceleracion(*cuerpos[j], axTemp, ayTemp); // Acceder al objeto usando puntero
                ax += axTemp;
                ay += ayTemp;
            }
        }
        // Actualizar posición y velocidad
        const float deltaTime = 0.016f;
        cuerpos[i]->actualizar(deltaTime, ax, ay);
    }
}

void MainWindow::actualizarInterfazGrafica() {

    const auto& cuerpos = Cuerpo::obtenerCuerpos();
    auto items = this->scene->items(); // Esto accede correctamente a scene

    if (items.size() != cuerpos.size()) {
        qDebug() << "Error: Inconsistencia entre cuerpos y elementos gráficos.";
        return;
    }
    // Actualizar la posición de cada gráfico
    for (size_t i = 0; i < cuerpos.size(); ++i) {
        if (i < items.size()) {
            items[i]->setPos(cuerpos[i]->getPosX(), cuerpos[i]->getPosY());
        }
    }
}

void MainWindow::onComenzarClicked() {
    timer->start();
    qDebug() << "Simulación reanudada.";
}
void MainWindow::onPausarClicked() {
    timer->stop();
    qDebug() << "Simulación pausada.";
}

void MainWindow::onContinuarClicked() {
    timer->start();
    qDebug() << "Simulación reanudada.";
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

void MainWindow::onAgregarClicked() {
    // Validar las entradas
    bool okX, okY, okM, okR, okVx, okVy;
    float posX = posXEdit->text().toFloat(&okX);
    float posY = posYEdit->text().toFloat(&okY);
    float masa = mEdit->text().toFloat(&okM);
    float radio = rEdit->text().toFloat(&okR);
    float velX = speedXEdit->text().toFloat(&okVx);
    float velY = speedYEdit->text().toFloat(&okVy);

    if (okX && okY && okM && okR && okVx && okVy && masa > 0 && radio > 0) {
        // Crear un nuevo cuerpo y agregarlo al vector
        cuerpo = new Cuerpo(posX, posY, masa, radio, velX, velY);
        qDebug() << "Cuerpo agregado:" << posX << posY << masa << radio << velX << velY;

        // Actualizar la interfaz gráfica
        actualizarInterfazGrafica();
    } else {
        qDebug() << "Entradas inválidas.";
    }
}


/*
bool validarEntradas() {
    bool ok = true;
    double posX = posXEdit->text().toDouble(&ok);
    if (!ok) return false;
    double posY = posYEdit->text().toDouble(&ok);
    if (!ok) return false;
    double m = mEdit->text().toDouble(&ok);
    if (!ok || m <= 0) return false; // La masa debe ser positiva
    double r = rEdit->text().toDouble(&ok);
    if (!ok || r <= 0) return false; // El radio debe ser positivo
    return true;
}
*/
