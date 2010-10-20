/********************************************************************************
** Form generated from reading UI file 'QtSpeech.ui'
**
** Created: Thu 14. Oct 14:28:16 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSPEECH_H
#define UI_QTSPEECH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtSpeech
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *hmm;
    QPushButton *hmmBrowseButton;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *language;
    QPushButton *lmBrowseButton;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *dict;
    QPushButton *dictBrowseButton;
    QLabel *label_4;
    QPushButton *decodingButton;
    QPlainTextEdit *plainTextEdit;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *QtSpeech)
    {
        if (QtSpeech->objectName().isEmpty())
            QtSpeech->setObjectName(QString::fromUtf8("QtSpeech"));
        QtSpeech->resize(339, 496);
        centralwidget = new QWidget(QtSpeech);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hmm = new QLineEdit(centralwidget);
        hmm->setObjectName(QString::fromUtf8("hmm"));

        horizontalLayout->addWidget(hmm);

        hmmBrowseButton = new QPushButton(centralwidget);
        hmmBrowseButton->setObjectName(QString::fromUtf8("hmmBrowseButton"));

        horizontalLayout->addWidget(hmmBrowseButton);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        language = new QLineEdit(centralwidget);
        language->setObjectName(QString::fromUtf8("language"));

        horizontalLayout_2->addWidget(language);

        lmBrowseButton = new QPushButton(centralwidget);
        lmBrowseButton->setObjectName(QString::fromUtf8("lmBrowseButton"));

        horizontalLayout_2->addWidget(lmBrowseButton);


        verticalLayout->addLayout(horizontalLayout_2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        dict = new QLineEdit(centralwidget);
        dict->setObjectName(QString::fromUtf8("dict"));

        horizontalLayout_3->addWidget(dict);

        dictBrowseButton = new QPushButton(centralwidget);
        dictBrowseButton->setObjectName(QString::fromUtf8("dictBrowseButton"));

        horizontalLayout_3->addWidget(dictBrowseButton);


        verticalLayout->addLayout(horizontalLayout_3);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        decodingButton = new QPushButton(centralwidget);
        decodingButton->setObjectName(QString::fromUtf8("decodingButton"));

        verticalLayout->addWidget(decodingButton);

        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout->addWidget(plainTextEdit);

        QtSpeech->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(QtSpeech);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QtSpeech->setStatusBar(statusbar);
        menubar = new QMenuBar(QtSpeech);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 339, 20));
        QtSpeech->setMenuBar(menubar);

        retranslateUi(QtSpeech);

        QMetaObject::connectSlotsByName(QtSpeech);
    } // setupUi

    void retranslateUi(QMainWindow *QtSpeech)
    {
        QtSpeech->setWindowTitle(QApplication::translate("QtSpeech", "QtSpeech", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QtSpeech", "HMM Folder", 0, QApplication::UnicodeUTF8));
        hmmBrowseButton->setText(QApplication::translate("QtSpeech", "Browse...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QtSpeech", "Language Model", 0, QApplication::UnicodeUTF8));
        lmBrowseButton->setText(QApplication::translate("QtSpeech", "Browse...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QtSpeech", "Dictionary", 0, QApplication::UnicodeUTF8));
        dictBrowseButton->setText(QApplication::translate("QtSpeech", "Browse...", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("QtSpeech", "Decoding", 0, QApplication::UnicodeUTF8));
        decodingButton->setText(QApplication::translate("QtSpeech", "Start", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtSpeech: public Ui_QtSpeech {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSPEECH_H
