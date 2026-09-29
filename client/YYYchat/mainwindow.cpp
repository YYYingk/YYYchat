#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpMgr.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    // _login_dlg->show();

    _ui_status = LOGIN_UI;

    // 创建和注册消息连接
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);

    // 连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

    // 连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_switch_chatdlg, this, &MainWindow::SlotSwitchChat);

    // 链接服务器踢人消息
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_notify_offline, this, &MainWindow::SlotOffline);

    // 链接服务器断开逻辑
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_connection_closed, this, &MainWindow::SlotExcepOffline);

    // 测试用
    // emit TcpMgr::GetInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login_dlg){
    //     delete _login_dlg;
    //     _login_dlg = nullptr;
    // }

    // if(_reg_dlg){
    //     delete _reg_dlg;
    //     _reg_dlg = nullptr;
    // }
}

void MainWindow::SlotSwitchReg()
{
    _ui_status = REGISTER_UI;
    // 动态初始化，到用的时候再进行，可以自动帮我们回收注册界面
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 连接注册界面返回登录信号
    connect(_reg_dlg, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::SlotSwitchLogin()
{
    _ui_status = LOGIN_UI;
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _reg_dlg->hide();
    _login_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    _ui_status = RESET_UI;
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);

    _login_dlg->hide();
    _reset_dlg->show();
    //注册返回登录信号和槽函数
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}

//从重置界面返回登录界面
void MainWindow::SlotSwitchLogin2()
{
    _ui_status = LOGIN_UI;
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _reset_dlg->hide();
    _login_dlg->show();
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

void MainWindow::SlotSwitchChat()
{
    _ui_status = CHAT_UI;
    _chat_dlg = new ChatDialog(this);
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_chat_dlg);

    _login_dlg->hide();
    _chat_dlg->show();

    this->setMinimumSize(QSize(1050,900));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

void MainWindow::SlotOffline(){
    // 使用静态方法直接弹出一个信息框
    QMessageBox::information(this, "下线提示", "同账号异地登录，该终端下线！");
    TcpMgr::GetInstance()->CloseConnection();
    offlineLogin();
}

void MainWindow::SlotExcepOffline()
{
    // 使用静态方法直接弹出一个信息框
    QMessageBox::information(this, "下线提示", "心跳超时或者服务器异常，该终端下线！");
    TcpMgr::GetInstance()->CloseConnection();
    offlineLogin();
}

void MainWindow::offlineLogin(){
    if(_ui_status == LOGIN_UI){
        return;
    }
    _ui_status = LOGIN_UI;
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _chat_dlg->hide();
    this->setMaximumSize(300,500);
    this->setMinimumSize(300,500);
    this->resize(300,500);
    _login_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}