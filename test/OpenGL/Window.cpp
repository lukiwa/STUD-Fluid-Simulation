#include <GL/glew.h>
#include "Window.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>


class WindowTextFixture : public ::testing::Test {
public:
    WindowTextFixture()
    {
        setenv("DISPLAY", "127.0.0.1:0", true);
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
    EXPECT_NO_THROW(GLFW::Window window(10, 10, "Test"));
}

TEST_F(WindowTextFixture, GlewInitShouldSucceedAfterWindowCreation)
{
    GLFW::Window window(10, 10, "Test");
    EXPECT_EQ(glewInit(), GLEW_OK);
}

