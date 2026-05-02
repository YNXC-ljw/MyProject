#include "widget.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QSharedMemory sharedMemoty("MiniMusic");
    
    // 1. 程序第一次运行时，共享内存未创建，attach()会失败（返回false）；
    // 2. 程序第二次运行时，共享内存已被第一次运行的进程创建，attach()会成功（返回true）；
    // 3. 由于限制只能运行一个实例，因此让第二次启动的进程直接退出。
    if (sharedMemoty.attach()) 
    {
        QMessageBox::information(nullptr, "MiniMusic提示", "MiniMusic已经在运行...");
        return 0; // 重复启动时，直接退出进程
    }
    
    // 第一次运行时，attach()失败，执行create()创建共享内存（大小为1字节，仅需标识“存在”即可）
    sharedMemoty.create(1);
    
    
    Widget w;
    w.show();
    return a.exec();
}
