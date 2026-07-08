#include "vmd_algorithm.h"
#include <math.h>
#include <string.h>

static float modes[NUM_MODES][SAMPLE_LENGTH];
static float omega[NUM_MODES];
static float lambda_fft[NUM_MODES][SAMPLE_LENGTH * 2];
static float modes_fft[NUM_MODES][SAMPLE_LENGTH * 2];
static float prev_modes[NUM_MODES][SAMPLE_LENGTH * 2];
static float energy[NUM_MODES];
static int valid_modes[NUM_MODES];

#ifdef USE_ARM_MATH
#include "arm_math.h"
#endif

void initialize_vmd(void)
{
    for (int i = 0; i < NUM_MODES; i++) {
        omega[i] = 0.0f;
        for (int j = 0; j < SAMPLE_LENGTH; j++) {
            modes[i][j] = 0.0f;
        }
        for (int j = 0; j < SAMPLE_LENGTH * 2; j++) {
            lambda_fft[i][j] = 0.0f;
            modes_fft[i][j] = 0.0f;
            prev_modes[i][j] = 0.0f;
        }
    }
}

static void update_modes_in_frequency_domain(float* input_fft)
{
    for (int k = 0; k < NUM_MODES; k++) {
        for (int n = 0; n < SAMPLE_LENGTH; n++) {
            float sum_real = 0.0f, sum_imag = 0.0f;
            for (int m = 0; m < NUM_MODES; m++) {
                if (m != k) {
                    sum_real += modes_fft[m][2 * n];
                    sum_imag += modes_fft[m][2 * n + 1];
                }
            }
            float real_part = input_fft[2 * n] - sum_real - lambda_fft[k][2 * n];
            float imag_part = input_fft[2 * n + 1] - sum_imag - lambda_fft[k][2 * n + 1];
            float denom = 1.0f + ALPHA * (n - omega[k]) * (n - omega[k]);
            modes_fft[k][2 * n] = real_part / denom;
            modes_fft[k][2 * n + 1] = imag_part / denom;
        }
    }
}

static void update_frequencies(void)
{
    for (int k = 0; k < NUM_MODES; k++) {
        float num = 0.0f, den = 0.0f;
        for (int n = 0; n < SAMPLE_LENGTH; n++) {
            float real = modes_fft[k][2 * n];
            float imag = modes_fft[k][2 * n + 1];
            float mag2 = real * real + imag * imag;
            num += n * mag2;
            den += mag2;
        }
        omega[k] = (den > 1e-10f) ? (num / den) : 0.0f;
    }
}

static void update_lagrange_multipliers(float* input_fft)
{
    for (int k = 0; k < NUM_MODES; k++) {
        for (int n = 0; n < SAMPLE_LENGTH; n++) {
            float sum_real = 0.0f, sum_imag = 0.0f;
            for (int m = 0; m < NUM_MODES; m++) {
                sum_real += modes_fft[m][2 * n];
                sum_imag += modes_fft[m][2 * n + 1];
            }
            lambda_fft[k][2 * n] += TAU * (input_fft[2 * n] - sum_real);
            lambda_fft[k][2 * n + 1] += TAU * (input_fft[2 * n + 1] - sum_imag);
        }
    }
}

static double compute_norm_squared(float old[], float new_arr[])
{
    double norm = 0.0;
    for (int i = 0; i < SAMPLE_LENGTH * 2; i++) {
        float diff = new_arr[i] - old[i];
        norm += diff * diff;
    }
    return norm;
}

void update_modes(float input_signal[])
{
    for (int i = 0; i < SAMPLE_LENGTH; i++) {
        modes[0][i] = input_signal[i];
        for (int k = 1; k < NUM_MODES; k++) {
            modes[k][i] = 0.0f;
        }
    }
    for (int k = 0; k < NUM_MODES; k++) {
        valid_modes[k] = (k == 0) ? 1 : 0;
    }
}

void calculate_energy(void)
{
    double total = 0.0;
    for (int i = 0; i < NUM_MODES; i++) {
        double sum = 0.0;
        for (int j = 0; j < SAMPLE_LENGTH; j++) {
            sum += modes[i][j] * modes[i][j];
        }
        energy[i] = (float)sum;
        total += sum;
    }
    if (total > 1e-10f) {
        for (int i = 0; i < NUM_MODES; i++) {
            energy[i] /= total;
        }
    }
}

void filter_modes_by_energy(void)
{
    for (int i = 0; i < NUM_MODES; i++) {
        valid_modes[i] = (energy[i] >= ENERGY_THRESHOLD) ? 1 : 0;
    }
}

void output_results(float reconstructed[])
{
    for (int n = 0; n < SAMPLE_LENGTH; n++) {
        reconstructed[n] = 0.0f;
        for (int k = 0; k < NUM_MODES; k++) {
            if (valid_modes[k]) {
                reconstructed[n] += modes[k][n];
            }
        }
    }
}

float calculate_mean(float data[], int len)
{
    float sum = 0.0f;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum / len;
}

float calculate_variance(float data[], float mean, int len)
{
    float sum = 0.0f;
    for (int i = 0; i < len; i++) {
        float diff = data[i] - mean;
        sum += diff * diff;
    }
    return sum / len;
}
