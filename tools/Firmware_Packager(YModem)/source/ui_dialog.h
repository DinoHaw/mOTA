/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout_10;
    QTabWidget *tabWidget;
    QWidget *tabFirmwarePackage;
    QGridLayout *gridLayout_9;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QPushButton *pbChooseFw;
    QLabel *label;
    QLineEdit *leOpenFwPath;
    QLineEdit *leOpenFwName;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QLineEdit *leSaveFwPath;
    QPushButton *pbSaveFw;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QLineEdit *leSaveFwName;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_6;
    QLabel *label_6;
    QComboBox *combEncrypt;
    QLabel *label_7;
    QLineEdit *leKey;
    QComboBox *combKeyFormat;
    QLabel *label_10;
    QLineEdit *leIV;
    QComboBox *combIvFormat;
    QLabel *label_14;
    QLineEdit *leUserString;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_5;
    QLabel *label_23;
    QComboBox *combPartName;
    QLabel *label_13;
    QSpinBox *sbVer3;
    QSpinBox *sbVer2;
    QSpinBox *sbVer4;
    QSpinBox *sbVer1;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_12;
    QLabel *labPkgResult;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbStartPkg;
    QWidget *tabPackageParser;
    QGridLayout *gridLayout_24;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_12;
    QLabel *label_5;
    QLineEdit *leOpenPkgPath;
    QPushButton *pbChoosePkg;
    QLineEdit *leOpenPkgName;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_8;
    QTableWidget *twFirmwareInfo;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_15;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_16;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setEnabled(true);
        Dialog->resize(695, 745);
        Dialog->setMinimumSize(QSize(0, 0));
        Dialog->setMaximumSize(QSize(999999, 999999));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog->setWindowIcon(icon);
        gridLayout_10 = new QGridLayout(Dialog);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        tabWidget->setFont(font);
        tabFirmwarePackage = new QWidget();
        tabFirmwarePackage->setObjectName(QString::fromUtf8("tabFirmwarePackage"));
        gridLayout_9 = new QGridLayout(tabFirmwarePackage);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        groupBox = new QGroupBox(tabFirmwarePackage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 112));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        groupBox->setFont(font1);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pbChooseFw = new QPushButton(groupBox);
        pbChooseFw->setObjectName(QString::fromUtf8("pbChooseFw"));
        pbChooseFw->setMinimumSize(QSize(150, 35));
        pbChooseFw->setFont(font1);

        gridLayout->addWidget(pbChooseFw, 1, 3, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font1);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        leOpenFwPath = new QLineEdit(groupBox);
        leOpenFwPath->setObjectName(QString::fromUtf8("leOpenFwPath"));
        leOpenFwPath->setMinimumSize(QSize(0, 30));
        leOpenFwPath->setFont(font1);

        gridLayout->addWidget(leOpenFwPath, 0, 1, 1, 4);

        leOpenFwName = new QLineEdit(groupBox);
        leOpenFwName->setObjectName(QString::fromUtf8("leOpenFwName"));
        leOpenFwName->setMinimumSize(QSize(0, 30));
        leOpenFwName->setFont(font);
        leOpenFwName->setReadOnly(true);

        gridLayout->addWidget(leOpenFwName, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(tabFirmwarePackage);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 112));
        groupBox_2->setFont(font1);
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        leSaveFwPath = new QLineEdit(groupBox_2);
        leSaveFwPath->setObjectName(QString::fromUtf8("leSaveFwPath"));
        leSaveFwPath->setMinimumSize(QSize(0, 30));
        leSaveFwPath->setFont(font1);

        gridLayout_4->addWidget(leSaveFwPath, 0, 1, 1, 4);

        pbSaveFw = new QPushButton(groupBox_2);
        pbSaveFw->setObjectName(QString::fromUtf8("pbSaveFw"));
        pbSaveFw->setMinimumSize(QSize(150, 35));
        pbSaveFw->setFont(font1);

        gridLayout_4->addWidget(pbSaveFw, 1, 3, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        gridLayout_4->addWidget(label_3, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        leSaveFwName = new QLineEdit(groupBox_2);
        leSaveFwName->setObjectName(QString::fromUtf8("leSaveFwName"));
        leSaveFwName->setMinimumSize(QSize(0, 30));
        leSaveFwName->setFont(font);
        leSaveFwName->setReadOnly(true);

        gridLayout_4->addWidget(leSaveFwName, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_4, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(tabFirmwarePackage);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 187));
        groupBox_3->setFont(font1);
        gridLayout_8 = new QGridLayout(groupBox_3);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(0, 0));
        label_6->setFont(font1);

        gridLayout_6->addWidget(label_6, 0, 0, 1, 1);

        combEncrypt = new QComboBox(groupBox_3);
        combEncrypt->addItem(QString());
        combEncrypt->addItem(QString());
        combEncrypt->setObjectName(QString::fromUtf8("combEncrypt"));
        combEncrypt->setMinimumSize(QSize(0, 30));

        gridLayout_6->addWidget(combEncrypt, 0, 1, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(0, 0));
        label_7->setMaximumSize(QSize(80, 16777215));
        label_7->setFont(font1);

        gridLayout_6->addWidget(label_7, 1, 0, 1, 1);

        leKey = new QLineEdit(groupBox_3);
        leKey->setObjectName(QString::fromUtf8("leKey"));
        leKey->setEnabled(false);
        leKey->setMinimumSize(QSize(0, 30));
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        leKey->setFont(font2);
        leKey->setMaxLength(32);
        leKey->setEchoMode(QLineEdit::Normal);

        gridLayout_6->addWidget(leKey, 1, 1, 1, 1);

        combKeyFormat = new QComboBox(groupBox_3);
        combKeyFormat->addItem(QString());
        combKeyFormat->addItem(QString());
        combKeyFormat->setObjectName(QString::fromUtf8("combKeyFormat"));
        combKeyFormat->setEnabled(false);
        combKeyFormat->setMinimumSize(QSize(0, 30));
        combKeyFormat->setMaximumSize(QSize(60, 16777215));

        gridLayout_6->addWidget(combKeyFormat, 1, 2, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(0, 0));
        label_10->setFont(font1);

        gridLayout_6->addWidget(label_10, 2, 0, 1, 1);

        leIV = new QLineEdit(groupBox_3);
        leIV->setObjectName(QString::fromUtf8("leIV"));
        leIV->setEnabled(false);
        leIV->setMinimumSize(QSize(0, 30));
        leIV->setFont(font2);
        leIV->setInputMethodHints(Qt::ImhNone);
        leIV->setMaxLength(16);
        leIV->setEchoMode(QLineEdit::Normal);

        gridLayout_6->addWidget(leIV, 2, 1, 1, 1);

        combIvFormat = new QComboBox(groupBox_3);
        combIvFormat->addItem(QString());
        combIvFormat->addItem(QString());
        combIvFormat->setObjectName(QString::fromUtf8("combIvFormat"));
        combIvFormat->setEnabled(false);
        combIvFormat->setMinimumSize(QSize(0, 30));
        combIvFormat->setMaximumSize(QSize(60, 16777215));

        gridLayout_6->addWidget(combIvFormat, 2, 2, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(0, 0));
        label_14->setMaximumSize(QSize(100, 16777215));
        label_14->setFont(font1);

        gridLayout_6->addWidget(label_14, 3, 0, 1, 1);

        leUserString = new QLineEdit(groupBox_3);
        leUserString->setObjectName(QString::fromUtf8("leUserString"));
        leUserString->setMinimumSize(QSize(0, 30));
        leUserString->setFont(font2);
        leUserString->setFocusPolicy(Qt::StrongFocus);
        leUserString->setMaxLength(16);

        gridLayout_6->addWidget(leUserString, 3, 1, 1, 1);


        gridLayout_8->addLayout(gridLayout_6, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_3, 2, 0, 1, 1);

        groupBox_5 = new QGroupBox(tabFirmwarePackage);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(0, 73));
        groupBox_5->setFont(font1);
        gridLayout_13 = new QGridLayout(groupBox_5);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_23 = new QLabel(groupBox_5);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setMaximumSize(QSize(100, 16777215));
        label_23->setFont(font1);
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_23, 0, 3, 1, 1);

        combPartName = new QComboBox(groupBox_5);
        combPartName->addItem(QString());
        combPartName->addItem(QString());
        combPartName->addItem(QString());
        combPartName->setObjectName(QString::fromUtf8("combPartName"));
        combPartName->setMinimumSize(QSize(85, 30));

        gridLayout_5->addWidget(combPartName, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(110, 0));
        label_13->setMaximumSize(QSize(130, 16777215));
        label_13->setFont(font1);

        gridLayout_5->addWidget(label_13, 0, 0, 1, 1);

        sbVer3 = new QSpinBox(groupBox_5);
        sbVer3->setObjectName(QString::fromUtf8("sbVer3"));
        sbVer3->setMinimumSize(QSize(51, 25));
        sbVer3->setMaximumSize(QSize(51, 25));
        sbVer3->setMaximum(255);
        sbVer3->setValue(0);

        gridLayout_5->addWidget(sbVer3, 0, 6, 1, 1);

        sbVer2 = new QSpinBox(groupBox_5);
        sbVer2->setObjectName(QString::fromUtf8("sbVer2"));
        sbVer2->setMinimumSize(QSize(51, 25));
        sbVer2->setMaximumSize(QSize(51, 25));
        sbVer2->setMaximum(255);
        sbVer2->setValue(0);

        gridLayout_5->addWidget(sbVer2, 0, 5, 1, 1);

        sbVer4 = new QSpinBox(groupBox_5);
        sbVer4->setObjectName(QString::fromUtf8("sbVer4"));
        sbVer4->setMinimumSize(QSize(51, 25));
        sbVer4->setMaximumSize(QSize(51, 25));
        sbVer4->setMaximum(255);
        sbVer4->setValue(0);

        gridLayout_5->addWidget(sbVer4, 0, 7, 1, 1);

        sbVer1 = new QSpinBox(groupBox_5);
        sbVer1->setObjectName(QString::fromUtf8("sbVer1"));
        sbVer1->setMinimumSize(QSize(51, 25));
        sbVer1->setMaximumSize(QSize(51, 25));
        sbVer1->setMaximum(255);
        sbVer1->setValue(1);

        gridLayout_5->addWidget(sbVer1, 0, 4, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 2, 1, 1);


        gridLayout_13->addLayout(gridLayout_5, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_5, 3, 0, 1, 1);

        groupBox_4 = new QGroupBox(tabFirmwarePackage);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(0, 0));
        groupBox_4->setFont(font1);
        gridLayout_7 = new QGridLayout(groupBox_4);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMaximumSize(QSize(100, 16777215));
        label_12->setFont(font1);

        horizontalLayout_2->addWidget(label_12);

        labPkgResult = new QLabel(groupBox_4);
        labPkgResult->setObjectName(QString::fromUtf8("labPkgResult"));
        labPkgResult->setMaximumSize(QSize(16777215, 200));
        QPalette palette;
        QBrush brush(QColor(0, 170, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        labPkgResult->setPalette(palette);
        QFont font3;
        font3.setPointSize(18);
        font3.setBold(true);
        labPkgResult->setFont(font3);

        horizontalLayout_2->addWidget(labPkgResult);

        horizontalSpacer_4 = new QSpacerItem(192, 29, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pbStartPkg = new QPushButton(groupBox_4);
        pbStartPkg->setObjectName(QString::fromUtf8("pbStartPkg"));
        pbStartPkg->setEnabled(false);
        pbStartPkg->setMinimumSize(QSize(170, 60));
        pbStartPkg->setFont(font);

        horizontalLayout_2->addWidget(pbStartPkg);


        gridLayout_7->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_4, 4, 0, 1, 1);

        tabWidget->addTab(tabFirmwarePackage, QString());
        tabPackageParser = new QWidget();
        tabPackageParser->setObjectName(QString::fromUtf8("tabPackageParser"));
        gridLayout_24 = new QGridLayout(tabPackageParser);
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        groupBox_6 = new QGroupBox(tabPackageParser);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setMinimumSize(QSize(0, 112));
        groupBox_6->setFont(font1);
        gridLayout_11 = new QGridLayout(groupBox_6);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font1);

        gridLayout_12->addWidget(label_5, 1, 0, 1, 1);

        leOpenPkgPath = new QLineEdit(groupBox_6);
        leOpenPkgPath->setObjectName(QString::fromUtf8("leOpenPkgPath"));
        leOpenPkgPath->setMinimumSize(QSize(0, 30));
        leOpenPkgPath->setFont(font1);

        gridLayout_12->addWidget(leOpenPkgPath, 0, 1, 1, 4);

        pbChoosePkg = new QPushButton(groupBox_6);
        pbChoosePkg->setObjectName(QString::fromUtf8("pbChoosePkg"));
        pbChoosePkg->setMinimumSize(QSize(150, 35));
        pbChoosePkg->setFont(font1);

        gridLayout_12->addWidget(pbChoosePkg, 1, 3, 1, 1);

        leOpenPkgName = new QLineEdit(groupBox_6);
        leOpenPkgName->setObjectName(QString::fromUtf8("leOpenPkgName"));
        leOpenPkgName->setMinimumSize(QSize(0, 30));
        leOpenPkgName->setFont(font);
        leOpenPkgName->setReadOnly(true);

        gridLayout_12->addWidget(leOpenPkgName, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        label_8 = new QLabel(groupBox_6);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font1);

        gridLayout_12->addWidget(label_8, 0, 0, 1, 1);


        gridLayout_11->addLayout(gridLayout_12, 0, 0, 1, 1);


        gridLayout_24->addWidget(groupBox_6, 0, 0, 1, 1);

        twFirmwareInfo = new QTableWidget(tabPackageParser);
        if (twFirmwareInfo->columnCount() < 2)
            twFirmwareInfo->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        twFirmwareInfo->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        twFirmwareInfo->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (twFirmwareInfo->rowCount() < 13)
            twFirmwareInfo->setRowCount(13);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(5, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(6, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(7, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(8, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(9, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(10, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(11, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        twFirmwareInfo->setVerticalHeaderItem(12, __qtablewidgetitem14);
        QFont font4;
        font4.setBold(true);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        __qtablewidgetitem15->setFont(font4);
        __qtablewidgetitem15->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(0, 0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(0, 1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        __qtablewidgetitem17->setFont(font4);
        __qtablewidgetitem17->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(1, 0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        __qtablewidgetitem18->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(1, 1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        __qtablewidgetitem19->setFont(font4);
        __qtablewidgetitem19->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(2, 0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setFont(font4);
        __qtablewidgetitem20->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(3, 0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        __qtablewidgetitem21->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(3, 1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        __qtablewidgetitem22->setFont(font4);
        __qtablewidgetitem22->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(4, 0, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        __qtablewidgetitem23->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(4, 1, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        __qtablewidgetitem24->setFont(font4);
        __qtablewidgetitem24->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(5, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        __qtablewidgetitem25->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(5, 1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        __qtablewidgetitem26->setFont(font4);
        __qtablewidgetitem26->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(6, 0, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        __qtablewidgetitem27->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(6, 1, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        __qtablewidgetitem28->setFont(font4);
        __qtablewidgetitem28->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(7, 0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        __qtablewidgetitem29->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(7, 1, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        __qtablewidgetitem30->setFont(font4);
        __qtablewidgetitem30->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(8, 0, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        __qtablewidgetitem31->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(8, 1, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        __qtablewidgetitem32->setFont(font4);
        twFirmwareInfo->setItem(9, 0, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        __qtablewidgetitem33->setFont(font4);
        __qtablewidgetitem33->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(10, 0, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        __qtablewidgetitem34->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(10, 1, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        __qtablewidgetitem35->setFont(font4);
        __qtablewidgetitem35->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(11, 0, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        __qtablewidgetitem36->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(11, 1, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        __qtablewidgetitem37->setFont(font4);
        __qtablewidgetitem37->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(12, 0, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        __qtablewidgetitem38->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        twFirmwareInfo->setItem(12, 1, __qtablewidgetitem38);
        twFirmwareInfo->setObjectName(QString::fromUtf8("twFirmwareInfo"));
        twFirmwareInfo->setFont(font1);
        twFirmwareInfo->setContextMenuPolicy(Qt::DefaultContextMenu);

        gridLayout_24->addWidget(twFirmwareInfo, 1, 0, 1, 1);

        tabWidget->addTab(tabPackageParser, QString());

        gridLayout_10->addWidget(tabWidget, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_15 = new QLabel(Dialog);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMaximumSize(QSize(350, 16777215));
        QPalette palette1;
        QBrush brush2(QColor(129, 129, 129, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_15->setPalette(palette1);
        QFont font5;
        font5.setPointSize(9);
        font5.setBold(false);
        label_15->setFont(font5);

        horizontalLayout_3->addWidget(label_15);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        label_16 = new QLabel(Dialog);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMaximumSize(QSize(200, 16777215));
        QPalette palette2;
        QBrush brush3(QColor(140, 140, 140, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_16->setPalette(palette2);
        label_16->setFont(font5);
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_16);


        gridLayout_10->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        retranslateUi(Dialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Dialog", "1\357\274\210\351\200\211\346\213\251\351\234\200\350\246\201\346\211\223\345\214\205\347\232\204\345\233\272\344\273\266\357\274\211", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\345\220\215\357\274\232", nullptr));
        pbChooseFw->setText(QCoreApplication::translate("Dialog", "1.\351\200\211\346\213\251\345\233\272\344\273\266", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "\350\267\257\345\276\204\357\274\232", nullptr));
        leOpenFwPath->setText(QString());
        leOpenFwName->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("Dialog", "2\357\274\210\346\211\223\345\214\205\345\220\216\347\232\204\345\233\272\344\273\266\344\277\235\345\255\230\350\267\257\345\276\204\357\274\211", nullptr));
        leSaveFwPath->setText(QString());
        pbSaveFw->setText(QCoreApplication::translate("Dialog", "2.\344\277\235\345\255\230\350\267\257\345\276\204", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\345\220\215\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "\350\267\257\345\276\204\357\274\232", nullptr));
        leSaveFwName->setText(QString());
        groupBox_3->setTitle(QCoreApplication::translate("Dialog", "3\357\274\210\345\217\257\351\200\211\357\274\211", nullptr));
        label_6->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206\347\256\227\346\263\225\357\274\232", nullptr));
        combEncrypt->setItemText(0, QCoreApplication::translate("Dialog", "\344\270\215\345\212\240\345\257\206", nullptr));
        combEncrypt->setItemText(1, QCoreApplication::translate("Dialog", "AES256", nullptr));

        label_7->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206\345\257\206\351\222\245\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        leKey->setToolTip(QCoreApplication::translate("Dialog", "\350\276\223\345\205\24532\344\275\215\347\232\204\345\257\206\351\222\245", nullptr));
#endif // QT_CONFIG(tooltip)
        leKey->setText(QCoreApplication::translate("Dialog", "0123456789ABCDEF0123456789ABCDEF", nullptr));
        combKeyFormat->setItemText(0, QCoreApplication::translate("Dialog", "ASCII", nullptr));
        combKeyFormat->setItemText(1, QCoreApplication::translate("Dialog", "HEX", nullptr));

#if QT_CONFIG(tooltip)
        combKeyFormat->setToolTip(QCoreApplication::translate("Dialog", "\346\232\202\344\270\215\345\256\236\347\216\260", nullptr));
#endif // QT_CONFIG(tooltip)
        label_10->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206   IV\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        leIV->setToolTip(QCoreApplication::translate("Dialog", "\350\276\223\345\205\24516\344\275\215\347\232\204IV", nullptr));
#endif // QT_CONFIG(tooltip)
        leIV->setInputMask(QString());
        leIV->setText(QCoreApplication::translate("Dialog", "0123456789ABCDEF", nullptr));
        combIvFormat->setItemText(0, QCoreApplication::translate("Dialog", "ASCII", nullptr));
        combIvFormat->setItemText(1, QCoreApplication::translate("Dialog", "HEX", nullptr));

#if QT_CONFIG(tooltip)
        combIvFormat->setToolTip(QCoreApplication::translate("Dialog", "\346\232\202\344\270\215\345\256\236\347\216\260", nullptr));
#endif // QT_CONFIG(tooltip)
        label_14->setText(QCoreApplication::translate("Dialog", "\345\255\227\347\254\246\346\260\264\345\215\260\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        leUserString->setToolTip(QCoreApplication::translate("Dialog", "\346\234\200\351\225\277\345\217\257\350\276\223\345\205\24516\344\270\252\345\255\227\347\254\246\357\274\214\346\232\202\344\270\215\346\224\257\346\214\201\344\270\255\346\226\207", nullptr));
#endif // QT_CONFIG(tooltip)
        leUserString->setText(QCoreApplication::translate("Dialog", "ChinaDino", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("Dialog", "4", nullptr));
        label_23->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        combPartName->setItemText(0, QCoreApplication::translate("Dialog", "app", nullptr));
        combPartName->setItemText(1, QCoreApplication::translate("Dialog", "download", nullptr));
        combPartName->setItemText(2, QCoreApplication::translate("Dialog", "factory", nullptr));

        label_13->setText(QCoreApplication::translate("Dialog", "\345\255\230\346\224\276\345\233\272\344\273\266\347\232\204\345\210\206\345\214\272\345\220\215\357\274\232", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Dialog", "5", nullptr));
        label_12->setText(QCoreApplication::translate("Dialog", "\346\211\223\345\214\205\347\273\223\346\236\234\357\274\232", nullptr));
        labPkgResult->setText(QCoreApplication::translate("Dialog", "\346\211\223\345\214\205\346\210\220\345\212\237", nullptr));
        pbStartPkg->setText(QCoreApplication::translate("Dialog", "\345\274\200\345\247\213\346\211\223\345\214\205", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabFirmwarePackage), QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\346\211\223\345\214\205", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("Dialog", "\351\200\211\346\213\251\351\234\200\350\247\243\346\236\220\347\232\204fpk\345\233\272\344\273\266\345\214\205", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\345\220\215\357\274\232", nullptr));
        leOpenPkgPath->setText(QString());
        pbChoosePkg->setText(QCoreApplication::translate("Dialog", "\351\200\211\346\213\251\345\233\272\344\273\266\345\214\205", nullptr));
        leOpenPkgName->setText(QString());
        label_8->setText(QCoreApplication::translate("Dialog", "\350\267\257\345\276\204\357\274\232", nullptr));
        QTableWidgetItem *___qtablewidgetitem = twFirmwareInfo->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Dialog", "\345\217\202\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = twFirmwareInfo->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Dialog", "\345\200\274", nullptr));

        const bool __sortingEnabled = twFirmwareInfo->isSortingEnabled();
        twFirmwareInfo->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem2 = twFirmwareInfo->item(0, 0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\347\211\210\346\234\254\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = twFirmwareInfo->item(1, 0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Dialog", "\346\227\247\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = twFirmwareInfo->item(2, 0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Dialog", "\346\226\260\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = twFirmwareInfo->item(3, 0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Dialog", "\345\255\227\347\254\246\346\260\264\345\215\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = twFirmwareInfo->item(4, 0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\345\255\230\346\224\276\347\232\204\345\210\206\345\214\272\345\220\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = twFirmwareInfo->item(5, 0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206\347\256\227\346\263\225", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = twFirmwareInfo->item(6, 0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("Dialog", "\346\272\220\345\233\272\344\273\266\345\244\247\345\260\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = twFirmwareInfo->item(7, 0);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("Dialog", "\346\211\223\345\214\205\345\220\216\345\233\272\344\273\266\345\244\247\345\260\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = twFirmwareInfo->item(8, 0);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("Dialog", "\346\226\207\344\273\266\345\244\247\345\260\217\357\274\210\345\220\253\350\241\250\345\244\264\357\274\211", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = twFirmwareInfo->item(9, 0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\346\211\223\345\214\205\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = twFirmwareInfo->item(10, 0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("Dialog", "\346\272\220\345\233\272\344\273\266\347\232\204CRC32", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = twFirmwareInfo->item(11, 0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("Dialog", "\346\211\223\345\214\205\345\220\216\347\232\204CRC32", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = twFirmwareInfo->item(12, 0);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("Dialog", "\350\241\250\345\244\264\347\232\204CRC32", nullptr));
        twFirmwareInfo->setSortingEnabled(__sortingEnabled);

        tabWidget->setTabText(tabWidget->indexOf(tabPackageParser), QCoreApplication::translate("Dialog", "\345\233\272\344\273\266\345\214\205\350\247\243\346\236\220", nullptr));
        label_15->setText(QCoreApplication::translate("Dialog", "Design by Dino 2022", nullptr));
        label_16->setText(QCoreApplication::translate("Dialog", "V1.0.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
