#include "segment7_controller.h"



void segmentCallback(Segment7* segment7)
{
	printf("Callback!\n");

	printf("New value of segment7 id %i is: %i\n", segment7->id, segment7->value);
}

int main()
{
	printf("Starting segment7 test program\n");
#ifdef ARM
    wiringPiSetup();
#endif    
// Clear printf buffer immediately
    setbuf(stdout, NULL);
    uint8_t isRunning = 1;

    static char buffer[50] = {};

	if(segment7Setup(2, segmentCallback))
	{
		printf("Successfully setup segment7\n");
	}

	startSegment7();

	uint8_t data[4] = {};
	int len, val;

	while(isRunning)
	{
		printf("Input 4 new data values. \n");

		for(size_t i = 0; i < 4; i++)
		{
			fgets(buffer, 50, stdin);

			len = strlen(buffer) - 1;

			for(size_t i = 0; i < len; ++i)
			{
				if(!isdigit(buffer[i])) 
				{
					printf("Invalid input");
					return 1;
				}
			}
			val = atoi(buffer);
			data[i] = val;
		}
			

		setTestData(data);

	}


	sleep(5);

	stopSegment7();

	sleep(2);

    return 0;
}
