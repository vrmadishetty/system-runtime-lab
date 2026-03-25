#include <stdio.h>

enum sensor_type {
    TEMPERATURE,
    HUMIDITY,
    PRESSURE
};

enum sensor_state {
    ACTIVE,
    INACTIVE,
    ERROR
};

struct temperature_data {
    short int min;
    short int max;
    float reading;
} __attribute__((packed));

struct humidity_data {
    float calibration;
    float reading;
}__attribute__((packed));;

struct pressure_data {
    short int altitude;
    float reading;
}__attribute__((packed));;

union readings {
    struct temperature_data temp;
    struct humidity_data humidity;
    struct pressure_data pressure;
};

struct sensor {
    unsigned char id;
    char name[20];
    enum sensor_type type;
    enum sensor_state status;
    union readings value;
}__attribute__((packed));

int main ()
{
    struct sensor sensor_list[10] = {0};
    struct sensor tmp = {0};
    unsigned int add_sensor;
    unsigned int sensor_count = 0;
    unsigned int init_sensor = 0;

    while (1) {
        if (sensor_count >= 10) {
            printf("No slots add more sensors\n");
        }
        init_sensor = 0;
        printf("Want to add sensor? Enter 1 to add, 0 to skip: ");
        scanf("%u", &init_sensor);

        if(init_sensor == 0) {
            break;
        }

        printf("ID: ");
        scanf("%c", &tmp.id);
        scanf("%*c");

        printf("Name: ");
        scanf("%s", (char *)&tmp.name);
        scanf("%*c");

        printf("Type (t, h, p): ");
        char type;
        scanf("%c", &type);
        scanf("%*c");

        switch (type) {
            case 't': tmp.type = TEMPERATURE;
                break;
            case 'h': tmp.type = HUMIDITY;
                break;
            case 'p': tmp.type = PRESSURE;
                break;
        }
        tmp.status = INACTIVE;

        switch (tmp.type) {
            case TEMPERATURE:
                printf("min: ");
                scanf("%hd", &tmp.value.temp.min);
                scanf("%*c");
                
                printf("max: ");
                scanf("%hd", &tmp.value.temp.max);
                scanf("%*c");

                printf("reading: ");
                scanf("%f", &tmp.value.temp.reading);
                scanf("%*c");
            break;
            case HUMIDITY:
                printf("calibration: ");
                scanf("%f", &tmp.value.humidity.calibration);
                scanf("%*c");

                printf("reading: ");
                scanf("%f", &tmp.value.humidity.reading);
                scanf("%*c");
            break;
            case PRESSURE:
                printf("altitude: ");
                scanf("%hd", &tmp.value.pressure.altitude);
                scanf("%*c");

                printf("reading: ");
                scanf("%f", &tmp.value.pressure.reading);
                scanf("%*c");
            break;
        }
        break;

    }
    printf("ID: %u\n", tmp.id);
    printf("Name: %s\n", tmp.name);
    printf("Type: %u\n", tmp.type);

    return 0;
}