#include "mainwindow.h"

MainWindow::MainWindow()
{
    readSettings();

    window = new QWidget;
    setCentralWidget(window);
    window->resize(640,360);
    QGridLayout *mainLayout = new QGridLayout(window);

    QFrame* myFrame1 = new QFrame();
    myFrame1->setFrameShape(QFrame::HLine);
    QFrame* myFrame2 = new QFrame();
    myFrame2->setFrameShape(QFrame::HLine);
    QFrame* myFrame3 = new QFrame();
    myFrame3->setFrameShape(QFrame::HLine);
    QFrame* myFrame4 = new QFrame();
    myFrame4->setFrameShape(QFrame::HLine);


    calibName = new QLabel("No calibration set!");
    QLabel* calibName1=new QLabel("Calibration File:");
    if(!calibrationAddress.isEmpty())
        calibName->setText(calibrationAddress);
    calibNameButton = new QPushButton(QObject::tr("Choose"));
    mainLayout->addWidget(calibName1,0,0,1,1);
    mainLayout->addWidget(calibName, 0,1,1,2);
    mainLayout->addWidget(calibNameButton, 0,3,1,1);
    connect(calibNameButton,SIGNAL(clicked()),this,SLOT(calibButtonClicked()));

    mainLayout->addWidget(myFrame1, 1, 0, 1, 4);

    corName = new QLabel("No correlation set!");
    QLabel* corName1=new QLabel("Correlation File:");
    if(!corAddress.isEmpty())
        corName->setText(corAddress);
    corNameButton = new QPushButton(QObject::tr("Choose"));
    mainLayout->addWidget(corName1,2,0,1,1);
    mainLayout->addWidget(corName, 2,1,1,2);
    mainLayout->addWidget(corNameButton, 2,3,1,1);
    connect(corNameButton,SIGNAL(clicked()),this,SLOT(corButtonClicked()));

    mainLayout->addWidget(myFrame2, 3, 0, 1, 4);

    effName = new QLabel("No efficiency set!");
    QLabel* effName1=new QLabel("Efficiency File:");
    if(!effAddress.isEmpty())
        effName->setText(effAddress);
    effNameButton = new QPushButton(QObject::tr("Choose"));
    mainLayout->addWidget(effName1,4,0,1,1);
    mainLayout->addWidget(effName, 4,1,1,2);
    mainLayout->addWidget(effNameButton, 4,3,1,1);
    connect(effNameButton,SIGNAL(clicked()),this,SLOT(effButtonClicked()));

    mainLayout->addWidget(myFrame3, 5, 0, 1, 4);

    fileName = new QLabel("No file to be analyzed set!");
    QLabel* fileName1=new QLabel("Data Files:");
    if(!fileAddress.isEmpty())
        fileName->setText(fileAddress.join("\n"));
    fileNameButton = new QPushButton(QObject::tr("Choose"));
    mainLayout->addWidget(fileName1,6,0,1,1);
    mainLayout->addWidget(fileName, 6,1,1,2);
    mainLayout->addWidget(fileNameButton, 6,3,1,1);
    connect(fileNameButton,SIGNAL(clicked()),this,SLOT(fileButtonClicked()));

    mainLayout->addWidget(myFrame4, 7, 0, 1, 4);

    outName = new QLabel("No output folder selected!");
    QLabel* outName1=new QLabel("Output folder:");
    if(!outAddress.isEmpty())
        outName->setText(outAddress);
    outNameButton = new QPushButton(QObject::tr("Choose"));
    mainLayout->addWidget(outName1,8,0,1,1);
    mainLayout->addWidget(outName, 8,1,1,2);
    mainLayout->addWidget(outNameButton, 8,3,1,1);
    connect(outNameButton,SIGNAL(clicked()),this,SLOT(outButtonClicked()));

    runButton = new QPushButton(QObject::tr("Run!"));
    mainLayout->addWidget(runButton, 9,0,1,4);
    connect(runButton,SIGNAL(clicked()),this,SLOT(runButtonClicked()));

    progressLabel = new QLabel("Ready to go");
    ratioLabel = new QLabel("");
    progressLabel->setAlignment(Qt::AlignCenter);
    ratioLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(progressLabel, 10,0,1,5);
    progressLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(ratioLabel, 11,0,1,5);
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Corelator",
                                                                   tr("Do you want to save?\n"),
                                                                   QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::Yes);
       if (resBtn == QMessageBox::Yes) {
           writeSettings();
           event->accept();
       }

       else if (resBtn == QMessageBox::No)
       {
           event->accept();
       }
       else
       {
           event->ignore();
       }
}

void MainWindow::readSettings()
{
    QSettings settings("IFIN-HH", "Corelator");

        settings.beginGroup("MainWindow");
        calibrationAddress=settings.value("Calibration Address").toString();
        corAddress=settings.value("Correlation Address").toString();
        fileAddress=settings.value("File Name").toString().split("\n");
        effAddress=settings.value("Efficiency Address").toString();
        outAddress=settings.value("Output Folder").toString();
        settings.endGroup();
}


void MainWindow::writeSettings()
{
    QSettings settings("IFIN-HH", "Corelator");

    settings.beginGroup("MainWindow");
    settings.setValue("Calibration Address", calibrationAddress);
    settings.setValue("Correlation Address", corAddress);
    settings.setValue("File Name", fileAddress.join("\n"));
    settings.setValue("Efficiency Address", effAddress);
    settings.setValue("Output Folder", outAddress);
    settings.endGroup();
}

void MainWindow::calibButtonClicked()
{
    calibrationAddress=QFileDialog::getOpenFileName(0,QObject::tr("Choose calibration file name"), "/home/Desktop/Corelator",QObject::tr("Text (*.cal)"));
    calibName->setText(calibrationAddress);
}

void MainWindow::corButtonClicked()
{
    corAddress=QFileDialog::getOpenFileName(0,QObject::tr("Choose correlation file name"), "/home/Desktop/Corelator",QObject::tr("Text (*.cor)"));
    corName->setText(corAddress);
}

void MainWindow::effButtonClicked()
{
    effAddress=QFileDialog::getOpenFileName(0,QObject::tr("Choose efficiency file name"), "/home/Desktop/Corelator",QObject::tr("Text (*.eff)"));
    effName->setText(effAddress);
}

void MainWindow::fileButtonClicked()
{
    fileAddress=QFileDialog::getOpenFileNames(0,QObject::tr("Choose data file name"), "/home/Desktop/Corelator",QObject::tr("Text (*)"));
    fileName->setText(fileAddress.join("\n"));
}

void MainWindow::outButtonClicked()
{
    outAddress=QFileDialog::getExistingDirectory(0,QObject::tr("Choose output folder"),"/home",QFileDialog::ShowDirsOnly);
    outName->setText(outAddress);
}

void MainWindow::runButtonClicked()
{
    int corReturn, calReturn, readReturn;

    corReturn=detectorCorrelating();
    if(corReturn==1)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: Correlation file not set or not found!");
        progressLabel->setText(QObject::tr("Failure!"));
        error->exec();
    }
    else if(corReturn==2)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: One of the rows of the correlation file is incomplete!");
        progressLabel->setText(QObject::tr("Failure!"));
        error->exec();
    }
    else if(corReturn==3)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: Invalid detector index in the correlation file!");
        progressLabel->setText(QObject::tr("Failure!"));
        error->exec();
    }

    calReturn=findCalib();
    if(calReturn==1)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: Calibration file not set or not found!");
        progressLabel->setText(QObject::tr("Failure!"));
        error->exec();
    }
    else if(calReturn==2)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: One of calibration rows has less parameters than declared!");
        progressLabel->setText(QObject::tr("Failure!"));
        error->exec();
    }

    findEff();

    if(!(corReturn+calReturn))
    {
        for(int currentGroup=1;currentGroup<=nrMatrices;currentGroup++)
        {
            dataMatrix.clear();
            dataMatrix.resize(8192);

            for(int i=0;i<8192;i++)
            {
                dataMatrix[i].resize(8192);
                dataMatrix[i].fill(0);
            }

            readReturn=readData(currentGroup);

            progressLabel->setText(QObject::tr("Success!"));

            if(readReturn)
            {
                currentGroup=nrMatrices+1;
                progressLabel->setText(QObject::tr("Failure!"));
            }
        }

    }
    else progressLabel->setText(QObject::tr("Failure!"));

    if(readReturn==1)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: One or more data files not found!");
        error->exec();
    }
    else if(readReturn==2)
    {
        QMessageBox* error=new QMessageBox();
        error->setText("Error: Could not write to disk!");
        error->exec();
    }

    if(!(readReturn+corReturn+calReturn))
    {
        QString program = "./home/vme/Desktop/Corelator/post.sh";
        QStringList arguments;
        arguments<<"post.sh"<<outAddress;

        QProcess *myProcess = new QProcess();
        myProcess->startDetached("/bin/bash", arguments);
    }

}

int MainWindow::detectorCorrelating()
{
   int n,s=0,num,num2;
   std::string str;

   QByteArray ba = corAddress.toLatin1();
   const char *c_str2 = ba.data();
   std::ifstream file(c_str2,std::ios::in);

   if(file.is_open())
   {
       file>>n;

       // Initializare matrice A

       for(int i=0;i<n;i++)
       {
           QVector<int> p;
           for(int j=0;j<n;j++)
           {
               p.push_back(0);
           }
           corMatrix.push_back(p);
        }

       // Citirea datelor din fisier

       if (file.good())
       {
           while(getline(file, str))
           {
               std::istringstream ss(str);
               while(ss >> num)       //Completarea Matricei
               {
                   if(!(ss>>num2))
                   {
                       std::cout<<"A pair is incomplete! Row "<<s<<std::endl;
                       return 2;
                       break;
                   }
                   if((num>=n)||(num2>=n))
                   {
                       std::cout<<"Invalid number in data file"<<std::endl;
                       return 3;
                       break;
                   }
                   corMatrix[num][num2]=s;
               }
               s++;
           }
       }
       nrMatrices=s-1;
       return 0;
   }
   else return 1;
}

int MainWindow::findCalib()
{
   QByteArray ba = calibrationAddress.toLatin1();
   const char *c_str2 = ba.data();
   std::ifstream fileCalib(c_str2 ,std::ios::in);

   QVector <double> row;
   int nmax=0;

  // Initializare matrice A

   // Citirea datelor din fisier

   if (fileCalib.good())
   {
       std::string str;
       while(getline(fileCalib, str))
       {
           std::istringstream ss(str);
           int num1,num2,num3;
           while(ss >> num1)
           {
               ss>>num2;
               ss>>num3;
               if(num2+1>nmax)
               {
                   nmax=num2+1;
                   calMatrix.resize(num2+1);
               }
               row.clear();
               row.resize(num3);
               for(int i=0;i<num3;i++)
                  {
                   if(!(ss>>row[i]))
                       return 2;
                  }
               calMatrix[num2]=row;
           }
       }

       findMaxRange();

       return 0;
   }
   else return 1;
}

void MainWindow::findMaxRange()
{
    maxRange=0;
    for(int i=0;i<calMatrix.size();i++)
    {
        double range=0;
        for(int j=0;j<calMatrix[i].size();j++)
        {
            range=range+calMatrix[i][j]*pow(8192,j);
        }

        if(range>maxRange)
            maxRange=range;
    }

    maxRange=ceil(maxRange*100/8192)*8192/100;

    ratioLabel->setText(QObject::tr("%1 keV/channel").arg(maxRange/8192));
}

void MainWindow::findEff()
{
   QByteArray ba = effAddress.toLatin1();
   const char *c_str2 = ba.data();
   std::ifstream file(c_str2 ,std::ios::in);
   std::string str;
   double temp;
   QVector <double> v;

   // Citirea datelor din fisier

   if (file.good())
   {
       effGood=1;
       while(getline(file, str))
       {
           v.clear();
           std::istringstream ss(str);
           while(ss >> temp)
           {
               v.append(temp);
           }
           effMatrix.append(v);
       }
   }
   else effGood=0;

   if(effGood)
       normalizeEff();
   else effTerm=1;
}

void MainWindow::normalizeEff()
{
    double maxEff=0, minEff, midEff, eff;
    for(int i=0;i<maxRange;i++)
    {
        eff=effMatrix[0][0]*exp(-effMatrix[0][1]*log(i-effMatrix[0][2]+effMatrix[0][3]*exp(-effMatrix[0][4]*i)));
        if(maxEff<eff)
            maxEff=eff;
    }
    minEff=effMatrix[0][0]*exp(-effMatrix[0][1]*log(maxRange-effMatrix[0][2]+effMatrix[0][3]*exp(-effMatrix[0][4]*maxRange)));

    midEff=(maxEff+minEff)/2;

    effTerm=10*pow(midEff,2);
}

int MainWindow::readData(int groupIndex)
{
    int index,length, bin1, bin2;
    double energy1, energy2, eff1, eff2;
    QVector <u_int16_t> det;
    QVector <u_int16_t> energy;

    srand(time(NULL));
    QDataStream input;
    QFile inputFile;

    for(int k=0;k<fileAddress.size();k++)
    {
        inputFile.setFileName(fileAddress[k]);
        inputFile.open(QIODevice::ReadOnly);

        input.setDevice(&inputFile);
        input.setByteOrder(QDataStream::LittleEndian);

        if(inputFile.isOpen())
        {

            QVector<u_int16_t> read;
            read.resize(16384);

            for(int i=0;i<16384;i++)
                input>>read[i];

            while(!input.atEnd())
            {
                read.clear();
                read.resize(16384);
                for(int i=0;i<16384;i++)
                    input>>read[i];

                index=16;
                while(index<16384)
                {
                    if(read[index]!=0)
                        length=read[index]-61440;
                    else break;

                    det.clear();
                    energy.clear();

                    if(read[index+2]>1)
                    {
                        for(int i=0;i<read[index+2];i++)
                        {
                            det.append(read[index+4+i*3]);
                            energy.append(read[index+5+i*3]);
                        }

                        for(int i=0;i<det.size()-1;i++)
                            for(int j=i+1;j<det.size();j++)
                            {
                                energy1=calibrate(det[i],energy[i]);
                                energy2=calibrate(det[j],energy[j]);
                                bin1=(energy1*8192/maxRange);
                                bin2=(energy2*8192/maxRange);
                                eff1=calcEfficiency(det[i],energy1);
                                eff2=calcEfficiency(det[j],energy2);

                                if (bin1>8192)
                                    bin1=8192;
                                else if (bin1<0)
                                    bin1=0;

                                if (bin2>8192)
                                    bin2=8192;
                                else if (bin2<0)
                                    bin2=0;

                                if(corMatrix[det[i]][det[j]]==groupIndex)
                                    dataMatrix[bin1][bin2]+=effTerm/(eff1*eff2);
                                if(corMatrix[det[j]][det[i]]==groupIndex)
                                    dataMatrix[bin2][bin1]+=effTerm/(eff1*eff2);
                            }
                    }

                    index=index+length+1;
                }
            }
            inputFile.close();
        }
        else return 1;
    }

    QDataStream output;
    QFile outputFile;
    outputFile.setFileName(QObject::tr("%1/group%2.txt").arg(outAddress).arg(groupIndex));
    outputFile.open(QIODevice::WriteOnly);

    output.setDevice(&outputFile);
    output.setByteOrder(QDataStream::LittleEndian);

    if(outputFile.isOpen())
    {
        for(int i=0;i<8192;i++)
        {
            for (int j=0;j<8192;j++)
            {
                output<<(u_int32_t) dataMatrix[i][j];
            }
        }
        return 0;
    }
    else return 2;
}

double MainWindow::calibrate(int index, int channel)
{
    double adjusted;
    adjusted=channel+((double) rand() / (RAND_MAX));

    double energy=0;
    for(int i=0;i<calMatrix[index].size();i++)
        energy=energy+calMatrix[index][i]*pow(adjusted,i);

    return energy;
}

double MainWindow::calcEfficiency(int index, double energy)
{
    if(effGood)
    {
        double efficiency;
        efficiency=effMatrix[index][0]*exp(-effMatrix[index][1]*log(energy-effMatrix[index][2]+effMatrix[index][3]*exp(-effMatrix[index][4]*energy)));
        return efficiency;
    }
    else return 1;
}

