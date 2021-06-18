#ifndef GTKMM_EMPWINDOW_H
#define GTKMM_EMPWINDOW_H

#include <gtkmm.h>

class TreeView_WithPopup : public Gtk::TreeView
{
public:
	TreeView_WithPopup();
	virtual ~TreeView_WithPopup();
	//void on_button_add();
	//void on_button_del();

protected:
	void cellrenderer_validated_on_editing_started(Gtk::CellEditable* cell_editable, const Glib::ustring& path);
	void cellrenderer_validated_on_edited(const Glib::ustring& path_string, const Glib::ustring& new_text);

	//Tree model columns:
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ModelColumns()
		{ add(m_col_id); add(m_col_name); add(m_col_position); add(m_col_date); }

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_col_position;
		Gtk::TreeModelColumn<Glib::ustring> m_col_date;
	};

	ModelColumns m_Columns;

	//The Tree model:
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

	Gtk::CellRendererText m_cellrenderer_validated;
	Gtk::TreeView::Column m_treeviewcolumn_validated;
	bool m_validate_retry;
	Glib::ustring m_invalid_text_for_retry;
};

#endif //GTKMM_EMPWINDOW_H
