#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include "Stream.h"
#include "utility/twi.h"

#define BUFFER_LENGTH    (128)
#define STANDARDMODE     (100000UL)
#define FASTMODE         (400000UL)
#define HIGHSPEEDMODE    (3300000UL)
 
class TwoWire : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
  public:
    TwoWire();
    void begin();
    void begin(uint8_t);
    void begin(uint32_t, uint8_t slaveaddr=0x7f);
    void begin(int);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
	virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );
#if defined (DMP_LINUX)
	bool send(uint8_t, uint8_t*, int);
	bool receive(uint8_t, uint8_t*, uint8_t);
	bool sensorRead(uint8_t, uint8_t, uint8_t*, uint8_t);
	bool sensorReadEX(uint8_t, uint8_t*, int, uint8_t*, uint8_t);
#endif
  
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

class TwoWireLEGO : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
  public:
    TwoWireLEGO();
    void begin(unsigned long nHz=10000L);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
	virtual void flush(void);
#if defined (DMP_LINUX)
	bool send(uint8_t, uint8_t*, int);
	bool receive(uint8_t, uint8_t*, uint8_t);
	bool sensorRead(uint8_t, uint8_t, uint8_t*, uint8_t);
	bool sensorReadEX(uint8_t, uint8_t*, int, uint8_t*, uint8_t);
#endif
};

extern TwoWire Wire;
extern TwoWireLEGO WireLEGO;
#endif

