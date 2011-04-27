IplImage *bilinear(IplImage *img, int newWidth, int newHeight);
IplImage *bilinear1(IplImage *img, int newWidth, int newHeight);
IplImage *biquadratic(IplImage *img, int newWidth, int newHeight);
IplImage *bicubic(IplImage *img, int newWidth, int newHeight);
IplImage *bicubicBSpilne(IplImage *img, int newWidth, int newHeight);
IplImage *nearRest(IplImage *img, int newWidth, int newHeight);
IplImage *createImage(int w, int h);
