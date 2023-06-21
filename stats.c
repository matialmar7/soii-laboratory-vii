#include "stats.h"
#include "string.h"


void getRunTimeStats(char *buffer)
{
    TaskStatus_t *pxTaskStatusArray; //Structure to store task status
    UBaseType_t uxArraySize, x, uxCurrentNumberOfTasks;
    uint32_t ulTotalTime, ulStatsAsPercentage;

    *buffer = (char)0x00;
    uxCurrentNumberOfTasks = uxTaskGetNumberOfTasks();
    uxArraySize = uxCurrentNumberOfTasks;

    pxTaskStatusArray = pvPortMalloc(uxCurrentNumberOfTasks * sizeof(TaskStatus_t));

    if (pxTaskStatusArray != NULL)
    {
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalTime);
        ulTotalTime /= 100UL;

        if (ulTotalTime > 0UL)
        {
            for (x = 0; x < uxArraySize; x++)
            {
                ulStatsAsPercentage = pxTaskStatusArray[x].ulRunTimeCounter / ulTotalTime;
                getStatesTasks(buffer, pxTaskStatusArray[x].xTaskNumber, (char *)pxTaskStatusArray[x].pcTaskName, pxTaskStatusArray[x].eCurrentState, pxTaskStatusArray[x].uxCurrentPriority);
                getStatsMEM(buffer, pxTaskStatusArray[x].usStackHighWaterMark);
                getStatsCPU(buffer, pxTaskStatusArray[x].ulRunTimeCounter, ulStatsAsPercentage);
            }
            strcat(buffer, divider);
        }

        buffer += strlen(buffer);
    }

    vPortFree(pxTaskStatusArray);
    
   return;
}

void getStatesTasks(char *dest, UBaseType_t TaskNum, char *task_name, UBaseType_t state, UBaseType_t priority)
{
    char buf[3] = {0};
    getStateString(state, buf);

    strcat(dest, itoa(TaskNum, 10));
    strcat(dest, "\t");
    strcat(dest, task_name);
    strcat(dest, "\t ");
    strcat(dest, buf);
    strcat(dest, "\t ");
    strcat(dest, ltoc(priority, buf, 10));
}

void getStatsMEM(char *dest, uint32_t usStackHighWaterMark)
{
    char buf[10] = {0};
    strcat(dest, "\t");
    strcat(dest, ltoc(usStackHighWaterMark, buf, 10));
}

void getStatsCPU(char *dest, uint32_t runTime, uint32_t statsPorcentage)
{
    char buffer[10] = {0};
    strcat(dest, "\t");
    (runTime != 0) ? strcat(dest, ltoc(runTime, buffer, 10)) : strcat(dest, "< 1");
    strcat(dest, "\t  ");
    
    if (statsPorcentage > 0)
        strcat(dest, ltoc(statsPorcentage, buffer, 10));
    else
        strcat(dest, "< 1");

    strcat(dest, "%\r\n");
}

void getStateString(UBaseType_t state, char *buf)
{
    if (state > 4)
        return;

    const char *states = "XRBS";
    char cState[2] = { states[state], '\0' };
    strcpy(buf, cState);
}

char *itoa(int val, int base)
{

    static char buf[32] = {0};

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];
}

char *ltoc(unsigned long value, char *ptr, int base)
{
    if (NULL == ptr)
        return NULL;

    unsigned long t = 0, res = 0, tmp = value;

    int count = 0;

    if (tmp == 0)
        count++;

    while (tmp > 0)
    {
        tmp = tmp / base;
        count++;
    }

    ptr += count;

    *ptr = '\0';

    do
    {
        res = value - base * (t = value / base);
        if (res < 10)
            *--ptr = '0' + res;

        else if ((res >= 10) && (res < 16))
            *--ptr = 'A' - 10 + res;

    } while ((value = t) != 0);

    return (ptr);
}

void print(const char *msg)
{
    do{
        UARTCharPut(UART0_BASE, *msg);
    }while (*(msg++) != 0x00);
}
