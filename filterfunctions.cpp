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

	int img_width = image.cols;
	int img_height = image.rows;
	int temp;
	//cout << "x" << pixelx << "y" << pixely << endl;
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
			//cout << "Ux" << used_x << "Uy" << used_y <<" " <<window[i][j] << " ";
		}
		//cout << endl;
	}
	//cout << endl;
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
	int kernel1[3][3] = { {-1,0,1}
						,{-2,0,2} 
						,{-1,0,1} };
	int kernel2[3][3] = { {1,2,1}
						,{0,0,0}
						,{-1,-2,-1} };
	int img_width = image.cols;
	int img_height = image.rows;

	int** window = new int* [3];
	Mat result1 = image.clone(),result2 = image.clone(),result;
	
	for (int i = 0; i < 3; i++)
		window[i] = new int[3];
	vector<Mat> bgr_channels;
	split(image, bgr_channels);


	//apply sobel filter  3*3 matrix multiplication
	for (int imagex = 0; imagex < img_width; imagex++) {
		
        for (int imagey = 0; imagey < img_height ; imagey++) {
			//cout<<"loop top: row" << imagey <<"  colomn" <<imagex << endl;
			fill_window(window, imagex, imagey, image,0);
			result1.at<uchar>(imagey,imagex) = dot_product(window, kernel1);
			
			fill_window(window, imagex, imagey, image, 0);
			result2.at<uchar>(imagey, imagex) = dot_product(window, kernel2);
			}
    }
	add(result1, result2, result);
	delete [] window;
	return result;
}

Mat orange_filter(int img_width,int img_height, Mat image){
	 
	Mat result;

	
	int r=0, g=0, b=0;
	//fill rgb values 
     for (int x = 0; x<img_width;x++){
        for(int y =0 ;y<img_height;y++)
        {	// orange has a range of 255, >50 , _
			Vec3f intensity = image.at<Vec3f>(y, x);
			float blue = intensity.val[0];
			float green = intensity.val[1];
			float red = intensity.val[2];

			if (red == 255, green >= 50)
			{
				red = 0, green = 0, blue = 0;
				
				//move those to the mat result 
				result.at<Vec3f>(y, x)[0] = blue;
				result.at<Vec3f>(y, x)[1] = green;
				result.at<Vec3f>(y, x)[2] = red;
			}
            
        }
    } 
	 //memcpy(result.data, imagecopy, img_width * img_height * sizeof(int));
	 return result;
}

int main (){

  int choice_fltr,choice_img,int_width,int_height;
  Mat image;
  string image_names[] = { "image-2","test","test2" };
  string default_path = "D:\\Redirected Profile\\Documents\\projects\\git\\IPvisualstudio\\";
  cout<<"pick an operation:1 sobel filter, 2 orange color filter";
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

  else if (choice_fltr==2){ 
	  image = imread(default_path + image_names[choice_img] + ".png", IMREAD_GRAYSCALE); 
	  result = orange_filter(int_width, int_height, image);
  }
      
  imwrite(image_names[choice_img] + "out.png",result);
  imshow("result", result);
  waitKey(0);

  return 0;
}