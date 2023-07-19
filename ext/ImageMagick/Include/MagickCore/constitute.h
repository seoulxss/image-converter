/*
  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization
  dedicated to making software imaging solutions freely available.
  
  You may not use this file except in compliance with the License.  You may
  obtain a copy of the License at
  
    https://imagemagick.org/script/license.php
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  MagickCore image constitute methods.
*/
#ifndef MAGICKCORE_CONSTITUTE_H
#define MAGICKCORE_CONSTITUTE_H

#include "MagickCore/pixel.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport Image
  *ConstituteImage(const size_t,const size_t,const char *,const StorageType,
    const void *,ExceptionInfo *),
  *PingImage(const ImageInfo *,ExceptionInfo *),
  *PingImages(ImageInfo *,const char *,ExceptionInfo *),
  *ReadImage(const ImageInfo *,ExceptionInfo *),
  *ReadImages(ImageInfo *,const char *,ExceptionInfo *),
  *ReadInlineImage(const ImageInfo *,const char *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  WriteImage(const ImageInfo *,Image *,ExceptionInfo *),
  WriteImages(const ImageInfo *,Image *,const char *,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
