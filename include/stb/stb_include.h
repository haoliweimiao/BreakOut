//
// @description: include stb_image.h helper file
// @author: von.wu
// @date: 2021-06-27 16:37:56
//

#ifndef STB_INCLUDE_H
#define STB_INCLUDE_H
// include something

// stb_image only can include once, use this header file to import
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef __cplusplus
extern "C"
{
#endif
    // your code

#ifdef __cplusplus
}
#endif
#endif // STB_INCLUDE_H