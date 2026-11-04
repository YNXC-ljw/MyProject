#include "netclient.h"
#include <QNetworkReply>
#include "../model/data.h"

namespace network{

NetClient::NetClient(model::DataCenter *dataCenter)
    :dataCenter(dataCenter)
{

}

void NetClient::ping()
{
    // 1.制作请求
    QNetworkRequest httpReq;
    httpReq.setUrl(HTTP_URL + "/ping");

    // 2.获取响应(响应和请求不同，请求是立即发送的，但是响应回来需要时间 )
    QNetworkReply* httpResp = httpClient.get(httpReq);
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // 此时说明响应已经回来
        if(httpResp->error() != QNetworkReply::NoError) // 确认网络请求没有出错
        {
            LOG() << "HTTP请求失败!" << httpResp->errorString();
            return;
        }
        // 获取响应的body
        QByteArray body = httpResp->readAll();
    });
}



} // end namespace