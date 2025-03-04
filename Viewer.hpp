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
#include <QGridLayout>
#include <QSizePolicy>

class Viewer : public QWidget
{
    Q_OBJECT                 //  Qt magic macro
    private:
        QDoubleSpinBox* d;    //  Spinbox for dim

        QDoubleSpinBox* gx; // Spinbox to change origin x position
        QDoubleSpinBox* gy; // Spinbox to change origin y position
        QDoubleSpinBox* gz; // Spinbox to change origin z position

        QPushButton* pelvis = new QPushButton(" ");
        QPushButton* lumbar  = new QPushButton(" ");
        QPushButton* torso = new QPushButton(" ");
        QPushButton* head = new QPushButton(" ");

        QPushButton* l_scapula = new QPushButton(" "); // shoulder blade
        QPushButton* l_humerus = new QPushButton(" "); // upper arm
        QPushButton* l_ulna = new QPushButton(" "); 
        QPushButton* l_radius = new QPushButton(" ");
        QPushButton* l_hand = new QPushButton(" ");

        QPushButton* r_scapula = new QPushButton(" "); // shoulder blade
        QPushButton* r_humerus = new QPushButton(" "); // upper arm
        QPushButton* r_ulna = new QPushButton(" "); 
        QPushButton* r_radius = new QPushButton(" ");
        QPushButton* r_hand = new QPushButton(" ");

        QPushButton* l_femur = new QPushButton(" "); // upper leg
        QPushButton* l_tibfib = new QPushButton(" "); // lower leg
        QPushButton* l_talus = new QPushButton(" "); // ball of foot
        QPushButton* l_foot = new QPushButton(" ");
        QPushButton* l_toes = new QPushButton(" ");

        QPushButton* r_femur = new QPushButton(" "); // upper leg
        QPushButton* r_tibfib = new QPushButton(" "); // lower leg
        QPushButton* r_talus = new QPushButton(" "); // ball of foot
        QPushButton* r_foot = new QPushButton(" ");
        QPushButton* r_toes = new QPushButton(" ");
    
    public:
        Viewer(QWidget* parent=0); //  Constructor

    private slots:
        // void setBone(const QString&);
};

#endif
