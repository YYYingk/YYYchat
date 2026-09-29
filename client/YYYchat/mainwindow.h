#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      主窗口
 *
 * @author     YYYing
 * @date       2026/07/20
 * @history
 *****************************************************************************/

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum UIStatus{
    LOGIN_UI,
    REGISTER_UI,
    RESET_UI,
    CHAT_UI,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchLogin2();
    void SlotSwitchChat();
    void SlotOffline();
    void SlotExcepOffline();
private:
    void offlineLogin();
    Ui::MainWindow *ui;
    LoginDialog *_login_dlg;
    RegisterDialog *_reg_dlg;
    ResetDialog *_reset_dlg;
    ChatDialog * _chat_dlg;
    UIStatus _ui_status;
};
#endif // MAINWINDOW_H
