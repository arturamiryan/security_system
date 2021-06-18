#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include <gtkmm.h>
#include "camera_drawing_area.hpp"
#include "text_area.hpp"
#include "emp_window.hpp"
#include <thread>
#include <atomic>
#include <mutex>

class MainWindow : public Gtk::Window
{
public:
	MainWindow(const Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();

private:
	//Signal handlers:
	void on_action_file_train();
	void on_action_file_quit();
	//void on_notebook_switch_page(Gtk::Widget* page, guint page_num);

	//Child widgets:
	Gtk::Box m_Box;
	Gtk::Box g_Box;
	Gtk::Box t1_box;
	Gtk::Notebook g_Notebook;

	cv::Mat camMatThread;
	cv::Mat camMat;
	CvDrawer camDrawer;
	std::thread camThread;
	std::mutex camMutex;
	std::atomic_bool camClose;
	Glib::Dispatcher signalCamFrame;
	cv::VideoCapture cam;

	TextArea text_area;

	TreeView_WithPopup m_TreeView;
	//Gtk::Box TreeView_Box, m_ButtonBox;
	//Gtk::Button m_Button_Add, m_Button_Del;

	Glib::RefPtr<Gtk::Builder> m_refBuilder;
	Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
	Glib::RefPtr<Gio::SimpleAction> m_refActionRain;
};

#endif //GTKMM_MAINWINDOW_H
