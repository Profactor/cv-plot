// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#include <catch.hpp>
#include <CvPlot/cvplot.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "DoublePendulum.h"

namespace {

cv::Mat lenna() {
	auto mat = cv::imread("exampledata/Lenna.jpg");
	CHECK(!mat.empty());
	return mat;
}

std::string testCaseName() {
    return Catch::getResultCapture().getCurrentTestName();
}

}

using namespace CvPlot;

TEST_CASE("live_image") {
	cv::Mat3b org = lenna() - 20;
	cv::Mat3b live(org.size());
	auto getLive = [&]() {
		cv::randu(live, 0, 40);
		live += org;
		return live;
	};

	Axes axes(plotImage(org));
	axes.setXLim({ 200,300 }).setYLim({ 200,300 });
	Window window(testCaseName(),axes,600,800);
	int count = 0;
	do {
		axes.title(std::to_string(count++));
		axes.find<Image>()->setMat(getLive());
		window.update();
    } while (window.waitKey(1) == -1 && window.valid());
}

TEST_CASE("multiple_Windows") {
	Axes axes = plot(std::vector<double>{ 3, 4, 4, 6 }, "-o");
	Window window(testCaseName() + " 1", axes, 400, 400);
	cv::moveWindow(testCaseName() + " 1", 300, 300);

	Axes axes2 = plot(std::vector<double>{ 5,4,3,1 }, "or");
	Window window2(testCaseName() + " 2", axes2, 400, 400);
	cv::moveWindow(testCaseName() + " 2", 750, 300);
	
    Window::waitKey({ &window,&window2 });
}

TEST_CASE("paint") {
	cv::Mat3b mat = lenna();
	Axes axes = makeImageAxes();
	Image &image = axes.create<Image>(mat);
	Window window(testCaseName(),axes);
	window.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		if (mouseEvent.flags() == cv::MouseEventFlags::EVENT_FLAG_LBUTTON) {
			cv::circle(mat, mouseEvent.pos(), 6, cv::Scalar::all(255), cv::FILLED, cv::LINE_AA);
			image.setMat(mat);
			return true; //request update
		}
		return false;
	});
	window.waitKey();
}

TEST_CASE("linked_viewports") {
	cv::Mat channels[3];
	cv::split(lenna(), channels);
	
	Axes axes1 = plotImage(channels[2]);
	Window window1(testCaseName() + " red", axes1, 400, 500);
	cv::moveWindow(testCaseName() + " red", 200, 300);

	Axes axes2 = plotImage(channels[0]);
	Window window2(testCaseName() + " blue", axes2, 400, 500);
	cv::moveWindow(testCaseName() + " blue", 750, 300);

	auto sync = [](const Axes &s, Axes &d) {
		if (d.getXLim() == s.getXLim() && d.getYLim() == s.getYLim()
			&& d.getXLimAuto() == s.getXLimAuto() && d.getYLimAuto() == s.getYLimAuto()) {
			return false;
		}
		d.setXLim(s.getXLim());
		d.setYLim(s.getYLim());
		d.setXLimAuto(s.getXLimAuto());
		d.setYLimAuto(s.getYLimAuto());
		return true;
	}; 
	window1.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		if (sync(axes1, axes2)) {
			window2.update();
		}
		return false; //no update required for window1
	});
	window2.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		if (sync(axes2, axes1)) {
			window1.update();
		}
		return false; //no update required for window2
	});

    Window::waitKey({ &window1,&window2 });
}

TEST_CASE("profile") {
	cv::Mat mat = lenna();
	cv::Mat channels[3];
	cv::split(mat, channels);

	Axes axesImage = plotImage(mat);
	HorizontalLine &horizontalLineImage = axesImage.create<HorizontalLine>(0, "-k");
	VerticalLine &verticalLineImage = axesImage.create<VerticalLine>(0, "-k");
	axesImage.setXLim({ 220, 380 }).setYLim({ 270, 270 });

	Axes axesProfile = makePlotAxes();;
	Series &blue = axesProfile.create<Series>("-b");
	Series &green = axesProfile.create<Series>("-g");
	Series &red = axesProfile.create<Series>("-r");
	VerticalLine &verticalLineProfile = axesProfile.create<VerticalLine>(0, "-k");
	axesProfile.setXLim(axesImage.getXLim());
	axesProfile.setXTight();
	axesProfile.setYLim({ 0, 255 });

	auto setCol = [&](int col) {
		verticalLineImage.setPos(col);
		verticalLineProfile.setPos(col);
	};

	auto setRow = [&](int row) {
		horizontalLineImage.setPos(row);
		if (row < 0 || row > channels[0].rows) {
			return;
		}
		blue.setY(channels[0].row(row));
		green.setY(channels[1].row(row));
		red.setY(channels[2].row(row));
	};

	setRow(265);
	setCol(265);

	Window windowImage(testCaseName() + " Image", axesImage, 300, 800);
	cv::moveWindow(testCaseName() + " Image", 300, 100);
	Window windowProfile(testCaseName() + " Profile", axesProfile, 300, 800);
	cv::moveWindow(testCaseName() + " Profile", 300, 450);

	windowImage.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		if (mouseEvent.flags() & cv::MouseEventFlags::EVENT_FLAG_LBUTTON) {
			setRow(int(std::round(mouseEvent.pos().y)));
		}
		setCol(int(std::round(mouseEvent.pos().x)));
		windowProfile.update();
		return true; //updates windowImage
	});

	windowProfile.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		setCol(int(std::round(mouseEvent.pos().x)));
		windowImage.update();
		return true; //updates windowProfile
	});

    Window::waitKey({ &windowImage,&windowProfile });
}

TEST_CASE("dragpoint") {
	struct MyDragPoint :public Drawable {
		cv::Point2d _point = { 1,1 };
		bool _dragging = false;
		cv::Vec2d _dragOffset;
		const double _size = 30;
		cv::Rect2d getRect(const Projection &projection) {
			auto p = projection.project(_point);
			cv::Point2d diag2(_size / 2, _size / 2);
			return cv::Rect2d(p - diag2, p + diag2);
		}
		void render(RenderTarget &renderTarget) override {
			cv::rectangle(renderTarget.innerMat(), getRect(renderTarget), cv::Scalar::all(0), cv::FILLED);
		}
		bool getBoundingRect(cv::Rect2d &rect) override {
			return false;
		}
		bool hitTest(const MouseEvent &mouseEvent) {
			return getRect(mouseEvent.projection()).contains(mouseEvent.innerPoint());
		}
		bool mouseEvent(const MouseEvent &mouseEvent) {
			if (mouseEvent.event() == cv::MouseEventTypes::EVENT_LBUTTONDOWN
				&& hitTest(mouseEvent)){
				_dragging = true;
				_dragOffset = mouseEvent.pos() - _point;
				return true;
			}
			if (_dragging && mouseEvent.event() == cv::MouseEventTypes::EVENT_MOUSEMOVE) {
				_point = cv::Vec2d(mouseEvent.pos()) - _dragOffset;
				return true;
			}
			if (_dragging && mouseEvent.event() == cv::MouseEventTypes::EVENT_LBUTTONUP) {
				_dragging = false;
				return true;
			}
			return false;
		};
	};
	Axes axes = makePlotAxes();
	axes.setXLim({ 0,2 });
	axes.setYLim({ 0,2 });
	auto &myDragPoint = axes.create<MyDragPoint>();

	int rows = 480;
	int cols = 640;
	Window window(testCaseName(), axes, rows, cols);
	
	window.setMouseEventHandler([&](const MouseEvent &mouseEvent) {
		return myDragPoint.mouseEvent(mouseEvent);
	});

	window.waitKey();
}

TEST_CASE("double_pendulum") {
	DoublePendulum doublePendulum;
	Axes axes = makePlotAxes();
	axes.setXLim({ -2,2 })
		.setYLim({ -2,2 })
		.setFixedAspectRatio();
	auto& trace = axes.create<Series>().setLineSpec("-g");
	auto& pendulum = axes.create<Series>().setLineSpec("-ok");
	Window window(testCaseName(), axes, 500, 500);
	do {
		pendulum.setPoints(std::vector<cv::Point2d>{ {}, doublePendulum.point1(), doublePendulum.point2() });
		trace.setPoints(doublePendulum.trace());
		window.update();
		doublePendulum.step();
	} while (window.waitKey(10) == -1);
}

