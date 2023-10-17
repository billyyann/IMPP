#include "JwsMainWindow.h"
#include <QLabel>
#include <loader.h>
#include <memory>
#include "IPlugin.h"
#include <iostream>
#include <QPushButton>
#include "global.h"
#include <chrono>
#include <QImage>


JwsMainWindow::JwsMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //int nWidth = GetSystemMetrics(SM_CXSCREEN);
    //int nHeight = GetSystemMetrics(SM_CYSCREEN);
    this->resize(2400, 1500);

    ui.menuBar->addMenu(tr("File"));
    // �����Զ�ɨ�蹦�ܽ���dat
    // save �������
    // export ���ͼ��
    // download ����dat�����أ��޸����ݿ�·��Ϊ����·��
    // ����
    // ���������dat 
    // open

    ui.menuBar->addMenu(tr("Edit"));
    // undo
    // redo
    // ���ݱ�ע����ţ�˫��dat �������ţ�

    ui.menuBar->addMenu(tr("Display"));
    //���� �鿴 op json
    //�鿴���ݿ�
    // �鿴 ͳ����Ϣ

    ui.menuBar->addMenu(tr("Views"));
    // �鿴��������ͬ��״̬
    // �鿴��ʷ�������
    // �鿴������ʷ����
    // 
    ui.menuBar->addMenu(tr("Help"));
    ui.mainToolBar->setVisible(false);

    /***
    * ����˵���
    * ���ܣ�
    * ͳ����Ϣ��
    * ֱ��ͼ
    * �Ŵ�
    * ��С
    * ����Ϣ����
    * 
    ***/
    QToolBar* hToolBar = new QToolBar(this);  // ����������
    //toolBar->resize(1000, 1500);
    addToolBar(Qt::TopToolBarArea, hToolBar); // ����Ĭ��ͣ����Χ [Ĭ��ͣ�����]

    hToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);   // ���������϶�
    hToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);   // ���������϶�
    hToolBar->setFloatable(false);       // �����Ƿ񸡶�
    hToolBar->setMovable(false);         // ���ù������������ƶ�

    QPushButton* statistic = new QPushButton(this);
    statistic->setText("statistic");
    statistic->resize(100, 150);
    hToolBar->addWidget(statistic);

    QPushButton* zoomin = new QPushButton(this);
    zoomin->setText("zoomin");
    zoomin->resize(100, 150);
    hToolBar->addWidget(zoomin);

    QPushButton* zoomout = new QPushButton(this);
    zoomout->setText("zoomout");
    zoomout->resize(100, 150);
    hToolBar->addWidget(zoomout);

    QPushButton* play = new QPushButton(this);
    play->setText("play");
    play->resize(100, 150);
    hToolBar->addWidget(play);

    QPushButton* stop = new QPushButton(this);
    stop->setText("stop");
    stop->resize(100, 150);
    hToolBar->addWidget(stop);


    QToolBar* vToolBar = new QToolBar(this);  // ����������
    //toolBar->resize(1000, 1500);
    addToolBar(Qt::RightToolBarArea, vToolBar); // ����Ĭ��ͣ����Χ [Ĭ��ͣ�����]

    vToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);   // ���������϶�
    vToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);   // ���������϶�
    vToolBar->setFloatable(false);       // �����Ƿ񸡶�
    vToolBar->setMovable(false);         // ���ù������������ƶ�


    PluginManager ma;
    IPluginFactory* infoA = ma.addPlugin("DeNoiseModule");

    assert(infoA != nullptr);
    std::cout << " ---- Plugin Information --------- " << "\n"
        << "  =>              Name = " << infoA->Name() << "\n"
        << "  =>           Version = " << infoA->Version() << "\n"
        << "  => Number of classes = " << infoA->NumberOfClasses()
        << "\n\n";

    for (int i = 0;i < infoA->NumberOfClasses();i++) {
        std::cout << infoA->GetClassName(i) << std::endl;
   } 

    
    QPushButton * module1 = new QPushButton(this);
    module1->setText("||===== DeNoiseModule =====||");
    module1->resize(100, 150);
    vToolBar->addWidget(module1);

    for (int i = 0;i < infoA->NumberOfClasses();i++) {
        //std::cout << infoA->GetClassName(i) << std::endl;
        QPushButton* tmpClass = new QPushButton(this);
        tmpClass->resize(100, 150);
        tmpClass->setText("||----- "+ QString(infoA->GetClassName(i)) +" -----||");
        vToolBar->addWidget(tmpClass);
    }

    QPushButton* module2 = new QPushButton(this);
    module2->setText("||=====================||");
    module2->resize(100, 150);
    vToolBar->addWidget(module2);

    // ��������Ӳ˵���
    //toolBar->addAction(newAction);
    //toolBar->addSeparator();
    //toolBar->addAction(openAction);
    //ui.mainToolBar->setGeometry(0, 2000, 400, 1200);

    auto dpt = GenericImageService::GetInstance();
    RawImageIO rawImage;
   
    //std::string fn = "C:\\Users\\86138\\Desktop\\project\\zdg_calibration\\data\\data\\ECC\\C\\YDJJ\\HY1403_20230609_00000006_LGXJ_02_DPD1.dat";
    //std::string fn = "C:\\Users\\86138\\Desktop\\project\\zdg_calibration\\data\\data\\ECC\\DSC\\HY1402_20230609_00066298_YDXJ_02_DBHY.dat";
    //std::string fn = "C:\\Users\\86138\\Desktop\\project\\zdg_calibration\\data\\data\\ECC\\DPD4\\HY1402_20230609_00062605_LGXJ_02_HDPY.dat";
    std::string fn = "C:\\Users\\86138\\Desktop\\project\\zdg_calibration\\data\\data\\ECC\\C\\DPD\\HY1403_20230609_00000006_LGXJ_02_DPD1.dat";
    int cycleId = 0;
    dpt->loadImage(fn, IMAGELEVEL::ONE);
    /*std::future<bool> response=dpt->loadImage(fn, IMAGELEVEL::ONE);
    std::future_status status = response.wait_for(std::chrono::seconds(1));*/

    //if (status == std::future_status::ready && response.get()) {
    //    std::vector<uint32_t> nums=rawImage.getFrameNums();
    //    cv::Mat img;
    //    rawImage.getImageViaFrame(nums[0], img);
    //    QImage tempImg = QImage(img.data,img.cols, img.rows, QImage::Format_Grayscale16);
    //    QLabel* ll=new QLabel(this);
    //    ll->resize(1024, 1024);
    //    ll->move(150, 150);
    //    ll->setPixmap(QPixmap::fromImage(tempImg).scaled(ll->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //    ll->show();
    //}
}

JwsMainWindow::~JwsMainWindow()
{}
