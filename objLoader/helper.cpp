#include "helper.h"

//
//  bool readTGA(string filename, int &imageWidth, int &imageHeight, unsigned char* &imageData)
//
//  This function reads RGB (or RGBA) data from the Targa image file specified by filename.
//  It does not handle color-mapped images -- only RGB/RGBA images, although it correctly supports
//      both run-length encoding (RLE) for compression and image origins at the top- or bottom-left.
//
//  This function fills in imageWidth and imageHeight with the corresponding height and width of 
//      the image that it reads in from disk, and allocates imageData appropriately. It also sets
//      wasRGBA to true if the data contained an alpha channel and false if it did not. If the function
//      completes successfully and wasRGBA is true, then imageData is a pointer to valid image data
//      that is (imageWidth*imageHeight*4) bytes in size; if wasRBGA is false, it is (imageWidth*imageHeight*3)
//      bytes large.
//
//  If the function fails for any reason, the error message will be sent to stderr, the function will
//      return false, and imageData will be set to NULL (and deallocated, if the error occurred post-allocation).
//
//  The TGA file format, including the data layout with RLE, is based on a spec found at:
//      http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
//
void renderGLUTText(float x, float y, const char *str, mPoint color)
{
    //save the current state of every glEnable()/glDisable(); we'll mess
    //with a couple, but we can push before and pop afterwards so that it
    //goes back to the way it was once we're done here.
    glPushAttrib(GL_ENABLE_BIT);
   
    glDisable(GL_LIGHTING);     //disable these...
    glDisable(GL_TEXTURE_2D);   //... just in case
    glColor3f(color.x, color.y, color.z);
   
    //the character rendering function uses the 'raster poition' as an insertion point.
    glRasterPos2f(x,y);
    for(const char *c = str; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);       //you can change out GLUT_BITMAP_8_BY_13; check the man page for more details.
       
    glPopAttrib();
}

bool readTGA(const string filename, int &imageWidth, int &imageHeight, unsigned char* &imageData, bool &wasRGBA)
{
    FILE *fp = fopen(filename.c_str(), "rb");
    if(!fp)
    {
        fprintf(stderr, "Error: could not open TGA file: %s.", filename.c_str());
        imageData = NULL;
        return false;
    }
    
    //bunch of data fields in the file header that we read in en masse
    unsigned char idLength, colorMapType, imageType;
    unsigned short idxOfFirstColorMapEntry, countOfColorMapEntries;
    unsigned char numBitsPerColorMapEntry;
    unsigned short xCoordOfLowerLeft, yCoordOfLowerLeft;
    unsigned short width, height;
    unsigned char bytesPerPixel;
    unsigned char imageAttributeFlags;

    fread(&idLength, sizeof(unsigned char), 1, fp);
    fread(&colorMapType, sizeof(unsigned char), 1, fp);
    fread(&imageType, sizeof(unsigned char), 1, fp);
    fread(&idxOfFirstColorMapEntry, sizeof(unsigned short), 1, fp);
    fread(&countOfColorMapEntries, sizeof(unsigned short), 1, fp);
    fread(&numBitsPerColorMapEntry, sizeof(unsigned char), 1, fp);
    fread(&xCoordOfLowerLeft, sizeof(unsigned short), 1, fp);
    fread(&yCoordOfLowerLeft, sizeof(unsigned short), 1, fp);
    fread(&width, sizeof(unsigned short), 1, fp);
    fread(&height, sizeof(unsigned short), 1, fp);
    fread(&bytesPerPixel, sizeof(unsigned char), 1, fp);
    fread(&imageAttributeFlags, sizeof(unsigned char), 1, fp);

    //now check to make sure that we actually have the capability to read this file.
    if(colorMapType != 0)
    {
        fprintf(stderr, "Error: TGA file (%s) uses colormap instead of RGB/RGBA data; this is unsupported.\n", filename.c_str());
        imageData = NULL;
        return false;
    }

    if(imageType != 2 && imageType != 10)
    {
        fprintf(stderr, "Error: unspecified TGA type: %d. Only supports 2 (uncompressed RGB/A) and 10 (RLE, RGB/A).\n", imageType); 
        imageData = NULL;
        return false;
    }

    if(bytesPerPixel != 24 && bytesPerPixel != 32)
    {
        fprintf(stderr, "Error: unsupported image depth (%d bits per pixel). Only supports 24bpp and 32bpp.\n", bytesPerPixel);
        imageData = NULL;
        return false;
    }


    //set some helpful variables based on the header information:
    bool usingRLE = (imageType == 10);              //whether the file uses run-length encoding (compression)
    wasRGBA = (bytesPerPixel == 32);                //whether the file is RGB or RGBA (RGBA = 32bpp)
    bool topLeft = (imageAttributeFlags & 32);      //whether the origin is at the top-left or bottom-left


    //this should never happen, since we should never have a color map,
    //but just in case the data is setting around in there anyway... skip it.
    if(idLength != 0)
    {
        fseek(fp, idLength, SEEK_CUR);
    }


    //allocate our image data before we get started.
    imageWidth = width;
    imageHeight = height;
    imageData = new unsigned char[imageWidth*imageHeight*(wasRGBA?4:3)];



    //ok so we can assume at this point that there's no colormap, and 
    //consequently that the next part of the image is the actual image data.
    if(usingRLE)
    {
        //ok... the data comes in in packets, but we don't know how many of these there'll be.
        int numberOfPixelsRead = 0;
        unsigned char dummy;
        while(numberOfPixelsRead < imageWidth*imageHeight)
        {
            //alright let's read the packet header.
            fread(&dummy, sizeof(unsigned char), 1, fp);
            bool isRLEPacket = (dummy & 0x80);

            unsigned char theOtherBitsYesThatWasAPun = (dummy & 0x7F);
            if(isRLEPacket)
            {
                //the other bits (+1) gives the number of times we need to 
                //repeat the next real set of color values (RGB/A).
                unsigned char repeatedR, repeatedG, repeatedB, repeatedA;
                fread(&repeatedR, sizeof(unsigned char), 1, fp);
                fread(&repeatedG, sizeof(unsigned char), 1, fp);
                fread(&repeatedB, sizeof(unsigned char), 1, fp);
                if(wasRGBA) fread(&repeatedA, sizeof(unsigned char), 1, fp);

                //and go ahead and copy those into the new image. repetitively.
                for(int i = 0; i < ((int)theOtherBitsYesThatWasAPun+1); i++)
                {
                    int idx = numberOfPixelsRead * (wasRGBA ? 4 : 3);
                    imageData[idx+2] = repeatedR;
                    imageData[idx+1] = repeatedG;
                    imageData[idx+0] = repeatedB;
                    if(wasRGBA) imageData[idx+3] = repeatedA;

                    numberOfPixelsRead++;
                }
            } else {
                //the other bits (+1) gives the number of consecutive 
                //pixels we get to read in from the stream willy nilly.
                for(int i = 0; i < ((int)theOtherBitsYesThatWasAPun+1); i++)
                {
                    int idx = numberOfPixelsRead * (wasRGBA ? 4 : 3);
                    fread(&imageData[idx+2], sizeof(unsigned char), 1, fp);
                    fread(&imageData[idx+1], sizeof(unsigned char), 1, fp);
                    fread(&imageData[idx+0], sizeof(unsigned char), 1, fp);
                    if(wasRGBA) fread(&imageData[idx+3], sizeof(unsigned char), 1, fp);

                    numberOfPixelsRead++;
                }
            }
        }


        //and you know what? we're not going to have worried about flipping the image before
        //if its origin was in the bottom left or top left or whatever. flip it afterwards here if need be.
        if(!topLeft)
        {
            unsigned char *tempCopy = new unsigned char[imageWidth*imageHeight*(wasRGBA?4:3)];
            for(int i = 0; i < imageHeight; i++)
            {
                for(int j = 0; j < imageWidth; j++)
                {
                    int copyIdx = (i*imageWidth + j)*(wasRGBA?4:3);
                    int pullIdx = ((imageHeight - i - 1)*imageWidth + j)*(wasRGBA?4:3);
                    tempCopy[copyIdx+0] = imageData[pullIdx+0];
                    tempCopy[copyIdx+1] = imageData[pullIdx+1];
                    tempCopy[copyIdx+2] = imageData[pullIdx+2];
                    if(wasRGBA) tempCopy[copyIdx+3] = imageData[pullIdx+3];
                }
            }

            delete imageData;
            imageData = tempCopy;
        }

    } else {

        //uh well if we're not using run-length encoding, i guess we'll 
        //just try reading bytes in straight like a normal binary file.
        unsigned char byte1, byte2, byte3, maybeEvenByte4;
        for(int i = 0; i < imageHeight; i++)
        {
            for(int j = 0; j < imageWidth; j++)
            {
                int multiplierThing = wasRGBA ? 4 : 3;

                //read in the data from file...
                fread(&byte1, sizeof(unsigned char), 1, fp);
                fread(&byte2, sizeof(unsigned char), 1, fp);
                fread(&byte3, sizeof(unsigned char), 1, fp);
                if(wasRGBA) fread(&maybeEvenByte4, sizeof(unsigned char), 1, fp);

                //flip the vertical index if the origin is in the bottom-left.
                int wutHeight = topLeft ? i : (imageHeight - 1 - i);
                int idx = (wutHeight*imageWidth+j)*multiplierThing;

                //and load that image into file. seems to be BGR instead of RGB...
                imageData[idx+2] = byte1;
                imageData[idx+1] = byte2;
                imageData[idx+0] = byte3;
                if(wasRGBA) imageData[idx+3] = maybeEvenByte4;
            }
        }
    }

    fclose(fp);
    return true;
}

//  bool registerOpenGLTexture(unsigned char *imageData, int texWidth, int texHeight, GLuint &texHandle)
//
//  Attempts to register an image buffer with OpenGL. Sets the texture handle
//      appropriately upon success, and uses a number of default texturing
//      values. This function only provides the basics: just sets up this image
//      as an unrepeated 2D texture.
//
bool registerOpenGLTexture(unsigned char *imageData, const int texWidth, const int texHeight, GLuint &texHandle)
{
    //first off, get a real texture handle.
    glGenTextures(1, &texHandle);

    //make sure that future texture functions affect this handle
    glBindTexture(GL_TEXTURE_2D, texHandle);

    //set this texture's color to be multiplied by the surface colors --
    //  GL_MODULATE allows us to take advantage of OpenGL lighting
    //  GL_REPLACE uses only the color from the texture.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //set the minification ang magnification functions to be linear; not perfect
    //  but much better than nearest-texel (GL_NEAREST).
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Set the texture to repeat in S and T -- though it doesn't matter here
    //  because our texture coordinates are always in [0,0] to [1,1].
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //actually transfer the texture to the GPU!
    glTexImage2D(GL_TEXTURE_2D,                 //still working with 2D textures, obv.
                    0,                          //not using mipmapping, so this is the highest level.
                    GL_RGBA,                     //we're going to provide OpenGL with R, G, and B components...
                    texWidth,                   //...of this width...
                    texHeight,                  //...and this height.
                    0,                          //give it a border of 0 pixels (none)
                    GL_RGBA,                     //and store it, internally, as RGB (OpenGL will convert to floats between 0.0 and 1.0f)
                    GL_UNSIGNED_BYTE,           //this is the format our data is in, and finally,
                    imageData);                 //there's the data!

    //whoops! i guess this function can't fail. in an ideal world, there would
    //be checks with glGetError() that we could use to determine if this failed.
    return true;
}
//
//  vector<string> tokenizeString(string input, string delimiters)
//
//      This is a helper function to break a single string into std::vector
//  of strings, based on a given set of delimiter characters.
//
vector<string> tokenizeString(const string input, const string delimiters)
{
	if(input.size() == 0)
		return vector<string>();
	
	vector<string> retVec = vector<string>();
	size_t oldR = 0, r = 0; 
	
	//strip all delimiter characters from the front and end of the input string.
	string strippedInput;
	int lowerValidIndex = 0, upperValidIndex = input.size() - 1; 
	while((unsigned int)lowerValidIndex < input.size() && delimiters.find_first_of(input.at(lowerValidIndex), 0) != string::npos)
		lowerValidIndex++;
	
	while((unsigned int)upperValidIndex >= 0 && delimiters.find_first_of(input.at(upperValidIndex), 0) != string::npos)
		upperValidIndex--;
	
	//if the lowest valid index is higher than the highest valid index, they're all delimiters! return nothing.
	if((unsigned int)lowerValidIndex >= input.size() || upperValidIndex < 0 || lowerValidIndex >= upperValidIndex)
		return vector<string>();
	
	//remove the delimiters from the beginning and end of the string, if any.
	strippedInput = input.substr(lowerValidIndex, upperValidIndex-lowerValidIndex+1);
	
	//search for each instance of a delimiter character, and create a new token spanning
	//from the last valid character up to the delimiter character.
	while((r = strippedInput.find_first_of(delimiters, oldR)) != string::npos)
	{    
		if(oldR != r)           //but watch out for multiple consecutive delimiters!
			retVec.push_back(strippedInput.substr(oldR, r-oldR));
		oldR = r+1; 
	}    
	if(r != 0)
		retVec.push_back(strippedInput.substr(oldR, r-oldR));
	
	return retVec;

}
