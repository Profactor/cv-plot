// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/Drawable.h>
#include <deque>
#include <memory>
#include <CvPlot/Internal/no_warning.h>
#include <opencv2/core.hpp>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE DrawableDeque {
public:
	typedef std::deque<std::unique_ptr<Drawable>> Drawables;

	DrawableDeque();
	DrawableDeque(const DrawableDeque&) = delete;
	DrawableDeque &operator=(const DrawableDeque&) = delete;
	DrawableDeque(DrawableDeque &&);
	DrawableDeque& operator=(DrawableDeque&&);

	Drawables& drawables();

	template<typename SomeDrawable, typename... Args>
	SomeDrawable& create(Args&&... args);

    template<typename SomeDrawable>
    SomeDrawable* find();

    template<typename SomeDrawable>
    SomeDrawable* find(const std::string &name);

	template<typename SomeDrawable>
	SomeDrawable& findOrCreate();

    template<typename SomeDrawable>
    Drawables::iterator findIterator(const std::string &name);

    template<typename SomeDrawable>
    Drawables::iterator findIterator();

	Drawables::iterator findIterator(Drawable &drawable);

protected:
	bool getBoundingRect(cv::Rect2d &rect);

private:
	CVPLOT_NO_WARNING_DLL_INTERFACE(Drawables, _drawables);
};

template<typename SomeDrawable, typename... Args>
SomeDrawable& DrawableDeque::create(Args&&... args) {
	std::unique_ptr<SomeDrawable> ptr(new SomeDrawable(std::forward<Args>(args)...));
	SomeDrawable& ref = *ptr.get();
	_drawables.push_back(std::move(ptr));
	return ref;
}

template<typename SomeDrawable>
SomeDrawable* DrawableDeque::find() {
    auto it = DrawableDeque::findIterator<SomeDrawable>();
    if (it != _drawables.end()) {
        return static_cast<SomeDrawable*>(it->get());
    }
    return nullptr;
}

template<typename SomeDrawable>
SomeDrawable* DrawableDeque::find(const std::string &name) {
    auto it = DrawableDeque::findIterator<SomeDrawable>(name);
    if (it != _drawables.end()) {
        return static_cast<SomeDrawable*>(it->get());
    }
    return nullptr;
}

template<typename SomeDrawable>
DrawableDeque::Drawables::iterator DrawableDeque::findIterator() {
    return std::find_if(_drawables.begin(), _drawables.end(), [](const std::unique_ptr<Drawable> &ptr) {
        return dynamic_cast<SomeDrawable*>(ptr.get()) != nullptr;
        });
}

template<typename SomeDrawable>
DrawableDeque::Drawables::iterator DrawableDeque::findIterator(const std::string &name) {
    return std::find_if(_drawables.begin(), _drawables.end(), [&](const std::unique_ptr<Drawable> &ptr) {
        return ptr->getName() == name && dynamic_cast<SomeDrawable*>(ptr.get()) != nullptr;
        });
}

template<typename SomeDrawable>
SomeDrawable& DrawableDeque::findOrCreate() {
	SomeDrawable *p = find<SomeDrawable>();
	return p ? *p : create<SomeDrawable>();
}

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/DrawableDeque.ipp>
#endif