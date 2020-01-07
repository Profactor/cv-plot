// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#include <opencv2/core/core.hpp>

inline bool cvMatIsEqual(const cv::Mat &a_, const cv::Mat &b_) {
    cv::Mat a = a_.clone();
    cv::Mat b = b_.clone();
    return a.dims == b.dims
           && 0 == std::memcmp(a.size.p, b.size.p, a.dims)
           && a.flags == b.flags
           && (a.total() == 0 || 0 == std::memcmp(a.data, b.data, a.total() * a.elemSize()));
}
