#include "helpers.h"
#include <math.h>

typedef struct {
    int Avgr;
    int Avgg;
    int Avgb;
} BlurAvg;

typedef struct {
    int AvgR;
    int AvgG;
    int AvgB;
} edgesAvg;


BlurAvg find_blur_Avg(int height, int width, int pi, int pj, RGBTRIPLE image[height][width]);
edgesAvg find_edges_Avg(int height, int width, int pi, int pj, RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double tmpRed = 0;
    double tmpBlue = 0;
    double tmpGreen = 0;
    double tmpAvg = 0;
    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            tmpRed = image[i][j].rgbtRed;
            tmpBlue = image[i][j].rgbtBlue;
            tmpGreen = image[i][j].rgbtGreen;
            tmpAvg = (int)round((tmpRed+tmpBlue+tmpGreen)/3);
            image[i][j].rgbtRed = tmpAvg;
            image[i][j].rgbtBlue = tmpAvg;
            image[i][j].rgbtGreen = tmpAvg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int Red = 0;
    int Blue = 0;
    int Green = 0;
    if (width == 1)
    {
        return;
    }
    if (width % 2 == 0)
    {
        for (int i = 0; i < height ; i++)
        {
            for (int j = 0; j < width/2 ; j++)
            {
                Red = image[i][j].rgbtRed;
                Blue = image[i][j].rgbtBlue;
                Green = image[i][j].rgbtGreen;
                image[i][j].rgbtRed = image[i][width-1 - j].rgbtRed;
                image[i][j].rgbtBlue = image[i][width-1 - j].rgbtBlue;
                image[i][j].rgbtGreen = image[i][width-1 - j].rgbtGreen;
                image[i][width-1 - j].rgbtRed = Red;
                image[i][width-1 - j].rgbtBlue = Blue;
                image[i][width-1 - j].rgbtGreen = Green;
            }
        }
    }
    else
    {
        for (int i = 0; i < height ; i++)
        {
            for (int j = 0; j < (width-1)/2 ; j++)
            {
                Red = image[i][j].rgbtRed;
                Blue = image[i][j].rgbtBlue;
                Green = image[i][j].rgbtGreen;
                image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
                image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
                image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
                image[i][width-1 - j].rgbtRed = Red;
                image[i][width-1 - j].rgbtBlue = Blue;
                image[i][width-1 - j].rgbtGreen = Green;
            }
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BlurAvg avg = find_blur_Avg(height, width, i, j, image);
            temp[i][j].rgbtRed = avg.Avgr;
            temp[i][j].rgbtBlue = avg.Avgb;
            temp[i][j].rgbtGreen = avg.Avgg;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}


BlurAvg find_blur_Avg(int height, int width, int pi, int pj, RGBTRIPLE image[height][width])
{
    BlurAvg avg = {0, 0, 0};  // Initialize avg structure
    double count = 0;

    for (int i = -1; i <= 1; i++)
    {
        int ni = pi + i;
        if (ni < 0 || ni >= height) continue;

        for (int j = -1; j <= 1; j++)
        {
            int nj = pj + j;
            if (nj < 0 || nj >= width) continue;

            avg.Avgr += image[ni][nj].rgbtRed;
            avg.Avgb += image[ni][nj].rgbtBlue;
            avg.Avgg += image[ni][nj].rgbtGreen;
            count += 1.0;
        }
    }

    double finalr = round(avg.Avgr/count);
    double finalb = round(avg.Avgb/count);
    double finalg = round(avg.Avgg/count);

    avg.Avgr = (int)finalr;
    avg.Avgb = (int)finalb;
    avg.Avgg = (int)finalg;

    return avg;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edgesAvg avg = find_edges_Avg(height, width, i, j, image);
            temp[i][j].rgbtRed = avg.AvgR;
            temp[i][j].rgbtBlue = avg.AvgB;
            temp[i][j].rgbtGreen = avg.AvgG;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}

edgesAvg find_edges_Avg(int height, int width, int pi, int pj, RGBTRIPLE image[height][width])
{
    edgesAvg avg;
    int Gx[3][3] ={
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] ={
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    double gx_red = 0.0;
    double gy_red = 0.0;

    double gx_blue = 0.0;
    double gy_blue = 0.0;

    double gx_green = 0.0;
    double gy_green = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        int ni = pi + i;
        if (ni < 0 || ni >= height) continue;

        for (int j = -1; j <= 1; j++)
        {
            int nj = pj + j;
            if (nj < 0 || nj >= width) continue;

            gx_red += image[ni][nj].rgbtRed * Gx[i + 1][j + 1];
            gy_red += image[ni][nj].rgbtRed * Gy[i + 1][j + 1];

            gx_blue += image[ni][nj].rgbtBlue * Gx[i + 1][j + 1];
            gy_blue += image[ni][nj].rgbtBlue * Gy[i + 1][j + 1];

            gx_green += image[ni][nj].rgbtGreen * Gx[i + 1][j + 1];
            gy_green += image[ni][nj].rgbtGreen * Gy[i + 1][j + 1];
        }
    }

    int final_red = (int)round(sqrt(gx_red * gx_red + gy_red * gy_red));
    int final_blue = (int)round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));
    int final_green = (int)round(sqrt(gx_green * gx_green + gy_green * gy_green));

    avg.AvgR = (final_red > 255) ? 255 : final_red;
    avg.AvgB = (final_blue > 255) ? 255 : final_blue;
    avg.AvgG = (final_green > 255) ? 255 : final_green;

    return avg;
}
