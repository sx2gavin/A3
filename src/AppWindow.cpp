#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
}

void AppWindow::setSceneNode(SceneNode* node) {
	m_viewer->setSceneNode(node);
}	

void AppWindow::createActions() {

	// Application menu
    QAction* reset_pos_act = new QAction(tr("&Reset Position"), this);
    m_menu_app_actions.push_back(reset_pos_act);
    reset_pos_act->setShortcut(Qt::Key_I);
    reset_pos_act->setStatusTip(tr("Reset position"));
    connect(reset_pos_act, SIGNAL(triggered()), this, SLOT(app_reset_position()));

    QAction* reset_orient_act = new QAction(tr("&Reset Orientation"), this);
    m_menu_app_actions.push_back(reset_orient_act);
    reset_orient_act->setShortcut(Qt::Key_O);
    reset_orient_act->setStatusTip(tr("Reset Orientation"));
    connect(reset_orient_act, SIGNAL(triggered()), this, SLOT(app_reset_orientation()));

    QAction* reset_joints_act = new QAction(tr("&Reset Joints"), this);
    m_menu_app_actions.push_back(reset_joints_act);
    reset_joints_act->setShortcut(Qt::Key_N);
    reset_joints_act->setStatusTip(tr("Reset Joints"));
    connect(reset_joints_act, SIGNAL(triggered()), this, SLOT(app_reset_joints()));

    QAction* reset_all_act = new QAction(tr("&Reset All"), this);
    m_menu_app_actions.push_back(reset_all_act);
    reset_all_act->setShortcut(Qt::Key_A);
    reset_all_act->setStatusTip(tr("Reset All"));
    connect(reset_all_act, SIGNAL(triggered()), this, SLOT(app_reset_all()));

	// Quit
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_app_actions.push_back(quitAct);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Exits the file"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	
	// Picking the body parts to rotate.
	QAction* head_act = new QAction(tr("head"), this);
    m_menu_picking_actions.push_back(head_act);
	// head_act->setShortcut(Qt::Key_S);
    head_act->setStatusTip(tr("Pick head"));
    connect(head_act, SIGNAL(triggered()), this, SLOT(picking_head_act()));

	QAction* neck_act = new QAction(tr("neck"), this);
    m_menu_picking_actions.push_back(neck_act);
	// neck_act->setShortcut(Qt::Key_S);
    neck_act->setStatusTip(tr("Pick neck"));
    connect(neck_act, SIGNAL(triggered()), this, SLOT(picking_neck_act()));

	QAction* upper_arm_right_act = new QAction(tr("right upper arm"), this);
    m_menu_picking_actions.push_back(upper_arm_right_act);
	// upper_arm_right_act->setShortcut(Qt::Key_S);
    upper_arm_right_act->setStatusTip(tr("Pick right upper arm"));
    connect(upper_arm_right_act, SIGNAL(triggered()), this, SLOT(picking_upper_arm_right_act()));
	
	QAction* forearm_right_act = new QAction(tr("right forearm"), this);
    m_menu_picking_actions.push_back(forearm_right_act);
	// forearm_right_act->setShortcut(Qt::Key_S);
    forearm_right_act->setStatusTip(tr("Pick forearm_right"));
    connect(forearm_right_act, SIGNAL(triggered()), this, SLOT(picking_forearm_right_act()));

	
	QAction* hand_right_act = new QAction(tr("right hand"), this);
    m_menu_picking_actions.push_back(hand_right_act);
	// hand_right_act->setShortcut(Qt::Key_S);
    hand_right_act->setStatusTip(tr("Pick hand_right"));
    connect(hand_right_act, SIGNAL(triggered()), this, SLOT(picking_hand_right_act()));

	QAction* thigh_right_act = new QAction(tr("right thigh"), this);
    m_menu_picking_actions.push_back(thigh_right_act);
	// thigh_right_act->setShortcut(Qt::Key_S);
    thigh_right_act->setStatusTip(tr("Pick thigh_right"));
    connect(thigh_right_act, SIGNAL(triggered()), this, SLOT(picking_thigh_right_act()));

	QAction* calf_right_act = new QAction(tr("right calf"), this);
    m_menu_picking_actions.push_back(calf_right_act);
	// calf_right_act->setShortcut(Qt::Key_S);
    calf_right_act->setStatusTip(tr("Pick calf_right"));
    connect(calf_right_act, SIGNAL(triggered()), this, SLOT(picking_calf_right_act()));

	QAction* foot_right_act = new QAction(tr("right foot"), this);
    m_menu_picking_actions.push_back(foot_right_act);
	// foot_right_act->setShortcut(Qt::Key_S);
    foot_right_act->setStatusTip(tr("Pick foot_right"));
    connect(foot_right_act, SIGNAL(triggered()), this, SLOT(picking_foot_right_act()));

	QAction* upper_arm_left_act = new QAction(tr("left upper arm"), this);
    m_menu_picking_actions.push_back(upper_arm_left_act);
	// upper_arm_left_act->setShortcut(Qt::Key_S);
    upper_arm_left_act->setStatusTip(tr("Pick left upper arm"));
    connect(upper_arm_left_act, SIGNAL(triggered()), this, SLOT(picking_upper_arm_left_act()));
	
	QAction* forearm_left_act = new QAction(tr("left forearm"), this);
    m_menu_picking_actions.push_back(forearm_left_act);
	// forearm_left_act->setShortcut(Qt::Key_S);
    forearm_left_act->setStatusTip(tr("Pick forearm_left"));
    connect(forearm_left_act, SIGNAL(triggered()), this, SLOT(picking_forearm_left_act()));

	
	QAction* hand_left_act = new QAction(tr("left hand"), this);
    m_menu_picking_actions.push_back(hand_left_act);
	// hand_left_act->setShortcut(Qt::Key_S);
    hand_left_act->setStatusTip(tr("Pick hand_left"));
    connect(hand_left_act, SIGNAL(triggered()), this, SLOT(picking_hand_left_act()));

	QAction* thigh_left_act = new QAction(tr("left thigh"), this);
    m_menu_picking_actions.push_back(thigh_left_act);
	// thigh_left_act->setShortcut(Qt::Key_S);
    thigh_left_act->setStatusTip(tr("Pick thigh_left"));
    connect(thigh_left_act, SIGNAL(triggered()), this, SLOT(picking_thigh_left_act()));

	QAction* calf_left_act = new QAction(tr("left calf"), this);
    m_menu_picking_actions.push_back(calf_left_act);
	// calf_left_act->setShortcut(Qt::Key_S);
    calf_left_act->setStatusTip(tr("Pick calf_left"));
    connect(calf_left_act, SIGNAL(triggered()), this, SLOT(picking_calf_left_act()));

	QAction* foot_left_act = new QAction(tr("left foot"), this);
    m_menu_picking_actions.push_back(foot_left_act);
	// foot_left_act->setShortcut(Qt::Key_S);
    foot_left_act->setStatusTip(tr("Pick foot_left"));
    connect(foot_left_act, SIGNAL(triggered()), this, SLOT(picking_foot_left_act()));


	// Modes 
	QAction* mode_pos_act = new QAction(tr("&Position/Orientation"), this);
    m_menu_mode_actions.push_back(mode_pos_act);
	mode_pos_act->setShortcut(Qt::Key_P);
    mode_pos_act->setStatusTip(tr("position/orientation"));
    connect(mode_pos_act, SIGNAL(triggered()), this, SLOT(mode_position_orientation()));

	QAction* mode_joint_act = new QAction(tr("&Joint"), this);
    m_menu_mode_actions.push_back(mode_joint_act);
	mode_joint_act->setShortcut(Qt::Key_J);
    mode_joint_act->setStatusTip(tr("Joint"));
    connect(mode_joint_act, SIGNAL(triggered()), this, SLOT(mode_joint()));

	// Option
	
	QAction* option_circle_act = new QAction(tr("&Circle"), this);
    m_menu_option_actions.push_back(option_circle_act);
	option_circle_act->setShortcut(Qt::Key_C);
    option_circle_act->setStatusTip(tr("Circle"));
    connect(option_circle_act, SIGNAL(triggered()), this, SLOT(option_circle()));

	QAction* option_z_buffer_act = new QAction(tr("&Z-buffer"), this);
    m_menu_option_actions.push_back(option_z_buffer_act);
	option_z_buffer_act->setShortcut(Qt::Key_Z);
    option_z_buffer_act->setStatusTip(tr("Z-buffer"));
    connect(option_z_buffer_act, SIGNAL(triggered()), this, SLOT(option_z_buffer()));

	QAction* option_backface_act = new QAction(tr("&Backface Cull"), this);
    m_menu_option_actions.push_back(option_backface_act);
	option_backface_act->setShortcut(Qt::Key_B);
    option_backface_act->setStatusTip(tr("Backface Cull"));
    connect(option_backface_act, SIGNAL(triggered()), this, SLOT(option_backface()));

	QAction* option_frontface_act = new QAction(tr("&Frontface Cull"), this);
    m_menu_option_actions.push_back(option_frontface_act);
	option_frontface_act->setShortcut(Qt::Key_F);
    option_frontface_act->setStatusTip(tr("Frontface cull"));
    connect(option_frontface_act, SIGNAL(triggered()), this, SLOT(option_frontface()));

}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
	m_menu_mode = menuBar()->addMenu(tr("Mode"));
	m_menu_option = menuBar()->addMenu(tr("Option"));
	m_menu_picking = menuBar()->addMenu(tr("&Picking"));

    for (auto& action : m_menu_app_actions) {
        m_menu_app->addAction(action);
    }

	for (auto& action : m_menu_picking_actions) {
		action->setCheckable(true);
		m_menu_picking->addAction(action);
	}

	for (auto& action : m_menu_mode_actions) {
		m_menu_mode->addAction(action);
	}
	
	for (auto& action : m_menu_option_actions) {
		action->setCheckable(true);
		m_menu_option->addAction(action);
	}
}

void AppWindow::picking_head_act()
{
	m_viewer->togglePickedName("n_h_joint");
}
void AppWindow::picking_neck_act()
{
	m_viewer->togglePickedName("s_n_joint");
}
void AppWindow::picking_upper_arm_right_act()
{
	m_viewer->togglePickedName("s_u_joint_r");
}
void AppWindow::picking_forearm_right_act()
{
	m_viewer->togglePickedName("u_f_joint_r");
}
void AppWindow::picking_hand_right_act()
{
	m_viewer->togglePickedName("f_h_joint_r");
}
void AppWindow::picking_thigh_right_act()
{
	m_viewer->togglePickedName("h_t_joint_r");
}
void AppWindow::picking_calf_right_act()
{
	m_viewer->togglePickedName("t_c_joint_r");
}
void AppWindow::picking_foot_right_act()
{
	m_viewer->togglePickedName("c_f_joint_r");
}
void AppWindow::picking_upper_arm_left_act()
{
	m_viewer->togglePickedName("s_u_joint_l");
}
void AppWindow::picking_forearm_left_act()
{
	m_viewer->togglePickedName("u_f_joint_l");
}
void AppWindow::picking_hand_left_act()
{
	m_viewer->togglePickedName("f_h_joint_l");
}
void AppWindow::picking_thigh_left_act()
{
	m_viewer->togglePickedName("h_t_joint_l");
}
void AppWindow::picking_calf_left_act()
{
	m_viewer->togglePickedName("t_c_joint_l");
}
void AppWindow::picking_foot_left_act()
{
	m_viewer->togglePickedName("c_f_joint_l");
}

void AppWindow::mode_position_orientation()
{
	m_viewer->setMode(POSITION_ORIENTATION);
}

void AppWindow::mode_joint()
{
	m_viewer->setMode(JOINTS);
}

void AppWindow::option_circle()
{
	m_viewer->toggleCircle();
}
void AppWindow::option_z_buffer()
{
	m_viewer->toggleZbuffer();
}
void AppWindow::option_backface()
{
	m_viewer->toggleBackfaceCull();
}
void AppWindow::option_frontface()
{
	m_viewer->toggleFrontfaceCull();
}


void AppWindow::app_reset_position()
{
	m_viewer->resetPosition();
}

void AppWindow::app_reset_orientation()
{
	m_viewer->resetOrientation();
}

void AppWindow::app_reset_joints()
{
	m_viewer->resetJoints();
}

void AppWindow::app_reset_all()
{
	m_viewer->resetAll();
}


