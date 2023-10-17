#pragma once
#include "GenericImageService.h"
#include <QtWidgets/QMainWindow>
#include "ui_JwsMainWindow.h"

class JwsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    JwsMainWindow(QWidget *parent = nullptr);
    ~JwsMainWindow();

private:
    Ui::JwsMainWindowClass ui;
};
