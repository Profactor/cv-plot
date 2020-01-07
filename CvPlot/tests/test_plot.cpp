// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#include <catch.hpp>
#include <CvPlot/cvplot.h>
#include "cvMatIsEqual.h"
#include <opencv2/imgproc.hpp>

namespace {

cv::Mat lenna() {
	auto mat = cv::imread("testdata/Lenna.jpg");
	CHECK(!mat.empty());
	return mat;
}

}

using namespace CvPlot;

TEST_CASE("version") {
    CHECK(!version().empty());
}

TEST_CASE("construct_Axes") {
	Axes axes;
}

TEST_CASE("render") {
	Axes axes;
	cv::Mat3b mat = axes.render();
	CHECK(!mat.empty());
}

TEST_CASE("plot") {
	std::vector<int> y{ 1, 2, 3, 4, 5 };
	Axes axes = plot(y, ".");
	axes.enableHorizontalGrid(false);
	axes.enableVerticalGrid(false);
	cv::Mat3b mat = axes.render();
	cv::Rect innerRect = axes.getProjection(mat.size()).innerRect();
	cv::Mat3b inner = mat(innerRect);
	cv::Mat innerGray;
	cv::cvtColor(inner, innerGray, cv::COLOR_BGR2GRAY);
	int count = cv::countNonZero(innerGray != 255);
    CHECK(count == (int)y.size());
}

TEST_CASE("Image") {
	Axes axes;
	axes.setFixedAspectRatio();
	axes.setYReverse();
	axes.setXTight();
	axes.setYTight();
	axes.setTightBox();
	axes.setMargins(0, 0, 0, 0);
	cv::Mat src = lenna();
	CHECK(!src.empty());
	axes.create<Image>(src);
	cv::Mat3b dst = axes.render(src.rows, src.cols);
    CHECK(cvMatIsEqual(src, dst));
}

TEST_CASE("name") {
    Axes axes;
    axes.create<Series>().setName("series1");
    axes.create<Series>().setName("series2");

    axes.find<Series>("series1")->setY(1);
    axes.find<Series>("series2")->setY(2);

    CHECK(axes.find<Series>("series1")->getY().front() == 1);
    CHECK(axes.find<Series>("series2")->getY().front() == 2);
    CHECK(axes.drawables().size() == 2);
}

