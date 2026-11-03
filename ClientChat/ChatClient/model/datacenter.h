#ifndef DATACENTER_H
#define DATACENTER_H

#include <QWidget>
#include "data.h"

namespace model {

class DataCenter : public QObject
{
    Q_OBJECT
public:
    static DataCenter* getInstance();
private:
    DataCenter();
    static DataCenter* instance;

    // 列出 DataCenter 中要组织的所有数据

    // 客户端登录到服务器的会话id
    QString loginSessionId;
    // 当前用户信息
    UserInfo* myself = nullptr;
signals:
};

} // end namespace
#endif // DATACENTER_H
