#pragma comment(lib, "public/lib/libtesseract302.lib")
#pragma comment(lib, "lib/liblept.lib")
#include "public/include/tesseract/baseapi.h"
#include "public/include/tesseract/basedir.h"
#include "public/include/tesseract/strngs.h"
#include "include/leptonica/allheaders.h"




char* mystudyTesseract(char* filepath)
{

	tesseract::TessBaseAPI api;
	api.SetInputName(filepath);
	api.SetPageSegMode(tesseract::PSM_AUTO);
	int nRet = api.Init("tessdata", 0);
	if (nRet != 0)
	{

		return 0;
	}

	PIX *pix = pixRead(filepath);
	api.SetImage(pix);


	char *pdata = NULL;

	pdata = api.GetUTF8Text();

	
	pixDestroy(&pix);
	api.Clear();
	api.End();
	if (pdata == NULL)
		return " _NULL_ ";
	return pdata;
}

