#ifndef INC_DESIOT_GATEWAY_H_
#define INC_DESIOT_GATEWAY_H_

#ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>
#endif

//UART Library
#include <driver/uart.h>
#include <soc/uart_struct.h>
#include <soc/uart_reg.h>

#define DESIOT_UART_NUM UART_NUM_2 //Using uart 2

#define DESIOT_CIR_BUF_SIZE 1024u
#define DESIOT_MAX_DATA_SIZE 1023u
#define DESIOT_GATEWAYID_SIZE 25u
#define DESIOT_CONNECTION_TYPE_SIZE 1u
#define DESIOT_CONNECTION_ID_SIZE 1u
#define DESIOT_ADDITIONAL_GATEWAY_FRAME_SIZE DESIOT_GATEWAYID_SIZE + DESIOT_CONNECTION_TYPE_SIZE + DESIOT_CONNECTION_ID_SIZE

// attributes
#define DESIOT_ATT_PACKED __attribute__((__packed__))
#define DESIOT_ATT_UNUSED __attribute__((__unused__))
#define DESIOT_ATT_WEAK __attribute__((weak))

// Lengths of packet types
#define DESIOT_H1_LEN 1u
#define DESIOT_H2_LEN 1u
#define DESIOT_HEAD_LEN (DESIOT_H1_LEN + DESIOT_H2_LEN)
#define DESIOT_T1_LEN 1u
#define DESIOT_T2_LEN 1u
#define DESIOT_TRAIL_LEN (DESIOT_T1_LEN + DESIOT_T2_LEN)
#define DESIOT_HEADERS_LEN (DESIOT_HEAD_LEN + DESIOT_TRAIL_LEN)
#define DESIOT_CRC_LEN 2u
#define DESIOT_CMD_LEN 0x1u
#define DESIOT_DATALEN_LEN 0x2u
#define DESIOT_HEAD_FRAME_LEN (DESIOT_HEAD_LEN + DESIOT_CMD_LEN + DESIOT_DATALEN_LEN)
#define DESIOT_TRAIL_FRAME_LEN DESIOT_TRAIL_LEN + DESIOT_CRC_LEN
#define DESIOT_FIXED_COMPOMENTS_LENGTH DESIOT_HEAD_FRAME_LEN + DESIOT_TRAIL_FRAME_LEN


void DESIoT_UART_begin();
void DESIoT_G_loop();
void DESIoT_G_frameArbitrating();
void DESIoT_G_frameProssessLoop();

//Circular buffer 
typedef struct{
    uint16_t start : 10;
    uint16_t end : 10;
    uint16_t startRestore : 10;
    uint8_t buffer[DESIOT_CIR_BUF_SIZE];
} DESIoT_CBUF_t;

enum DESIOT_CBUF_STATUS{
    DESIOT_CBUF_OK,
    DESIOT_CBUF_ERROR
};

//Circular buffer methods
uint8_t DESIoT_CBUF_getByte(DESIoT_CBUF_t *hCBuf, uint8_t *rx);
void DESIoT_CBUF_putByte(DESIoT_CBUF_t *hCBuf, uint8_t rx);
uint8_t DESIoT_CBUF_isEmpty(DESIoT_CBUF_t *hCBuf);

//Data packet structure
typedef struct{
    uint8_t cmd;
    union
    {
        uint8_t dataLenArr[2];
        uint16_t dataLen : 10;
    };
    uint8_t data[DESIOT_MAX_DATA_SIZE];
} DESIOT_ATT_PACKED DESIoT_dataPacket_t;

typedef struct
{
    uint8_t gateway_id[DESIOT_GATEWAYID_SIZE];
    uint8_t connection_type;
    uint8_t connection_id;
} DESIoT_additionalGatewayData_t;

//Frame structure
typedef struct
{
    uint8_t h1;
    uint8_t h2;
    DESIoT_dataPacket_t dataPacket;
    uint8_t t1;
    uint8_t t2;
    union
    {
        uint16_t crc;
        uint8_t crcArr[2];
    };
} DESIOT_ATT_PACKED DESIoT_Frame_t;

//Frame handler structure
typedef struct
{
    uint8_t index;
    uint8_t status;
    uint64_t millis;
    uint64_t loopTick;
    DESIoT_Frame_t frame;

    DESIoT_CBUF_t *curCBuf;

    char gateway_id[DESIOT_GATEWAYID_SIZE];
    char mqttTopic[14 + DESIOT_GATEWAYID_SIZE];
} DESIoT_Frame_Hander_t;

//Define frame handler methods
#define DESIOT_SET_FRAME_FAILED_STATUS(status) status--
#define DESIOT_SET_FRAME_SUCCESS_STATUS(status) status -= 2
#define DESIOT_IS_FRAME_ON_PROCESS_STATUS(status) ((status != DESIOT_FRAME_IDLE) && !(status % 3))
#define DESIOT_TIMEOUT_DURATION 1000

//define frame state 
enum DESIOT_FRAME_STATUSES
{
    DESIOT_FRAME_IDLE,
    //
    DESIOT_FRAME_UART2_SUCCESS,
    DESIOT_FRAME_UART2_FAILED,
    DESIOT_FRAME_IN_UART2_PROGRESS,
    //
    DESIOT_FRAME_MQTT_SUCCESS,
    DESIOT_FRAME_MQTT_FAILED,
    DESIOT_FRAME_IN_MQTT_PROGRESS,
};

//Define frame index
enum DESIOT_HEAD_FRAME_INDEXES
{
    DESIOT_H1_INDEX, //0 
    DESIOT_H2_INDEX, //1
    DESIOT_CMD_INDEX, //2
    DESIOT_DATALEN_INDEX //3
};

// DESIoT default values
#define DESIOT_H1_DEFAULT 0x7u
#define DESIOT_H2_DEFAULT 0x17u
#define DESIOT_T1_DEFAULT 0x70u
#define DESIOT_T2_DEFAULT 0x71u

// CRC generator and CRC methods
#define DESIOT_CRC_GENERATOR 0x1305
void DESIoT_CalculateTable_CRC16();
uint16_t DESIoT_Compute_CRC16(uint8_t *bytes, const int32_t BYTES_LEN);

// Connection types
enum DESIOT_CONNECTION_TYPES{
    DESIOT_SERIAL_CONNECTION_TYPE
};

// Connection IDs
enum DESIOT_CONNECTION_IDS{
    DESIOT_UART2_ID
};

//Frame processing methods configuration
void DESIoT_setUpStartOfParsing(DESIoT_Frame_Hander_t *hFrame, DESIoT_CBUF_t *curCBuf);
void DESIoT_FRAME_parsing(DESIoT_Frame_Hander_t *hFrame, uint8_t byte, DESIoT_CBUF_t *curCBuf);
void DESIoT_frameFailedHandler();
void DESIoT_frameSuccessHandler();
void DESIoT_restartFrameIndexes();
void DESIoT_restartCBufIndexes();
void DESIoT_frameTimeoutHandler();
void DESIoT_sendFrameToServer(uint8_t connection_type, uint8_t connection_id); //send frame to API server
void DESIoT_sendFrameToDevice(); //send frame to STM32

#ifdef ESP32
static intr_handle_t handle_console;
extern DESIoT_Frame_Hander_t hFrame;

static void IRAM_ATTR DESIoT_UART_INTR_HANDLE(void *arg);
unsigned long DESIoT_millis();
#endif

//Loop tick check
static void DESIoT_loopTickCheck(void *parameter)
{
    DESIoT_Frame_Hander_t *phFrame = (DESIoT_Frame_Hander_t *)parameter;
    for (;;)
    {
        if ((int)(DESIoT_millis() - phFrame->loopTick) > 1000)
            ESP.restart();
    }
}

static void DESIoT_G_begin()
{
    hFrame.loopTick = DESIoT_millis(); // bug

    DESIoT_UART_begin();
    //DESIoT_MQTT_begin();
    //
#if defined(DESIOT_USER_GATEWAY_ID)
    strcpy(hFrame.gateway_id, DESIOT_USER_GATEWAY_ID);
    char topic[] = DESIOT_DESIIOT_MQTT_TOPIC_PREFIX;
    strcpy(hFrame.mqttTopic, topic);
    strcat(hFrame.mqttTopic, DESIOT_USER_GATEWAY_ID);

#endif

    xTaskCreate(DESIoT_loopTickCheck, "DESIoT_loopTickCheck", 2000, &hFrame, 1, NULL);
}

#endif /* INC_DESIOT_GATEWAY_H_ */