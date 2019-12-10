//from http://docs.opencv.org/trunk/d7/dff/tutorial_how_to_use_OpenCV_parallel_for_.html

#include <opencv2/core.hpp>

namespace MandelBrotImp{
using namespace std;
using namespace cv;
int mandelbrot(const complex<float> &z0, const int max){
	complex<float> z = z0;
	for (int t = 0; t < max; t++)
	{
		if (z.real()*z.real() + z.imag()*z.imag() > 4.0f) return t;
		z = z*z + z0;
	}
	return max;
}
int mandelbrotFormula(const complex<float> &z0, const int maxIter = 500) {
	int value = mandelbrot(z0, maxIter);
	if (maxIter - value == 0){
		return 0;
	}
	return cvRound(sqrt(value / (float)maxIter) * 255);
}
class ParallelMandelbrot : public ParallelLoopBody{
public:
	ParallelMandelbrot(Mat &img, const float x1, const float y1, const float scaleX, const float scaleY)
		: m_img(img), m_x1(x1), m_y1(y1), m_scaleX(scaleX), m_scaleY(scaleY){
	}
	virtual void operator ()(const Range& range) const{
		for (int r = range.start; r < range.end; r++){
			int i = r / m_img.cols;
			int j = r % m_img.cols;

			float x0 = j / m_scaleX + m_x1;
			float y0 = i / m_scaleY + m_y1;

			complex<float> z0(x0, y0);
			uchar value = (uchar)mandelbrotFormula(z0);
			m_img.ptr<uchar>(i)[j] = value;
		}
	}
	ParallelMandelbrot& operator=(const ParallelMandelbrot &) {
		return *this;
	}
private:
	Mat &m_img;
	float m_x1;
	float m_y1;
	float m_scaleX;
	float m_scaleY;
};
}

cv::Mat1b makeMandelbrot(cv::Rect2f rect, int rows, int cols){
	using namespace MandelBrotImp;
	cv::Mat1b mat(rows, cols);
	float scaleX = mat.cols / rect.width;
	float scaleY = mat.rows / rect.height;
	ParallelMandelbrot parallelMandelbrot(mat, rect.x, rect.y, scaleX, scaleY);
	parallel_for_(Range(0, rows*cols), parallelMandelbrot);
	return mat;
}
