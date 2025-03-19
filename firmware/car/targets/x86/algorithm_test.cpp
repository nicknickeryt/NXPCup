#include <gtest/gtest.h>
#include <algorithms/camera.hpp>
#include "line_test_data.hpp"

TEST(Algorithm, findThreshold) {
    uint16_t cameraBuffer[128] = {0};
    Algorithm algorithm;

    for (size_t i = 0; i < 1000; i++) {
        algorithm.calculatePosition(cameraBuffer);
    }

    ASSERT_EQ(0, algorithm.calculatePosition(cameraBuffer));    
}

TEST(Algorithm, LeftSide) {
    Algorithm algorithm;

    for (size_t i = 0; i < 1000; i++) {
        algorithm.calculatePosition(left_side);
    }

    EXPECT_NEAR(-28, algorithm.calculatePosition(left_side), 5);
}

TEST(Algorithm, RightSide) {
    Algorithm algorithm;

    for (size_t i = 0; i < 1000; i++) {
        algorithm.calculatePosition(right_side);
    }

    EXPECT_NEAR(32, algorithm.calculatePosition(right_side), 5);
}