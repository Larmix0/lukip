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
        printf(
            "[\033[1;33mWARNING\033[0m] function called in line %d: %s|%s() had no assertions.\n",
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
            printf(
                "[\033[1;31mFAIL\033[0m] line %d: %s|%s(): \"%s\"\n",
                test->failures[j].line,
                test->info.fileName,
                test->info.funcName,
                test->failures[j].message
            );
        }
    }
}

static void show_fail(LukipUnit lukip) {
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
        "\nFailed with %i/%i test results in 9999 seconds.\n\nFAIL.\n",
        lukip.testsLength - failures, lukip.testsLength
    );
}

static void show_success(LukipUnit lukip) {
    for (int i = 0; i < lukip.testsLength; i++) {
        lukip.tests[i].info.status == SUCCESS ? putchar('.') : putchar('?');
    }
    printf("\nSuccessfully ran %i tests in 9999 seconds.\n\nOK.\n", lukip.testsLength);
}

void show_results(LukipUnit lukip) {
    printf("\n\n\n");
    long_line('-');
    show_warnings(lukip);
    if (lukip.successful) {
        show_success(lukip);
    } else {
        show_fail(lukip);
    }
}
