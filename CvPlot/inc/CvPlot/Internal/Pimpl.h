// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <memory>

namespace CvPlot {
namespace Internal {

template<class T>
class Pimpl {
public:
    Pimpl(Pimpl<T>&& o)
        : m(std::move(o.m)) {
    }
    template<typename ...Args>
    Pimpl(Args&& ...args)
        : m(new T(std::forward<Args>(args)...)) {
    }
    ~Pimpl() {
        static_assert(0 < sizeof(T), "Pimpl error: Missing destructor in the pimpled class?");
    }
    T* operator->() {
        return m.get();
    }
    const T* operator->() const {
        return m.get();
    }
	Pimpl& operator=(Pimpl &&a) {
		m = std::move(a.m);
		return *this;
	}
private:
    Pimpl(const Pimpl<T>& o);
    std::unique_ptr<T> m;
};

}
}
