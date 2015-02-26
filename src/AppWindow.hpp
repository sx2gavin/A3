#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include "scene.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();
	void setSceneNode(SceneNode* node);	

private slots :
	void picking_head_act();
	void picking_neck_act();
	void picking_upper_arm_right_act();
	void picking_forearm_right_act();
	void picking_hand_right_act();
	void picking_thigh_right_act();
	void picking_calf_right_act();
	void picking_foot_right_act();
	void picking_upper_arm_left_act();
	void picking_forearm_left_act();
	void picking_hand_left_act();
	void picking_thigh_left_act();
	void picking_calf_left_act();
	void picking_foot_left_act();

	void mode_position_orientation();
	void mode_joint();

	void option_circle();
	void option_z_buffer();
	void option_backface();
	void option_frontface();



private:
    void createActions();
    void createMenu();
	

    // Each menu itself
    QMenu* m_menu_app;
	QMenu* m_menu_mode;
	QMenu* m_menu_option;
	QMenu* m_menu_picking;

    std::vector<QAction*> m_menu_app_actions;
	std::vector<QAction*> m_menu_mode_actions;
	std::vector<QAction*> m_menu_option_actions;
	std::vector<QAction*> m_menu_picking_actions;
    Viewer* m_viewer;
};

#endif
