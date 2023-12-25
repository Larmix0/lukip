#include <stdio.h>

#include "lukip_asserts.h"

#define DEFAULT "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"

static void long_line(char lineChar) {
    for (int i = 0; i < 100; i++) {
        putchar(lineChar);
    }
    putchar('\n');
}

static void long_line_message(char lineChar, char *message, char *mode) {
    size_t messageLength = strlen(message);
    int halfLineLength = 50 - 2 - (messageLength / 2);
    for (int i = 0; i < halfLineLength; i++) {
        putchar(lineChar);
    }
    printf("[ %s%s" DEFAULT " ]", mode, message);
    for (int i = 0; i < halfLineLength; i++) {
        putchar(lineChar);
    }
    putchar('\n');
}

static void show_warnings(LukipUnit lukip) {
    bool hadWarnings = false;
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status != UNKNOWN) {
            continue; // no need to warn
        }
        hadWarnings = true;
        LineInfo *caller = &lukip.tests[i].caller;
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

static void errors_info(LukipUnit lukip) {
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status != FAILURE) {
            continue;
        }
        for (int j = 0; j < lukip.tests[i].failsLength; j++) {
            printf("[" RED "FAIL" DEFAULT "] ");
            printf(
                "Line %d: %s|%s(): \"%s\"\n",
                lukip.tests[i].failures[j].line,
                lukip.tests[i].info.fileName,
                lukip.tests[i].info.funcName,
                lukip.tests[i].failures[j].message
            );
        }
    }
}

static void show_fail(LukipUnit lukip, double executionTime) {
    int failures = 0;
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status == FAILURE) {
            putchar('F');
            failures++;
        } else if (lukip.tests[i].info.status == SUCCESS) {
            putchar('.');
        } else {
            putchar('?');
        }
    }
    putchar('\n');
    long_line('=');
    errors_info(lukip);
    printf(
        "\nFailed with %i/%i test results in %.3lf seconds.\n\n",
        lukip.testsLength - failures, lukip.testsLength, executionTime
    );
    char *failMessage = strf_alloc("Failed in %.3lfs.", executionTime);
    long_line_message('=', failMessage, RED);
    free(failMessage);
}

static void show_success(LukipUnit lukip, double executionTime) {
    for (int i = 0; i < lukip.testsLength; i++) {
        lukip.tests[i].info.status == SUCCESS ? putchar('.') : putchar('?');
    }
    putchar('\n');
    long_line('=');
    printf("[" GREEN "SUCCESS" DEFAULT "] ");
    printf(
        "Successfully ran %d tests in %.3lf seconds.\n\n",
        lukip.testsLength, executionTime
    );
    printf("OK.\n\n");
    char *successMessage = strf_alloc("Succeeded in %.3lfs.", executionTime);
    long_line_message('=', successMessage, GREEN);
    free(successMessage);
}

void show_results(LukipUnit lukip) {
    printf("\n\n\n");
    long_line('-');
    show_warnings(lukip);

    clock_t endTime = clock();
    double executionTime = (double)(endTime - lukip.startTime) / CLOCKS_PER_SEC;
    if (lukip.successful) {
        show_success(lukip, executionTime);
    } else {
        show_fail(lukip, executionTime);
    }
}
