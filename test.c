/*#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

const double PI = 3.14159278;

IplImage * xoay(IplImage *img, float goc)
{ 
  
  int i,j,k;
  uchar tmp;
  int height, width, step, channels;
  uchar * data;
  IplImage *imgTemp;
  int x0,y0,x2,y2, Xmax=0, Xmin=0,Ymax=0,Ymin=0;
  
  // 
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  
  // phan tu xoay
  x0 = height/2;
  y0= width/2; 
  
  goc=(float)goc/180;
  goc=(float)goc*PI;
  int w,h,dx=0,dy=0;
  w=0;
  h=0;
  

  for(i=0; i<height; i++)  for(j=0;j<width; j++)
      {
        x2 = cos(goc)*((i)-x0) - sin(goc)*((j)-y0) + x0;
        y2 = sin(goc)*((i)-x0) + cos(goc)*((j)-y0) + y0;
        if(x2>Xmax) Xmax=x2;
        if(x2<Xmin) Xmin=x2;
        if(y2>Ymax) Ymax=y2;
        if(y2<Ymin) Ymin=y2;
      }
      printf("%d :%d :%d :%d",Xmax,Xmin,Ymax,Ymin);
  h= Xmax-Xmin;
  w=Ymax-Ymin;
  dy=(w-width)/2;
  dx=(h-height)/2;
  
  x0 = h/2;
  y0= w/2; 
  
  imgTemp = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3);  
  for(i=0; i<h; i++)  for(j=0;j<w; j++)
  {  
      x2 = cos(goc)*((i)-x0) - sin(goc)*((j)-y0) + x0;
      y2 = sin(goc)*((i)-x0) + cos(goc)*((j)-y0) + y0;
      x2-=dx;
      y2-=dy;
      if((x2>=0) && (x2<height) && (y2>=0) && (y2<width))
      for(k=0; k<channels; k++)    
      imgTemp->imageData[(i)*imgTemp->widthStep + (j)*imgTemp->nChannels + k] = img->imageData[x2*step + y2*channels + k] ;    
  }  
  return imgTemp;
}
// BICUBIC
float P(float x)
{
      return (x>0) ? x : 0; 
}

float R(float x)
{
      return (1/6*pow(P(x+2),3) - 4*pow(P(x+1),3) + 6*pow(P(x),3) - 4*pow(P(x-1),3));
}
IplImage *bicubic(IplImage *img,int w, int h)
{
  int dx,dy;
  int x,y;
  int tx, ty;
  int i,j,m,n;
  IplImage *tmp=0;
  
  tmp = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
  
  tx = w/img->width;
  ty = h/img->height;
  printf("%d %d",tx,ty);
  
  for(i=0; i< img->width; i++)
  for(j=0; j< img->height; j++)
  {
    x = i*tx;
    y = j*ty;
    
    dx = x-i;
    dy = y-j;
    for(m=-1;m<=2;m++)
    for(n=-1;n<=2;n++)
    {
    tmp->imageData[ x*tmp->widthStep + y*tmp->nChannels] += img->imageData[ (i+m)*img->widthStep + (j+n)*img->nChannels]*R(m-dx)*R(dy-n);
    tmp->imageData[ x*tmp->widthStep + y*tmp->nChannels + 1] += img->imageData[ (i+m)*img->widthStep + (j+n)*img->nChannels +1]*R(m-dx)*R(dy-n);
    tmp->imageData[ x*tmp->widthStep + y*tmp->nChannels + 2] += img->imageData[ (i+m)*img->widthStep + (j+n)*img->nChannels +2]*R(m-dx)*R(dy-n);    
    }
  }
  
  
  
  return tmp;
}




// BICUBIC

IplImage * nearest(IplImage *img, int w, int h)
{
         IplImage *img2=0;
         
         img2 = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
         
         int i,j,k;
         double tx = (double)img->width/w;
         double ty = (double)img->height/ h;
         double px,py;

         for(i=0; i<h; i++)
         for(j=0; j<w; j++)
         {
           px = ceil(j*tx);
           py = ceil(i*ty);
//           printf("%f %f\n",px,py);
         //  k=0;
           for(k=0; k<3; k++)
           img2->imageData[i*img2->widthStep + j*img2->nChannels +k] =  img->imageData[(int)py*img->widthStep + (int)px*img->nChannels +k] ;
//           img2->imageData[i*img2->widthStep + j*img2->nChannels +k] =  img->imageData[i*img->widthStep + j*img->nChannels +k] ;
         }
         return img2;         
}

// BILINEAR

IplImage * bilinear (IplImage *img, int w, int h)
{
         IplImage * img2 =0;
         img2 = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
         uchar * Data = img2->imageData;
         uchar * data = img->imageData;
         int a,b,c,d,x,y,index;
         float tx = (float)(img->width-1)/w;
         float ty = (float)(img->height-1)/h;
         float x_diff, y_diff;
         int i,j,k;

         for(i=0; i<h; i++)
         for(j=0; j<w; j++)
         {
                  x = (int)(tx * j);
                  y = (int)(ty * i);
                  
                  x_diff = ((tx * j) -x);
                  y_diff = ((ty * i) -y );

                  index = y*img->widthStep + x*img->nChannels;
                  // cac diem lan can
                  a = (int)index;
                  b = (int)(index + img->nChannels);                  
                  c = (int)(index + img->widthStep);
                  d = (int)(index + img->widthStep + img->nChannels);
                  
                  for(k=0; k<3; k++)
                  Data[i*img2->widthStep + j*img2->nChannels  +k] = 
                  data[a+k]*(1-x_diff)*(1-y_diff)
                  +data[b+k]*(1-y_diff)*(x_diff)
                  +data[c+k]*(y_diff)*(1-x_diff)
                  +data[d+k]*(y_diff)*(x_diff);
         }
         return img2;
}
/*
int main(int argc, char *argv[])
{
  IplImage* img = 0; 
  IplImage* img2 = 0, *img3; 
  int height,width,step,channels;
  uchar *data;
  uchar tmp;
  int i,j,k;
    
  img=cvLoadImage(argv[1],3);
  
  if(!img)
  {
    printf("Khong the load anh %s\n",argv[1]);
    getch();
    exit(0);
  }
  
  // thuc hien phep xoay va bien doi

//  img3=xoay(img,45); 
  img3 = bilinear(img, 400, 400);
  img2 = nearest(img, 400, 400);
  //img3 = bicubic(img,500,500);
  
  
  
  // show image
  cvShowImage("BILI NEAR ", img3 );
  cvShowImage("NEAREST ", img2 );

  cvWaitKey(0);

  cvReleaseImage(&img);
  cvReleaseImage(&img2);
  cvReleaseImage(&img3);
  
  getch();
  
}
*/
