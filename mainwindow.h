#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void getFrame();

private slots:
    void on_startButton_clicked();

    void moveOne();
    void setUpGame();
    void updateGame();
    void endGame();
    void collisionDetection();
    void newGoal();

private:
    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent *p);
    void keyPressEvent(QKeyEvent *k);

    QTimer *timer;

    bool running = false;
    bool gameOver = true;
    int snakeLength = 5;
    int score = 0;

    const static int left = 0;
    const static int right = 1;
    const static int up = 2;
    const static int down = 3;
    int direction = right;

    const static int pieceSize = 20;

    QVector<int> xPosv;
    QVector<int> yPosv;

    int goal[2];

    int xFrame;
    int yFrame;
    int wFrame;
    int hFrame;


};

#endif // MAINWINDOW_H
