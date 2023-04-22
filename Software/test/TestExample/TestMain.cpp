/**
 * @file TestMain.cpp
 * @brief This is entry point to all UTs in this directory
 *        If you want to create your own unit test, copy this
 *        file to your test's root directory.
 */

#include <CppUTest/CommandLineTestRunner.h>

int main(int argc, char* argv[])
{
    /**
     * Install CppUtest plugins here
     */

    return CommandLineTestRunner::RunAllTests(argc, argv);
}