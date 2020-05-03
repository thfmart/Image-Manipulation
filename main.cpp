#include <cstdlib>
#include <iostream>

#include <windows.h>
#include <stdio.h>

#include "CImg.h"
#include "resources/guicon.h"
#include "resources/imagehelper.h"

/// ---- include your own files here
#include "resources/imageeditor.h"

/// ----

using namespace std;
using namespace cimg_library;
using namespace imaging;

int main(int argc, char *argv[])
{
	// Open a console and bind I/O to it
	RedirectIOToConsole();
	
	cout << "Loading input image...\n";    
    ImageHelper *helper = new ImageHelper();
    
    // input image
    unsigned char *image;
	int width;
	int height;
	int channel;
	
	helper->getImageInfo("assets/lena.bmp", width, height, channel);
	image = new unsigned char[width * height * channel];
	helper->loadImage("assets/lena.bmp", &image);
	
	//helper->showImage(image, width, height, channel);
	cout << "Input image width:" << width << " height:" << height << endl;
    
    //Variables
    ImageEditor *editor;
    ImageEditor *denoiser;
    unsigned char *grayImage;
    int grayChannel;
    unsigned char *blurredImage;
    int blurredChannel;
    unsigned char *portraitImage;
    int portraitChannel;
    unsigned char *backImage;
    int backChannel;
    unsigned char *sobelHorImage;
    int sobelHorChannel;
    unsigned char *sobelImage;
    int sobelChannel;
    unsigned char *cartoonImage;
    int cartoonChannel;
    unsigned char *denoisedImage;
    int denoisedChannel;
    unsigned char *sharpenedImage;
    int sharpenedChannel;
    
    editor = new ImageEditor("assets/lena.bmp", width, height);
    
    //Gray Image
    grayImage = new unsigned char[width * height];
    editor->convertGreyScale(&grayImage, grayChannel);
    helper->saveImage("assets/output/Gray.bmp", grayImage, width, height, grayChannel);
    
    //Blur (average filter)
    blurredImage = new unsigned char[width * height* channel];
    editor->applyBlur(&blurredImage, blurredChannel);
    helper->saveImage("assets/output/Blur.bmp", blurredImage, width, height, blurredChannel);
    
    //Portrait
	portraitImage = new unsigned char[width * height * channel];
	editor->portraitMode(blurredImage, blurredChannel, "assets/lena_mask.bmp", &portraitImage, portraitChannel);
    helper->saveImage("assets/output/Portrait.bmp", portraitImage, width, height, portraitChannel);
    
    //Background Image
	backImage = new unsigned char[width * height * channel];
	editor->backgroundEditor("assets/eiffel.bmp", "assets/lena_mask.bmp", &backImage, backChannel);
    helper->saveImage("assets/output/Background.bmp", backImage, width, height, backChannel);
    
    //Sobel filter
    sobelHorImage = new unsigned char[width * height * grayChannel];
    editor->applyHorSobel(&sobelHorImage, sobelHorChannel); //Horizontal = True, Normalized=True
    helper->saveImage("assets/output/Sobel-Horizontal.bmp", sobelHorImage, width, height, sobelHorChannel);
    
    sobelImage = new unsigned char[width * height * grayChannel];
    editor->edgeMask(&sobelImage, sobelChannel);
    helper->saveImage("assets/output/Sobel-Vertical.bmp", sobelImage, width, height, sobelChannel);
    
    //Cartoonized image
    cartoonImage = new unsigned char[width * height * channel];
    editor->cartoonMask(sobelImage, sobelChannel, &cartoonImage, cartoonChannel);
    helper->saveImage("assets/output/Cartoonized.bmp", cartoonImage, width, height, cartoonChannel);
    
    //Noise reduction
    denoiser = new ImageEditor("assets/lena_noise.bmp", width, height);
    denoisedImage = new unsigned char[width * height* channel];
    denoiser->denoise(&denoisedImage, denoisedChannel);
    helper->saveImage("assets/output/Denoised.bmp", denoisedImage, width, height, denoisedChannel);
    
    //Sharpening filter
    sharpenedImage = new unsigned char[width * height* channel];
    float intensity = 1.0; //Filter intensity
    editor->sharpener(&sharpenedImage, sharpenedChannel, intensity);
    helper->saveImage("assets/output/Sharpened.bmp", sharpenedImage, width, height, sharpenedChannel);
    
    /// ----
    
    cout << "END\n";
    system("PAUSE");
    
    delete helper;
    delete[] image;
    
    return EXIT_SUCCESS;
}
