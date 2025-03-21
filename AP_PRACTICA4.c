#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    IplImage* ImgDestino = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 3);
    // Always check if the program can find the image file
    if (!ImgOrigen) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("fundir", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window


    int fila, cc;
    for (int i = 0; i < 256; i++) {
        for (fila = 0; fila < ImgOrigen->height; fila++) {
            __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + fila * ImgOrigen->
                    widthStep);
            __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + fila * ImgDestino
                    ->widthStep);
            for (cc = 0; cc < ImgOrigen->widthStep; cc = cc + 16) {
                __m128i A = _mm_min_epu8(*pImgOrigen, _mm_set1_epi8(1));
                __m128i B = _mm_min_epu8(*pImgOrigen, *pImgDestino);
                *pImgDestino = _mm_adds_epu8(B, A);
                pImgDestino++;
                pImgOrigen++;
            }
        }
        cvShowImage("fundir", ImgDestino);
        cvWaitKey(1);
    }




    // crea y muestras las ventanas con las im genes


    // memory release for images before exiting the application

    cvReleaseImage(&ImgDestino);

    // Self-explanatory
    cvDestroyWindow("fundir");

    return EXIT_SUCCESS;
}