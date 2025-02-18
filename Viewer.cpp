//
//  Viewer Widget
//
#include "Viewer.hpp"
#include "Skeleton.hpp"

#include <QComboBox>
#include <QGroupBox>
#include <QLayout>

//
//  Constructor for Viewer widget
//
Viewer::Viewer(QWidget* parent)
    :  QWidget(parent)
{
   //  Set window title
   setWindowTitle(tr("Skeleton Model"));

   //  Create new Lorenz widget
   Skeleton * skeleton = new Skeleton;

   //  Create spinboxes
   d = new QDoubleSpinBox(); // dim
   gx  = new QDoubleSpinBox();
   gy  = new QDoubleSpinBox();
   gz  = new QDoubleSpinBox();

   //  Set range and values
   gx->setDecimals(1);  gx->setSingleStep(5.0);     gx->setRange(-50.0,50.0);        gx->setValue(0.0);
   gy->setDecimals(1);  gy->setSingleStep(5.0);     gy->setRange(-50.0,50.0);        gy->setValue(-30.0);
   gz->setDecimals(1);  gz->setSingleStep(5.0);     gz->setRange(-50.0,50.0);        gz->setValue(0.0);
   d->setDecimals(1); d->setSingleStep(1);      d->setRange(5,100);        d->setValue(60);

   //  Display widget for angles and dimension
   QLabel* angl = new QLabel(); // angles th,ph
   //  Pushbutton to reset view angle
   QPushButton* reset = new QPushButton("Reset");

   // Combo box to select bones
   QLabel* selected_bone = new QLabel();
   bones = new QComboBox();
   // add bones
   QStringList bones_list;
   bones_list   << "none"
                << "pelvis and tailbone"
                << "lumbar/ lower spine"
                << "torso and thorax/ mid-spine"
                << "head and neck/ upper spine"

                << "left scapula/ shoulder blade"
                << "left humerus/ upper arm"
                << "left ulna / lower arm"
                << "left radius/ lower arm"
                << "left hand"

                << "right scapula/ shoulder blade"
                << "right humerus/ upper arm"
                << "right ulna / lower arm"
                << "right radius/ lower arm"
                << "right hand"

                << "left femur/ upper leg"
                << "left tibia and fibula/ lower leg"
                << "left talus/ ball of foot"
                << "left foot"
                << "left toes"

                << "right femur/ upper leg"
                << "right tibia and fibula/ lower leg"
                << "right talus/ ball of foot"
                << "right foot"
                << "right toes";
    bones->addItems(bones_list);

   //  Connect valueChanged() signals to Skeleton slots
   connect(gx           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGX(double)));
   connect(gy           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGY(double)));
   connect(gz           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGZ(double)));
   connect(d          , SIGNAL(valueChanged(double)) , skeleton , SLOT(setDIM(double)));
   connect(reset        , SIGNAL(clicked(void))        , skeleton , SLOT(reset(void)));
   connect(bones, SIGNAL(currentIndexChanged(int)), skeleton, SLOT(setBone(int)));

   //  Connect skeleton signals to display widgets
   connect(skeleton , SIGNAL(setAngles(QString)) , angl , SLOT(setText(QString)));
   connect(skeleton , SIGNAL(setDim(double))   , d    , SLOT(setValue(double)));

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,400);
   layout->setRowStretch(4,100);

   //  Lorenz widget
   layout->addWidget(skeleton,0,0,5,1);

   //  Group Origin parameters
   QGroupBox* xyzbox = new QGroupBox("Origin");
   QGridLayout* xyzlay = new QGridLayout;
   xyzlay->addWidget(new QLabel("x"),0,0);  xyzlay->addWidget(gx,0,1);
   xyzlay->addWidget(new QLabel("y"),1,0);  xyzlay->addWidget(gy,1,1);
   xyzlay->addWidget(new QLabel("z"),2,0);  xyzlay->addWidget(gz,2,1);
   xyzbox->setLayout(xyzlay);
   layout->addWidget(xyzbox,0,1);

   //  Group Display parameters
   QGroupBox* dspbox = new QGroupBox("Display");
   QGridLayout* dsplay = new QGridLayout;
   dsplay->addWidget(new QLabel("dim"),0,0); dsplay->addWidget(d,0,1);
   dsplay->addWidget(angl,1,0);            dsplay->addWidget(reset,1,1);
   dspbox->setLayout(dsplay);
   layout->addWidget(dspbox,1,1);

   // Group Bones parameters
//    QGroupBox* bonebox = new QGroupBox("Bones");
//    QGridLayout* bonelay = new QGridLayout;
//    // bonelay->addWidget(new QLabel("bone"),0,0); bonelay->addWidget(selected_bone,0,1);
//    bonelay->addWidget(bones,0,0);
//    bonebox->setLayout(bonelay);
//    layout->addWidget(dspbox,2,1);

   layout->addWidget(bones,2,1);

   //  Overall layout
   setLayout(layout);
}
