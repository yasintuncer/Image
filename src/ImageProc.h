
#include "Channel.h"
#include "Color.h"
#include "Channel.h"
#include <stdint.h>
#include <cmath>
#include <execution>
#include <algorithm>

IMAGE_NAMESPACE_BEGIN

namespace ImageProc
{

    //--------------------------------------------------------------------------------------------------
    /* Interleave RGB data from R,G,B channels
     * @param R - pointer to red channel
     * @param G - pointer to green channel
     * @param B - pointer to blue channel
     * @param dest - pointer to destination buffer
     * @param count - number of pixels
     */
    void interLeaveRGBFromRGB(uint8_t *R, uint8_t *G, uint8_t *B, uint8_t *dest, unsigned int count);

    //--------------------------------------------------------------------------------------------------
    /* Interleave RGB data from C,M,Y,K channels
     * @param C - pointer to cyan channel
     * @param M - pointer to magenta channel
     * @param Y - pointer to yellow channel
     * @param K - pointer to black channel
     * @param dest - pointer to destination buffer
     * @param count - number of pixels
     */
    void interLeaveRGBFromCMYK(uint8_t *C, uint8_t *M, uint8_t *Y, uint8_t *K, uint8_t *dest, unsigned int count);

    //--------------------------------------------------------------------------------------------------
    /* Interleave RGB data from L,A,B channels
     * @param L - pointer to lightness channel
     * @param A - pointer to a channel
     * @param B - pointer to b channel
     * @param dest - pointer to destination buffer
     * @param count - number of pixels
     */
    void interleaveRGBFromLAB(uint8_t *L, uint8_t *A, uint8_t *B, uint8_t *dest, unsigned int count);

    //--------------------------------------------------------------------------------------------------
    /* Interleave RGB data from H,S,V channels
     * @param H - pointer to hue channel
     * @param S - pointer to saturation channel
     * @param V - pointer to value channel
     * @param dest - pointer to destination buffer
     * @param count - number of pixels
     */
    void interleaveRGBFromHSV(uint8_t *H, uint8_t *S, uint8_t *V, uint8_t *dest, unsigned int count);

    //--------------------------------------------------------------------------------------------------
    /* Interleave RGB data from multichannel
     * @param channels - pointer to array of channels
     * @param dest - pointer to destination buffer
     * @param channel_count - number of channels
     * @param count - number of pixels
     */
    void interleaveRGBFromMultichannel(Channel **channels, uint8_t *dest, unsigned int channel_count, unsigned int count);

    //--------------------------------------------------------------------------------------------------
    /* resize image
     * @param src - pointer to source image
     * @param dest - pointer to destination image
     * @param dest_width - width of destination image
     * @param dest_height - height of destination image
     * @param keep_aspect_ratio - keep aspect ratio of source image
     */
    int resizeImage(Image *src, Image *dest, unsigned int dest_width, unsigned int dest_height, bool keep_aspect_ratio);

}
IMAGE_NAMESPACE_END