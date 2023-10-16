#pragma once

#include "ppsspp_config.h"

#include <map>
#include "Common/Render/Text/draw_text.h"

#if defined(USE_SDL2_TTF_FONTCONFIG)
#include <fontconfig/fontconfig.h>
#endif

// SDL2_ttf's TTF_Font is a typedef of _TTF_Font.
struct _TTF_Font;

class TextDrawerSDL : public TextDrawer {
public:
	TextDrawerSDL(Draw::DrawContext *draw);
	~TextDrawerSDL();

	uint32_t SetFont(const char *fontName, int size, int flags) override;
	void SetFont(uint32_t fontHandle) override;  // Shortcut once you've set the font once.
	void MeasureString(const char *str, size_t len, float *w, float *h) override;
	void MeasureStringRect(const char *str, size_t len, const Bounds &bounds, float *w, float *h, int align = ALIGN_TOPLEFT) override;
	void DrawString(DrawBuffer &target, const char *str, float x, float y, uint32_t color, int align = ALIGN_TOPLEFT) override;
	void DrawStringBitmap(std::vector<uint8_t> &bitmapData, TextStringEntry &entry, Draw::DataFormat texFormat, const char *str, int align = ALIGN_TOPLEFT) override;
	// Use for housekeeping like throwing out old strings.
	void OncePerFrame() override;

protected:
	void ClearCache() override;
	void PrepareFallbackFonts();
	uint32_t CheckMissingGlyph(const std::string& text);
	bool FindFallbackFonts(uint32_t missingGlyph, int ptSize);

	uint32_t fontHash_;
	std::map<uint32_t, _TTF_Font *> fontMap_;

	std::map<CacheKey, std::unique_ptr<TextStringEntry>> cache_;
	std::map<CacheKey, std::unique_ptr<TextMeasureEntry>> sizeCache_;

	std::vector<std::pair<std::string, int>> fallbackFontPaths_; // path and font face index

#if defined(USE_SDL2_TTF_FONTCONFIG)
	FcConfig *config;
	std::map<std::pair<uint32_t, int>, _TTF_Font *> glyphMap_;
	std::map<std::pair<std::string, int>, _TTF_Font *> fallbackFontsMap_;
#else
	std::vector<_TTF_Font *> fallbackFonts_;
#endif

};
