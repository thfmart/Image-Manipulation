#include "imagehelper.h"

namespace imaging
{

ImageHelper::ImageHelper()
{
	display = CImgDisplay();
}

void ImageHelper::getImageInfo(const char *filename, int &width, int &height, int &channel)
{
	CImg<unsigned char> img(filename);
	width = img.width();
	height = img.height();
	channel = img.spectrum();
}

void ImageHelper::loadImage(const char *filename, unsigned char **ptr)
{
	CImg<unsigned char> img(filename);
	memcpy(*ptr, img.data(), img.width()*img.height()*img.spectrum());
}

void ImageHelper::showImage(CImg<unsigned char> image, boolean pause)
{
	display.display(image);
	display.show();
	
	if (pause) {
    	while (!display.is_closed()) {
    		display.wait();
		};
	}
}

void ImageHelper::showImage(unsigned char *ptr, int width, int height, int channel, boolean pause)
{
	CImg<unsigned char> image(ptr, width, height, 1, channel, true);
	showImage(image, pause);
}

void ImageHelper::saveImage(const char *filename, unsigned char *ptr, int width, int height, int channel)
{
	CImg<unsigned char> image(ptr, width, height, 1, channel, true);
	image.save(filename);
}

}; // namespace imaging
