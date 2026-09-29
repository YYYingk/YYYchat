#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "global.h"
#include "statewidget.h"
#include "userdata.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void handleGlobalMousePress(QMouseEvent *event) ;
private:
    void addChatUserList();
    void ShowSearch(bool bsearch = false);
    void AddLBGroup(StateWidget* lb);
    void ClearLabelState(StateWidget* lb);
    void SetSelectChatItem(int uid = 0);
    void SetSelectChatPage(int uid = 0);
    void loadMoreConUser();
    void loadMoreChatUser();
    void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData> > msgdata);
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    Ui::ChatDialog *ui;
    QList<StateWidget*>  _lb_list;
    QWidget* _last_widget;
    QMap<int, QListWidgetItem*> _chat_items_added;
    int _cur_chat_uid;
    QTimer* _timer;
public slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString & str);
    void slot_loading_contact_user();
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> ui);
    void slot_friend_info_page(std::shared_ptr<UserInfo> user_info);
    void slot_switch_apply_friend_page();
    void slot_item_clicked(QListWidgetItem *item);
    void slot_append_send_chat_msg(std::shared_ptr<TextChatData> msgdata);
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
private slots:
};

#endif // CHATDIALOG_H
