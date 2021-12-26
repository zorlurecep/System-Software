#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_TEMPERATURE 35
#define MIN_TEMPERATURE -10
#define FREQUENCY 5 

double generate_temperature();

int main(int argc, char const *argv[])
{
    srand(time(NULL));    
    while (1)
    {
        double current_temperature = generate_temperature();

        printf("Temperature = %.2f @%ld\n", current_temperature, time(NULL));
        fflush( stdout );
        sleep(FREQUENCY);
    }

    return 0;
}

double generate_temperature()
{
    double random_temperature = (rand() % (MAX_TEMPERATURE - MIN_TEMPERATURE + 1)) + MIN_TEMPERATURE;
    return random_temperature;
}
