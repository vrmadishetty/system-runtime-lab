#include <stdio.h>
#include <math.h>


int main(){
    float samples_per_second = 44100;
    int tone_freq = 440;
    int duration = 2;
    int max_value_encoding = 32767;

    int i = 0;
    int total_samples = duration * samples_per_second;

    float time = 0;
    float angle;

    short int sample;

    FILE *f;
    f = fopen("sine.raw", "wb");
    if (f == NULL) {
        printf("could not open a file. \n");
        return -1;
    }

    while (i < total_samples){
        time = i * (1/samples_per_second);
        angle = 2 * 3.14 * tone_freq * time;

        sample = max_value_encoding * sin(angle);

        fwrite(&sample, 2, 1, f);
        printf("%d ", sample);
        i = i + 1; 
    }

    fclose(f);
    return 0;
}

