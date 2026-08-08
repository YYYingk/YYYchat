QT += widgets network

RC_ICONS = wec.ico
DESTDIR = ./bin

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatitembase.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    chatview.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    customizeedit.cpp \
    findsuccessdlg.cpp \
    global.cpp \
    httpmgr.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagetextedit.cpp \
    picturebubble.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    searchlist.cpp \
    statewidget.cpp \
    tcpmgr.cpp \
    textbubble.cpp \
    timerbtn.cpp \
    userdata.cpp \
    usermgr.cpp

HEADERS += \
    adduseritem.h \
    applyfriend.h \
    bubbleframe.h \
    chatdialog.h \
    chatitembase.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    chatview.h \
    clickedbtn.h \
    clickedlabel.h \
    customizeedit.h \
    findsuccessdlg.h \
    global.h \
    httpmgr.h \
    listitembase.h \
    loadingdlg.h \
    logindialog.h \
    mainwindow.h \
    messagetextedit.h \
    picturebubble.h \
    registerdialog.h \
    resetdialog.h \
    searchlist.h \
    singleton.h \
    statewidget.h \
    tcpmgr.h \
    textbubble.h \
    timerbtn.h \
    userdata.h \
    usermgr.h

FORMS += \
    adduseritem.ui \
    applyfriend.ui \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    findsuccessdlg.ui \
    loadingdlg.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

# ========== 文件拷贝（qmake 阶段执行，MinGW 绝对可用） ==========
# 将 Qt 内部路径转为 Windows 原生路径
WIN_PWD = $$replace(PWD, /, \\)
WIN_OUT_PWD = $$replace(OUT_PWD, /, \\)

# 目标目录
DST_DIR = $$WIN_OUT_PWD\\bin

# 创建目标目录（若不存在）
system(cmd /c "if not exist \"$$DST_DIR\" mkdir \"$$DST_DIR\"")

# 复制 config.ini
system(cmd /c "copy /Y \"$$WIN_PWD\\config.ini\" \"$$DST_DIR\"")

# 复制 static 文件夹
system(cmd /c "xcopy /Y /E /I \"$$WIN_PWD\\static\" \"$$DST_DIR\\static\\\"")

# 调试输出（会打印在“编译输出”窗口）
message("Copied config.ini to $$DST_DIR")
message("Copied static/    to $$DST_DIR\\static")