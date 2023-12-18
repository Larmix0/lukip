#include <stdio.h>

#include "lar_asserts.h"

static void long_line(char ch) {
    for (int i = 0; i < 100; i++) {
        putchar(ch);
    }
    putchar('\n');
}

static void show_warnings(LarUnit larUnit) {
    bool hadWarnings;
    for (int i = 0; i < larUnit.testsLength; i++) {
        if (larUnit.tests[i].info.status != UNKNOWN) {
            continue; // no need to warn
        }
        hadWarnings = true;
        LineInfo *caller = &larUnit.tests[i].caller;
        printf(
            "[\033[1;33mWARNING\033[0m] function called in line %d: %s|%s() had no assertions.\n",
            caller->line, caller->fileName, caller->funcName
        );
    }
    if (hadWarnings) {
        long_line('=');
    }
}

static void errors_info(LarUnit larUnit) {
    for (int i = 0; i < larUnit.testsLength; i++) {
        TestFunc *test = &larUnit.tests[i];
        
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

static void show_fail(LarUnit larUnit) {
    int failures = 0;
    for (int i = 0; i < larUnit.testsLength; i++) {
        if (larUnit.tests[i].info.status == FAILURE) {
            putchar('F');
            failures++;
        } else if (larUnit.tests[i].info.status == SUCCESS) {
            putchar('.');
        } else {
            putchar('?');
        }
    }
    printf("\n");
    long_line('=');
    errors_info(larUnit);
    printf(
        "\nFailed with %i/%i test results in 9999 seconds.\n\nFAIL.\n",
        larUnit.testsLength - failures, larUnit.testsLength
    );
}

static void show_success(LarUnit larUnit) {
    for (int i = 0; i < larUnit.testsLength; i++) {
        larUnit.tests[i].info.status == SUCCESS ? putchar('.') : putchar('?');
    }
    printf("\nSuccessfully ran %i tests in 9999 seconds.\n\nOK.\n", larUnit.testsLength);
}

void show_results(LarUnit larUnit) {
    printf("\n\n\n");
    long_line('-');
    show_warnings(larUnit);
    if (larUnit.successful) {
        show_success(larUnit);
    } else {
        show_fail(larUnit);
    }
}
