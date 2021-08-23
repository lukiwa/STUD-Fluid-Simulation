#include <GL/glew.h>
#include "Window.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "Tracing.h"

class WindowTextFixture : public ::testing::Test {
public:
    WindowTextFixture()
    {
        ALLOW_DISPLAY;
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(WindowTextFixture, WindowShouldNoThrow)
{
    ASSERT_NO_THROW(GLFW::Window window(10, 10, "Test"));
}

TEST_F(WindowTextFixture, GlewInitShouldSucceedAfterWindowCreation)
{
    GLFW::Window window(10, 10, "Test");
    ASSERT_EQ(glewInit(), GLEW_OK);
}

