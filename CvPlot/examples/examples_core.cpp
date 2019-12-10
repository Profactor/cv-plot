// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#include <catch.hpp>
#include <CvPlot/core.h>
#include <opencv2/opencv.hpp>
#include <CvPlot/gui/show.h>
#include "mandelbrot.h"
#include <chrono>

namespace {

cv::Mat lenna() {
	auto mat = cv::imread("exampledata/lenna.jpg");
	CHECK(!mat.empty());
	return mat;
}

cv::Mat bigLenna(int rows, int cols) {
	cv::Mat mat;
	cv::resize(lenna(), mat, cv::Size(cols, rows));
	cv::Mat3b noise(mat.size());
	cv::randu(noise, 0, 40);
	mat = mat - 20 + noise;
	return mat;
}

std::string testCaseName() {
    return Catch::getResultCapture().getCurrentTestName();
}

}

using namespace CvPlot;

TEST_CASE("version") {
	std::cout << version() << std::endl;
}

TEST_CASE("one_minute") {
	//render to cv::Mat
	Axes axes = plot(std::vector<double>{ 3, 3, 4, 6, 4, 3 }, "-o");
	cv::Mat mat = axes.render(400, 600);
	cv::imshow(testCaseName(), mat);
	cv::waitKey();
	cv::destroyAllWindows();
}

TEST_CASE("two_minutes") {
	//add Series to Axes and show in interactive window
	std::vector<double> x(20*1000), y1(x.size()), y2(x.size()), y3(x.size());
	for (size_t i = 0; i < x.size(); i++) {
		x[i] = i * CV_2PI / x.size();
		y1[i] = std::sin(x[i]);
		y2[i] = y1[i] * std::sin(x[i]*50);
		y3[i] = y2[i] * std::sin(x[i]*500);
	}
	Axes axes = makePlotAxes();
	axes.create<Series>(x, y3, "-g");
	axes.create<Series>(x, y2, "-b");
	axes.create<Series>(x, y1, "-r");
	axes.title("Right Button: Zoom, Middle Button: Pan, Right DoubleClick: Home");
	show(testCaseName(), axes);
}

TEST_CASE("show") {
	//plot directly to interactive window
	showPlot(std::vector<double>{ 3, 3, 4, 6, 4, 3 });
	showImage(lenna());
}

TEST_CASE("Data") {
	//supported inputs for data argument in Series(), plot(), showPlot()
	Axes axes = makePlotAxes();

	//single y value
	axes.create<Series>(0, "ok"); 
	
	//y values only
	axes.create<Series>(std::vector<double>{ 0.5, 1.5, 2.5 }, "-or");
	axes.create<Series>(cv::Mat((cv::Mat1b(1, 3) << 1, 2, 3)), "-og");
	axes.create<Series>(cv::Mat((cv::Mat1d(3, 1) << 1.5, 2.5, 3.5)), "-ob");
	
	//points
	axes.create<Series>(cv::Mat((cv::Mat1i(2, 3) << 0, 1, 2, 2, 3, 4)), "-oc");
	axes.create<Series>(cv::Mat((cv::Mat2f(1, 3) << cv::Vec2d(0, 2.5), cv::Vec2d(1, 3.5), cv::Vec2d(2, 4.5))), "-om");
	axes.create<Series>(std::vector<cv::Point>{ { 0, 3 }, { 1,4 }, { 2,5 } }, "-oy");
	axes.create<Series>(std::vector<cv::Point2d>{ { 0, 3.5 }, { 1,4.5 }, { 2,5.5 } }, "-ok");
	axes.create<Series>(std::vector<cv::Vec2f>{ { 0,4 }, { 1,5 }, { 2,6 } }, "-r");
	show(testCaseName(), axes);
}

TEST_CASE("logarithmic") {
	std::vector<double> values(50);
	for (size_t i = 0; i < values.size(); i++) {
		values[i] = std::pow(10., i / 10.);
	}
	
	cv::Mat3b mat(600, 1000);

	Axes axes = plot(values, values, "-mo");
	axes.title("linear");
	axes.setMargins(70, 10, 30, 25);
	axes.render(mat(cv::Rect(0, 0, 500, 300)));

	axes.setYLog();
	axes.title("lin-log");
	axes.render(mat(cv::Rect(500, 0, 500, 300)));

	axes.setXLog();
	axes.setYLog(false);
	axes.title("log-lin");
	axes.render(mat(cv::Rect(0, 300, 500, 300)));

	axes.setYLog();
	axes.title("log-log");
	axes.render(mat(cv::Rect(500, 300, 500, 300)));
	cv::imshow(testCaseName(), mat);
	cv::waitKey();
	cv::destroyAllWindows();
}

TEST_CASE("1e6_points") {
	const size_t n = 1000*1000;
	std::vector<double> x(n), y(n);
	cv::randu(x, 0, 1);
	cv::randu(y, 0, 1);
	Axes axes = plot(x, y, ".");
	show(testCaseName(), axes);
}

TEST_CASE("mixed") {
	Axes axes = plot(std::vector<double>{ 3, 4, 4}, "-b");
	axes.create<Image>(lenna(), cv::Rect2d(2, 2, 1, 1));
	axes.create<Image>(lenna(), cv::Rect2d(2, 2, 1, -1));
	axes.create<Image>(lenna(), cv::Rect2d(2, 2, -1, -1));
	axes.create<Image>(lenna(), cv::Rect2d(2, 2, -1, 1));
	axes.create<Series>(std::vector<double>{ 2,2,3}, std::vector<double>{ 2,3,3 })
		.setLineType(LineType::None)
		.setMarkerType(MarkerType::Circle)
        .setMarkerSize(30)
		.setColor(cv::Scalar(0, 0, 255));
    axes.create<Series>(std::vector<double>{ 1, 2, 2, 1 }, std::vector<double>{ -1, -1, 1, 1})
        .setColor(cv::Scalar::all(127))
        .setLineWidth(5)
        .setMarkerType(MarkerType::Circle)
        .setMarkerSize(5);
	axes.create<VerticalLine>(0, "-m");
	axes.create<HorizontalLine>()
		.setColor(cv::Scalar(255, 0, 255))
		.setPos(0)
		.setLineType(LineType::Solid);
	axes.xLabel("x-label");
	axes.yLabel("y-label");
	axes.title("Mixed Example");
	show(testCaseName(), axes);
}

TEST_CASE("100MB_image") {
	Axes axes = plotImage(bigLenna(6 * 1000, 6 * 1000));
	axes.find<Image>()->setInterpolation(cv::INTER_NEAREST);
	show(testCaseName(), axes);
}

TEST_CASE("pixel_text") {
    Axes axes = plotImage(cv::Mat1i::eye(3,3));
    show(testCaseName(), axes);
}

TEST_CASE("double_matrix") {
    cv::Mat1d mat(5, 5, 0.);
    mat(0, 0) = 12;
    mat(0, 1) = 12.3;
    mat(0, 2) = 12.34;
    mat(0, 3) = CV_PI;
    mat(1, 0) = NAN;
    Axes axes = plotImage(mat);
    axes.find<Image>()->setNanColor(cv::Scalar(0, 0, 255));
    show(testCaseName(), axes);
}

TEST_CASE("image_overlay") {
	Axes axes = plotImage(lenna());
	axes.xLabel("Col");
	axes.yLabel("Row");
	axes.title("Overlay");
	std::vector<double> x{ 100, 100, 400, 400, 100 };
	std::vector<double> y{ 100,400,400,100,100 };
	axes.create<Series>(x,y,"-ow")
		.setLineWidth(2);
	show(testCaseName(), axes);
}

TEST_CASE("no_margin_no_axis") {
	Axes axes;
	axes.setFixedAspectRatio();
	axes.setYReverse();
	axes.setXTight();
	axes.setYTight();
	axes.setTightBox();
	axes.setXLim({ 200,300 });
	axes.setYLim({ 200,300 });
	axes.setMargins(0, 0, 0, 0);
	axes.create<Image>(lenna());
	show(testCaseName(), axes);
}

TEST_CASE("group") {
	Axes axes = makePlotAxes();
	Group &group = axes.create<Group>();
	group.create<Series>(std::vector<int>{1, 2}, "-b").setLineWidth(20);
	group.create<Series>(std::vector<int>{1, 3}, "-b").setLineWidth(20);
	axes.create<Series>(std::vector<int>{1, 5}, "-r").setLineWidth(20);

	show(testCaseName(), axes);

	axes.title("add to group");
	group.create<Series>(std::vector<int>{1, 4}, "-b").setLineWidth(20);
	show(testCaseName(), axes);

	axes.title("erase group");
	axes.drawables().erase(axes.findIterator(group));
	show(testCaseName(), axes);
}

TEST_CASE("name") {
    Axes axes = makePlotAxes();
    axes.create<Series>("-k").setName("black series");
    axes.create<Series>("-r").setName("red series");

    axes.find<Series>("black series")->setY(std::vector<double>{15, 16, 15});
    axes.find<Series>("red series")->setY(std::vector<double>{14, 13, 14});

    show(testCaseName(), axes);
}

TEST_CASE("move_erase") {
	Axes axes = makePlotAxes();
	Series &blue = axes.create<Series>(std::vector<int>{1, 2}, "-b").setLineWidth(20);
	Series &green = axes.create<Series>(std::vector<int>{1, 3}, "-g").setLineWidth(20);
	Series &red = axes.create<Series>(std::vector<int>{1, 4}, "-r").setLineWidth(20);
	show(testCaseName(), axes);

	axes.title("move red behind green");
	{
		auto it = axes.findIterator(red);
		auto drawable = std::move(*it);
		axes.drawables().erase(it);
		axes.drawables().insert(axes.findIterator(green), std::move(drawable));
		show(testCaseName(), axes);
	}

	axes.title("move blue to front");
	{
		auto it = axes.findIterator(blue);
		auto drawable = std::move(*it);
		axes.drawables().erase(it);
		axes.drawables().insert(axes.drawables().end(), std::move(drawable));
		show(testCaseName(), axes);
	}

	axes.title("erase blue");
	axes.drawables().erase(axes.findIterator(blue));
	show(testCaseName(), axes);

	axes.title("erase all Series");
	while (true) {
		auto it = axes.findIterator<Series>();
		if (it == axes.drawables().end()) {
			break;
		}
		axes.drawables().erase(it);
	}
	show(testCaseName(), axes);
}

TEST_CASE("custom_drawable") {
	struct MyRect :public Drawable {
		cv::Rect2d _rect = cv::Rect2d(3, 4, 5, 6);
		void render(RenderTarget &renderTarget) override {
			auto p1 = renderTarget.project({ _rect.x, _rect.y });
			auto p2 = renderTarget.project({ _rect.x + _rect.width, _rect.y + _rect.height });
			cv::rectangle(renderTarget.innerMat(), cv::Rect2d(p1, p2), cv::Scalar(0, 0, 255), 3);
		}
		bool getBoundingRect(cv::Rect2d &rect) override {
			rect = _rect;
			return true;
		}
	};
	Axes axes = makePlotAxes();
	axes.create<MyRect>();
	show(testCaseName(), axes);
}

TEST_CASE("custom_transformation") {
	struct MyPolarTransformation : public Transformation {
		cv::Point2d transform(const cv::Point2d &p)const override {
			return{ std::cos(p.x)*p.y,std::sin(p.x)*p.y };
		}
		cv::Point2d untransform(const cv::Point2d &p)const override {
			return{ std::atan2(p.y, p.x), std::sqrt(p.x *p.x + p.y*p.y) };
		}
		cv::Rect2d transformBoundingRect(const cv::Rect2d &r)const override{
			double radius = r.y + r.height;
			return cv::Rect2d(-radius, -radius, 2 * radius, 2 * radius);
		}
	};
	Axes axes;
	axes.setFixedAspectRatio();
	axes.setMargins(0, 0, 0, 0);
	axes.setTransformation(std::make_unique<MyPolarTransformation>());
	std::vector<double> y(1000), x(y.size());
	for (size_t i = 0; i < y.size(); i++) {
		x[i] = i * CV_2PI / (y.size()-1);
		y[i] = std::sin(x[i]*6);
	}
	axes.create<Series>(x, y, "-o");
	show(testCaseName(), axes);
}

TEST_CASE("mandelbrot") {
	struct Mandelbrot :public Drawable {
		void render(RenderTarget &renderTarget) override {
			cv::Rect innerRect = renderTarget.innerRect();
			cv::Rect2d logicalRect(renderTarget.unproject(cv::Point2d()), renderTarget.unproject(cv::Point2d(innerRect.width, innerRect.height)));
			cv::Mat1b mat = makeMandelbrot(logicalRect, renderTarget.innerMat().rows, renderTarget.innerMat().cols);
			cv::cvtColor(mat, renderTarget.innerMat(), cv::COLOR_GRAY2BGR);
		}
		bool getBoundingRect(cv::Rect2d &rect) override {
			rect = cv::Rect2d(-1.5, -1, 2, 2);
			return true;
		}
	};
	Axes axes = makePlotAxes();
	axes.create<Mandelbrot>();
	axes.setFixedAspectRatio();
	axes.setYReverse();
	show(testCaseName(), axes, 400, 400);
}

TEST_CASE("benchmark_Series") {
	const size_t n = 100'000;
	cv::Mat1d x(1, n), y(1, n);
	cv::randu(x, 0, 1);
	cv::randu(y, 0, 1);
	std::map<int, std::vector<cv::Point2d>> result;
	for (int renderWidth : {100, 500, 1000}) {
		auto &vec = result[renderWidth];
		vec.reserve(100);
		for (double p = 0; p <= 5; p += .5) {
			int points = (int)std::pow(10, p);
			using namespace std::chrono;
			auto start = steady_clock::now();
			int count = 0;
			double elapsed;
			do {
				plot(x.colRange(0, points), y.colRange(0, points), "-").render(renderWidth, renderWidth);
				count++;
				elapsed = duration<double>(steady_clock::now() - start).count();
			} while (elapsed < .2);
			double fps = count / elapsed;
			vec.push_back({(double)points,fps});
		}
	}
	auto axes = makePlotAxes();
	axes.setXLog();
	axes.setYLog();
	axes.xLabel("point count");
	axes.yLabel("frames per second");
	for (const auto &pair : result) {
		axes.create<Series>(pair.second);
	}
	show(testCaseName(), axes);
}

TEST_CASE("benchmark_Image") {
	std::map<int, std::vector<cv::Point2d>> result;
	for (int renderWidth : {100, 500, 1000}) {
		auto &vec = result[renderWidth];
		vec.reserve(100);
		for (double p = 0; p <= 4; p += .1) {
			int imageWidth = (int)std::pow(10, p);
			cv::Mat1b image(imageWidth, imageWidth);
			using namespace std::chrono;
			auto start = steady_clock::now();
			int count = 0;
			double elapsed;
			do {
				plotImage(image).render(renderWidth, renderWidth);
				count++;
				elapsed = duration<double>(steady_clock::now() - start).count();
			} while (elapsed < .2);
			double fps = count / elapsed;
			vec.push_back({(double)imageWidth,fps});
		}
	}
	auto axes = makePlotAxes();
	axes.setXLog();
	axes.setYLog();
	axes.xLabel("source image width and height");
	axes.yLabel("frames per second");
	for (const auto &pair : result) {
		axes.create<Series>(pair.second);
	}
	show(testCaseName(), axes);
}
