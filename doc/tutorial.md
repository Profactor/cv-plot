# Plot Numbers
The easiest way to get a diagram, is to use the plot() function. It takes a [cv::InputArray](https://docs.opencv.org/3.4/d4/d32/classcv_1_1__InputArray.html), so you can plot cv::Mat, std::vector, etc.
```c++
auto axes = CvPlot::plot(std::vector<double>{ 3, 3, 4, 6, 4, 3 }, "-o");
cv::Mat mat = axes.render(300, 400);
```
