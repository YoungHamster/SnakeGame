#include "Renderer.h"

bool Renderer::Init(HWND windowHandle)
{

	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;
	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&rendertarget);
	if (res != S_OK) return false;

	this->WindowWidth = rect.right - rect.left;
	this->WindowHeigth = rect.bottom - rect.top;

	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeHeadUp.png", &bitmaps[0])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeHeadDown.png", &bitmaps[1])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeHeadLeft.png", &bitmaps[2])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeHeadRight.png", &bitmaps[3])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeBody.png", &bitmaps[4])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeTailUp.png", &bitmaps[5])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeTailDown.png", &bitmaps[6])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeTailLeft.png", &bitmaps[7])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\BlueSnakeTailRight.png", &bitmaps[8])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeHeadUp.png", &bitmaps[9])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeHeadDown.png", &bitmaps[10])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeHeadLeft.png", &bitmaps[11])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeHeadRight.png", &bitmaps[12])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeBody.png", &bitmaps[13])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeTailUp.png", &bitmaps[14])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeTailDown.png", &bitmaps[15])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeTailLeft.png", &bitmaps[16])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\GreenSnakeTailRight.png", &bitmaps[17])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeHeadUp.png", &bitmaps[18])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeHeadDown.png", &bitmaps[19])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeHeadLeft.png", &bitmaps[20])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeHeadRight.png", &bitmaps[21])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeBody.png", &bitmaps[22])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeTailUp.png", &bitmaps[23])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeTailDown.png", &bitmaps[24])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeTailLeft.png", &bitmaps[25])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\YellowSnakeTailRight.png", &bitmaps[26])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeHeadUp.png", &bitmaps[27])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeHeadDown.png", &bitmaps[28])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeHeadLeft.png", &bitmaps[29])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeHeadRight.png", &bitmaps[30])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeBody.png", &bitmaps[31])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeTailUp.png", &bitmaps[32])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeTailDown.png", &bitmaps[33])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeTailLeft.png", &bitmaps[34])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\VioletSnakeTailRight.png", &bitmaps[35])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\Red.png", &bitmaps[36])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\Apple.png", &bitmaps[37])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\Border.png", &bitmaps[38])) return false;

	return true;
}

void Renderer::RenderFrame(std::vector<PhysicalObject>& physics, int numberOfObjects)
{
	int renderHeigth = this->rendertarget->GetPixelSize().height;
	BeginDraw();
	ClearScreen(0.0f, 0.0f, 0.0f);

	SDL_Rect rect;
	SDL_Rect srcrect;
	for (int i = 0; i < numberOfObjects; i++)
	{
		if (physics[i].type != DEAD_SNAKE && physics[i].type < 40 && physics[i].type > 0)
		{
			rect.x = physics[i].borders.min.x * 20;
			rect.y = renderHeigth - physics[i].borders.max.y * 20;
			rect.w = (physics[i].borders.max.x - physics[i].borders.min.x) * 20;
			rect.h = (physics[i].borders.max.y - physics[i].borders.min.y) * 20;
			DrawBitmap(bitmaps[physics[i].type - 1], &rect, NULL);
		}
		
		//physics = physics + sizeOfSingleObject;
	}
	EndDraw();
}

void Renderer::DrawBitmap(ID2D1Bitmap* bmp, SDL_Rect* rect, SDL_Rect* srcrect)
{
	if (rect != NULL && srcrect != NULL)
	{
		rendertarget->DrawBitmap(
			bmp, // Bitmap
			D2D1::RectF(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h), // Destination rectangle
			1.0, // Opacity
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			D2D1::RectF(srcrect->x, srcrect->y, srcrect->x + srcrect->w, srcrect->y + srcrect->h) // Source rectangle
		);
		return;
	}
	if (srcrect == NULL)
	{
		rendertarget->DrawBitmap(
			bmp, // Bitmap
			D2D1::RectF(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h), // Destination rectangle
			1.0, // Opacity
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			D2D1::RectF(0, 0, bmp->GetPixelSize().width, bmp->GetPixelSize().height) // Source rectangle
		);
		return;
	}
	if (rect == NULL)
	{
		rendertarget->DrawBitmap(
			bmp, // Bitmap
			D2D1::RectF(0, 0, srcrect->w, srcrect->h), // Destination rectangle
			1.0, // Opacity
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			D2D1::RectF(srcrect->x, srcrect->y, srcrect->x + srcrect->w, srcrect->y + srcrect->h) // Source rectangle
		);
		return;
	}
	rendertarget->DrawBitmap(
		bmp, // Bitmap
		D2D1::RectF(0, 0, bmp->GetPixelSize().width, bmp->GetPixelSize().height), // Destination rectangle
		1.0, // Opacity
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0, 0, bmp->GetPixelSize().width, bmp->GetPixelSize().height) // Source rectangle
	);
}

void Renderer::ClearScreen(float r, float g, float b)
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}

bool Renderer::LoadID2D1Bitmap(LPCWSTR filename, ID2D1Bitmap **ppBitmap)
{
	//this->gfx = gfx;
	*ppBitmap = NULL;
	HRESULT hr;

	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		//CLSID_WICPngDecoder,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory);

	if (hr != S_OK) return false;

	// Create a decoder
	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(
		filename,		// The file name
		NULL,			// No preffered vendor
		GENERIC_READ,	// Reading the file, not writing
		WICDecodeMetadataCacheOnLoad, // Cache on load*
		&wicDecoder);	// The decoder we're making

	if (hr != S_OK) return false;

						// Read a frame from the image
	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame);

	if (hr != S_OK) return false;

	// Create a converter
	IWICFormatConverter *wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	if (hr != S_OK) return false;

	// Setup converter!
	hr = wicConverter->Initialize(
		wicFrame,	// Frame
		GUID_WICPixelFormat32bppPBGRA, // Pixel format
		WICBitmapDitherTypeNone,	// Irrelevant
		NULL,		// No palette needed, irrelevant
		0.0,		// Alpha transparency, irrelevant
		WICBitmapPaletteTypeCustom); // Irrelevant

	if (hr != S_OK) return false;

									 // Use a converter to create an D2D1Bitmap
	hr = rendertarget->CreateBitmapFromWicBitmap(
		wicConverter, // Converter
		NULL,		  // D2D1_BITMAP_PROPERTIES
		ppBitmap);		  // Destination D2D1 bitmap

	if (hr != S_OK) return false;

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();

	return true;
}