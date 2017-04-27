#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <letmecreate/core.h>
#include <awa/client.h>
#include <awa/common.h>
#include <sys/time.h>

static bool run = true;

#define OPERATION_PERFORM_TIMEOUT 1000

static void signalHandler(int signo) {
    printf("Requested termination \n");
    fflush(stdout);
    fflush(stderr);
    run = false;
}

void installSignalActions() {
    struct sigaction psa;

    psa.sa_handler = signalHandler;
    sigaction(SIGTSTP, &psa, NULL);
    sigaction(SIGINT, &psa, NULL);
    sigaction(SIGQUIT, &psa, NULL);
    sigaction(SIGABRT, &psa, NULL);
    sigaction(SIGSTOP, &psa, NULL);
}



int defineAndCreateObjects(AwaClientSession *session) {
    printf("Defining IPSO 3335 object...");

    if (AwaClientSession_IsObjectDefined(session, 3335) == false) {
        AwaObjectDefinition *objectDefinition = AwaObjectDefinition_New(3335, "LightControl", 0, 1);
        AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition,5706, "Color", false, AwaResourceOperations_ReadWrite,NULL);
        AwaClientDefineOperation *operationDefine = AwaClientDefineOperation_New(session);
        AwaClientDefineOperation_Add(operationDefine, objectDefinition);
        AwaClientDefineOperation_Perform(operationDefine,OPERATION_PERFORM_TIMEOUT);
        AwaClientDefineOperation_Free(&operationDefine);
        printf("done\n");
    } else {
        printf("Object Already defined\n");
    }

    AwaClientGetOperation *operationGet = AwaClientGetOperation_New(session);
    AwaClientGetOperation_AddPath(operationGet, "/3335/0/5706");
    printf("Creating IPSO 3335 object...");
    if (AwaError_Success != AwaClientGetOperation_Perform(operationGet, OPERATION_PERFORM_TIMEOUT)) {
        AwaClientSetOperation *operation = AwaClientSetOperation_New(session);
        AwaError error;
        if (operation == NULL) {
            printf("Failed to create set operation\n");
            return -1;
        }
        if (AwaError_Success != AwaClientSetOperation_CreateObjectInstance(operation, "/3335/0")) {
            printf("Failed to create object instance\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
        if (AwaError_Success != AwaClientSetOperation_CreateOptionalResource(operation, "/3335/0/5706")) {
            printf("Failed to create object resource\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
        if (AwaError_Success != AwaClientSetOperation_AddValueAsCString(operation, "/3335/0/5706", "none")) {
            printf("Failed to set default value\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
        if (AwaError_Success != AwaClientSetOperation_AddValueAsInteger(operation, "/1/1/2", 0)) {
            printf("Failed to set /1/1/2 value\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }

        if (AwaError_Success != (error = AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT))) {
            printf("Failed to perform operation %d\n", error);
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
        printf("done\n");
    } else {
        printf("Already exist\n");
    }

    AwaClientGetOperation_Free(&operationGet);

    return 0;
}

AwaClientSession* initAwa() {
    printf("Creating new Awa Session...");
    AwaClientSession *session = AwaClientSession_New();
    if (NULL == session) {
        printf("AwaClientSession_New() failed\n");
        return NULL;
    }
    if (AwaError_Success != AwaClientSession_SetIPCAsUDP(session, "127.0.0.1", 12345)) {
        printf("AwaClientSession_SetIPCAsUDP() failed\n");
        AwaClientSession_Free(&session);
        return NULL;
    }
    if (AwaError_Success != AwaClientSession_Connect(session)) {
        printf("AwaClientSession_Connect() failed\n");
        AwaClientSession_Free(&session);
        return NULL;
    }
    printf("done\n");

    if (defineAndCreateObjects(session) != 0) {
        printf("Destroying Awa Session...");
        AwaClientSession_Free(&session);
        printf("done\n");
        return NULL;
    }

    return session;
}

int sendToAwa(uint8_t* buffer, AwaClientSession *session) {
    printf("Sending to awa...");

//--- set :)
    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);
    if (NULL != operation) {
        if (AwaError_Success != AwaClientSetOperation_AddValueAsCString(operation, "/3335/0/5706", buffer)) {
            printf("AwaClientSetOperation_AddValueAsCString() failed\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
        if (AwaError_Success != AwaClientSetOperation_Perform(operation, 2000)) {
            printf("AwaClientSetOperation_Perform() failed\n");
            AwaClientSetOperation_Free(&operation);
            return -1;
        }
    }
    AwaClientSetOperation_Free(&operation);
    printf("done.\n");
    return 0;

}

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int main(int argc, char **argv) {
    installSignalActions();
    if (uart_init() != 0) {
        printf("Can't init uart!");
        return -1;
    }
    uart_select_bus(MIKROBUS_2);
    if (uart_set_baudrate(57600) != 0) {
        printf("Baudrate failed\n");
    }
    uart_set_timeout(0);


    uint8_t buffer[200];
    int index = 0;
    uint8_t tmp;
    bool skipEndLine = false;

    printf("v 0.2, Started UART read.\n");
    long long startTime = current_timestamp();
    AwaClientSession *session = NULL;
    while (run) {

        while (session == NULL && run) {
            session = initAwa();
        }
        int len = uart_receive(&tmp, 1);
        if (len != 0) {
            if (tmp > 32) {
                buffer[index] = tmp;
                index++;
            }
            if (tmp == 10 && skipEndLine == false) {
                buffer[index] = 0;
                long long now = current_timestamp();
                printf("[%lldms]%s\n", (now - startTime), (char*) buffer);
                index = 0;
                skipEndLine = true;
                if (sendToAwa(buffer, session) != 0) {
                    AwaClientSession_Free(&session);
                    printf("Freed Awa Session\n");
                    session = NULL;
                }
            } else {
                skipEndLine = false;
            }
            tmp = 0;
            len = 0;
        }


    }
    if (session != NULL) {
        AwaClientSession_Free(&session);
    }

    uart_release();
    return 0;
}
