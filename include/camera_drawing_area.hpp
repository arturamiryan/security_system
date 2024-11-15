#ifndef CVDRAWER_H
#define CVDRAWER_H

#include <gtkmm.h>
#include <opencv2/opencv.hpp>

class CvDrawer : public Gtk::DrawingArea{
public:

	enum DRAW_MODE{
		SCALE,
		CENTER,
		CENTER_FIT,
	};

	CvDrawer();

	void setCvMat(const cv::Mat& mat);

	// Widget interface
protected:
	void on_draw(const::Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
	void get_preferred_width_vfunc(int &minimum_width, int &natural_width) const;
	void get_preferred_height_for_width_vfunc(int width, int &minimum_height, int &natural_height) const;
	void get_preferred_height_vfunc(int &minimum_height, int &natural_height) const;
	void get_preferred_width_for_height_vfunc(int height, int &minimum_width, int &natural_width) const;

private:
	cv::Mat mat;
	DRAW_MODE drawMode;
};

#endif // CVDRAWER_H
