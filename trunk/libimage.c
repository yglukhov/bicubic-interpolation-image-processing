#include <cv.h>
#include <highgui.h>
#include "libimage.h"


IplImage * createImage(int w, int h)
{
   IplImage * img = NULL;
   img = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
   return img;
}

IplImage * bilinear(IplImage *img, int newWidth, int newHeight)
{
    int w= newWidth;
    int h = newHeight;
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
IplImage * biquadric(IplImage *img, int newWidth, int newHeight)
{
    int w= newWidth;
    int h = newHeight;
    IplImage * img2 =0;
    img2 = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
    uchar * Data = img2->imageData;
    uchar * data = img->imageData;
    int a,b,c,d,x,y,index;
    float tx = (float)(img->width-1)/w;
    float ty = (float)(img->height-1)/h;
    float x_diff, y_diff;
    int i,j,k;
    float qx,qy;
    uchar Ca,Cb,Cc;
    float dx,dy;
     for(i=0; i<h; i++)
     for(j=0; j<w; j++)
     {
              x = (int)(tx * j);
              y = (int)(ty * i);
              
              x_diff = ((tx * j) -x);
              y_diff = ((ty * i) -y );
              
              qx=0.5*x_diff*x_diff;
              qy=0.5*y_diff*y_diff;
              
              dx=0.5*dx;
              dy=0.5*dy;
              
              index = y*img->widthStep + x*img->nChannels;
              for(k=0;k<3;k++)
              {
              Ca = data[(i-1)*img->widthStep + j*img->nChannels  +k] +  (data[(i-1)*img->widthStep + (j+1)*img->nChannels  +k]  +    
              data[(i-1)*img->widthStep + (j-1)*img->nChannels  +k])*x_diff +
              (data[(i-1)*img->widthStep + (j-1)*img->nChannels  +k]-2*data[(i-1)*img->widthStep + (j)*img->nChannels  +k] + data[(i-1)*img->widthStep + (j+1)*img->nChannels  +k] )*qx;                                  
              
              Cb = data[(i)*img->widthStep + j*img->nChannels  +k] +  data[(i)*img->widthStep + (j+1)*img->nChannels  +k]  +    
              data[(i-1)*img->widthStep + (j-1)*img->nChannels  +k]*x_diff +
              (data[(i-1)*img->widthStep + (j-1)*img->nChannels  +k]-2*data[(i-1)*img->widthStep + (j)*img->nChannels  +k] + data[(i-1)*img->widthStep + (j+1)*img->nChannels  +k] )*qx;                                  
              }              
     }
     return img2;
}

IplImage * bicubic(IplImage *img, int newWidth, int newHeight)
{
      
        IplImage *img2 = createImage(newWidth,newHeight);
        img2 = createImage(newWidth,newHeight);
        uchar * data = img->imageData;
        uchar * Data = img2->imageData;
        int a,b,c,d,index;
        uchar Ca,Cb,Cc;
        uchar C[5];
        uchar d0,d2,d3,a0,a1,a2,a3;
        int i,j,k,ii,jj;
        int x,y;
        float dx,dy;
        float tx,ty;
        
        tx = (float)img->width /newWidth ;
        ty =  (float)img->height / newHeight;
        printf("%d %d", newWidth, newHeight);
        
        for(i=0; i<newHeight; i++)
        for(j=0; j<newWidth; j++)
        {
                  // printf("%d : %d\n",i,j);
           x = (int)(tx*j);
           y =(int)(ty*i);
           
           dx= tx*j-x;
           dy=ty*i -y;
           
           
           index = y*img->widthStep + x*img->nChannels ;
           a = y*img->widthStep + (x+1)*img->nChannels ;
           b = (y+1)*img->widthStep + x*img->nChannels ;
           c = (y+1)*img->widthStep + (x+1)*img->nChannels ;

           for(k=0;k<3;k++)
           {
              for(jj=0;jj<=3;jj++)
              {
                                  
                 d0 = data[(y-1+jj)*img->widthStep + (x-1)*img->nChannels +k] - data[(y-1+jj)*img->widthStep + (x)*img->nChannels +k] ;
                 d2 = data[(y-1+jj)*img->widthStep + (x+1)*img->nChannels +k] - data[(y-1+jj)*img->widthStep + (x)*img->nChannels +k] ;
                 d3 = data[(y-1+jj)*img->widthStep + (x+2)*img->nChannels +k] - data[(y-1+jj)*img->widthStep + (x)*img->nChannels +k] ;
                 a0 = data[(y-1+jj)*img->widthStep + (x)*img->nChannels +k];
                 a1 =  -1.0/3*d0 + d2 -1.0/6*d3;
                 a2 = 1.0/2*d0 + 1.0/2*d2;
                 a3 = -1.0/6*d0 - 1.0/2*d2 + 1.0/6*d3;
                 C[jj] = a0 + a1*dx + a2*dx*dx + a3*dx*dx*dx;
                 
                 d0 = C[0]-C[1];
                 d2 = C[2]-C[1];
                 d3 = C[3]-C[1];
                 a0=C[1];
                 a1 =  -1.0/3*d0 + d2 -1.0/6*d3;
                 a2 = 1.0/2*d0 + 1.0/2*d2;
                 a3 = -1.0/6*d0 - 1.0/2*d2 + 1.0/6*d3;
                 Cc = a0 + a1*dy + a2*dy*dy + a3*dy*dy*dy;
                // if((int)Cc>255) Cc=255;
//                 if((int)Cc<0) Cc=0;
                 Data[i*img2->widthStep +j*img2->nChannels +k ] = Cc;
              }
           }
           
        }
        return img2;   
}

IplImage * bicubicBSpilne(IplImage *img, int newWidth, int newHeight)
{
}

IplImage *nearRest(IplImage *img, int newWidth, int newHeight)
{
        IplImage *img2;
        img2 = createImage(newWidth,newHeight);
        uchar * data = img->imageData;
        uchar * Data = img2->imageData;
        int i,j,k;
        int x,y;
        float tx,ty;
        uchar cc;
        tx = (float)(img->width-1) /newWidth ;
        ty =  (float)(img->height-1) / newHeight;
        
        for(i=0;i<newHeight;i++)
        for(j=0;j<newWidth;j++)
        {
           x = ceil(tx*j);
           y =ceil(ty*i);
           for(k=0;k<3;k++)
           {
             cc= data [y*img->widthStep + x*img->nChannels +k]; 
             Data[i*img2->widthStep + j*img2->nChannels +k]=cc;       
             }
        }
        return img2;
}
IplImage *bilinear1(IplImage *img, int newWidth, int newHeight)
{
        IplImage *img2 = createImage(newWidth,newHeight);
        img2 = createImage(newWidth,newHeight);
        uchar * data = img->imageData;
        uchar * Data = img2->imageData;
        int a,b,c,d,index;
        uchar Ca,Cb;
        int i,j,k;
        int x,y;
        float dx,dy;
        float tx,ty;
        
        tx = (float)img->width /newWidth ;
        ty =  (float)img->height / newHeight;
        
        for(i=0;i<newHeight;i++)
        for(j=0;j<newWidth;j++)
        {
           x = (int)(tx*j);
           y =(int)(ty*i);
           
           dx= tx*j-x;
           dy=ty*i -y;
           
           index = y*img->widthStep + x*img->nChannels ;
           a = y*img->widthStep + (x+1)*img->nChannels ;
           b = (y+1)*img->widthStep + x*img->nChannels ;
           c = (y+1)*img->widthStep + (x+1)*img->nChannels ;
            
           for(k=0;k<3;k++)
           {
              
              Data[i*img2->widthStep + j*img2->nChannels +k]  = 
              data[index+k]*(1-dx)*(1-dy) + data[a+k]*dx*(1-dy) + data[b+k]*(1-dx)*dy + data[c+k]*dx*dy; 
           }
           
        }
        return img2;
}
