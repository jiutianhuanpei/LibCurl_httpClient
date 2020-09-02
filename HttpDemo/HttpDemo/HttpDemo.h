#pragma once

#include <QtWidgets/QWidget>
#include "ui_HttpDemo.h"

class HttpDemo : public QWidget
{
    Q_OBJECT

public:
    HttpDemo(QWidget *parent = Q_NULLPTR);

private:
    Ui::HttpDemoClass ui;





private:
	void m_didClickedBtn();




};
