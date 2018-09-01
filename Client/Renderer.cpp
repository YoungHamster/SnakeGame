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

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\A.png", &bitmaps[39])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\B.png", &bitmaps[40])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\C.png", &bitmaps[41])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\D.png", &bitmaps[42])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\E.png", &bitmaps[43])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\F.png", &bitmaps[44])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\G.png", &bitmaps[45])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\H.png", &bitmaps[46])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\I.png", &bitmaps[47])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\J.png", &bitmaps[48])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\K.png", &bitmaps[49])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\L.png", &bitmaps[50])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\M.png", &bitmaps[51])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\N.png", &bitmaps[52])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\O.png", &bitmaps[53])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\P.png", &bitmaps[54])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\Q.png", &bitmaps[55])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\R.png", &bitmaps[56])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\S.png", &bitmaps[57])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\T.png", &bitmaps[58])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\U.png", &bitmaps[59])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\V.png", &bitmaps[60])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\W.png", &bitmaps[61])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\X.png", &bitmaps[62])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\Y.png", &bitmaps[63])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\Z.png", &bitmaps[64])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\+.png", &bitmaps[65])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\-.png", &bitmaps[66])) return false;

	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\0.png", &bitmaps[67])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\1.png", &bitmaps[68])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\2.png", &bitmaps[69])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\3.png", &bitmaps[70])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\4.png", &bitmaps[71])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\5.png", &bitmaps[72])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\6.png", &bitmaps[73])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\7.png", &bitmaps[74])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\8.png", &bitmaps[75])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\9.png", &bitmaps[76])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\SPACE.png", &bitmaps[77])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\DOT.png", &bitmaps[78])) return false;
	if (!LoadID2D1Bitmap(L"textures\\NewTextures\\ASCII\\EXCLAMATION POINT.png", &bitmaps[79])) return false;

	for (int i = 0; i < BITMAPSNUMBER; i++)
	{
		bitmapsHWs[i] = bitmaps[i]->GetPixelSize().width;
	}

	return true;
}

void Renderer::RenderFrame(std::vector<PhysicalObject>& physics, std::vector<button>& buttons, bool menumode)
{
	float opacity;
	float backgroundR;
	float backgroundG;
	float backgroundB;
	if (menumode)
	{
		backgroundR = 0.5f;
		backgroundG = 0.4f;
		backgroundB = 0.4f;
		opacity = 0.3f;
	}
	else
	{
		backgroundR = 0.4f;
		backgroundG = 0.5f;
		backgroundB = 0.4f;
		opacity = 0.7f;
	}

	int renderWidth = this->rendertarget->GetPixelSize().width;
	int renderHeigth = this->rendertarget->GetPixelSize().height;
	int xFactor = renderWidth / GameFieldWidth;
	int yFactor = renderHeigth / GameFieldHeigth;
	BeginDraw();
	ClearScreen(backgroundR, backgroundG, backgroundB);

	SDL_Rect rect;
	for (int i = 0; i < physics.size(); i++)
	{
		if (physics[i].type != DEAD_SNAKE && physics[i].type < 40 && physics[i].type > 0)
		{
			rect.x = physics[i].borders.min.x * xFactor;
			rect.y = renderHeigth - physics[i].borders.max.y * yFactor;
			rect.w = (physics[i].borders.max.x - physics[i].borders.min.x) * xFactor;
			rect.h = (physics[i].borders.max.y - physics[i].borders.min.y) * yFactor;
			DrawBitmap(bitmaps[physics[i].type - 1], &rect, NULL, opacity);
		}
	}

	for (int i = 0; i < buttons.size(); i++)
	{
		RenderButton(buttons[i]);
	}
	EndDraw();
}

void Renderer::RenderFrame(char compressedPhysics[GAMEFIELDHEIGTH * GAMEFIELDWIDTH], std::vector<button>& buttons)
{
	float backgroundR = 0.4f;
	float backgroundG = 0.5f;
	float backgroundB = 0.4f;
	float opacity = 0.7f;
	int renderWidth = this->rendertarget->GetPixelSize().width;
	int renderHeigth = this->rendertarget->GetPixelSize().height;
	int xFactor = renderWidth / GameFieldWidth;
	int yFactor = renderHeigth / GameFieldHeigth;
	BeginDraw();
	ClearScreen(backgroundR, backgroundG, backgroundB);

	SDL_Rect rect;
	rect.w = xFactor;
	rect.h = yFactor;
	for (int i = 0; i < GAMEFIELDHEIGTH + 3; i++)
	{
		for (int j = 0; j < GAMEFIELDWIDTH; j++)
		{
			rect.x = j * xFactor;
			rect.y = renderHeigth - (i - 1) * yFactor;
			if (compressedPhysics[i * GAMEFIELDWIDTH + j] != 0)
			{
				DrawBitmap(bitmaps[compressedPhysics[i * GAMEFIELDWIDTH + j] - 1], &rect, NULL, opacity);
			}
		}
	}
	for (int i = 0; i < buttons.size(); i++)
	{
		RenderButton(buttons[i]);
	}
	EndDraw();
}

void Renderer::DrawBitmap(ID2D1Bitmap* bmp, SDL_Rect* rect, SDL_Rect* srcrect, float opacity)
{
	if (rect != NULL && srcrect != NULL)
	{
		rendertarget->DrawBitmap(
			bmp, // Bitmap
			D2D1::RectF(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h), // Destination rectangle
			opacity, // Opacity
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
			opacity, // Opacity
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
			opacity, // Opacity
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			D2D1::RectF(srcrect->x, srcrect->y, srcrect->x + srcrect->w, srcrect->y + srcrect->h) // Source rectangle
		);
		return;
	}
	rendertarget->DrawBitmap(
		bmp, // Bitmap
		D2D1::RectF(0, 0, bmp->GetPixelSize().width, bmp->GetPixelSize().height), // Destination rectangle
		opacity, // Opacity
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

void Renderer::DrawTextOnRend(const wchar_t* text, int Size, int minX, int minY)
{
	int DrawOffset = 0;
	int CurrentCharacterInBitmapsArrayID = 77;
	SDL_Rect rect;
	rect.y = minY;
	for (int i = 0; i < wstrlen(text); i++)
	{
		if (text[i] >= L'A' && text[i] <= L'Z')	{ CurrentCharacterInBitmapsArrayID = text[i] - 26; }
		if (text[i] == L'+') { CurrentCharacterInBitmapsArrayID = 65; }
		if (text[i] == L'-') { CurrentCharacterInBitmapsArrayID = 66; }
		if (text[i] >= L'0' && text[i] <= L'9')	{ CurrentCharacterInBitmapsArrayID = text[i] + 19; }
		if (text[i] == L' ') { CurrentCharacterInBitmapsArrayID = 77; }
		if (text[i] == L'.') { CurrentCharacterInBitmapsArrayID = 78; }
		if (text[i] == L'!') { CurrentCharacterInBitmapsArrayID = 79; }
		rect.x = minX + DrawOffset;
		rect.w = Size * bitmaps[CurrentCharacterInBitmapsArrayID]->GetPixelSize().width;
		rect.h = Size * bitmaps[CurrentCharacterInBitmapsArrayID]->GetPixelSize().height;
		DrawBitmap(bitmaps[CurrentCharacterInBitmapsArrayID], &rect, NULL, 0.9f);
		DrawOffset = DrawOffset + rect.w + Size;
	}
}

void Renderer::RenderButton(button button)
{
	if (!button.Enlarged)
	{
		if (button.Centered)
		{
			DrawTextOnRend(button.Text, button.Size, (rendertarget->GetPixelSize().width / 2) - CountTextWidth(button.Text, button.Size) / 2, button.ClickBox.min.y);
		}
		else
		{
			DrawTextOnRend(button.Text, button.Size, button.ClickBox.min.x, button.ClickBox.min.y);
		}
	}
	else
	{
		if (button.Centered)
		{
			DrawTextOnRend(button.Text, button.Size + 1, (rendertarget->GetPixelSize().width / 2) - CountTextWidth(button.Text, button.Size + 1) / 2, button.ClickBox.min.y);
		}
		else
		{
			DrawTextOnRend(button.Text, button.Size + 1, button.ClickBox.min.x, button.ClickBox.min.y);
		}
	}
}

int Renderer::CountTextWidth(const wchar_t* text, int Size)
{
	int Width = 0;
	int CurrentCharacterInBitmapsArrayID = 77;
	int currentW = 0;
	for (int i = 0; i < wstrlen(text); i++)
	{
		if (text[i] >= L'A' && text[i] <= L'Z') { CurrentCharacterInBitmapsArrayID = text[i] - 26; }
		if (text[i] == L'+') { CurrentCharacterInBitmapsArrayID = 65; }
		if (text[i] == L'-') { CurrentCharacterInBitmapsArrayID = 66; }
		if (text[i] >= L'0' && text[i] <= L'9') { CurrentCharacterInBitmapsArrayID = text[i] + 19; }
		if (text[i] == L' ') { CurrentCharacterInBitmapsArrayID = 77; }
		if (text[i] == L'.') { CurrentCharacterInBitmapsArrayID = 78; }
		if (text[i] == L'!') { CurrentCharacterInBitmapsArrayID = 79; }
		currentW = bitmapsHWs[CurrentCharacterInBitmapsArrayID] * Size;//bitmaps[CurrentCharacterInBitmapsArrayID]->GetPixelSize().width;
		Width = Width + currentW + Size;
	}
	return Width;
}

D2D1_SIZE_U Renderer::GetRenderTargetSize()
{
	return rendertarget->GetPixelSize();
}