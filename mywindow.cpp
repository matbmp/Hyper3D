#include "mywindow.h"
#include <QtAlgorithms>

// Dolaczamy plik naglowkowy komponentu layoutu pionowego
// Komponent ten sluzy do rozmieszczania elementow GUI w oknie
#include <QVBoxLayout>
#include <QLabel>
#include <QtMath>


MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Hyper3D");
    setMouseTracking(true);

    windowW = windowH = 700;
    canvasW = canvasH = 700;
    canvasX = canvasY = 0;
    //Zmieniamy rozmiar okna
    resize(windowW,windowH);

    group = new QGroupBox(this);
    group->setGeometry((windowW-700)/2, (windowH-200)/2, 700, 200);
    QVBoxLayout *boxLayout = new QVBoxLayout();

    QLabel *text = new QLabel("Sterowanie:\n WASD+myszka - przekształcenie kamery \n P - zmiana projekcji (Poincare / Beltrami-Klein) \n O - zmiana cieni (Brak/Zwykłe/Kolory/Latarka) \n I - przekształcenie światła (Środek, pozycja kamery, ostatnia pozycja kamery)", this);
    boxLayout->addWidget(text);
    button = new QPushButton("Ok", this);
    boxLayout->addWidget(button);
    group->setLayout(boxLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(okPressed()));

    canvas1 = new QImage(canvasW,canvasH,QImage::Format_RGB32);
}


MyWindow::~MyWindow()
{
    delete canvas1;
}
void MyWindow::okPressed(){

    QCursor cursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(cursor);
    QApplication::changeOverrideCursor(cursor);
    group->setVisible(false);
    this->game->Pause = false;
}
void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if(!this->game->Pause){
        p.drawImage(canvasX, canvasY, (*(game->graphics->defaultRenderTarget.backbufferImage)).scaled(canvasW, canvasH));
    }

}

void MyWindow::keyPressEvent(QKeyEvent *event){
    game->input->keyPressEvent(event);
}
void MyWindow::keyReleaseEvent(QKeyEvent *event){
    game->input->keyReleaseEvent(event);
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event){
}

void MyWindow::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos = Vector2(event->pos().x()-canvasX, event->pos().y()-canvasY);
    pos = pos - Vector2(canvasW/2, canvasH/2);
    pos = pos * Vector2(1.0F/canvasW, 1.0F/canvasH);
    game->input->setMousePos(pos);
}

void MyWindow::closeEvent (QCloseEvent *event){
    game->Running = false;
}




