#include "main_window.hpp"
#include "train.hpp"
#include "extern_def.h"
#include <iostream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
: m_Box(Gtk::Orientation::VERTICAL),
	g_Box(Gtk::Orientation::VERTICAL),
	t1_box(Gtk::Orientation::HORIZONTAL)
{
	set_title("Security system");
	set_default_size(900, 560);
	set_resizable(false);

	set_child(m_Box); //We can put a MenuBar at the top of the box and other stuff below it.

	//Define the actions:
	m_refActionGroup = Gio::SimpleActionGroup::create();

	m_refActionGroup->add_action("quit",
		sigc::mem_fun(*this, &MainWindow::on_action_file_quit) );

	m_refActionGroup->add_action("train",
		sigc::mem_fun(*this, &MainWindow::on_action_file_train) );

	insert_action_group("example", m_refActionGroup);

	//Define how the actions are presented in the menus and toolbars:
	m_refBuilder = Gtk::Builder::create();

	//Layout the actions in a menubar and toolbar:
	const char* ui_info =
    "<interface>"
    "  <menu id='menubar'>"
    "    <submenu>"
    "      <attribute name='label' translatable='yes'>_File</attribute>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_Train</attribute>"
    "          <attribute name='action'>example.train</attribute>"
    "          <attribute name='accel'>&lt;primary&gt;t</attribute>"
    "        </item>"
    "      </section>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_Quit</attribute>"
    "          <attribute name='action'>example.quit</attribute>"
    "          <attribute name='accel'>&lt;primary&gt;q</attribute>"
    "        </item>"
    "      </section>"
    "    </submenu>"
    "  </menu>"
    "</interface>";

	// When the menubar is a child of a Gtk::Window, keyboard accelerators are not
	// automatically fetched from the Gio::Menu.
	// See the examples/book/menus/main_menu example for an alternative way of
	// adding the menubar when using Gtk::ApplicationWindow.
	app->set_accel_for_action("example.quit", "<Primary>q");
	app->set_accel_for_action("example.train", "<Primary>t");

	try
	{
		m_refBuilder->add_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "Building menu failed: " << ex.what();
	}

	//Get the menubar:
	auto object = m_refBuilder->get_object("menubar");
	auto gmenu = std::dynamic_pointer_cast<Gio::Menu>(object);
	if (!gmenu)
		g_warning("GMenu not found");
	else
	{
		auto pMenuBar = Gtk::make_managed<Gtk::PopoverMenuBar>(gmenu);

		//Add the PopoverMenuBar to the window:
		m_Box.append(*pMenuBar);
	}

	cam.open(0);
	//listen for camera in thread
	camThread = std::thread([&]{
		cv::Mat m;
		camClose = false;
		while(cam.isOpened() && !camClose){
			if(cam.read(m)){
				camMutex.lock();
				camMatThread = m.clone();
				camMutex.unlock();
				signalCamFrame.emit();
			}
		}
	});

	//copy camera mat in thread
	signalCamFrame.connect([&]{
		camMutex.lock();
		camMat = camMatThread.clone();
		camMutex.unlock();

		capture_frame = camMat;
		camDrawer.setCvMat(camMat);
	});

	t1_box.append(text_area);
	t1_box.append(camDrawer);

	g_Notebook.set_expand();
	m_Box.append(g_Notebook);
/*
	TreeView_Box.append(m_TreeView);
	TreeView_Box.append(m_ButtonBox);

	m_Button_Add.set_valign(Gtk::Align::END);
	m_Button_Del.set_valign(Gtk::Align::END);
	m_ButtonBox.append(m_Button_Add);
	m_ButtonBox.append(m_Button_Del);
	m_Button_Add.signal_clicked().connect( sigc::mem_fun(m_TreeView, &TreeView_WithPopup::on_button_add) );

	m_Button_Del.signal_clicked().connect( sigc::mem_fun(m_TreeView, &TreeView_WithPopup::on_button_del) );
*/
	g_Notebook.append_page(t1_box, "Распознавание");
	g_Notebook.append_page(m_TreeView, "Сотрудники");

	//g_Notebook.signal_switch_page().connect(sigc::mem_fun(*this, &MainWindow::on_notebook_switch_page) );
}

MainWindow::~MainWindow()
{
	camClose = true;
	if(camThread.joinable())
		camThread.join();
	cam.release();
}

void MainWindow::on_action_file_quit()
{
	hide(); //Closes the main window to stop the app->make_window_and_run().
}

void MainWindow::on_action_file_train()
{
	train();
}

/*
void MainWindow::on_notebook_switch_page(Gtk::Widget*, guint page_num)
{
	std::cout << "Switched to tab with index " << page_num << std::endl;

	//You can also use m_Notebook.get_current_page() to get this index.
}
*/
