/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Fri Jun 8 17:06:23 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *glWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QDockWidget *dockSelectionMethod;
    QWidget *dockWidgetContents;
    QFormLayout *formLayout;
    QRadioButton *btnRadius;
    QDoubleSpinBox *spinRadius;
    QSpinBox *spinKNearest;
    QLabel *label;
    QSpinBox *spinVertexID;
    QRadioButton *btnKNearest;
    QDockWidget *dockRenderSettings;
    QWidget *dockWidgetContents_3;
    QFormLayout *formLayout_2;
    QCheckBox *btnRenderKDTree;
    QLabel *label_2;
    QDoubleSpinBox *spinPointSize;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(847, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        glWidget = new GLWidget(centralwidget);
        glWidget->setObjectName(QString::fromUtf8("glWidget"));

        horizontalLayout->addWidget(glWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 847, 29));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockSelectionMethod = new QDockWidget(MainWindow);
        dockSelectionMethod->setObjectName(QString::fromUtf8("dockSelectionMethod"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        formLayout = new QFormLayout(dockWidgetContents);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        btnRadius = new QRadioButton(dockWidgetContents);
        btnRadius->setObjectName(QString::fromUtf8("btnRadius"));

        formLayout->setWidget(0, QFormLayout::LabelRole, btnRadius);

        spinRadius = new QDoubleSpinBox(dockWidgetContents);
        spinRadius->setObjectName(QString::fromUtf8("spinRadius"));

        formLayout->setWidget(0, QFormLayout::FieldRole, spinRadius);

        spinKNearest = new QSpinBox(dockWidgetContents);
        spinKNearest->setObjectName(QString::fromUtf8("spinKNearest"));

        formLayout->setWidget(1, QFormLayout::FieldRole, spinKNearest);

        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        spinVertexID = new QSpinBox(dockWidgetContents);
        spinVertexID->setObjectName(QString::fromUtf8("spinVertexID"));

        formLayout->setWidget(3, QFormLayout::FieldRole, spinVertexID);

        btnKNearest = new QRadioButton(dockWidgetContents);
        btnKNearest->setObjectName(QString::fromUtf8("btnKNearest"));

        formLayout->setWidget(1, QFormLayout::LabelRole, btnKNearest);

        dockSelectionMethod->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockSelectionMethod);
        dockRenderSettings = new QDockWidget(MainWindow);
        dockRenderSettings->setObjectName(QString::fromUtf8("dockRenderSettings"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        formLayout_2 = new QFormLayout(dockWidgetContents_3);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        btnRenderKDTree = new QCheckBox(dockWidgetContents_3);
        btnRenderKDTree->setObjectName(QString::fromUtf8("btnRenderKDTree"));

        formLayout_2->setWidget(0, QFormLayout::SpanningRole, btnRenderKDTree);

        label_2 = new QLabel(dockWidgetContents_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        spinPointSize = new QDoubleSpinBox(dockWidgetContents_3);
        spinPointSize->setObjectName(QString::fromUtf8("spinPointSize"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, spinPointSize);

        dockRenderSettings->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockRenderSettings);

        retranslateUi(MainWindow);
        QObject::connect(btnRadius, SIGNAL(toggled(bool)), spinRadius, SLOT(setEnabled(bool)));
        QObject::connect(btnKNearest, SIGNAL(toggled(bool)), spinKNearest, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        dockSelectionMethod->setWindowTitle(QApplication::translate("MainWindow", "Selection Method", 0, QApplication::UnicodeUTF8));
        btnRadius->setText(QApplication::translate("MainWindow", "In Radius:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Vertex ID:", 0, QApplication::UnicodeUTF8));
        btnKNearest->setText(QApplication::translate("MainWindow", "k Nearest:", 0, QApplication::UnicodeUTF8));
        dockRenderSettings->setWindowTitle(QApplication::translate("MainWindow", "Render Settings", 0, QApplication::UnicodeUTF8));
        btnRenderKDTree->setText(QApplication::translate("MainWindow", "Render KD-Tree", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Point Size:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
