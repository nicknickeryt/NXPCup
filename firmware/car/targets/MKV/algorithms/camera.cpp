/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "camera.hpp"
#include "motor.hpp"


float Algorithm::calculatePosition(uint16_t* data, uint32_t currentMillis) {
    // Brightness
    brightness = 0;

    for (size_t i = 0; i != cameraBufferWith; i++) brightness += data[i];
    brightness /= cameraBufferWith;

    // EMA filter on brightness
    brightnessMean = (brightnessMean * (1.0f - params.getBrightnessMeanAlpha())) + (brightness * params.getBrightnessMeanAlpha());

    // Right line position
    rightLineRange = ((position / 2) - lineSearchOffset);

    for (size_t i = 0; i != rightLineRange; i++)
        if (data[i] < brightness) rightLinePosition = i;

    // Left line position
    leftLineRange = ((position / 2) + lineSearchOffset);

    for (size_t i = (cameraBufferWith - 1); i != leftLineRange; i--)
        if (data[i] < brightness) leftLinePosition = i;

    // Calculte position
    position         = (leftLinePosition + rightLinePosition);
    adjustedPosition = (static_cast<float>(position) / 2.0f) - halfCameraBufferWith;

    // Lo pass filter position
    filteredPosition = (filteredPosition * (1 - params.getAlgorithmFilterAlpha())) + ((adjustedPosition + algorithmOffset) * params.getAlgorithmFilterAlpha());


    // EMA filter on data
    for (size_t i = 0; i != cameraBufferWith; i++) {
        filteredData[i] = (filteredData[i] * (1.0f - params.getCameraCrossingFilterAlpha())) + (data[i] * params.getCameraCrossingFilterAlpha());
    }

    findCrossings(filteredData);

    if (findPatterns(filteredData, currentMillis)) differential.setPatternDetected(true);


    // Return filtered position
    return (filteredPosition);
}


// przykładowy wzorzec — MUSISZ dopasować z realnych danych!
static constexpr float pattern[128] = {7,   8,   13,  11, 18, 19, 28, 32, 36, 37, 41, 39, 34, 30, 40, 55, 71, 75, 78, 79,  81,  80,  84,  84,  89,  86,  88,  89,  89,  83,  68,  48,
                                   36,  37,  37,  37, 38, 37, 40, 40, 41, 44, 44, 45, 48, 47, 52, 51, 62, 78, 97, 105, 108, 105, 107, 107, 110, 108, 109, 108, 106, 111, 112, 108,
                                   111, 100, 91,  79, 78, 75, 79, 77, 80, 77, 79, 79, 82, 79, 83, 82, 84, 80, 82, 81,  84,  86,  98,  105, 110, 105, 107, 104, 108, 105, 106, 103,
                                   103, 100, 100, 96, 96, 93, 92, 80, 59, 40, 41, 50, 60, 58, 58, 52, 52, 47, 44, 38,  37,  26,  20,  16,  14,  9,   10,  6,   8,   4,   0,   0};

bool Algorithm::findPatterns(uint16_t* data, uint32_t currentMillis) {
    if (currentMillis - algorithmStartTime < patternDetectTimeoutMs) return false;

    // ===== SMOOTH =====
    for (int i = 1; i < 127; i++) smoothedData[i] = (data[i - 1] + 5 * data[i] + data[i + 1]) / 7;

    // ===== NORMALIZACJA DATA =====
    float meanData = 0.0f;
    for (int i = 0; i < 128; i++) meanData += smoothedData[i];
    meanData /= 128.0f;

    float stdData = 0.0f;
    for (int i = 0; i < 128; i++) {
        float d = smoothedData[i] - meanData;
        stdData += d * d;
    }
    stdData = sqrtf(stdData);

    if (stdData < 1e-3f) return false;

    // ===== NORMALIZACJA PATTERN =====
    static float meanPattern = 0.0f;
    static float stdPattern  = 0.0f;
    static bool  patternInit = false;

    if (!patternInit) {
        for (int i = 0; i < 128; i++) meanPattern += pattern[i];
        meanPattern /= 128.0f;

        for (int i = 0; i < 128; i++) {
            float d = pattern[i] - meanPattern;
            stdPattern += d * d;
        }
        stdPattern = sqrtf(stdPattern);

        patternInit = true;
    }

    // ===== KORELACJA =====
    float corr = 0.0f;

    for (int i = 0; i < 128; i++) {
        float d1 = smoothedData[i] - meanData;
        float d2 = pattern[i] - meanPattern;
        corr += d1 * d2;
    }

    corr /= (stdData * stdPattern);

    // ===== DECYZJA ===== // na wysokich predkoscuach nadal nie wykrywa prawdziwego a wykrywa zle...

    if (corr > params.getPatternCorrelationThreshold() && crossings >= 5) {
        if (!patternDetected) {
            patternDetected   = true;
            patternsStartTime = currentMillis;
            return true;
        }

        if (currentMillis - patternsStartTime > 5) {
            patternDetected = false;
            return true;
        }

    } else {
        patternDetected = false;
    }

    return false;
}

// // przykładowy wzorzec — MUSISZ dopasować z realnych danych!
// static const uint32_t pattern[60] = {
//     100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//     0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,
//     100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,
//     0,0,0,0,0,0,0,0,0,0,0,0
// };
// bool Algorithm::findPatterns(uint16_t* data, uint32_t currentMillis)
// {
//     if (currentMillis - algorithmStartTime < patternDetectTimeoutMs)
//         return false;

//     // ===== SMOOTH =====
//     for (int i = 1; i < 127; i++)
//         smoothedData[i] = (data[i - 1] + 5 * data[i] + data[i + 1]) / 7;

//     // ===== SLIDING CROSS-CORRELATION =====
//     int32_t bestCorr = 0;
//     int bestIndex = -1;

//     // Slide pattern over data
//     for (int offset = 20; offset <= 100 - 60; offset++) {

//         int32_t corr = 0;

//         for (int i = 0; i < 60; i++) {
//             corr += (int32_t)smoothedData[offset + i] * pattern[i];
//         }

//         if (corr > bestCorr) {
//             bestCorr = corr;
//             bestIndex = offset;
//         }
//     }

//     // ===== THRESHOLD =====
//     // MUST be tuned experimentally
//     // originally was 200000
//     const int32_t THRESHOLD = 2000000000;  

//     if (bestCorr > THRESHOLD) {

//         if (!patternDetected) {
//             patternDetected   = true;
//             patternsStartTime = currentMillis;
//             // detectedIndex     = bestIndex; // optional: where pattern was found
//             return true;
//         }

//         if (currentMillis - patternsStartTime > 5) {
//             patternDetected = false;
//             return true;
//         }

//     } else {
//         patternDetected = false;
//     }

//     return false;
// }

bool Algorithm::findCrossings(uint16_t* data) {
    crossings = 0;

    brightnessCrossThreshold = brightnessMean * params.getCrossingsBrightnessMultiplier();

    for (size_t i = 1 + params.getCrossingsCut(); i < cameraBufferWith - 3 - params.getCrossingsCut(); i++) {
        if ((data[i - 1] < brightnessCrossThreshold) && (data[i] < brightnessCrossThreshold) && (data[i + 1] > brightnessCrossThreshold) && (data[i + 2] > brightnessCrossThreshold)) crossings++;
        if ((data[i - 1] > brightnessCrossThreshold) && (data[i] > brightnessCrossThreshold) && (data[i + 1] < brightnessCrossThreshold) && (data[i + 2] < brightnessCrossThreshold)) crossings++;
    }

    return false;
}

void Algorithm::clearPatterns(uint32_t currentMillis) {
    patternDetected   = false;
    patternsStartTime = currentMillis;
    differential.clearAllFlags();
}

void Algorithm::setAlgorithmStartTime(uint32_t currentMillis) { algorithmStartTime = currentMillis; }