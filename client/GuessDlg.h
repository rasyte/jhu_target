#pragma once

#include <QDialog>
#include <QString>

class QComboBox;

class guessDlg : public QDialog
{
    Q_OBJECT

public:
    guessDlg(QWidget* parent, bool bMode);

    QString   getGuess() { return m_qstrGuess; }

    void setRoom(int r);

private slots:
    void accept();
    void reject();

private:
    void setupUI();

    bool m_bMode;         // are we showing a suggestion (true) or accusation(false) dialog box?
    QWidget*   m_pParent;
    QComboBox* m_cboRoom;
    QComboBox* m_cboWeapon;
    QComboBox* m_cboSuspect;

    QString    m_qstrGuess;

};
