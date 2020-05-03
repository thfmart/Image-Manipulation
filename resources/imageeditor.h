#include "CImg.h"
#include <vector>

namespace imaging
{
using namespace cimg_library;

class ImageEditor
{
public:
	CImg<unsigned char> img;
	CImg<unsigned char> gray;
	CImg<unsigned char> border;
	CImg<unsigned char> grayBorder;
	
	ImageEditor(const char *filename, int width, int height);
	~ImageEditor();
	void convertGreyScale(unsigned char **ptr, int &channel);
	void multiplyVectorByScalar(std::vector<float> *myVector, float scalar);
	void applyBlur(unsigned char **ptr, int &channel);
	void addBorder();
	void portraitMode(unsigned char *blurredptr, int blurredChannel, const char *filenamemask, unsigned char **ptr, int &portraitChannel);
	void backgroundEditor(const char *filenamebackground, const char *filenamemask, unsigned char **ptr, int &backChanne);
	void applyHorSobel(unsigned char **ptr, int &sobelChannel);
	void edgeMask(unsigned char **ptr, int &sobelChannel);
	void cartoonMask(unsigned char *edgeptr, int edgeChannel, unsigned char **ptr, int &cartoonChannel);
	void denoise(unsigned char **ptr, int &channel);
	void sharpener(unsigned char **ptr, int &channel, float intensity);
	
private:
	static const float REDSCALE = 0.2126;
	static const float GREENSCALE = 0.7152;
	static const float BLUESCALE = 0.0722;
};
}; // namespace imaging


