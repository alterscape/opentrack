/* Copyright (c) 2012 Patrick Ruoff
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * 20130312, WVR: Add 7 lines to resizeGL after resize_frame. This should lower CPU-load.
 */

#include "video_widget.h"

#include <QDebug>
#include <QHBoxLayout>

using namespace cv;
using namespace std;
#ifndef OPENTRACK_API
using namespace boost;
#endif
#ifndef OPENTRACK_API
// ----------------------------------------------------------------------------
void VideoWidget::initializeGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void VideoWidget::resizeGL(int w, int h)
{
	// setup 1 to 1 projection
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
	resize_frame();
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(1,0);
	glVertex2f(1,1);
	glVertex2f(0,1);
	glEnd(); 
}

void VideoWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!resized_qframe.isNull())
	{
		glDrawPixels(resized_qframe.width(), resized_qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, resized_qframe.bits());
		
		const int crosshair_radius = 10;
		const int crosshair_thickness = 1;

		if (points)
		{
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(crosshair_thickness);
			int x,y;
			for (vector<Vec2f>::iterator iter = points->begin();
				 iter != points->end();
				 ++iter)
			{
				x = (*iter)[0] * resized_qframe.width() + resized_qframe.width()/2.0  + 0.5;
				y = (*iter)[1] * resized_qframe.width() + resized_qframe.height()/2.0 + 0.5;
			
				glBegin(GL_LINES);
				glVertex2i(x-crosshair_radius, y);
				glVertex2i(x+crosshair_radius, y);
				glEnd();
				glBegin(GL_LINES);
				glVertex2i(x, y-crosshair_radius);
				glVertex2i(x, y+crosshair_radius);
				glEnd();
			}
		}
	}
	glFlush();
}


void VideoWidget::resize_frame()
{
	if (!qframe.isNull())
		resized_qframe = qframe.scaled(this->size(), Qt::KeepAspectRatio);
}


void VideoWidget::update_frame_and_points()
{
	if (!get_frame_and_points(frame, points)) return;

	// convert to QImage
	if (frame.channels() == 3)
		qframe = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888).rgbSwapped();
	else if (frame.channels() == 1)
		qframe = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_Indexed8);
	qframe = QGLWidget::convertToGLFormat(qframe);

	resize_frame();
	updateGL();
}

#else
void VideoWidget2::update_image(const cv::Mat& frame)
{
    QMutexLocker foo(&mtx);
    QImage qframe = QImage(frame.cols, frame.rows, QImage::Format_RGB888);
    uchar* data = qframe.bits();
    const int pitch = qframe.bytesPerLine();
    for (int y = 0; y < frame.rows; y++)
        for (int x = 0; x < frame.cols; x++)
        {
            const int pos = 3 * (y*frame.cols + x);
            data[y * pitch + x * 3 + 0] = frame.data[pos + 2];
            data[y * pitch + x * 3 + 1] = frame.data[pos + 1];
            data[y * pitch + x * 3 + 2] = frame.data[pos + 0];
        }
    qframe = qframe.scaled(size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    pixmap = QPixmap::fromImage(qframe);
}
#endif

// ----------------------------------------------------------------------------
VideoWidgetDialog::VideoWidgetDialog(QWidget *parent, FrameProvider* provider)
    : QDialog(parent),
      video_widget(NULL)
{
    const int VIDEO_FRAME_WIDTH  = 640;
    const int VIDEO_FRAME_HEIGHT = 480;

#ifdef OPENTRACK_API
    video_widget = new VideoWidget2(this, provider);
#else
    video_widget = new VideoWidget(this, provider);
#endif

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(video_widget);
    if (this->layout()) delete this->layout();
    setLayout(layout);
    resize(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT);
}
