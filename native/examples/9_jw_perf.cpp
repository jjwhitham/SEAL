     // Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "examples.h"
#include <sys/time.h>

using namespace std;
using namespace seal;

// Return current wallclock time, for performance measurement
inline uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

void jw_perf()
{
    // [(random.randint(10, 99), random.randint(10, 99)) for i in range(1000)]
    int nums_for_multiplication[][2] = {
        { 25, 15 }, { 51, 38 }, { 18, 13 }, { 99, 31 }, { 88, 59 }, { 53, 31 }, { 85, 87 }, { 65, 89 }, { 62, 55 },
        { 99, 94 }, { 90, 78 }, { 44, 79 }, { 79, 71 }, { 50, 78 }, { 16, 81 }, { 97, 55 }, { 67, 22 }, { 36, 38 },
        { 91, 45 }, { 26, 53 }, { 17, 35 }, { 86, 95 }, { 40, 55 }, { 28, 78 }, { 66, 14 }, { 32, 65 }, { 75, 60 },
        { 97, 42 }, { 10, 62 }, { 57, 47 }, { 98, 60 }, { 41, 92 }, { 33, 77 }, { 61, 85 }, { 75, 30 }, { 41, 12 },
        { 35, 31 }, { 44, 61 }, { 68, 31 }, { 25, 43 }, { 32, 76 }, { 92, 21 }, { 14, 87 }, { 66, 70 }, { 16, 72 },
        { 19, 62 }, { 27, 30 }, { 88, 42 }, { 90, 99 }, { 80, 92 }, { 40, 35 }, { 97, 59 }, { 73, 43 }, { 98, 79 },
        { 35, 92 }, { 48, 69 }, { 41, 45 }, { 57, 84 }, { 76, 13 }, { 25, 50 }, { 24, 18 }, { 84, 14 }, { 29, 14 },
        { 62, 70 }, { 11, 59 }, { 49, 86 }, { 10, 15 }, { 94, 39 }, { 51, 85 }, { 48, 25 }, { 40, 62 }, { 81, 28 },
        { 10, 34 }, { 17, 12 }, { 68, 23 }, { 56, 30 }, { 48, 82 }, { 90, 66 }, { 32, 93 }, { 77, 70 }, { 73, 11 },
        { 91, 95 }, { 66, 66 }, { 69, 29 }, { 99, 55 }, { 17, 36 }, { 59, 63 }, { 98, 39 }, { 48, 38 }, { 88, 33 },
        { 57, 35 }, { 16, 20 }, { 96, 68 }, { 26, 92 }, { 56, 12 }, { 49, 58 }, { 48, 97 }, { 89, 53 }, { 44, 22 },
        { 27, 99 }, { 59, 47 }, { 96, 99 }, { 72, 36 }, { 18, 39 }, { 11, 93 }, { 58, 51 }, { 48, 67 }, { 57, 66 },
        { 36, 12 }, { 28, 79 }, { 64, 82 }, { 61, 29 }, { 51, 95 }, { 57, 21 }, { 57, 20 }, { 98, 12 }, { 83, 84 },
        { 28, 30 }, { 75, 11 }, { 35, 71 }, { 48, 26 }, { 42, 60 }, { 92, 85 }, { 75, 86 }, { 43, 53 }, { 16, 97 },
        { 16, 38 }, { 33, 70 }, { 43, 68 }, { 67, 29 }, { 84, 60 }, { 97, 66 }, { 44, 22 }, { 64, 94 }, { 45, 47 },
        { 33, 21 }, { 99, 31 }, { 10, 85 }, { 64, 74 }, { 90, 12 }, { 48, 90 }, { 52, 69 }, { 80, 86 }, { 35, 84 },
        { 60, 14 }, { 48, 42 }, { 59, 20 }, { 48, 54 }, { 52, 53 }, { 80, 57 }, { 81, 98 }, { 44, 53 }, { 37, 14 },
        { 68, 69 }, { 58, 18 }, { 40, 86 }, { 64, 51 }, { 17, 69 }, { 65, 55 }, { 66, 59 }, { 82, 76 }, { 18, 18 },
        { 73, 70 }, { 67, 76 }, { 89, 58 }, { 33, 11 }, { 27, 37 }, { 56, 12 }, { 59, 47 }, { 28, 71 }, { 94, 15 },
        { 75, 70 }, { 65, 93 }, { 53, 47 }, { 48, 90 }, { 68, 93 }, { 75, 87 }, { 90, 87 }, { 65, 27 }, { 82, 68 },
        { 11, 11 }, { 17, 92 }, { 48, 42 }, { 72, 52 }, { 85, 47 }, { 88, 31 }, { 65, 35 }, { 53, 37 }, { 52, 95 },
        { 69, 46 }, { 29, 38 }, { 19, 75 }, { 34, 46 }, { 96, 70 }, { 82, 70 }, { 34, 69 }, { 44, 23 }, { 73, 74 },
        { 53, 13 }, { 97, 69 }, { 90, 45 }, { 81, 21 }, { 24, 14 }, { 70, 46 }, { 44, 15 }, { 17, 33 }, { 48, 89 },
        { 48, 98 }, { 29, 31 }, { 96, 51 }, { 34, 78 }, { 48, 10 }, { 48, 94 }, { 65, 12 }, { 73, 28 }, { 10, 52 },
        { 11, 15 }, { 49, 18 }, { 92, 31 }, { 52, 23 }, { 21, 54 }, { 80, 53 }, { 17, 20 }, { 42, 53 }, { 11, 99 },
        { 54, 30 }, { 81, 90 }, { 26, 83 }, { 73, 84 }, { 40, 47 }, { 43, 38 }, { 96, 44 }, { 26, 83 }, { 89, 25 },
        { 58, 79 }, { 65, 53 }, { 18, 43 }, { 98, 36 }, { 89, 53 }, { 56, 49 }, { 42, 37 }, { 29, 79 }, { 74, 92 },
        { 33, 58 }, { 61, 63 }, { 56, 60 }, { 72, 44 }, { 50, 43 }, { 97, 90 }, { 80, 36 }, { 90, 61 }, { 25, 55 },
        { 64, 36 }, { 89, 22 }, { 56, 83 }, { 20, 87 }, { 33, 79 }, { 25, 28 }, { 36, 39 }, { 33, 10 }, { 25, 19 },
        { 41, 68 }, { 33, 83 }, { 89, 14 }, { 17, 95 }, { 72, 14 }, { 55, 71 }, { 11, 79 }, { 92, 12 }, { 81, 57 },
        { 12, 71 }, { 26, 20 }, { 59, 30 }, { 21, 78 }, { 75, 14 }, { 24, 95 }, { 46, 31 }, { 51, 20 }, { 10, 12 },
        { 67, 43 }, { 26, 71 }, { 16, 37 }, { 19, 37 }, { 25, 86 }, { 88, 99 }, { 66, 29 }, { 37, 38 }, { 48, 28 },
        { 58, 37 }, { 75, 93 }, { 17, 28 }, { 98, 12 }, { 97, 10 }, { 42, 86 }, { 25, 38 }, { 99, 23 }, { 35, 39 },
        { 25, 36 }, { 74, 47 }, { 97, 84 }, { 97, 51 }, { 96, 18 }, { 89, 45 }, { 18, 87 }, { 48, 73 }, { 84, 85 },
        { 16, 92 }, { 75, 54 }, { 73, 14 }, { 50, 93 }, { 46, 38 }, { 18, 39 }, { 37, 53 }, { 73, 87 }, { 21, 63 },
        { 74, 79 }, { 67, 51 }, { 59, 14 }, { 41, 22 }, { 92, 31 }, { 24, 45 }, { 69, 47 }, { 57, 60 }, { 42, 42 },
        { 50, 39 }, { 50, 27 }, { 94, 63 }, { 44, 77 }, { 19, 77 }, { 14, 31 }, { 20, 53 }, { 88, 33 }, { 50, 47 },
        { 97, 74 }, { 54, 47 }, { 97, 94 }, { 74, 60 }, { 21, 15 }, { 85, 53 }, { 27, 31 }, { 41, 69 }, { 40, 54 },
        { 20, 31 }, { 53, 79 }, { 76, 38 }, { 12, 77 }, { 85, 22 }, { 18, 71 }, { 37, 78 }, { 80, 18 }, { 90, 20 },
        { 82, 63 }, { 98, 60 }, { 70, 47 }, { 50, 39 }, { 79, 31 }, { 33, 67 }, { 16, 93 }, { 37, 38 }, { 21, 47 },
        { 84, 29 }, { 26, 13 }, { 88, 61 }, { 17, 66 }, { 43, 84 }, { 80, 99 }, { 34, 99 }, { 78, 31 }, { 32, 27 },
        { 28, 93 }, { 91, 55 }, { 17, 69 }, { 96, 20 }, { 89, 78 }, { 16, 29 }, { 49, 82 }, { 34, 39 }, { 27, 22 },
        { 64, 84 }, { 76, 13 }, { 56, 93 }, { 72, 86 }, { 16, 93 }, { 42, 55 }, { 80, 36 }, { 62, 38 }, { 65, 87 },
        { 51, 37 }, { 10, 15 }, { 52, 39 }, { 42, 94 }, { 22, 63 }, { 41, 38 }, { 65, 94 }, { 35, 78 }, { 20, 15 },
        { 88, 84 }, { 52, 44 }, { 58, 21 }, { 72, 60 }, { 24, 11 }, { 94, 14 }, { 54, 31 }, { 93, 39 }, { 88, 80 },
        { 10, 99 }, { 12, 69 }, { 76, 85 }, { 64, 25 }, { 26, 60 }, { 61, 71 }, { 74, 90 }, { 13, 45 }, { 44, 36 },
        { 17, 54 }, { 57, 30 }, { 51, 30 }, { 77, 15 }, { 43, 14 }, { 10, 61 }, { 61, 37 }, { 61, 22 }, { 26, 71 },
        { 51, 99 }, { 33, 95 }, { 19, 92 }, { 82, 75 }, { 17, 55 }, { 80, 61 }, { 18, 29 }, { 72, 81 }, { 52, 93 },
        { 53, 77 }, { 64, 50 }, { 91, 75 }, { 45, 39 }, { 66, 23 }, { 72, 85 }, { 14, 55 }, { 24, 38 }, { 68, 87 },
        { 33, 31 }, { 57, 20 }, { 76, 31 }, { 18, 69 }, { 24, 62 }, { 69, 71 }, { 19, 22 }, { 56, 75 }, { 48, 98 },
        { 75, 11 }, { 80, 54 }, { 45, 39 }, { 48, 92 }, { 32, 77 }, { 80, 75 }, { 26, 14 }, { 89, 84 }, { 25, 44 },
        { 17, 84 }, { 40, 22 }, { 12, 23 }, { 40, 23 }, { 43, 12 }, { 88, 91 }, { 27, 76 }, { 32, 12 }, { 40, 19 },
        { 72, 15 }, { 45, 15 }, { 96, 86 }, { 98, 35 }, { 75, 51 }, { 40, 92 }, { 96, 14 }, { 12, 14 }, { 40, 44 },
        { 65, 14 }, { 49, 11 }, { 72, 20 }, { 66, 85 }, { 56, 26 }, { 38, 79 }, { 70, 79 }, { 51, 21 }, { 96, 43 },
        { 21, 95 }, { 49, 85 }, { 64, 83 }, { 94, 78 }, { 32, 19 }, { 17, 79 }, { 75, 92 }, { 74, 36 }, { 41, 15 },
        { 55, 95 }, { 56, 46 }, { 31, 23 }, { 20, 69 }, { 56, 79 }, { 82, 21 }, { 83, 52 }, { 27, 68 }, { 16, 50 },
        { 11, 44 }, { 58, 75 }, { 67, 92 }, { 27, 36 }, { 42, 35 }, { 64, 88 }, { 73, 74 }, { 27, 62 }, { 21, 70 },
        { 42, 67 }, { 96, 23 }, { 86, 30 }, { 26, 77 }, { 74, 78 }, { 17, 86 }, { 81, 63 }, { 96, 79 }, { 83, 69 },
        { 59, 71 }, { 98, 55 }, { 82, 66 }, { 80, 10 }, { 93, 31 }, { 48, 33 }, { 68, 79 }, { 36, 14 }, { 28, 15 },
        { 83, 87 }, { 53, 87 }, { 69, 30 }, { 96, 52 }, { 24, 46 }, { 59, 87 }, { 68, 13 }, { 90, 51 }, { 66, 92 },
        { 26, 15 }, { 65, 95 }, { 42, 27 }, { 16, 70 }, { 57, 68 }, { 10, 83 }, { 82, 51 }, { 98, 93 }, { 34, 74 },
        { 20, 20 }, { 32, 74 }, { 51, 77 }, { 70, 95 }, { 56, 31 }, { 35, 75 }, { 41, 27 }, { 82, 75 }, { 89, 10 },
        { 27, 28 }, { 97, 30 }, { 58, 54 }, { 27, 92 }, { 96, 23 }, { 48, 42 }, { 68, 70 }, { 75, 12 }, { 73, 76 },
        { 86, 23 }, { 33, 70 }, { 49, 46 }, { 90, 86 }, { 82, 18 }, { 22, 22 }, { 11, 14 }, { 90, 36 }, { 18, 28 },
        { 59, 87 }, { 23, 55 }, { 72, 14 }, { 65, 62 }, { 43, 52 }, { 66, 11 }, { 18, 86 }, { 20, 13 }, { 96, 52 },
        { 56, 28 }, { 66, 84 }, { 85, 95 }, { 96, 93 }, { 88, 20 }, { 83, 62 }, { 57, 10 }, { 11, 36 }, { 28, 79 },
        { 10, 14 }, { 62, 55 }, { 59, 13 }, { 54, 39 }, { 82, 87 }, { 71, 55 }, { 41, 54 }, { 57, 26 }, { 80, 25 },
        { 58, 62 }, { 20, 46 }, { 37, 22 }, { 66, 93 }, { 75, 44 }, { 69, 63 }, { 53, 52 }, { 25, 37 }, { 16, 70 },
        { 12, 95 }, { 66, 43 }, { 16, 68 }, { 73, 55 }, { 44, 86 }, { 39, 87 }, { 98, 66 }, { 80, 77 }, { 74, 27 },
        { 75, 68 }, { 28, 39 }, { 16, 66 }, { 73, 62 }, { 34, 39 }, { 14, 63 }, { 78, 71 }, { 96, 98 }, { 16, 23 },
        { 19, 46 }, { 72, 86 }, { 89, 22 }, { 91, 78 }, { 32, 22 }, { 97, 62 }, { 48, 60 }, { 77, 14 }, { 17, 91 },
        { 59, 76 }, { 55, 23 }, { 72, 77 }, { 90, 28 }, { 99, 79 }, { 45, 23 }, { 80, 95 }, { 60, 14 }, { 49, 70 },
        { 77, 53 }, { 94, 95 }, { 12, 37 }, { 99, 95 }, { 12, 38 }, { 89, 69 }, { 67, 71 }, { 74, 70 }, { 80, 21 },
        { 56, 14 }, { 24, 85 }, { 12, 46 }, { 57, 46 }, { 19, 93 }, { 96, 26 }, { 37, 70 }, { 40, 31 }, { 91, 92 },
        { 17, 21 }, { 29, 63 }, { 17, 79 }, { 83, 29 }, { 99, 69 }, { 68, 47 }, { 51, 76 }, { 29, 45 }, { 67, 13 },
        { 34, 45 }, { 90, 79 }, { 19, 94 }, { 24, 15 }, { 34, 86 }, { 72, 89 }, { 64, 87 }, { 35, 29 }, { 43, 44 },
        { 73, 31 }, { 43, 67 }, { 45, 79 }, { 97, 53 }, { 52, 85 }, { 32, 37 }, { 88, 89 }, { 60, 61 }, { 22, 39 },
        { 16, 44 }, { 44, 20 }, { 93, 55 }, { 98, 69 }, { 92, 14 }, { 26, 69 }, { 65, 83 }, { 90, 69 }, { 81, 17 },
        { 82, 53 }, { 56, 66 }, { 86, 22 }, { 28, 87 }, { 62, 30 }, { 49, 27 }, { 24, 66 }, { 58, 86 }, { 64, 84 },
        { 60, 46 }, { 82, 23 }, { 25, 66 }, { 74, 91 }, { 92, 47 }, { 51, 11 }, { 18, 18 }, { 65, 57 }, { 57, 20 },
        { 20, 79 }, { 64, 59 }, { 63, 95 }, { 97, 62 }, { 12, 55 }, { 69, 61 }, { 12, 21 }, { 89, 89 }, { 12, 15 },
        { 26, 27 }, { 97, 14 }, { 96, 61 }, { 65, 93 }, { 84, 54 }, { 16, 58 }, { 62, 86 }, { 73, 70 }, { 33, 23 },
        { 40, 99 }, { 34, 68 }, { 37, 39 }, { 49, 26 }, { 64, 51 }, { 81, 67 }, { 81, 84 }, { 31, 39 }, { 64, 55 },
        { 74, 20 }, { 67, 75 }, { 91, 62 }, { 16, 68 }, { 48, 15 }, { 72, 58 }, { 73, 45 }, { 10, 69 }, { 48, 17 },
        { 66, 75 }, { 97, 62 }, { 17, 53 }, { 36, 30 }, { 67, 43 }, { 59, 60 }, { 90, 36 }, { 41, 95 }, { 90, 54 },
        { 88, 66 }, { 72, 54 }, { 80, 14 }, { 80, 33 }, { 18, 23 }, { 88, 38 }, { 59, 71 }, { 60, 71 }, { 89, 50 },
        { 57, 21 }, { 56, 67 }, { 73, 38 }, { 93, 79 }, { 10, 59 }, { 17, 31 }, { 88, 12 }, { 24, 14 }, { 88, 93 },
        { 64, 23 }, { 93, 30 }, { 82, 38 }, { 23, 39 }, { 60, 79 }, { 33, 31 }, { 48, 56 }, { 51, 21 }, { 19, 86 },
        { 35, 27 }, { 66, 21 }, { 30, 14 }, { 90, 55 }, { 10, 82 }, { 73, 86 }, { 50, 60 }, { 28, 77 }, { 56, 65 },
        { 58, 21 }, { 64, 73 }, { 75, 93 }, { 61, 46 }, { 59, 85 }, { 84, 12 }, { 64, 68 }, { 33, 35 }, { 58, 92 },
        { 25, 77 }, { 61, 87 }, { 88, 82 }, { 76, 87 }, { 58, 92 }, { 10, 95 }, { 56, 35 }, { 27, 36 }, { 75, 60 },
        { 16, 94 }, { 10, 23 }, { 73, 30 }, { 72, 44 }, { 41, 69 }, { 19, 22 }, { 99, 63 }, { 13, 77 }, { 25, 55 },
        { 72, 44 }, { 35, 29 }, { 59, 35 }, { 98, 71 }, { 56, 36 }, { 17, 12 }, { 88, 26 }, { 18, 76 }, { 11, 12 },
        { 14, 39 }, { 77, 70 }, { 16, 70 }, { 31, 23 }, { 41, 63 }, { 67, 55 }, { 91, 12 }, { 54, 55 }, { 34, 18 },
        { 56, 34 }, { 66, 92 }, { 73, 20 }, { 37, 77 }, { 59, 37 }, { 91, 37 }, { 84, 77 }, { 51, 54 }, { 32, 41 },
        { 95, 85 }, { 96, 78 }, { 74, 27 }, { 64, 35 }, { 40, 22 }, { 40, 33 }, { 80, 27 }, { 97, 44 }, { 74, 53 },
        { 32, 38 }, { 32, 31 }, { 18, 15 }, { 16, 37 }, { 11, 14 }, { 74, 83 }, { 17, 68 }, { 51, 29 }, { 16, 29 },
        { 43, 61 }, { 73, 60 }, { 11, 61 }, { 20, 45 }, { 91, 31 }, { 88, 71 }, { 97, 31 }, { 33, 75 }, { 32, 61 },
        { 34, 45 }, { 32, 92 }, { 89, 63 }, { 88, 39 }, { 49, 37 }, { 92, 53 }, { 10, 21 }, { 49, 95 }, { 50, 59 },
        { 27, 14 }, { 41, 71 }, { 66, 20 }, { 57, 54 }, { 74, 15 }, { 58, 61 }, { 61, 15 }, { 37, 23 }, { 91, 23 },
        { 16, 73 }, { 98, 66 }, { 62, 55 }, { 90, 15 }, { 77, 13 }, { 29, 85 }, { 97, 46 }, { 99, 93 }, { 48, 22 },
        { 83, 53 }, { 34, 86 }, { 73, 69 }, { 67, 37 }, { 32, 60 }, { 72, 11 }, { 90, 23 }, { 82, 29 }, { 18, 61 },
        { 63, 47 }, { 81, 85 }, { 36, 68 }, { 91, 15 }, { 75, 68 }, { 88, 33 }, { 16, 18 }, { 81, 51 }, { 61, 87 },
        { 10, 36 }, { 90, 31 }, { 56, 81 }, { 34, 15 }, { 68, 13 }, { 93, 13 }, { 24, 43 }, { 33, 23 }, { 96, 21 },
        { 18, 93 }, { 76, 37 }, { 48, 61 }, { 67, 19 }, { 58, 23 }, { 17, 82 }, { 64, 25 }, { 41, 61 }, { 10, 71 },
        { 89, 83 }, { 49, 62 }, { 75, 21 }, { 44, 55 }, { 44, 21 }, { 56, 50 }, { 17, 63 }, { 57, 14 }, { 72, 20 },
        { 11, 55 }, { 67, 45 }, { 84, 93 }, { 57, 90 }, { 42, 76 }, { 40, 94 }, { 96, 74 }, { 82, 59 }, { 49, 17 },
        { 58, 36 }, { 96, 33 }, { 90, 37 }, { 16, 74 }, { 44, 78 }, { 40, 67 }, { 95, 95 }, { 99, 21 }, { 69, 38 },
        { 91, 78 }, { 32, 26 }, { 67, 46 }, { 28, 46 }, { 97, 51 }, { 52, 46 }, { 88, 24 }, { 18, 19 }, { 36, 47 },
        { 99, 68 }, { 50, 84 }, { 75, 23 }, { 72, 74 }, { 97, 31 }, { 81, 22 }, { 65, 45 }, { 44, 69 }, { 64, 36 },
        { 57, 29 }, { 89, 57 }, { 25, 63 }, { 18, 14 }, { 43, 21 }, { 56, 27 }, { 26, 39 }, { 68, 20 }, { 72, 91 },
        { 41, 26 }
    };
    int nums_size = (sizeof nums_for_multiplication) / (sizeof nums_for_multiplication[0]);

    // Setup context with parameters
    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(1024);
    SEALContext context(parms);
    print_parameters(context);

    // Key generation
    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    // Operation instances
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    // Example
    uint64_t x = 6;
    Plaintext x_plain(uint64_to_hex_string(x));
    Ciphertext x_encrypted;
    encryptor.encrypt(x_plain, x_encrypted);
    Plaintext x_decrypted;
    decryptor.decrypt(x_encrypted, x_decrypted);

    Ciphertext x_sq_plus_one;
    evaluator.square(x_encrypted, x_sq_plus_one);
    Plaintext plain_one("1");
    evaluator.add_plain_inplace(x_sq_plus_one, plain_one);
    Plaintext decrypted_result;
    decryptor.decrypt(x_sq_plus_one, decrypted_result);

    Ciphertext x_plus_one_sq;
    evaluator.add_plain(x_encrypted, plain_one, x_plus_one_sq);
    evaluator.square_inplace(x_plus_one_sq);
    decryptor.decrypt(x_plus_one_sq, decrypted_result);
    Ciphertext encrypted_result;
    Plaintext plain_four("4");
    evaluator.multiply_plain_inplace(x_sq_plus_one, plain_four);
    evaluator.multiply(x_sq_plus_one, x_plus_one_sq, encrypted_result);

    // KeyGenerator keygen(context);
    // SecretKey secret_key = keygen.secret_key();
    // PublicKey public_key;
    // keygen.create_public_key(public_key);

    // Encryptor encryptor(context, public_key);
    // Evaluator evaluator(context);
    // Decryptor decryptor(context, secret_key);

    // uint64_t x = 6;
    // Plaintext x_plain(uint64_to_hex_string(x));
    // Ciphertext x_encrypted;
    // encryptor.encrypt(x_plain, x_encrypted);
    // Plaintext x_decrypted;
    // decryptor.decrypt(x_encrypted, x_decrypted);

    // Ciphertext x_sq_plus_one;
    // evaluator.square(x_encrypted, x_sq_plus_one);
    // Plaintext plain_one("1");
    // evaluator.add_plain_inplace(x_sq_plus_one, plain_one);
    // Plaintext decrypted_result;
    // decryptor.decrypt(x_sq_plus_one, decrypted_result);

    // Ciphertext x_plus_one_sq;
    // evaluator.add_plain(x_encrypted, plain_one, x_plus_one_sq);
    // evaluator.square_inplace(x_plus_one_sq);
    // decryptor.decrypt(x_plus_one_sq, decrypted_result);
    // Ciphertext encrypted_result;
    // Plaintext plain_four("4");
    // evaluator.multiply_plain_inplace(x_sq_plus_one, plain_four);
    // evaluator.multiply(x_sq_plus_one, x_plus_one_sq, encrypted_result);

    // // A better way
    // RelinKeys relin_keys;
    // keygen.create_relin_keys(relin_keys);

    // Ciphertext x_squared;
    // evaluator.square(x_encrypted, x_squared);

    // evaluator.relinearize_inplace(x_squared, relin_keys);
    // evaluator.add_plain(x_squared, plain_one, x_sq_plus_one);
    // decryptor.decrypt(x_sq_plus_one, decrypted_result);

    // Ciphertext x_plus_one;

    // evaluator.add_plain(x_encrypted, plain_one, x_plus_one);
    // evaluator.square(x_plus_one, x_plus_one_sq);
    // evaluator.relinearize_inplace(x_plus_one_sq, relin_keys);
    // decryptor.decrypt(x_plus_one_sq, decrypted_result);

    // evaluator.multiply_plain_inplace(x_sq_plus_one, plain_four);
    // evaluator.multiply(x_sq_plus_one, x_plus_one_sq, encrypted_result);
    // evaluator.relinearize_inplace(encrypted_result, relin_keys);

    // decryptor.decrypt(encrypted_result, decrypted_result);

    // parms.set_poly_modulus_degree(2048);
    // context = SEALContext(parms);
    // print_parameters(context);
}
