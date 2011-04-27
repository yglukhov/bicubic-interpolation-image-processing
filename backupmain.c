/*#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"

#include "libimage.h"

int main(int argc, char *argv[])
{
  IplImage *img,*img2 =0;
    
  img=cvLoadImage("abc.bmp",3);
  
  if(!img)
  {
    printf("Khong the load anh %s\n",argv[1]);
    getch();
    exit(0);
  }
  
  
  img2 = bilinear(img,400,400);
  
  cvShowImage("BILI NEAR ", img2);

  cvWaitKey(0);

  cvReleaseImage(&img);
  cvReleaseImage(&img2);
    
  system("PAUSE");	
  getch();
  return 0;
}
*/
