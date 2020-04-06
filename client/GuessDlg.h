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

private slots:
    void accept();
    void reject();

private:
    void setupUI();

    bool m_bMode;         // are we showing a suggestion (true) or accusation(false) dialog box?
    QComboBox* m_cboRoom;
    QComboBox* m_cboWeapon;
    QComboBox* m_cboSuspect;

    QString    m_qstrGuess;

};
