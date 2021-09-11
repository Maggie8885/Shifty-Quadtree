#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

/* Your code here!! */
    long width = im.width();
    long height = im.height();

    //set up 2D arrays
    sumRed = vector<vector<long>>(width,vector<long>(height));
    sumGreen = vector<vector<long>>(width,vector<long>(height));
    sumBlue = vector<vector<long>>(width,vector<long>(height));
    sumsqRed = vector<vector<long>>(width,vector<long>(height));
    sumsqGreen = vector<vector<long>>(width,vector<long>(height));
    sumsqBlue = vector<vector<long>>(width,vector<long>(height));

    //set sums to each color array
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            if (x == 0){
                if (y == 0){
                    //(x,y) = (0,0)
                    //get r,g,b at (0,0)
                    long red = im.getPixel(x,y)->r;
                    long green = im.getPixel(x,y)->g;
                    long blue = im.getPixel(x,y)->b;
                    //set r,g,b val to arrays
                    sumRed[x][y] = red;
                    sumGreen[x][y] = green;
                    sumBlue[x][y] = blue;
                    sumsqRed[x][y] = red * red;
                    sumsqGreen[x][y] = green * green;
                    sumsqBlue[x][y] = blue * blue;
                } else {
                    //(x,y) = (0,y)
                    //get r,g,b at (0,y)
                    long red = im.getPixel(x,y)->r;
                    long green = im.getPixel(x,y)->g;
                    long blue = im.getPixel(x,y)->b;
                    //set r,g,b val to arrays
                    sumRed[x][y] = red + sumRed[x][y-1];
                    sumGreen[x][y] = green + sumGreen[x][y-1];
                    sumBlue[x][y] = blue + sumBlue[x][y-1];
                    sumsqRed[x][y] = red * red + sumsqRed[x][y-1];
                    sumsqGreen[x][y] = green * green + sumsqGreen[x][y-1];
                    sumsqBlue[x][y] = blue * blue + sumsqBlue[x][y-1];
                }
            } else {
                if (y == 0){
                    //(x,y) = (x,0)
                    //get r,g,b at (x,0)
                    long red = im.getPixel(x,y)->r;
                    long green = im.getPixel(x,y)->g;
                    long blue = im.getPixel(x,y)->b;
                    //set r,g,b val to arrays
                    sumRed[x][y] = red + sumRed[x-1][y];
                    sumGreen[x][y] = green + sumGreen[x-1][y];
                    sumBlue[x][y] = blue + sumBlue[x-1][y];
                    sumsqRed[x][y] = red * red + sumsqRed[x-1][y];
                    sumsqGreen[x][y] = green * green + sumsqGreen[x-1][y];
                    sumsqBlue[x][y] = blue * blue + sumsqBlue[x-1][y];

                } else {
                    //(x,y) = (x,y)
                    //get r,g,b at (x,y)
                    long red = im.getPixel(x,y)->r;
                    long green = im.getPixel(x,y)->g;
                    long blue = im.getPixel(x,y)->b;
                    //set r,g,b val to arrays
                    sumRed[x][y] = red + sumRed[x-1][y]+sumRed[x][y-1]-sumRed[x-1][y-1];
                    sumGreen[x][y] = green + sumGreen[x-1][y]+sumGreen[x][y-1]-sumGreen[x-1][y-1];
                    sumBlue[x][y] = blue + sumBlue[x-1][y]+sumBlue[x][y-1]-sumBlue[x-1][y-1];
                    sumsqRed[x][y] = red * red + sumsqRed[x-1][y]+sumsqRed[x][y-1]-sumsqRed[x-1][y-1];
                    sumsqGreen[x][y] = green * green + sumsqGreen[x-1][y]+sumsqGreen[x][y-1]-sumsqGreen[x-1][y-1];
                    sumsqBlue[x][y] = blue * blue + sumsqBlue[x-1][y]+sumsqBlue[x][y-1]-sumsqBlue[x-1][y-1];
                }
            }
        }
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    long red;
    long green;
    long blue;

    int ul_first = ul.first;
    int ul_second = ul.second;

    if (channel == 'r'){
      //color = red
      if (ul_first == 0){
         if (ul_second == 0){
             //ul at (0,0)
             red = sumRed[ul_first+w-1][ul_second+h-1];
             return  red;
            } else {
             //ul at (0,y)
             red = sumRed[ul_first+w-1][ul_second+h-1]-sumRed[ul_first+w-1][ul_second-1];
             return  red;
         }
        } else {
          if (ul_second == 0){
              //ul at (x,0)
              red = sumRed[ul_first+w-1][ul_second+h-1] - sumRed[ul_first-1][ul_second+h-1];
              return red;
          } else {
              //ul at (x,y)
              red = sumRed[ul_first+w-1][ul_second+h-1]+sumRed[ul_first-1][ul_second-1]-sumRed[ul_first-1][ul_second+h-1]-sumRed[ul_first+w-1][ul_second-1];
              return red;
          }
      }
    } else if (channel == 'g'){
        //color = green
        if (ul_first == 0){
            if (ul_second == 0){
                //ul at (0,0)
                green = sumGreen[ul_first+w-1][ul_second+h-1];
                return  green;
            } else {
                //ul at (0,y)
                green = sumGreen[ul_first+w-1][ul_second+h-1]-sumGreen[ul_first+w-1][ul_second-1];
                return  green;
            }
        } else {
            if (ul_second == 0){
                //ul at (x,0)
                green = sumGreen[ul_first+w-1][ul_second+h-1] - sumGreen[ul_first-1][ul_second+h-1];
                return green;
            } else {
                //ul at (x,y)
                green = sumGreen[ul_first+w-1][ul_second+h-1]+sumGreen[ul_first-1][ul_second-1]-sumGreen[ul_first-1][ul_second+h-1]-sumGreen[ul_first+w-1][ul_second-1];
                return green;
            }
        }
    } else {
        //color = blue
        if (ul_first == 0){
            if (ul_second == 0){
                //ul at (0,0)
                blue = sumBlue[ul_first+w-1][ul_second+h-1];
                return  blue;
            } else {
                //ul at (0,y)
                blue = sumBlue[ul_first+w-1][ul_second+h-1]-sumBlue[ul_first+w-1][ul_second-1];
                return  blue;
            }
        } else {
            if (ul_second == 0){
                //ul at (x,0)
                blue = sumBlue[ul_first+w-1][ul_second+h-1] - sumBlue[ul_first-1][ul_second+h-1];
                return blue;
            } else {
                //ul at (x,y)
                blue = sumBlue[ul_first+w-1][ul_second+h-1]+sumBlue[ul_first-1][ul_second-1]-sumBlue[ul_first-1][ul_second+h-1]-sumBlue[ul_first+w-1][ul_second-1];
                return blue;
            }
        }
    }
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    long red;
    long green;
    long blue;

    int ul_first = ul.first;
    int ul_second = ul.second;

    if (channel == 'r'){
        //color = red
        if (ul_first == 0){
            if (ul_second == 0){
                //ul at (0,0)
                red = sumsqRed[ul_first+w-1][ul_second+h-1];
                return  red;
            } else {
                //ul at (0,y)
                red = sumsqRed[ul_first+w-1][ul_second+h-1]-sumsqRed[ul_first+w-1][ul_second-1];
                return  red;
            }
        } else {
            if (ul_second == 0){
                //ul at (x,0)
                red = sumsqRed[ul_first+w-1][ul_second+h-1] - sumsqRed[ul_first-1][ul_second+h-1];
                return red;
            } else {
                //ul at (x,y)
                red = sumsqRed[ul_first+w-1][ul_second+h-1]+sumsqRed[ul_first-1][ul_second-1]-sumsqRed[ul_first-1][ul_second+h-1]-sumsqRed[ul_first+w-1][ul_second-1];
                return red;
            }
        }
    } else if (channel == 'g'){
        //color = green
        if (ul_first == 0){
            if (ul_second == 0){
                //ul at (0,0)
                green = sumsqGreen[ul_first+w-1][ul_second+h-1];
                return  green;
            } else {
                //ul at (0,y)
                green = sumsqGreen[ul_first+w-1][ul_second+h-1]-sumsqGreen[ul_first+w-1][ul_second-1];
                return  green;
            }
        } else {
            if (ul_second == 0){
                //ul at (x,0)
                green = sumsqGreen[ul_first+w-1][ul_second+h-1] - sumsqGreen[ul_first-1][ul_second+h-1];
                return green;
            } else {
                //ul at (x,y)
                green = sumsqGreen[ul_first+w-1][ul_second+h-1]+sumsqGreen[ul_first-1][ul_second-1]-sumsqGreen[ul_first-1][ul_second+h-1]-sumsqGreen[ul_first+w-1][ul_second-1];
                return green;
            }
        }
    } else {
        //color = blue
        if (ul_first == 0){
            if (ul_second == 0){
                //ul at (0,0)
                blue = sumsqBlue[ul_first+w-1][ul_second+h-1];
                return  blue;
            } else {
                //ul at (0,y)
                blue = sumsqBlue[ul_first+w-1][ul_second+h-1]-sumsqBlue[ul_first+w-1][ul_second-1];
                return  blue;
            }
        } else {
            if (ul_second == 0){
                //ul at (x,0)
                blue = sumsqBlue[ul_first+w-1][ul_second+h-1] - sumsqBlue[ul_first-1][ul_second+h-1];
                return blue;
            } else {
                //ul at (x,y)
                blue = sumsqBlue[ul_first+w-1][ul_second+h-1]+sumsqBlue[ul_first-1][ul_second-1]-sumsqBlue[ul_first-1][ul_second+h-1]-sumsqBlue[ul_first+w-1][ul_second-1];
                return blue;
            }
        }
    }
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
    //get sum square for r,b,g
    double red_sq = (double)getSumSq('r',ul,w,h);
    double green_sq = (double)getSumSq('g',ul,w,h);
    double blue_sq = (double)getSumSq('b',ul,w,h);

    //get get sum for r,g,b
    double red_sum = (double)getSum('r',ul,w,h);
    double green_sum = (double)getSum('g',ul,w,h);
    double blue_sum = (double)getSum('b',ul,w,h);

    //area of rectangle
    double area = (double)((double)w*(double)h);

    //calculate var for r,g,b
    double red_var = (double)red_sq - (double)((double)red_sum * (double)red_sum) /(double)area;
    double green_var = (double)green_sq - (double)((double)green_sum * (double)green_sum) / (double)area;
    double blue_var = (double)blue_sq - (double)((double)blue_sum * (double)blue_sum) / (double)area;

    //sum of var of r,g,b
    return red_var + green_var + blue_var;


}
        
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */

    //get sum of r, g, b
    double red_sum = (double)getSum('r',ul,w,h);
    double green_sum = (double)getSum('g',ul,w,h);
    double blue_sum = (double)getSum('b',ul,w,h);

    //calculate avg val of r, g, b
    double  red_avg = (double)red_sum / (double)((double)w*(double)h);
    double green_avg = (double)green_sum / (double)((double)w*(double)h);
    double blue_avg = (double)blue_sum / (double)((double)w*(double)h);

    //form a new pixel with avg r, g, b
    RGBAPixel p = RGBAPixel(red_avg,green_avg,blue_avg);
    return p;

}
