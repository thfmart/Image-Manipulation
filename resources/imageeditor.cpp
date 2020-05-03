#include <iostream>
#include <stdio.h>
#include "ImageEditor.h"

namespace imaging
{
	using namespace std;

ImageEditor::ImageEditor(const char *filename, int width, int height): img(filename), gray(img.width(),img.height(),1,1,0), 
border((img.width()+2),(img.height()+2),1,3), grayBorder((img.width()+2), (img.height()+2),1,1,0)
{
	
}

void ImageEditor::convertGreyScale(unsigned char **ptr, int &channel)
{
	//XY data handling for CImg
	cimg_forXY(img,x,y) 
	{
	 	int R = (int)img(x,y,0,0);
	    int G = (int)img(x,y,0,1);
	    int B = (int)img(x,y,0,2);
	
	    // weighted gray scale conversion
	    int grayValueWeight = (int)(REDSCALE*R + GREENSCALE*G + BLUESCALE*B);
	    // saving píxel values into img information
	    gray(x,y,0,0) = grayValueWeight;
	}	
	
	// allocating results back to the gray image
	memcpy(*ptr, gray.data(),gray.width()*gray.height());
	channel = gray.spectrum();
}

void ImageEditor::addBorder()
{
	//Function to add Border to both gray and original image to posterior processing whe applying the kernels
	//Both of them have width = normal width + 2 and height = normal height + 2 
	cimg_forXY(img,x,y) 
	{
	    for (int i=0; i<3;i++)
	    {
	    	border(x+1,y+1,0,i) = (int)img(x,y,0,i);	
		}
		grayBorder(x+1,y+1) = (int)gray(x,y);
	}
	
	int wid = border.width();
	int hei = border.height();
	
	//Fill up the empty values with nearest pixel (horizontal)
	cimg_forXY(border,x,y) 
	{
	    if (x==0)
	    {
	    	for (int i=0; i<3;i++)
	    	{
	    		border(x,y,0,i) = border(x+1,y,0,i);
			}
			grayBorder(x,y) = grayBorder(x+1,y);	    	
		}
		else if (x==wid-1)
		{
			for (int i=0; i<3;i++)
			{
				border(x,y,0,i) = border(x-1,y,0,i);	
			}	
			grayBorder(x,y) = grayBorder(x-1,y);		
		}
	}
	
	//Fill up the empty values with nearest pixel (vertical)
		cimg_forXY(border,x,y) 
	{
	    if (y==0)
		{
			for (int i=0; i<3;i++)
			{
				border(x,y,0,i) = border(x,y+1,0,i);	
			}	
			grayBorder(x,y) = grayBorder(x,y+1);
		}
		else if (y==hei-1)
		{
			for (int i=0; i<3;i++)
			{
				border(x,y,0,i) = border(x,y-1,0,i);	
			}	
			grayBorder(x,y) = grayBorder(x,y-1);
		}
	}
}

void ImageEditor::applyBlur(unsigned char **ptr, int &channel)
{
	CImg<unsigned char> blurred(img.width(),img.height(),1,3);
	addBorder();
	
	//Applying the kernel on the image with aditional borders
	cimg_forXY(blurred,x,y)
	{
		for (int i=0; i<3;i++)
		{
			blurred(x,y,0,i) = (int) (border(x,y,0,i)+ border(x,y+1,0,i) + border(x,y+2,0,i) +
			border(x+1,y,0,i)+ border(x+1,y+1,0,i) + border(x+1,y+2,0,i) +
			border(x+2,y,0,i)+ border(x+2,y+1,0,i) + border(x+2,y+2,0,i) )/9;	
		}	
	} 

	channel = blurred.spectrum();
	memcpy(*ptr, blurred.data(),blurred.width()*blurred.height()*channel);
}

void ImageEditor::portraitMode(unsigned char *blurredptr, int blurredChannel, const char *filenamemask, unsigned char **ptr, int &portraitChannel)
{
	CImg<unsigned char> portrait(img.width(),img.height(),1,3);
	CImg<unsigned char> mask(filenamemask);
	CImg<unsigned char> blurred(blurredptr, img.width(), img.height(), 1, blurredChannel, true); //Receive the blurred image created on task 2
	//Applying logical condition to Portrait mode
	cimg_forXY(portrait,x,y) 
	{
		for (int i=0; i<3; i++)
		{
			portrait(x,y,0,i) = mask(x,y)>0 ? (int)(img(x,y,0,i)): (int)(blurred(x,y,0,i));
		}	
	}
	
	portraitChannel = portrait.spectrum();
	memcpy(*ptr, portrait.data(),portrait.width()*portrait.height()*portraitChannel);	
}

void ImageEditor::backgroundEditor(const char *filenamebackground, const char *filenamemask, unsigned char **ptr, int &bonusChannel)
{
	CImg<unsigned char> bonus(img.width(),img.height(),1,3);
	CImg<unsigned char> mask(filenamemask);
	CImg<unsigned char> back(filenamebackground);

	//Applying logical condition to Background mode
	cimg_forXY(bonus,x,y) 
	{
			for (int i=0; i<3; i++)
			{
				bonus(x,y,0,i) = mask(x,y)>0 ? (int)(img(x,y,0,i)): (int)(back(x,y,0,i));
			}	
	}
	bonusChannel = bonus.spectrum();
	memcpy(*ptr, bonus.data(),bonus.width()*bonus.height()*bonusChannel);	
}

void ImageEditor::applyHorSobel(unsigned char **ptr, int &sobelChannel)
{
	/*The result of this pice of code will be a image filtered by the horizontal Sobel
	described at the assignment, as well as normalized*/
	
	CImg<unsigned char> sobel(img.width(),img.height(),1,1,0);
	//Applying the kernel on the image with aditional borders
	int max = 0;
	int min = 0;
	int temp = 0;
	
	cimg_forXY(sobel,x,y) //Find Maximum and Minimum
	{	
		temp = (int) abs( (grayBorder(x+2,y) + 2*(grayBorder(x+2,y+1)) + (grayBorder(x+2,y+2))
		+ (grayBorder(x,y))*(-1) + (grayBorder(x,y+1))*(-2) + (grayBorder(x,y+2))*(-1)) );
		
		if (temp>max)
		{
			max = temp;
		} 
		if (temp<min)
		{
			min = temp;	
		}
	}
	cimg_forXY(sobel,x,y) //Populate Sobel Horizontal Filter normalized
	{	
		temp = (int) abs( (grayBorder(x+2,y) + 2*(grayBorder(x+2,y+1)) + (grayBorder(x+2,y+2))
		+ (grayBorder(x,y))*(-1) + (grayBorder(x,y+1))*(-2) + (grayBorder(x,y+2))*(-1)) );
		
		sobel(x,y) = (int) (temp-min)*255/(max-min); // Normalize results to fall whithin the 0 to 255 range
	}
	
	sobelChannel = sobel.spectrum();
	memcpy(*ptr, sobel.data(), sobel.width()*sobel.height()*sobelChannel);
}

void ImageEditor::edgeMask(unsigned char **ptr, int &sobelChannel)
{
	int max = 0;
	int min = 0;
	int temp = 0;
	int horizontal;
	int vertical;
	CImg<unsigned char> sobel(img.width(),img.height(),1,1,0);
	
	cimg_forXY(sobel,x,y) //Find Maximum and Minimum
	{
		horizontal = (int) ( (grayBorder(x+2,y) + 2*(grayBorder(x+2,y+1)) + (grayBorder(x+2,y+2)) //Horizontal Sobel
		+ (grayBorder(x,y))*(-1) + (grayBorder(x,y+1))*(-2) + (grayBorder(x,y+2))*(-1)) );
		vertical = (int) ( (grayBorder(x,y) + 2*(grayBorder(x+1,y)) + (grayBorder(x+2,y))         //Vertical Sobel
		+ (grayBorder(x,y+2))*(-1) + (grayBorder(x+1,y+2))*(-2) + (grayBorder(x+2,y+2))*(-1)) );
		
		temp = (double) sqrt(pow(horizontal,2) + pow(vertical,2));
		if (temp>max)
		{
			max = temp;
		} 
		if (temp<min)
		{
			min = temp;	
		}
	}
	
	cimg_forXY(sobel,x,y) //Populate Sobel Filter normalized
	{
		horizontal = (int) ( (grayBorder(x+2,y) + 2*(grayBorder(x+2,y+1)) + (grayBorder(x+2,y+2))
		+ (grayBorder(x,y))*(-1) + (grayBorder(x,y+1))*(-2) + (grayBorder(x,y+2))*(-1)) );
		vertical = (int) ( (grayBorder(x,y) + 2*(grayBorder(x+1,y)) + (grayBorder(x+2,y))
		+ (grayBorder(x,y+2))*(-1) + (grayBorder(x+1,y+2))*(-2) + (grayBorder(x+2,y+2))*(-1)) );
		
		temp = (double) sqrt(pow(horizontal,2) + pow(vertical,2));
		sobel(x,y) = (int) (temp-min)*255/(max-min);
	}
	
	sobelChannel = sobel.spectrum();
	memcpy(*ptr, sobel.data(), sobel.width()*sobel.height()*sobelChannel);
}

void ImageEditor::cartoonMask(unsigned char *edgeptr, int edgeChannel, unsigned char **ptr, int &cartoonChannel)
{
	CImg<unsigned char> edge(edgeptr, gray.width(), gray.height(), 1, edgeChannel, true); //Receive the Edge Sobel Image created on task 5
	CImg<unsigned char> cartoon(img.width(),img.height(),1,3);
	
	cimg_forXY(cartoon,x,y)
	{
		edge(x,y) = edge(x,y)>50 ? 0 : 255; //Only edges really strong become black
		for (int i = 0; i<3; i++) 
		{
			cartoon(x,y,0,i) = 	edge(x,y)==0 ? 0 : img(x,y,0,i); //if edge is black, edge value. Image value otherwise
		}	
	}
	
	cartoonChannel = cartoon.spectrum();
	memcpy(*ptr, cartoon.data(), cartoon.width()*cartoon.height()*cartoonChannel);
}
}; // namespace imaging
