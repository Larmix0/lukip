#include <stdio.h>

#include "lukip_asserts.h"

static void long_line(char ch) {
    for (int i = 0; i < 100; i++) {
        putchar(ch);
    }
    putchar('\n');
}

static void show_warnings(LukipUnit lukip) {
    bool hadWarnings;
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status != UNKNOWN) {
            continue; // no need to warn
        }
        hadWarnings = true;
        LineInfo *caller = &lukip.tests[i].caller;
        printf("[\033[1;33mWARNING\033[0m] ");
        printf(
            "Function called in line %d: %s|%s() had no assertions.\n",
            caller->line, caller->fileName, caller->funcName
        );
    }
    if (hadWarnings) {
        long_line('=');
    }
}

static void errors_info(LukipUnit lukip) {
    for (int i = 0; i < lukip.testsLength; i++) {
        TestFunc *test = &lukip.tests[i];
        
        if (test->info.status != FAILURE) {
            continue;
        }
        for (int j = 0; j < test->failsLength; j++) {
            printf("[\033[1;31mFAIL\033[0m] ");
            printf(
                "Line %d: %s|%s(): \"%s\"\n",
                test->failures[j].line,
                test->info.fileName,
                test->info.funcName,
                test->failures[j].message
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
    printf("\n");
    long_line('=');
    errors_info(lukip);
    printf(
        "\nFailed with %i/%i test results in %.3lf seconds.\n\nFAIL.\n",
        lukip.testsLength - failures, lukip.testsLength, executionTime
    );
}

static void show_success(LukipUnit lukip, double executionTime) {
    for (int i = 0; i < lukip.testsLength; i++) {
        lukip.tests[i].info.status == SUCCESS ? putchar('.') : putchar('?');
    }
    printf(
        "\nSuccessfully ran %i tests in %.3lf seconds.\n\nOK.\n", lukip.testsLength, executionTime
    );
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
