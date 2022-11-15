// NOT DONE YET - code not finished

#include "helpers.h"
#include "math.h"

int max(float n)
{
    if (n > 255)
    {
        return 255;
    }

    return round(n);
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // We need to get the average colour value.
            // Ensure they are floats to prevent integer division.
            float r = image[h][w].rgbtRed;
            float g = image[h][w].rgbtGreen;
            float b = image[h][w].rgbtBlue;
            int value = round((r + b + g) / 3);

            // Assign the new value to the pixel.
            image[h][w].rgbtRed = value;
            image[h][w].rgbtGreen = value;
            image[h][w].rgbtBlue = value;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
        for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float r = image[h][w].rgbtRed;
            float g = image[h][w].rgbtGreen;
            float b = image[h][w].rgbtBlue;

            // Assign the new value to the pixel.
            image[h][w].rgbtRed = max(.393 * r + .769 * g + .189 * b);
            image[h][w].rgbtGreen = max(.349 * r + .686 * g + .168 * b);
            image[h][w].rgbtBlue = max(.272 * r + .534 * g + .131 * b);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < round(width / 2); w++)
        {
            RGBTRIPLE buffer = image[h][w];
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create copy of image into buffer
    RGBTRIPLE buffer[height][width];

    // Iterate over the original image, pixel for pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Set a counter and floats for rgb: to prevent integer division.
            int c = 0;
            float r = 0;
            float b = 0;
            float g = 0;

            // Check relative vertical positioning.
            for (int vp = -1; vp < 2; vp++)
            {
                // Skip over heights outside image boundary.
                if (h + vp < 0 || h + vp > height - 1)
                {
                    continue;
                }

                for (int hp = -1; hp < 2; hp++)
                {
                    // Skip over widths outside image boundary.
                    if (w + hp < 0 || w + hp > width -1)
                    {
                        continue;
                    }

                    // Write values to the temporary value
                    r += image[h + vp][w + hp].rgbtRed;
                    g += image[h + vp][w + hp].rgbtGreen;
                    b += image[h + vp][w + hp].rgbtBlue;
                    c++;
                }
            }

            buffer[h][w].rgbtRed = round(r / c);
            buffer[h][w].rgbtGreen = round(g / c);
            buffer[h][w].rgbtBlue = round(b / c);
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = buffer[h][w];
        }
    }

    return;
}
