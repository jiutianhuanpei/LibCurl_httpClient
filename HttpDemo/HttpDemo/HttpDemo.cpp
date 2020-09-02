#include "HttpDemo.h"
#include "HttpClient.h"
#include <QDebug>
#include <QPushButton>

using namespace HttpClient;

HttpDemo::HttpDemo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &HttpDemo::m_didClickedBtn);

}


void HttpDemo::m_didClickedBtn()
{


	qDebug() << "m_didClickedBtn";

	QVariantMap param;
	param.insert("from", "en");
	param.insert("to", "zh");
	param.insert("q", "apple");
	param.insert("appid", "2015063000000001");
	param.insert("salt", "1435660288");
	param.insert("sign", "f89f9594663708c1605f3d736d01d2d4");

	QJsonObject json;

	QString urlStr = "http://api.fanyi.baidu.com/api/trans/vip/translate";

	Get(urlStr, param, json);
	//Form(urlStr, param, json);
	//Post(urlStr, param, json);

	qDebug() << "\n" << json;
}











