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

public slots:
    void onDone();

private:
    void setupUI();

    QPushButton*             m_btnSelect;
    QVector<QCheckBox*>      m_vecPushButton;
    QString                  m_qstrAvatars;
};
