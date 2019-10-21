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
void fill_window(int** window,int pixelx,int pixely,Mat image){

	int img_width = image.cols;
	int img_height = image.rows;
	int temp;
	
	for ( int i = 0; i < 3; i++) {
		//the one before
		
		for (int j=0;j<3;j++){
			int used_x = pixelx - 1 + j;
			int used_y = pixely - 1 + i;
			
			if(used_x >= img_width||used_x < 0 || used_y >= img_height||used_y < 0)
				window[i][j] = 0;
			else							// row , col , channel 
			{
 				temp = image.at<uchar>(used_y, used_x);
				window[i][j] = temp;
				
			}
			
		}
		
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

Mat sobel_filter_color(Mat image){
	int kernel1[3][3] = { {-1,0,1}						,{-2,0,2} 						,{-1,0,1} };
	int kernel2[3][3] = { {1,2,1}						,{0,0,0}						,{-1,-2,-1} };
	vector<Mat> bgr_channels;
	split(image, bgr_channels);

	int img_width = image.cols;
	int img_height = image.rows;

	int** window = new int* [3];
	Mat result1b = bgr_channels[0].clone(), result2b = bgr_channels[0].clone(), resultb;
	Mat result1g = bgr_channels[1].clone(), result2g = bgr_channels[1].clone(), resultg;
	Mat result1r = bgr_channels[2].clone(), result2r = bgr_channels[2].clone(), resultr;

	for (int i = 0; i < 3; i++)
		window[i] = new int[3];
	
	for (int imagex = 0; imagex < img_width; imagex++) {
		
        for (int imagey = 0; imagey < img_height ; imagey++) {
			fill_window(window, imagex, imagey, bgr_channels[0]);
			result1b.at<uchar>(imagey,imagex) = dot_product(window, kernel1);
			result2b.at<uchar>(imagey, imagex) = dot_product(window, kernel2);

			fill_window(window, imagex, imagey, bgr_channels[1]);
			result1g.at<uchar>(imagey, imagex) = dot_product(window, kernel1);
			result2g.at<uchar>(imagey, imagex) = dot_product(window, kernel2);

			fill_window(window, imagex, imagey, bgr_channels[2]);
			result1r.at<uchar>(imagey, imagex) = dot_product(window, kernel1);
			result2r.at<uchar>(imagey, imagex) = dot_product(window, kernel2);
		}
    }
	add(result1r, result2r, resultr);	
	add(result1g, result2r, resultg);
	add(result1r, result2r, resultb);
	vector < Mat > channels = {resultb, resultg, resultr};
	Mat result;
	merge(channels, result);
	delete [] window;
	return result;
}

Mat sobel_filter(Mat image){
	int kernel1[3][3] = { {-1,0,1}						,{-2,0,2} 						,{-1,0,1} };
	int kernel2[3][3] = { {1,2,1}						,{0,0,0}						,{-1,-2,-1} };
	vector<Mat> bgr_channels;
	split(image, bgr_channels);

	int img_width = image.cols;
	int img_height = image.rows;

	int** window = new int* [3];
	Mat result1b = bgr_channels[0].clone(), result2b = bgr_channels[0].clone(), resultb;
	Mat result1g = bgr_channels[1].clone(), result2g = bgr_channels[1].clone(), resultg;
	Mat result1r = bgr_channels[2].clone(), result2r = bgr_channels[2].clone(), resultr;

	for (int i = 0; i < 3; i++)
		window[i] = new int[3];
	
	for (int imagex = 0; imagex < img_width; imagex++) {
		
        for (int imagey = 0; imagey < img_height ; imagey++) {
			fill_window(window, imagex, imagey, bgr_channels[0]);
			result1b.at<uchar>(imagey,imagex) = dot_product(window, kernel1);
			result2b.at<uchar>(imagey, imagex) = dot_product(window, kernel2);

			fill_window(window, imagex, imagey, bgr_channels[1]);
			result1g.at<uchar>(imagey, imagex) = dot_product(window, kernel1);
			result2g.at<uchar>(imagey, imagex) = dot_product(window, kernel2);

			fill_window(window, imagex, imagey, bgr_channels[2]);
			result1r.at<uchar>(imagey, imagex) = dot_product(window, kernel1);
			result2r.at<uchar>(imagey, imagex) = dot_product(window, kernel2);
		}
    }
	add(result1r, result2r, resultr);	
	add(result1g, result2r, resultg);
	add(result1r, result2r, resultb);
	vector < Mat > channels = {resultb, resultg, resultr};
	Mat result;
	merge(channels, result);
	delete [] window;
	return result;
}

Mat orange_filter(int img_width,int img_height, Mat image){
	 
	
	for (int x = 0; x<img_width;x++){
        for(int y =0 ;y<img_height;y++){
			// orange has a range of 255, >50 , _
			Vec3b & intensity = image.at<Vec3b>(y, x);
			float blue = intensity.val[0];
			float green = intensity.val[1];
			float red = intensity.val[2];
			
			if (red == 255 && green >= 50 && green < 200 && blue < 100)	{
				intensity[0] = 0;
				intensity[1] = 0;
				intensity[2] = 0;
			}
        }
    } 
	 return image;
}

int main (){

  int choice_fltr,choice_img,int_width,int_height;
  Mat image;
  string image_names[] = { "image-2","test","test2" , "test3" };
  string default_path = "D:\\Redirected Profile\\Documents\\projects\\git\\IPvisualstudio\\";
  cout<<"pick an operation:1 sobel filter, 2 sobel colored,3  orange color filter";
  cin>>choice_fltr;
  cout<<"enter image number :" ;
  cin >> choice_img;
   
  //load image 
  image = imread(default_path+image_names[choice_img]+".png");
  
  Mat result;

  int_width = image.cols;
  int_height = image.rows;

  if (choice_fltr == 1){
	  image = imread(default_path + image_names[choice_img] + ".png",IMREAD_GRAYSCALE);
	  result = sobel_filter(image);
  }

  else if (choice_fltr == 2){
	  result = sobel_filter_color(image);
  }
  else if (choice_fltr==3){ 
	  result = orange_filter(int_width, int_height, image);
  }
      
  imwrite(image_names[choice_img] + "out.png",result);
  imshow("result", result);
  waitKey(0);

  return 0;
}