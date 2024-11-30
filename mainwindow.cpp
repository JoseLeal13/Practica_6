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
        cuerpos[i]->actualizar(deltaTime);
    }
}

void MainWindow::actualizarInterfazGrafica() {
    auto& cuerpos = Cuerpo::obtenerCuerpos();
    while (scene->items().size() < cuerpos.size()) {
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-10, -10, 20, 20);
        item->setBrush(Qt::blue);
        scene->addItem(item);
    }

    auto items = scene->items();
    for (size_t i = 0; i < cuerpos.size(); ++i) {
        items[i]->setPos(cuerpos[i]->getPosX(), cuerpos[i]->getPosY());
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
    auto& cuerpos = Cuerpo::obtenerCuerpos();
    cuerpos.clear(); // Limpia el vector estático
    scene->clear();  // Elimina todos los elementos gráficos
    timer->stop();
    qDebug() << "Simulación pausada.";
    qDebug() << "Simulación reiniciada.";
}

void MainWindow::onGuardarClicked() {
    qDebug() << "Botón Guardar presionado";

    // Cuadro de diálogo para elegir el archivo de guardado
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) {
        qDebug() << "No se seleccionó archivo para guardar";
        return;
    }

    // Abrir archivo para escritura
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para guardar";
        return;
    }

    QTextStream out(&file);

    // Guardar cada cuerpo en el archivo
    auto& cuerpos = Cuerpo::obtenerCuerpos();
    for (Cuerpo* cuerpo : cuerpos) {
        out << cuerpo->getPosX() << " "
            << cuerpo->getPosY() << " "
            << cuerpo->getSpeedX() << " "
            << cuerpo->getSpeedY() << " "
            << cuerpo->getM() << " "
            << cuerpo->getR() << "\n";
    }

    file.close();
    qDebug() << "Cuerpos guardados en el archivo";
}


void MainWindow::onCargarClicked() {
    qDebug() << "Botón Cargar presionado";

    // Cuadro de diálogo para elegir el archivo de carga
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) {
        qDebug() << "No se seleccionó archivo para cargar";
        return;
    }

    // Abrir archivo para lectura
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para cargar";
        return;
    }

    QTextStream in(&file);
    QString line;

    // Limpiar los cuerpos actuales antes de cargar los nuevos
    auto& cuerpos = Cuerpo::obtenerCuerpos();
    cuerpos.clear();
    scene->clear();

    // Leer cada línea y crear un cuerpo
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() != 6) {
            qDebug() << "Línea inválida: " << line;
            continue;  // Ignorar líneas mal formadas
        }
        if (parts.size() == 6) {
            bool okX, okY, okVx, okVy, okM, okR;
            float posX = parts[0].toFloat(&okX);
            float posY = parts[1].toFloat(&okY);
            float velX = parts[2].toFloat(&okVx);
            float velY = parts[3].toFloat(&okVy);
            float masa = parts[4].toFloat(&okM);
            float radio = parts[5].toFloat(&okR);

            if (okX && okY && okVx && okVy && okM && okR) {
                // Crear cuerpo y agregarlo al vector estático
                Cuerpo* nuevoCuerpo = new Cuerpo(posX, posY, velX, velY, masa, radio);
                cuerpos.push_back(nuevoCuerpo);

                // Crear representación gráfica
                QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-radio, -radio, radio * 2, radio * 2);
                item->setBrush(Qt::blue);
                item->setPos(posX, posY);
                scene->addItem(item);
            }
        }
    }

    file.close();
    qDebug() << "Cuerpos cargados desde el archivo";
}


void MainWindow::onAgregarClicked() {
    // Obtén los valores ingresados por el usuario
    bool okX, okY, okM, okR, okVx, okVy;
    float posX = posXEdit->text().toFloat(&okX);
    float posY = posYEdit->text().toFloat(&okY);
    float masa = mEdit->text().toFloat(&okM);
    float radio = rEdit->text().toFloat(&okR);
    float velX = speedXEdit->text().toFloat(&okVx);
    float velY = speedYEdit->text().toFloat(&okVy);

    // Valida las entradas
    if (!(okX && okY && okM && okR && okVx && okVy)) {
        QMessageBox::warning(this, "Error de Entrada", "Por favor, introduce valores válidos.");
        return;
    }

    // Crea un nuevo cuerpo con los valores ingresados
    Cuerpo* nuevoCuerpo = new Cuerpo(posX, posY, velX, velY, masa, radio);

    // Agrega el nuevo cuerpo al vector de cuerpos (en su clase estática)
    Cuerpo::obtenerCuerpos().push_back(nuevoCuerpo);

    // Crea la representación gráfica del cuerpo y agrégala a la escena
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-radio, -radio, radio * 2, radio * 2);
    item->setBrush(Qt::blue);           // Define el color del cuerpo
    item->setPos(posX, posY);           // Posición inicial
    scene->addItem(item);               // Añade el gráfico a la escena

    qDebug() << "Nuevo cuerpo agregado con posición (" << posX << "," << posY << ")"
             << "y velocidad (" << velX << "," << velY << ")";
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
