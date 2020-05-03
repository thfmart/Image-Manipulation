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
        
    /// ---- Add your code here
    
    //Variables
    ImageEditor *editor;
    unsigned char *grayImage;
    int grayChannel;
    unsigned char *blurredImage;
    int blurredChannel;
    unsigned char *portraitImage;
    int portraitChannel;
    unsigned char *bonusImage;
    int bonusChannel;
    unsigned char *sobelHorImage;
    int sobelHorChannel;
    unsigned char *sobelImage;
    int sobelChannel;
    unsigned char *cartoonImage;
    int cartoonChannel;
    
    editor = new ImageEditor("assets/lena.bmp", width, height);
    
    grayImage = new unsigned char[width * height];
    editor->convertGreyScale(&grayImage, grayChannel);
    helper->saveImage("assets/Gray.bmp", grayImage, width, height, grayChannel);
    
    blurredImage = new unsigned char[width * height* channel];
    editor->applyBlur(&blurredImage, blurredChannel);
    helper->saveImage("assets/Blur.bmp", blurredImage, width, height, blurredChannel);
    
	portraitImage = new unsigned char[width * height * channel];
	editor->portraitMode(blurredImage, blurredChannel, "assets/lena_mask.bmp", &portraitImage, portraitChannel);
    helper->saveImage("assets/Portrait.bmp", portraitImage, width, height, portraitChannel);
    
	bonusImage = new unsigned char[width * height * channel];
	editor->backgroundEditor("assets/background.bmp", "assets/lena_mask.bmp", &bonusImage, bonusChannel);
    helper->saveImage("assets/Background.bmp", bonusImage, width, height, bonusChannel);
    
    sobelHorImage = new unsigned char[width * height * grayChannel];
    editor->applyHorSobel(&sobelHorImage, sobelHorChannel); //Horizontal = True, Normalized=True
    helper->saveImage("assets/Sobel-Horizontal.bmp", sobelHorImage, width, height, sobelHorChannel);
    
    sobelImage = new unsigned char[width * height * grayChannel];
    editor->edgeMask(&sobelImage, sobelChannel);
    helper->saveImage("assets/Sobel-Vertical.bmp", sobelImage, width, height, sobelChannel);
    
    cartoonImage = new unsigned char[width * height * channel];
    editor->cartoonMask(sobelImage, sobelChannel, &cartoonImage, cartoonChannel);
    helper->saveImage("assets/Cartoonized.bmp", cartoonImage, width, height, cartoonChannel);
    
    // set channel=1 or 3 to save image as grayscale or RGB, respectively
    // save your images as Bitmap
    //helper->saveImage("output1.bmp", ptr, width, height, channel);  
    
    /// ----
    
    cout << "END\n";
    system("PAUSE");
    
    delete helper;
    delete[] image;
    
    return EXIT_SUCCESS;
}
