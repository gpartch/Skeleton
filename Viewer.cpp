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
   gy->setDecimals(1);  gy->setSingleStep(5.0);     gy->setRange(-50.0,50.0);        gy->setValue(0.0);
   gz->setDecimals(1);  gz->setSingleStep(5.0);     gz->setRange(-50.0,50.0);        gz->setValue(0.0);
   d->setDecimals(1); d->setSingleStep(1);      d->setRange(5,100);        d->setValue(60);

   //  Display widget for angles and dimension
   QLabel* angl = new QLabel(); // angles th,ph
   //  Pushbutton to reset view angle
   QPushButton* reset = new QPushButton("Reset");

   QPushButton* reset_bones = new QPushButton("Reset Bones");

   NUM_BONES = skeleton->getNumBones();
   for(int i = 0; i < NUM_BONES; i++)
   {
      bones.at(i)->setCheckable(true);
      bbtng->addButton(bones.at(i));
      bbtng->setId(bones.at(i), i);
   }
   bbtng->setExclusive(true);

   // connect viewer signals to skeleton
   connect(bbtng, SIGNAL(idClicked(int)), skeleton, SLOT(setSelectedBone(int)));

   //  Connect skeleton signals to display widgets
   connect(skeleton , SIGNAL(setAngles(QString)) , angl , SLOT(setText(QString)));
   connect(skeleton , SIGNAL(setDim(double))   , d    , SLOT(setValue(double)));
   connect(skeleton, SIGNAL(resetBoneSelectedBtn(int)), this, SLOT(resetBoneSelectedBtn(int)));
   
   connect(skeleton, SIGNAL(setWidgetGX(double)), gx, SLOT(setValue(double)));
   connect(skeleton, SIGNAL(setWidgetGY(double)), gy, SLOT(setValue(double)));
   connect(skeleton, SIGNAL(setWidgetGZ(double)), gz, SLOT(setValue(double)));

   connect(gx           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGX(double)));
   connect(gy           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGY(double)));
   connect(gz           , SIGNAL(valueChanged(double)) , skeleton , SLOT(setGZ(double)));
   connect(d            , SIGNAL(valueChanged(double)) , skeleton , SLOT(setDIM(double)));
   connect(reset        , SIGNAL(clicked(void))        , skeleton , SLOT(reset(void)));
   connect(reset_bones        , SIGNAL(clicked(void))        , skeleton , SLOT(resetBones(void)));

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->setColumnStretch(0,100);
   // layout->SetFixedSize()
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

   int width = 30;
   // set bone push button sizes
   pelvis->setFixedWidth(width*2);
   lumbar->setFixedWidth(width*2);
   torso->setFixedWidth(width*2);
   head->setFixedWidth(width*1.5);

   l_scapula->setFixedWidth(width);    l_scapula->setFixedHeight(width/2);
   l_humerus->setFixedWidth(width);
   l_ulna->setFixedWidth(width/2);
   l_radius->setFixedWidth(width/2);
   l_hand->setFixedWidth(width);       l_hand->setFixedHeight(width/2);

   r_scapula->setFixedWidth(width);    r_scapula->setFixedHeight(width/2);
   r_humerus->setFixedWidth(width);
   r_ulna->setFixedWidth(width/2);
   r_radius->setFixedWidth(width/2);
   r_hand->setFixedWidth(width);       r_hand->setFixedHeight(width/2);

   l_femur->setFixedWidth(width);      l_femur->setFixedHeight(width*1.5);
   l_tibfib->setFixedWidth(width);     l_tibfib->setFixedHeight(width*1.5);
   l_talus->setFixedWidth(width);      l_talus->setFixedHeight(width/2);
   l_foot->setFixedWidth(width);       l_foot->setFixedHeight(width/2);
   l_toes->setFixedWidth(width);       l_toes->setFixedHeight(width/2);

   r_femur->setFixedWidth(width);      r_femur->setFixedHeight(width*1.5);
   r_tibfib->setFixedWidth(width);     r_tibfib->setFixedHeight(width*1.5);
   r_talus->setFixedWidth(width);      r_talus->setFixedHeight(width/2);
   r_foot->setFixedWidth(width);       r_foot->setFixedHeight(width/2);
   r_toes->setFixedWidth(width);       r_toes->setFixedHeight(width/2);

   // Bones box 
   QGroupBox* bbox = new QGroupBox("Bones");
   QGridLayout* blay = new QGridLayout;

   blay->addWidget(r_scapula,1,0,1,2, Qt::AlignCenter | Qt::AlignBottom);  
   blay->addWidget(r_humerus,2,0,1,2, Qt::AlignCenter);                    
   blay->addWidget(r_radius,3,0, Qt::AlignCenter | Qt::AlignRight);        
   blay->addWidget(r_ulna,3,1, Qt::AlignCenter | Qt::AlignLeft);           
   blay->addWidget(r_hand,4,0,1,2, Qt::AlignCenter | Qt::AlignTop);        

   blay->addWidget(head,0,2,1,2, Qt::AlignCenter);                         
   blay->addWidget(torso,1,2,1,2, Qt::AlignCenter);                        
   blay->addWidget(lumbar,2,2,1,2, Qt::AlignCenter);                       
   blay->addWidget(pelvis,3,2,1,2, Qt::AlignCenter);                       

   blay->addWidget(r_femur,4,2, Qt::AlignCenter);                          
   blay->addWidget(r_tibfib,5,2, Qt::AlignCenter);                         
   blay->addWidget(r_talus,6,2, Qt::AlignCenter);                          
   blay->addWidget(r_foot,7,2, Qt::AlignCenter);                           
   blay->addWidget(r_toes,8,2, Qt::AlignCenter);                           

   blay->addWidget(l_femur,4,3, Qt::AlignCenter);                          
   blay->addWidget(l_tibfib,5,3, Qt::AlignCenter);                         
   blay->addWidget(l_talus,6,3, Qt::AlignCenter);                          
   blay->addWidget(l_foot,7,3, Qt::AlignCenter);                           
   blay->addWidget(l_toes,8,3, Qt::AlignCenter);                           

   blay->addWidget(l_scapula,1,4,1,2, Qt::AlignCenter | Qt::AlignBottom);  
   blay->addWidget(l_humerus,2,4,1,2, Qt::AlignCenter);                    
   blay->addWidget(l_ulna,3,4, Qt::AlignCenter | Qt::AlignRight);          
   blay->addWidget(l_radius,3,5, Qt::AlignCenter | Qt::AlignLeft);         
   blay->addWidget(l_hand,4,4,1,2, Qt::AlignCenter | Qt::AlignTop);     

   bbox->setLayout(blay);
   layout->addWidget(bbox,2,1);

   layout->addWidget(reset_bones,3,1);

   //  Overall layout
   setLayout(layout);
}

void Viewer::resetBoneSelectedBtn(int b)
{
   // have to disable exclusive, otherwise qt won't let you uncheck all buttons
   bbtng->setExclusive(false);
   for(int i = 0; i<NUM_BONES; i++)
   {
      if(i != b) 
      {
         bones.at(i)->setChecked(false);
      }
   }
   bbtng->setExclusive(true);
}
