#pragma once

#include <QObject>
#include <QVariantMap>
#include <QJsonObject>

namespace HttpClient {

	enum HttpType
	{
		GET,
		POST,
		FORM
	};

	void initNet();
	void releaseNet();

	int Get(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers = QVariantMap());
	int Post(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers = QVariantMap());
	int Form(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers = QVariantMap());

	
}
