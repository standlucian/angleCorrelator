#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include "QVector"
#include "QDataStream"
#include "QFile"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "math.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <stdlib.h>
#include <time.h>
#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void readSettings();

    QString calibrationAddress;
    QString effAddress;
    QString corAddress;
    QStringList fileAddress;
    QString outAddress;

    QWidget* window;

    QLabel* calibName;
    QPushButton* calibNameButton;

    QLabel* corName;
    QPushButton* corNameButton;

    QLabel* effName;
    QPushButton* effNameButton;

    QLabel* fileName;
    QPushButton* fileNameButton;

    QLabel* outName;
    QPushButton* outNameButton;

    QPushButton* runButton;

    QLabel* progressLabel;
    QLabel* ratioLabel;

    int detectorCorrelating();
    QVector <QVector <int> > corMatrix;
    u_int16_t nrMatrices;

    int findCalib();
    QVector <QVector <double> > calMatrix;

    void findMaxRange();
    double maxRange;

    void findEff();
    QVector <QVector <double> > effMatrix;
    bool effGood;
    void normalizeEff();
    double effTerm;

    void closeEvent(QCloseEvent *event);
    void writeSettings();

    QVector <QVector <double> > dataMatrix;
    int readData(int);

    double calibrate(int, int);
    double calcEfficiency(int, double);

public slots:
    void calibButtonClicked();
    void corButtonClicked();
    void effButtonClicked();
    void fileButtonClicked();
    void runButtonClicked();
    void outButtonClicked();
};

#endif // MAINWINDOW_H
