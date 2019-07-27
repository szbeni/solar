typedef struct 
{
    uint16_t adcRaw[5];
    float solarVoltage, solarCurrent;

} solar_struct;


void solar_main(void);
void solar_comm_init(void);

