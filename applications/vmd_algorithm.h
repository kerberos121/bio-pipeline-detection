#ifndef __VMD_ALGORITHM_H__
#define __VMD_ALGORITHM_H__

#define SAMPLE_LENGTH   128
#define NUM_MODES       6
#define MAX_ITER        200
#define ALPHA           1.0f
#define TAU             0.0f
#define TOL             1e-4f
#define ENERGY_THRESHOLD 0.01f

void initialize_vmd(void);
void update_modes(float input_signal[]);
void calculate_energy(void);
void filter_modes_by_energy(void);
void output_results(float reconstructed[]);
float calculate_mean(float data[], int len);
float calculate_variance(float data[], float mean, int len);

#endif
