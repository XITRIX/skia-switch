/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkFontTypes.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPoint.h"
#include "include/core/SkRect.h"
#include "include/core/SkScalar.h"
#include "include/core/SkString.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkTypes.h"
#include "include/private/SkTemplates.h"
#include "include/private/base/SkTo.h"
#include "src/core/SkMatrixPriv.h"
#include "tools/ToolUtils.h"

#include <string.h>

class PerspTextGM : public skiagm::GM {
public:
    PerspTextGM() {
        this->setBGColor(0xFFFFFFFF);
    }

protected:

    SkString onShortName() override {
        return SkString("persptext");
    }

    SkISize onISize() override {
        return SkISize::Make(1024, 768);
    }

    void onDraw(SkCanvas* canvas) override {

        canvas->clear(0xffffffff);

        SkPaint paint;
        paint.setAntiAlias(true);

        SkFont font(ToolUtils::create_portable_typeface("serif", SkFontStyle()));
        font.setSubpixel(true);
        font.setSize(32);

        const char* text = "Hamburgefons";
        const size_t textLen = strlen(text);

        SkScalar textWidth = font.measureText(text, textLen, SkTextEncoding::kUTF8,
                                              nullptr, nullptr);
        SkScalar textHeight = font.getMetrics(nullptr);

        SkScalar x = 10, y = textHeight + 5.f;
        const int kSteps = 8;
        for (auto pm : {PerspMode::kX, PerspMode::kY, PerspMode::kXY}) {
            for (int i = 0; i < kSteps; ++i) {
                canvas->save();
                SkMatrix persp = SkMatrix::I();
                switch (pm) {
                    case PerspMode::kX:
                        persp.setPerspX(i*0.00025f/kSteps);
                        break;
                    case PerspMode::kY:
                        persp.setPerspY(i*0.0025f/kSteps);
                        break;
                    case PerspMode::kXY:
                        persp.setPerspX(i*-0.00025f/kSteps);
                        persp.setPerspY(i*-0.00125f/kSteps);
                        break;
                }
                persp = SkMatrix::Concat(persp, SkMatrix::Translate(-x, -y));
                persp = SkMatrix::Concat(SkMatrix::Translate(x, y), persp);
                canvas->concat(persp);

                paint.setColor(SK_ColorBLACK);
                canvas->drawSimpleText(text, textLen, SkTextEncoding::kUTF8, x, y, font, paint);

                y += textHeight + 5.f;
                canvas->restore();
            }

            x += textWidth + 10.f;
            y = textHeight + 5.f;
        }

    }

private:
    enum class PerspMode { kX, kY, kXY };
};

DEF_GM(return new PerspTextGM;)