#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/hal/hal.hpp>
using namespace std;
using namespace cv;

// to do list 
// manage to read the png 
// sobel filter function 
// orange color filter function 

//extracts a 3*3 window from the image 
void fill_window(int** window,int pixelx,int pixely,Mat image,int channel){
	Size imsize = image.size();
	int img_width = imsize.width;
	int img_height = imsize.height;
	 
	for ( int i = 0; i < 3; i++) {
		//the one before
		for (int j=0;j<3;j++){
			int used_x = pixelx - 1 + i;
			int used_y = pixely - 1 + j;
			if(used_x > img_width||used_x < 0 || used_y > img_height||used_y < 0)
				window[i][j] = 0;
			else							// row , col , channel 
				window[i][j] = image.at<uchar>(used_y,used_x); }
	}
}

//dot product of 2 arrays of width*height
int dot_product(int** window,int kernel[3][3]){
	int result = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result += window[i][j] * kernel[i][j];
		}
	}
	return result;
}

Mat sobel_filter(Mat image){
	int kernel[3][3] = { {-1,0,1}
						,{-2,0,2} 
						,{-1,0,1} };
	
	int img_width = image.cols;
	int img_height = image.rows;

	
	int** window = new int* [3];
	Mat result = image.clone();
	

	for (int i = 0; i < 3; i++)
		window[i] = new int[3];
	/*int** imagecopy = new int* [img_width];
	for (int i = 0; i < img_height; i++) 
		imagecopy[i] = new int[img_height];
	
	memcpy(imagecopy, image.data, img_width * img_height * sizeof(int));
	*/
	//apply sobel filter  3*3 matrix multiplication
	for (int imagey = 0; imagey < img_height; imagey++){
        for (int imagex = 0; imagex < img_width; imagex++) {

			fill_window(window, imagex, imagey, image,0);
			if (imagey == img_height-1)
				int x=0;
			result.at<uchar>(imagey,imagex) = dot_product(window, kernel);

			/*fill_window(window, imagex, imagey, image, 1);
			result.at<uchar>(imagey, imagex, 1) = dot_product(window, kernel);

			fill_window(window, imagex, imagey, image, 2);
			result.at<uchar>(imagey, imagex, 2) = dot_product(window, kernel);*/
        }
    }
	//memcpy(result.data, imagecopy, img_width * img_height * sizeof(int));
	//delete imagecopy;
	delete window;
	return result;
}

Mat orange_filter(int img_width,int img_height, Mat image){
	 
	int** imagecopy = new int* [img_width];
	Mat result;

	for (int i = 0; i < img_height; i++)
		imagecopy[i] = new int[img_height];
	int r=0, g=0, b=0;
	//fill rgb values 
     for (int i = 0; i<img_width;i++){
        for(int j =0 ;i<img_height;j++)
        {	// orange has a range of 255, >50 , _
			if (r == 255, g >= 50) 
			{
				r = 0, g = 0, b = 0;
				//move those to the mat result 
			}
            
        }
    } 
	 delete imagecopy;
	 memcpy(result.data, imagecopy, img_width * img_height * sizeof(int));
	 return result;
}

int main (){

  int choice,int_width,int_height;
  Mat image;
  string default_path;
  cout<<"pick an operation:1 sobel filter, 2 orange color filter";
  cin>>choice;
  //cout<<"enter image ,0 to use default path:" + default_path;
  //load image 
  image = imread("D:\\Redirected Profile\\Documents\\projects\\git\\IPvisualstudio\\image-2.png");
  
  
  Mat result;

  int_width = image.cols;
  int_height = image.rows;

  if (choice==1)
      result = sobel_filter(image); 

  else if (choice==2)
      result = orange_filter(int_width, int_height,image); 

  imshow("result", result);
  waitKey(0);
 //display image
  return 0;
}