#include <iostream>
#include "emp_window.hpp"
#include "extern_def.h"

TreeView_WithPopup::TreeView_WithPopup()
	: m_validate_retry(false)
{
	//Create the Tree model:
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	set_model(m_refTreeModel);

	auto row = *(m_refTreeModel->append());
	unsigned int rows_num = sql_con.db_rows_num();
	for(unsigned int i = 0; i < rows_num--; ++i)
	{
		if(i>0) row = *(m_refTreeModel->append());
		sql_con.change_employee(i);
		row[m_Columns.m_col_id] = std::stoi(sql_con.get_row(5));
		row[m_Columns.m_col_name] = sql_con.get_row(1);
		row[m_Columns.m_col_position] = sql_con.get_row(2);
		row[m_Columns.m_col_date] = sql_con.get_row(3);
	}

	//Add the TreeView's view columns:
	append_column_numeric_editable("db_id", m_Columns.m_col_id, "%d");
	append_column_editable("ФИО", m_Columns.m_col_name);
	append_column_editable("Должность", m_Columns.m_col_position);
	append_column_editable("Дата рождения", m_Columns.m_col_date);

	//Make the CellRenderer editable, and handle its editing signals:
	m_cellrenderer_validated.property_editable() = true;

	m_cellrenderer_validated.signal_editing_started().connect(
		sigc::mem_fun(*this,
			&TreeView_WithPopup::cellrenderer_validated_on_editing_started) );

	m_cellrenderer_validated.signal_edited().connect( sigc::mem_fun(*this,
		&TreeView_WithPopup::cellrenderer_validated_on_edited) );
}

TreeView_WithPopup::~TreeView_WithPopup()
{
}
/*
void TreeView_WithPopup::on_button_add()
{
	std::cout << "Add" << std::endl;
}

void TreeView_WithPopup::on_button_del()
{
	std::cout << "Delete" << std::endl;
}
*/
void TreeView_WithPopup::cellrenderer_validated_on_editing_started(
				Gtk::CellEditable* cell_editable, const Glib::ustring& /* path */)
{
	std::cout << cell_editable << std::endl;
	//Start editing with previously-entered (but invalid) text,
	//if we are allowing the user to correct some invalid data.
	if(m_validate_retry)
	{
		//This is the CellEditable inside the CellRenderer.
		auto celleditable_validated = cell_editable;

		//It's usually an Entry, at least for a CellRendererText:
		auto pEntry = dynamic_cast<Gtk::Entry*>(celleditable_validated);
		if(pEntry)
		{
			pEntry->set_text(m_invalid_text_for_retry);
			m_validate_retry = false;
			m_invalid_text_for_retry.clear();
		}
	}
}

void TreeView_WithPopup::cellrenderer_validated_on_edited(
		const Glib::ustring& path_string,
		const Glib::ustring& new_text)
{
	Gtk::TreePath path(path_string);
	std::cout << path_string << "\t" << new_text << std::endl;

	//Start editing again, with the bad text, so that the user can correct it.
	//A real application should probably allow the user to revert to the
	//previous text.

	//Set the text to be used in the start_editing signal handler:
	m_invalid_text_for_retry = new_text;
	m_validate_retry = true;

	//Start editing again, when the message dialog has been closed:
	set_cursor(path, m_treeviewcolumn_validated,
					m_cellrenderer_validated, true /* start_editing */);
}
