#include "ckukarobot.h"

CKUKARobot::CKUKARobot( VariableDB* databaseVar, int updateInterval, QWidget *parent ):QDockWidget(parent){
    m_databaseVar = databaseVar;
    m_scene = new CGLScene();

    this->setAllowedAreas(Qt::AllDockWidgetAreas);

    mainContainer = new QWidget( this );
    QVBoxLayout* ml = new QVBoxLayout( mainContainer );
    mainContainer->setLayout( ml );
    this->setWidget( mainContainer );

    /*cl->addWidget( &m_ax1pos );
    cl->addWidget( &m_ax2pos );
    cl->addWidget( &m_ax3pos );
    cl->addWidget( &m_ax4pos );
    cl->addWidget( &m_ax5pos );
    connect( &m_ax1pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax2pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax3pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax4pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax5pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );*/

    //m_controlsContainer->layout()->setSizeConstraint(QLayout::SetFixedSize);

    m_controlsContainer = new QWidget( );
    m_controlsContainer->setFixedHeight( 60 );
    QHBoxLayout* cl = new QHBoxLayout( m_controlsContainer );
    m_controlsContainer->setLayout( cl );
    initSceneInteractionControls();
    mainContainer->layout()->addWidget( m_controlsContainer );

    mainContainer->layout()->addWidget( m_scene );


    connect( &m_pulsar, SIGNAL(timeout()), this, SLOT(updatePulsar()));
    m_pulsar.start( updateInterval );
}

CKUKARobot::~CKUKARobot(){
    delete m_scene;
}

void CKUKARobot::initSceneInteractionControls(){
    //this->addToolBar( &m_toolBar );

    QActionGroup* actionGroup = new QActionGroup( m_controlsContainer );
    actionGroup->setExclusive( true );

    //QMenuBar* mnuBar = new QMenuBar( m_controlsContainer );
    QToolBar* mnuBar = new QToolBar( m_controlsContainer );
    m_controlsContainer->layout()->addWidget( mnuBar );

    m_traslateScene = new QAction( QIcon(":traslate"),"Traslate scene", mnuBar );
    mnuBar->addAction( m_traslateScene );
    m_traslateScene->setCheckable(true);
    m_traslateScene->setActionGroup( actionGroup );
    connect( m_traslateScene, SIGNAL(toggled(bool)), this, SLOT(setTraslateSceneMode(bool)) );

    m_rotateScene = new QAction( QIcon(":rotate"),"Rotate scene", mnuBar );
    mnuBar->addAction( m_rotateScene );
    m_rotateScene->setCheckable(true);
    m_rotateScene->setActionGroup( actionGroup );
    connect( m_rotateScene, SIGNAL(toggled(bool)), this, SLOT(setRotateSceneMode(bool)) );

    /*m_traslateObject = new QAction( QIcon("./icons/traslateObj.png"),"Traslate object", this );
    m_toolBar.addAction( m_traslateObject );
    m_traslateObject->setCheckable(true);
    m_traslateObject->setActionGroup( actionGroup );
    connect( m_traslateObject, SIGNAL(toggled(bool)), this, SLOT(setTraslateObjMode(bool)) );

    m_rotateObject = new QAction( QIcon("./icons/rotateObj.png"),"Rotate object", this );
    m_toolBar.addAction( m_rotateObject );
    m_rotateObject->setCheckable(true);
    m_rotateObject->setActionGroup( actionGroup );
    connect( m_rotateObject, SIGNAL(toggled(bool)), this, SLOT(setRotateObjMode(bool)) );*/
}

void CKUKARobot::setTraslateObjMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_TRASLATE_OBJ );
}

void CKUKARobot::setRotateObjMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_ROTATE_OBJ );
}

void CKUKARobot::setTraslateSceneMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_TRASLATE_SCENE );
}

void CKUKARobot::setRotateSceneMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_ROTATE_SCENE );
}

/*void CKUKARobot::inputRobotChanged( QString s ){
    int r1 = m_ax1pos.text().toInt();
    int r2 = m_ax2pos.text().toInt();
    int r3 = m_ax3pos.text().toInt();
    int r4 = m_ax4pos.text().toInt();
    int r5 = m_ax5pos.text().toInt();
    setRobotPosition( r1, r2, r3, r4, r5 );
}*/

void CKUKARobot::setRobotPosition( int r1, int r2, int r3, int r4, int r5 ){
    m_scene->getRobot()->getRobotAxe(0)->setRotationRZ( -r1 );
    m_scene->getRobot()->getRobotAxe(1)->setRotationRX( r2 );
    m_scene->getRobot()->getRobotAxe(2)->setRotationRX( r3 );
    m_scene->getRobot()->getRobotAxe(3)->setRotationRZ( r4 );
    m_scene->getRobot()->getRobotAxe(4)->setRotationRX( r5 );
    m_scene->repaint();
}

void CKUKARobot::updatePulsar(){
    if( this->isVisible() ){
        //m_scene->setFixedSize( this->width()-20 , this->height() - 110 );
        m_scene->setMinimumSize( this->width()-20 , this->height() - 110 );

        int readTime = 0;
        QByteArray value;
        if( m_databaseVar->readVar( "$AXIS_ACT", QHostAddress("10.0.0.108"), &value, &readTime  ) ){
            int i = 0;
            int t = KukaVar::INT;
            KukaVar var( "$AXIS_ACT", ""+ value );
            //qDebug() << "__________________________-------------------------" << var.getStructureValue(0, t) << var.getStructureValue(1, t) << value;
            if( var.getVarType() == KukaVar::STRUCTURE ){
                if( var.getElementsNumber() > 4 ){
                    int r1 = var.getStructureValue(0, t).toFloat();
                    int r2 = var.getStructureValue(1, t).toFloat();
                    int r3 = var.getStructureValue(2, t).toFloat();
                    int r4 = var.getStructureValue(3, t).toFloat();
                    int r5 = var.getStructureValue(4, t).toFloat();
                    this->setRobotPosition( r1, r2, r3, r4, r5 );
                }
            }
        }else{
            int r1 = 0;
            int r2 = -90;
            int r3 = 90;
            int r4 = 0;
            int r5 = 0;
            this->setRobotPosition( r1, r2, r3, r4, r5 );
        }
    }
}
