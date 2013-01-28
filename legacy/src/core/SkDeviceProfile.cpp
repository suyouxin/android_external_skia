

#include "SkDeviceProfile.h"

#define DEFAULT_GAMMAEXP        2.2
#define DEFAULT_CONTRASTSCALE   0.5
#define DEFAULT_LCDCONFIG       SkDeviceProfile::kNone_LCDConfig
#define DEFAULT_FONTHINTLEVEL   SkDeviceProfile::kSlight_FontHintLevel

static float pin(float value, float min, float max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
    return value;
}

SkDeviceProfile::SkDeviceProfile(float gammaExp, float contrast,
                                 LCDConfig config, FontHintLevel level) {
    fGammaExponent = pin(gammaExp, 0, 10);
    fContrastScale = pin(contrast, 0, 1);
    fLCDConfig = config;
    fFontHintLevel = level;
}

void SkDeviceProfile::generateTableForLuminanceByte(U8CPU lumByte,
                                                    uint8_t table[256]) const {
}

///////////////////////////////////////////////////////////////////////////////

SkDeviceProfile* SkDeviceProfile::Create(float gammaExp,
                                         float contrast,
                                         LCDConfig config,
                                         FontHintLevel level) {
    return SkNEW_ARGS(SkDeviceProfile, (gammaExp, contrast, config, level));
}

static SkMutex gMutex;
static SkDeviceProfile* gDefaultProfile;
static SkDeviceProfile* gGlobalProfile;

SkDeviceProfile* SkDeviceProfile::GetDefault() {
    SkAutoMutexAcquire amc(gMutex);

    if (NULL == gDefaultProfile) {
        gDefaultProfile = SkDeviceProfile::Create(DEFAULT_GAMMAEXP,
                                                  DEFAULT_CONTRASTSCALE,
                                                  DEFAULT_LCDCONFIG,
                                                  DEFAULT_FONTHINTLEVEL);
    }
    return gDefaultProfile;
}

SkDeviceProfile* SkDeviceProfile::RefGlobal() {
    SkAutoMutexAcquire amc(gMutex);

    if (NULL == gGlobalProfile) {
        gGlobalProfile = SkDeviceProfile::GetDefault();
    }
    gGlobalProfile->ref();
    return gGlobalProfile;
}
    
void SkDeviceProfile::SetGlobal(SkDeviceProfile* profile) {
    SkAutoMutexAcquire amc(gMutex);

    SkRefCnt_SafeAssign(gGlobalProfile, profile);
}

