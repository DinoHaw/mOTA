/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *combSerialName;
    QLabel *label_2;
    QComboBox *combBaud;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOpenSerial;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLineEdit *leFilePath;
    QLineEdit *leFileName;
    QLabel *label_4;
    QPushButton *pbOpenFiles;
    QSpacerItem *verticalSpacer;
    QLabel *labFileSize;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_4;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbSendFiles;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(696, 352);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_5 = new QGridLayout(centralwidget);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_6 = new QGridLayout(groupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        combSerialName = new QComboBox(groupBox);
        combSerialName->setObjectName(QString::fromUtf8("combSerialName"));
        combSerialName->setMinimumSize(QSize(210, 0));

        gridLayout->addWidget(combSerialName, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        combBaud = new QComboBox(groupBox);
        combBaud->addItem(QString());
        combBaud->addItem(QString());
        combBaud->setObjectName(QString::fromUtf8("combBaud"));
        combBaud->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(combBaud, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        pbOpenSerial = new QPushButton(groupBox);
        pbOpenSerial->setObjectName(QString::fromUtf8("pbOpenSerial"));
        QFont font;
        font.setBold(true);
        pbOpenSerial->setFont(font);

        gridLayout->addWidget(pbOpenSerial, 0, 5, 1, 1);


        gridLayout_6->addLayout(gridLayout, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        leFilePath = new QLineEdit(groupBox_2);
        leFilePath->setObjectName(QString::fromUtf8("leFilePath"));

        gridLayout_2->addWidget(leFilePath, 0, 1, 1, 5);

        leFileName = new QLineEdit(groupBox_2);
        leFileName->setObjectName(QString::fromUtf8("leFileName"));
        leFileName->setMinimumSize(QSize(280, 0));
        leFileName->setFont(font);
        leFileName->setReadOnly(true);

        gridLayout_2->addWidget(leFileName, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        pbOpenFiles = new QPushButton(groupBox_2);
        pbOpenFiles->setObjectName(QString::fromUtf8("pbOpenFiles"));

        gridLayout_2->addWidget(pbOpenFiles, 2, 5, 1, 1);

        verticalSpacer = new QSpacerItem(648, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 6);

        labFileSize = new QLabel(groupBox_2);
        labFileSize->setObjectName(QString::fromUtf8("labFileSize"));

        gridLayout_2->addWidget(labFileSize, 2, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 4, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_7 = new QGridLayout(groupBox_3);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        progressBar = new QProgressBar(groupBox_3);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(500, 0));
        progressBar->setValue(0);
        progressBar->setTextVisible(true);
        progressBar->setTextDirection(QProgressBar::TopToBottom);

        gridLayout_4->addWidget(progressBar, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 1, 1, 1);

        pbSendFiles = new QPushButton(groupBox_3);
        pbSendFiles->setObjectName(QString::fromUtf8("pbSendFiles"));

        gridLayout_4->addWidget(pbSendFiles, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 696, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "1. \344\270\262\345\217\243\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243    \357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "   \346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        combBaud->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));
        combBaud->setItemText(1, QCoreApplication::translate("MainWindow", "9600", nullptr));

        pbOpenSerial->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "2. \351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\350\267\257\345\276\204    \357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\220\215\357\274\232", nullptr));
        pbOpenFiles->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        labFileSize->setText(QCoreApplication::translate("MainWindow", "xxx.xx kB", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "3. \345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        pbSendFiles->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
