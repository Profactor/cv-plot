// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/core/Drawable.h>

namespace CvPlot {

class Drawable::Impl {
public:
    std::string _name;
};

CVPLOT_DEFINE_FUN
Drawable::Drawable(std::string name) {
    impl->_name = std::move(name);
}

CVPLOT_DEFINE_FUN
Drawable::Drawable(Drawable &&a)
    : impl(std::move(a.impl)) {
}

CVPLOT_DEFINE_FUN
Drawable::~Drawable() {
}

CVPLOT_DEFINE_FUN
void Drawable::render(RenderTarget & renderTarget) {
}

CVPLOT_DEFINE_FUN
bool Drawable::getBoundingRect(cv::Rect2d &rect) {
    return false;
}

CVPLOT_DEFINE_FUN
Drawable & Drawable::setName(std::string name) {
    impl->_name = name;
    return *this;
}

CVPLOT_DEFINE_FUN
const std::string & Drawable::getName() {
    return impl->_name;
}

}
