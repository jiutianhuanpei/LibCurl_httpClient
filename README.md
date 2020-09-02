# LibCurl_httpClient


下载 demo 工程之后可直接运行、测试：

测试代码：

```C++
using namespace HttpClient;

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
```

<br/>

主要文件：

* HttpDemp/curl 文件夹，libCurl 的库文件
* HttpDemo/HttpClient.{h|m} 请求库的封装文件

<br/>

配置：

生成事件-->生成前事件-->命令行: `xcopy "$(MSBuildProjectDirectory)\curl\dll\*" "$(MSBuildStartupDirectory)\Debug\"`

> 这个是为了把 libCurl 库的 dll 文件拷贝到工程运行目录 `HttpDemo/Debug` 下，实际可手动配置
