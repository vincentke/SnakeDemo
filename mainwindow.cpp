#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->centralWidget->setFocusPolicy(Qt::StrongFocus);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGame()));
    setUpGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if(running){
        timer->stop();
        running=false;
        ui->startButton->setText("Start");
    }
    else if(!running){
        if(!gameOver) {
        }
        else{
            setUpGame();
        }
        running = true;
        ui->label->hide();
        ui->startButton->setText("Stop");
        timer->start(100);
    }
}


void MainWindow::getFrame()
{
    xFrame = ui->widget->x();
    yFrame = ui->widget->y();
    wFrame = (ui->widget->width() - pieceSize);
    hFrame = (ui->widget->height() - pieceSize);
}

void MainWindow::setUpGame(){
    score = 0;
    gameOver = false;
    ui->scoreLabel->setText("Score: " + QString::number(snakeLength));

    xPosv.clear();
    yPosv.clear();
    getFrame();
    newGoal();
    direction = right;
    snakeLength = 5;

    //Start position in top left of frame
    for(int i = 0; i < snakeLength; i++){
        xPosv.append(pieceSize * 5 - i * pieceSize);
        yPosv.append(20);
    }
}

void MainWindow::updateGame(){
    moveOne();
    collisionDetection();
    repaint();
}

void MainWindow::endGame(){
    running = false;
    gameOver = true;
    timer->stop();
    //show score
    ui->label->setText("Final score: " + QString::number(snakeLength));
    ui->label->show();
    ui->startButton->setText("Start");
}

void MainWindow::moveOne(){
    //Move body of snake
    for(int i = xPosv.length()-1; i > 0; i--){
        xPosv[i] = xPosv[i-1];
        yPosv[i] = yPosv[i-1];
    }
    //Move head of snake
    if(direction==right){
        xPosv[0] += pieceSize;
    }
    else if(direction==left){
        xPosv[0] -= pieceSize;
    }
    else if(direction==up){
        yPosv[0] -= pieceSize;
    }
    else if(direction==down){
        yPosv[0] += pieceSize;
    }
}

void MainWindow::collisionDetection(){
    //Check if head reaches goal
    if(xPosv[0] == goal[0] && yPosv[0] == goal[1]){
        snakeLength+=3;
        for(int i = xPosv.length(); i < snakeLength; i++){
            xPosv.append(xPosv[i-1]);
            yPosv.append(yPosv[i-1]);
        }
        ui->scoreLabel->setText("Score: " + QString::number(snakeLength));
        newGoal();
    }
    //Check for collision with snake or wall
    for(int i = 2; i < xPosv.length(); i++){
        if(xPosv[0]==xPosv[i] && yPosv[0] == yPosv[i]){
            endGame();
        }
        //If new goal is generated in occupied space put it somewhere else
        if(xPosv[i] == goal[0] && yPosv[i] == goal[1]){
            newGoal();
        }
    }
    if(xPosv[0] < xFrame || xPosv[0] >= xFrame + wFrame || yPosv[0] < yFrame || yPosv[0] >= yFrame + hFrame){
        endGame();
    }
}

void MainWindow::newGoal(){
    int rand = (xFrame + qrand() % wFrame);
    rand = (yFrame + qrand() % hFrame);
    goal[0] = rand - (rand % 20);;
    goal[1] = rand - (rand % 20);;
}


void MainWindow::paintEvent(QPaintEvent *p){
    Q_UNUSED(p);
    QPainter painter(this);
    QImage piece(":/new/seg.png");
    QImage goalPiece(":/new/goal.png");


    painter.fillRect(xFrame,yFrame,wFrame,hFrame,Qt::gray);
    painter.drawImage(goal[0],goal[1], goalPiece);

    for(int i = 0; i < xPosv.length(); i++){

        painter.drawImage(xPosv[i], yPosv[i], piece);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *k){
    int key = k->key();
    if((key==Qt::Key_Up || key==Qt::Key_I) && direction != down){
        direction = up;
    }
    else if((key==Qt::Key_Down || key==Qt::Key_K) && direction != up){
        direction = down;
    }
    else if((key==Qt::Key_Left || key==Qt::Key_J) && direction != right){
        direction = left;
    }
    else if((key==Qt::Key_Right || key==Qt::Key_L) && direction != left){
        direction = right;
    }
    else if(key==Qt::Key_Space){
        on_startButton_clicked();
    }
    QWidget::keyPressEvent(k);
}
