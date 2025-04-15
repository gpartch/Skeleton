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
#include <QDebug>

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

        QButtonGroup* bbtng = new QButtonGroup;
        QList<QPushButton*> bones = {pelvis, lumbar, torso, head, l_scapula, l_humerus, l_ulna, l_radius, l_hand, r_scapula, r_humerus, r_ulna, r_radius, r_hand, l_femur, l_tibfib, l_talus, l_foot, l_toes, r_femur, r_tibfib, r_talus, r_foot, r_toes};

        QLabel* selected_bone_label = new QLabel("Bone: ");
        QLabel* selected_bone = new QLabel("--");

        QLabel* bone_angle_label = new QLabel("Angle: ");
        QLabel* bone_angle = new QLabel("--");

        bool bone_selected = -1;
        int NUM_BONES;
    
    public:
        Viewer(QWidget* parent=0); //  Constructor

    private slots:
        void resetBoneSelectedBtn(int b);
        void updateBoneInfo(QString selected_bone, QString bone_angles);
};

#endif
