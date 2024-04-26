// LAF Text Library
// Copyright (C) 2020-2024  Igara Studio S.A.
// Copyright (C) 2016-2017  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "text/freetype_font.h"

#include "base/string.h"
#include "ft/algorithm.h"
#include "gfx/point.h"
#include "gfx/size.h"
#include "text/font_metrics.h"
#include "text/typeface.h"

namespace text {

FreeTypeFont::FreeTypeFont(ft::Lib& lib,
                           const char* filename,
                           const int height)
  : m_face(lib.open(filename))
{
  if (m_face.isValid())
    m_face.setSize(height);
}

FreeTypeFont::~FreeTypeFont()
{
}

bool FreeTypeFont::isValid() const
{
  return m_face.isValid();
}

FontType FreeTypeFont::type()
{
  return FontType::FreeType;
}

TypefaceRef FreeTypeFont::typeface() const
{
  return nullptr;               // TODO impl
}

float FreeTypeFont::metrics(FontMetrics* metrics) const
{
  // TODO impl

  if (metrics) {
    FT_Face ftFace = m_face;

    float yscale =
      float(m_face->size->metrics.y_ppem)
      / float(m_face->units_per_EM);

    metrics->ascent = -float(ftFace->ascender) * yscale;
    metrics->descent = -float(ftFace->descender) * yscale;
    metrics->underlineThickness = float(ftFace->underline_thickness) * yscale;
    metrics->underlinePosition = -float(ftFace->underline_position +
                                        ftFace->underline_thickness/2) * yscale;
  }

  return float(m_face.height());
}

int FreeTypeFont::height() const
{
  return int(m_face.height());
}

int FreeTypeFont::textLength(const std::string& str) const
{
  return ft::calc_text_bounds(m_face, str).w;
}

float FreeTypeFont::measureText(const std::string& str,
                                gfx::RectF* outBounds,
                                const os::Paint*) const
{
  auto bounds = ft::calc_text_bounds(m_face, str);
  if (outBounds)
    *outBounds = bounds;
  return bounds.w;
}

bool FreeTypeFont::isScalable() const
{
  return true;
}

void FreeTypeFont::setSize(int size)
{
  m_face.setSize(size);
}

bool FreeTypeFont::antialias() const
{
  return m_face.antialias();
}

void FreeTypeFont::setAntialias(bool antialias)
{
  m_face.setAntialias(antialias);
}

bool FreeTypeFont::hasCodePoint(int codepoint) const
{
  return m_face.hasCodePoint(codepoint);
}

gfx::RectF FreeTypeFont::getGlyphBounds(GlyphID glyph) const
{
  return gfx::RectF();        // TODO impl
}

base::Ref<FreeTypeFont> FreeTypeFont::LoadFont(
  ft::Lib& lib,
  const char* filename,
  const int height)
{
  auto font = base::make_ref<FreeTypeFont>(lib, filename, height);
  if (!font->isValid())
    font.reset();             // delete font
  return font;
}

} // namespace text