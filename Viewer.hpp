#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QOpenGLFunctions>
#include <QComboBox>

class Viewer : public QWidget
{
    Q_OBJECT                 //  Qt magic macro
    private:
        QDoubleSpinBox* d;    //  Spinbox for dim

        QDoubleSpinBox* gx; // Spinbox to change origin x position
        QDoubleSpinBox* gy; // Spinbox to change origin y position
        QDoubleSpinBox* gz; // Spinbox to change origin z position

        QComboBox* bones; // Dropdown to select a limb
    
    public:
        Viewer(QWidget* parent=0); //  Constructor

    private slots:
        // void setBone(const QString&);
};

#endif
