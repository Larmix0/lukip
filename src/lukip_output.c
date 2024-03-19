/**
 * @file lukip_output.c
 * @brief Outputs to the terminal the results of a Lukip unit-testing program.
 * 
 * @author Larmix
 */

#include <stdio.h>

#include "lukip_asserts.h"

#define LONG_LINE_LENGTH 100 /** Amount of characters placed to separate output. */

#define DEFAULT "\033[0m" /** Resets color to normal. */

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"


/**
 * @brief Prints a certain character a lot of times, acting as a long line.
 * 
 * @param lineChar The character to print the long line with.
 */
static void long_line(const char lineChar) {
    for (int i = 0; i < LONG_LINE_LENGTH; i++) {
        putchar(lineChar);
    }
    putchar('\n');
}

/**
 * @brief Prints a certain character a lot of times with a message in the middle.
 * 
 * Prints a certain character half of the normal length of a "long line" subtracted by
 * half of the length of the message on both sides. Then shows the message
 * followed by the same amount of the long line character half.
 * 
 * @param lineChar The character to put left and right of the message.
 * @param message The message in the middle.
 * @param mode The mode (usually a specific color) to print the message with.
 */
static void long_line_message(char lineChar, char *message, char *mode) {
    // -2 to account for bracket and space.
    const int halfLineLength = (LONG_LINE_LENGTH / 2) - 2 - (strlen(message) / 2);
    for (int i = 0; i < halfLineLength; i++) {
        putchar(lineChar);
    }
    printf("[ %s%s" DEFAULT " ]", mode, message);
    for (int i = 0; i < halfLineLength; i++) {
        putchar(lineChar);
    }
    putchar('\n');
}

/**
 * @brief Show a warning message for every function with unknown status (had no asserts).
 * 
 * @param lukip The Lukip unit to show the warnings of.
 */
static void show_warnings(const LukipUnit *lukip) {
    bool hadWarnings = false;
    for (int i = 0; i < lukip->testsLength; i++) {
        if (lukip->tests[i].info.status != UNKNOWN) {
            continue; // No need to warn.
        }
        hadWarnings = true;
        const LineInfo *caller = &lukip->tests[i].caller;
        printf("[" YELLOW "WARNING" DEFAULT "] ");
        printf(
            "Function called in line %d: %s|%s() had no assertions.\n",
            caller->line, caller->testInfo.fileName, caller->testInfo.funcName
        );
    }
    if (hadWarnings) {
        long_line('=');
    }
}

/**
 * @brief Show an error message for each unit-test failure.
 * 
 * @param lukip The Lukip unit to show the errors of.
 */
static void errors_info(const LukipUnit *lukip) {
    for (int i = 0; i < lukip->testsLength; i++) {
        const TestFunc *test = &lukip->tests[i];
        if (test->info.status != FAILURE) {
            continue;
        }
        for (int j = 0; j < test->failsLength; j++) {
            printf("[" RED "FAIL" DEFAULT "] ");
            printf(
                "Line %d: %s|%s(): %s\n",
                test->failures[j].line,
                test->info.fileName,
                test->info.funcName,
                test->failures[j].message
            );
        }
    }
}

/**
 * @brief Display the results of a failed Lukip unit.
 * 
 * Show F, dot or a question mark for each tests depending on the status,
 * then display the error messages, and a summary.
 * 
 * @param lukip The failed Lukip unit to display the results of.
 * @param executionTime The time it took the program to execute.
 */
static void show_fail(const LukipUnit *lukip, const double executionTime) {
    int failures = 0;
    for (int i = 0; i < lukip->testsLength; i++) {
        if (lukip->tests[i].info.status == FAILURE) {
            putchar('F');
            failures++;
        } else if (lukip->tests[i].info.status == SUCCESS) {
            putchar('.');
        } else {
            putchar('?');
        }
    }
    putchar('\n');
    long_line('=');
    errors_info(lukip);
    long_line('=');
    printf(
        "\nFailed with %d/%d tests (%d/%d assertions) in %.3lf seconds.\n\n",
        lukip->testsLength - failures, lukip->testsLength,
        lukip->asserts - lukip->failedAsserts, lukip->asserts, executionTime
    );
    char *failMessage = strf_alloc("Failed in %.3lfs.", executionTime);
    long_line_message('=', failMessage, RED);
    free(failMessage);
}

/**
 * @brief Display results of a successful Lukip unit.
 * 
 * Show a dot for tests with successful asserts and a question mark
 * for unknown test results that will show a warning.
 * Then show how many tests were ran, and a summary.
 * 
 * @param lukip The successful Lukip unit to show the results of.
 * @param executionTime The time it took the program to execute.
 */
static void show_success(const LukipUnit *lukip, const double executionTime) {
    for (int i = 0; i < lukip->testsLength; i++) {
        lukip->tests[i].info.status == SUCCESS ? putchar('.') : putchar('?');
    }
    putchar('\n');
    long_line('=');
    printf("[" GREEN "SUCCESS" DEFAULT "] ");
    printf(
        "Successfully ran %d tests (%d assertions total) in %.3lf seconds.\n\n",
        lukip->testsLength, lukip->asserts, executionTime
    );
    printf("OK.\n\n");

    char *successMessage = strf_alloc("Succeeded in %.3lfs.", executionTime);
    long_line_message('=', successMessage, GREEN);
    free(successMessage);
}

/**
 * Print some newlines, and a long line of dashes to seperate results from
 * the rest of the terminal. Show warnings and then the results after.
 */
void show_results(const LukipUnit *lukip) {
    printf("\n\n\n");
    long_line('-');
    show_warnings(lukip);

    const clock_t endTime = clock();
    const double executionTime = (double)(endTime - lukip->startTime) / CLOCKS_PER_SEC;
    if (lukip->successful) {
        show_success(lukip, executionTime);
    } else {
        show_fail(lukip, executionTime);
    }
}
