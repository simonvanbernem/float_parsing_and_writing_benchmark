#include <random>
#include <charconv>
#include "stdio.h"
#include "time.h"
#include "assert.h"
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_printf.h"

// const int random_number_count = 1024 * 1024 * 50;
const int random_number_count = 1024 * 1024;
float random_floats[random_number_count];
double random_doubles[random_number_count];

char snprintf_floats[random_number_count * 15];
char snprintf_doubles[random_number_count * 20];

char stbsp_snprintf_floats[random_number_count * 15];
char stbsp_snprintf_doubles[random_number_count * 20];

char to_chars_floats[random_number_count * 15];
char to_chars_doubles[random_number_count * 20];

double get_time(){
    return (double)clock()/(double)CLOCKS_PER_SEC;
}


int main(){
    {
        std::mt19937 random_generator(1);
        std::uniform_real_distribution<float> float_distribution(-1000000, 1000000);
        std::uniform_real_distribution<double> double_distribution(-1000000, 1000000);

        for(int i = 0; i < random_number_count; i++){
            random_floats[i] = float_distribution(random_generator);
            random_doubles[i] = double_distribution(random_generator);
        }

        printf("The first 10 random floats:");
        for(int i = 0; i < 10; i++)
            printf(" %.3f", random_floats[i], random_doubles[i]);
        
        printf("\n");
    }


    


#define TEST_PREFIX(output_buffer)                                  \
    {                                                               \
        int buffer_index = 0;                                       \
        auto start_time = get_time();                               \
                                                                    \
        for(int i = 0; i < random_number_count; i++){               \
            auto buffer = output_buffer + buffer_index;             \
            auto buffer_size = sizeof(output_buffer) - buffer_index;

#define TEST_POSTFIX(name, real_type_name) \
        } \
        auto duration = (get_time() - start_time); \
        auto total_in_ms = duration * 1000; \
        auto per_number_in_us = (duration / (double)random_number_count) * 1000 * 1000; \
        printf("Printing %d %-6s with %-20s took %.2fms (%.2fus per number).\n", random_number_count, real_type_name, name, total_in_ms, per_number_in_us);\
    }


TEST_PREFIX(snprintf_floats)
int result = snprintf(buffer, buffer_size, "%f", random_floats[i]);
assert(result >= 1);
buffer_index += result;
TEST_POSTFIX("sprintf", "float")

TEST_PREFIX(snprintf_doubles);
int result = snprintf(buffer, buffer_size, "%f", random_doubles[i]);
assert(result >= 1);
buffer_index += result;
TEST_POSTFIX("sprintf", "double");


TEST_PREFIX(stbsp_snprintf_floats)
int result = stbsp_snprintf(buffer, buffer_size, "%f", random_floats[i]);
assert(result >= 1);
buffer_index += result;
TEST_POSTFIX("stbsp_sprintf", "float")

TEST_PREFIX(stbsp_snprintf_doubles);
int result = stbsp_snprintf(buffer, buffer_size, "%f", random_doubles[i]);
assert(result >= 1);
buffer_index += result;
TEST_POSTFIX("stbsp_sprintf", "double");


TEST_PREFIX(to_chars_floats)
auto result = std::to_chars(buffer, buffer + buffer_size - 1, random_floats[i]);
assert(result.ec == std::errc());
*result.ptr = '\0';
result.ptr++;
buffer_index += result.ptr - buffer;
TEST_POSTFIX("to_chars", "float")

TEST_PREFIX(to_chars_doubles);
auto result = std::to_chars(buffer, buffer + buffer_size - 1, random_doubles[i]);
assert(result.ec == std::errc());
*result.ptr = '\0';
result.ptr++;
buffer_index += result.ptr - buffer;
TEST_POSTFIX("to_chars", "double");

    return 0;
}