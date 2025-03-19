#include <gtest/gtest.h>
#include "algorithm.hpp"
#include "line_test_data.hpp"

TEST(Algorithm, findThreshold) {
    uint16_t cameraBuffer[128] = {0};
    Algorithm algorithm;

    ASSERT_NO_THROW(algorithm.init(cameraBuffer, sizeof(cameraBuffer)));
    ASSERT_EQ(0, algorithm.calucaltePositionOnTrack(cameraBuffer, sizeof(cameraBuffer)));    
}

TEST(Algorithm, LeftSide) {
    Algorithm algorithm;
    ASSERT_NO_THROW(algorithm.init(left_side, sizeof(left_side)));

    ASSERT_EQ(28, algorithm.calucaltePositionOnTrack(left_side, sizeof(left_side)));
}

TEST(Algorithm, RightSide) {
    Algorithm algorithm;
    ASSERT_NO_THROW(algorithm.init(right_side, sizeof(right_side)));

    ASSERT_EQ(-32, algorithm.calucaltePositionOnTrack(right_side, sizeof(right_side)));
}