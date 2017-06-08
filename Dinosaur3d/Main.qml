import QtQuick 2.4

Item
{
    id: item

    LoginBound
    {
        id: loginBound
        visible: !loginView.isLogin
        anchors.fill: parent
    }

    LoginView
    {
        id: loginView
        visible: !isLogin
        anchors.centerIn: parent
    }
}
