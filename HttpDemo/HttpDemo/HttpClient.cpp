#include "HttpClient.h"
#include "curl/curl.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>


#ifdef _DEBUG

#pragma comment(lib, "libcurld.lib")
#else

#pragma comment(lib, "libcurl.lib")

#endif // _DEBUG


using namespace std;

namespace _HBTools {
	void eachVarmap(const QVariantMap& map, std::function<void(const QString& key, const QVariant& value)> callback)
	{
		auto keys = map.keys();

		for (int i = 0; i < keys.count(); i++)
		{
			QString key = keys[i];
			QVariant value = map[key];
			callback(key, value);
		}
	}

	void eachJsonObj(const QJsonObject& json, std::function<void(const QString& key, const QJsonValue& value)> callback)
	{

		auto keys = json.keys();
		for (int i = 0; i < keys.count(); i++)
		{
			QString key = keys[i];
			QJsonValue value = json.value(key);
			callback(key, value);
		}
	}

	QByteArray dataFromMap(const QVariantMap& map)
	{
		QJsonDocument doc = QJsonDocument::fromVariant(QVariant(map));
		return doc.toJson(QJsonDocument::Compact);
	}

	QString urlQueryFromMap(const QVariantMap& map)
	{
		QStringList list;

		auto keys = map.keys();
		for (int i = 0; i < keys.count(); i++)
		{
			QString key = keys[i];
			QString value = map[key].toString();

			list << QString("%1=%2").arg(key, value);
		}
		return list.join("&");
	}

	static size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
	{
		((string*)stream)->append((char*)ptr, 0, size * count);
		return size * count;
	}

	int http(const HttpClient::HttpType type, const QVariantMap& headers, const QString& urlStr, const QVariantMap& queryParam, const QVariantMap& param, QJsonObject& response)
	{

		CURL *curl = curl_easy_init();
		if (NULL == curl)
			return CURLE_FAILED_INIT;

		QVariantMap tempHeader = headers;

		if (type == HttpClient::FORM)
		{
			tempHeader.insert("Content-Type", "application/x-www-form-urlencoded");
		}
		else
		{
			tempHeader.insert("Content-Type", "application/json;charset=utf-8");
		}


		struct curl_slist *c_headers = NULL;

		eachVarmap(tempHeader, [&](const QString& key, const QVariant& value) {
			QString tempStr = QString("%1:%2").arg(key, value.toString());
			c_headers = curl_slist_append(c_headers, tempStr.toStdString().c_str());
		});

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, c_headers);

		QString queryStr = urlQueryFromMap(queryParam);

		QString url = urlStr;
		if (!queryStr.isEmpty())
		{
			if (!url.contains("?"))
			{
				url.append("?");
			}
			else
			{
				url.append("&");
			}
			url.append(queryStr);
		}

		qDebug() << "\n";
		qDebug() << "HBNet type: " << type;
		qDebug() << "HBNet url: " << url;
		qDebug() << "HBNet Query: " << queryStr;
		qDebug() << "HBNet Param: " << dataFromMap(param);
		qDebug() << "\n";

		if (type == HttpClient::GET)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
		}
		else if (type == HttpClient::POST)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1L);

			QByteArray bodyData = dataFromMap(param);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyData.data());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, bodyData.length());
		}
		else if (type == HttpClient::FORM)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
		}
		else
		{

			qDebug() << "HYNet request type has no realize";
			curl_slist_free_all(c_headers);
			curl_easy_cleanup(curl);

			return -1;
		}

		string responseStr = "";

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		CURLcode res = curl_easy_perform(curl);

		curl_slist_free_all(c_headers);
		curl_easy_cleanup(curl);

		if (res != 0)
		{
			response = QJsonObject();
			return res;
		}

		qDebug() << "\n" << "HBNet response: " << QString::fromStdString(responseStr);

		QJsonDocument doc = QJsonDocument::fromJson(responseStr.c_str());

		response = doc.object();
		return res;
	}

}


using namespace _HBTools;

void HttpClient::initNet()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void HttpClient::releaseNet()
{
	curl_global_cleanup();
}

int HttpClient::Get(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers /* = QVariantMap() */)
{
	int ret = http(GET, headers, urlStr, param, QVariantMap(), response);
	return ret;
}


int HttpClient::Post(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers /* = QVariantMap() */)
{
	int ret = http(POST, headers, urlStr, QVariantMap(), param, response);
	return ret;
}

int HttpClient::Form(const QString& urlStr, const QVariantMap& param, QJsonObject& response, QVariantMap headers /* = QVariantMap() */)
{
	int ret = http(FORM, headers, urlStr, param, QVariantMap(), response);
	return ret;
}





