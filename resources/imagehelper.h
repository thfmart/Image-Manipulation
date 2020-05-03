#include "CImg.h"

namespace imaging
{
using namespace cimg_library;

class ImageHelper
{
public:
	ImageHelper();
	
	void getImageInfo(const char *filename, int &width, int &height, int &channel);
	void loadImage(const char *filename, unsigned char **ptr);
	void showImage(unsigned char *ptr, int width, int height, int channel, boolean pause=true);
	void saveImage(const char *filename, unsigned char *ptr, int width, int height, int channel);
	void showImage(CImg<unsigned char> image, boolean pause);
	
private:
	CImgDisplay display;
	//void showImage(CImg<unsigned char> image, boolean pause);
};
}; // namespace imaging
