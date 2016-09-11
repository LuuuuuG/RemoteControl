#include "INS.h"

INS::INS(/*char *portName*/)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(/*portName*/COMPORT) ,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                /*FILE_ATTRIBUTE_NORMAL*/0,//ͬ��
                                NULL);
    if (this->handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
			printf("ERROR: Handle was not attached. Reason: %s not available\n", COMPORT);
        }
    else
        {
            printf("ERROR!!!");
        }
    }
    else {
        DCB dcbSerialParameters = {0};

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
        }
        else {
            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
           // dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

INS::~INS()
{
    if (this->connected){
        this->connected = false;
        CloseHandle(this->handler);
    }
}

int INS::readINS(char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    int toRead ;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0){
        if (this->status.cbInQue > buf_size){
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

	if (ReadFile(this->handler, buffer, toRead, &bytesRead, 0))	return bytesRead;


    return 0;
}

bool INS::writeINS(char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend, 0)){
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else return true;
}

bool INS::isConnected()
{
    return this->connected;
}