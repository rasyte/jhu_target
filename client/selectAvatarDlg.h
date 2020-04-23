#pragma once

#include <QDialog>
#include <QByteArray>


class QPushButton;
class QCheckBox;

class selectAvatarDlg : public QDialog
{
    Q_OBJECT

public:
    selectAvatarDlg(QWidget* parent, QString);
    QString  getAvatars(){ return m_qstrAvatars; }

    int getAvatar() { return m_avatar; }

public slots:
    void onDone();
    void avatarSelected(int);

private:
    void setupUI();

    int                      m_avatar;                     // which avatar was selected.

    QPushButton*             m_btnSelect;
    QVector<QCheckBox*>      m_vecPushButton;
    QString                  m_qstrAvatars;
};
