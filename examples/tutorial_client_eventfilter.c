/**
 * In this C file we will create a ClientFilter example.
 * Client configured on the server
 * */


#include <open62541/client.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/util.h>

#include <signal.h>
#ifdef _MSC_VER
#pragma warning(disable:4996) // warning C4996: 'UA_Client_Subscriptions_addMonitoredEvent': was declared deprecated
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

static UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

#ifdef UA_ENABLE_SUBSCRIPTIONS

static void
handler_events(UA_Client *client, UA_UInt32 subId, void *subContext,
               UA_UInt32 monId, void *monContext,
               size_t nEventFields, UA_Variant *eventFields) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Notification");

    /* The context should point to the monId on the stack */
    UA_assert(*(UA_UInt32*)monContext == monId);

    for(size_t i = 0; i < nEventFields; ++i) {
        if(UA_Variant_hasScalarType(&eventFields[i], &UA_TYPES[UA_TYPES_UINT16])) {
            UA_UInt16 severity = *(UA_UInt16 *)eventFields[i].data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Severity: %u", severity);
        } else if (UA_Variant_hasScalarType(&eventFields[i], &UA_TYPES[UA_TYPES_LOCALIZEDTEXT])) {
            UA_LocalizedText *lt = (UA_LocalizedText *)eventFields[i].data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Message: '%.*s'", (int)lt->text.length, lt->text.data);
        }
        else {
#ifdef UA_ENABLE_TYPEDESCRIPTION
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Don't know how to handle type: '%s'", eventFields[i].type->typeName);
#else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Don't know how to handle type, enable UA_ENABLE_TYPEDESCRIPTION "
                        "for typename");
#endif
        }
    }
}

const size_t nSelectClauses = 2;
const size_t nWhereClauses = 1;



static UA_ContentFilter *setupWhereClausesComplex(void){
    UA_ContentFilter *contentFilter = (UA_ContentFilter*)
        UA_Array_new(nWhereClauses,&UA_TYPES[UA_TYPES_CONTENTFILTER]);

    for(size_t i =0; i<nWhereClauses; ++i) {
        UA_ContentFilter_init(&contentFilter[i]);
    }

    contentFilter[0].elementsSize = 1;

    contentFilter[0].elements  = (UA_ContentFilterElement*)
        UA_Array_new(contentFilter->elementsSize, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT] );


    contentFilter[0].elements[0].filterOperator = UA_FILTEROPERATOR_OR; // set the Operator
    contentFilter[0].elements[0].filterOperandsSize = 2;                // set Operands size

    contentFilter[0].elements[0].filterOperands = (UA_ExtensionObject*)     // Set Operands Array
        UA_Array_new(contentFilter[0].elements[0].filterOperandsSize, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    if (!contentFilter[0].elements[0].filterOperands){
        UA_ContentFilter_clear(contentFilter);
        return NULL;
    }

    for(size_t i =0; i<contentFilter[0].elements[0].filterOperandsSize; ++i) {
        UA_ExtensionObject_init(&contentFilter[0].elements[0].filterOperands[i]);
    }



    // first Operand

    contentFilter[0].elements[0].filterOperands[0].content.decoded.type = &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT];
    contentFilter[0].elements[0].filterOperands[0].encoding = UA_EXTENSIONOBJECT_DECODED;

    UA_ContentFilterElement *pOperand;
    pOperand = UA_ContentFilterElement_new();
    UA_ContentFilterElement_init(pOperand);
    pOperand->filterOperator = UA_FILTEROPERATOR_OFTYPE; // set the Operator
    pOperand->filterOperandsSize = 1;   // set Operands size
    pOperand->filterOperands = (UA_ExtensionObject*)        // Set Operands Array
        UA_Array_new(pOperand->filterOperandsSize, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    if (!pOperand->filterOperands){
        UA_ContentFilter_clear(contentFilter);
        return NULL;
    }

    pOperand->filterOperands[0].content.decoded.type = &UA_TYPES[UA_TYPES_LITERALOPERAND];

    UA_LiteralOperand *pSubOperand;
    pSubOperand = UA_LiteralOperand_new();
    UA_LiteralOperand_init(pSubOperand);
    pSubOperand->value.type = &UA_TYPES[UA_TYPES_NODEID];
    pSubOperand->value.storageType = UA_VARIANT_DATA;


    UA_NodeId *baseEventTypeId;
    baseEventTypeId = UA_NodeId_new();
    UA_NodeId_init(baseEventTypeId);
    *baseEventTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_PROGRESSEVENTTYPE);      // filtern nach PROGRESSEVENTTYPE

    pSubOperand->value.data = baseEventTypeId;

    pOperand->filterOperands[0].content.decoded.data = pSubOperand;
    pOperand->filterOperands[0].encoding = UA_EXTENSIONOBJECT_DECODED;

    contentFilter[0].elements[0].filterOperands[0].content.decoded.data = pOperand;




    // second Operand


    contentFilter[0].elements[0].filterOperands[1].content.decoded.type = &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT];
    contentFilter[0].elements[0].filterOperands[1].encoding = UA_EXTENSIONOBJECT_DECODED;

    UA_ContentFilterElement *pSecondOperand;
    pSecondOperand = UA_ContentFilterElement_new();
    UA_ContentFilterElement_init(pSecondOperand);
    pSecondOperand->filterOperator = UA_FILTEROPERATOR_OFTYPE; // set the Operator
    pSecondOperand->filterOperandsSize = 1;   // Operands size
    pSecondOperand->filterOperands = (UA_ExtensionObject*)
        UA_Array_new(pSecondOperand->filterOperandsSize, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    if (!pSecondOperand->filterOperands){
        UA_ContentFilter_clear(contentFilter);
        return NULL;
    }

    pSecondOperand->filterOperands[0].content.decoded.type = &UA_TYPES[UA_TYPES_LITERALOPERAND];

    UA_LiteralOperand *pSecondSubOperand;
    pSecondSubOperand = UA_LiteralOperand_new();
    UA_LiteralOperand_init(pSecondSubOperand);
    pSecondSubOperand->value.type = &UA_TYPES[UA_TYPES_NODEID];
    pSecondSubOperand->value.storageType = UA_VARIANT_DATA;


    UA_NodeId *SecondBaseEventTypeId;
    SecondBaseEventTypeId = UA_NodeId_new();
    UA_NodeId_init(SecondBaseEventTypeId);
    *SecondBaseEventTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_AUDITSECURITYEVENTTYPE);      // filtern nach AUDITSECURITYEVENTTYPE

    pSecondSubOperand->value.data = SecondBaseEventTypeId;

    pSecondOperand->filterOperands[0].content.decoded.data = pSecondSubOperand;
    pSecondOperand->filterOperands[0].encoding = UA_EXTENSIONOBJECT_DECODED;

    contentFilter[0].elements[0].filterOperands[1].content.decoded.data = pSecondOperand;




    return contentFilter;
}





static UA_ContentFilter *setupWhereClauses(void){
    UA_ContentFilter *contentFilter = (UA_ContentFilter*)
        UA_Array_new(nWhereClauses,&UA_TYPES[UA_TYPES_CONTENTFILTER]);

    for(size_t i =0; i<nWhereClauses; ++i) {
        UA_ContentFilter_init(&contentFilter[i]);
    }

    contentFilter[0].elementsSize = 1;

    contentFilter[0].elements  = (UA_ContentFilterElement*)
        UA_Array_new(contentFilter->elementsSize, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT] );

    contentFilter[0].elements[0].filterOperator = UA_FILTEROPERATOR_OFTYPE;
    contentFilter[0].elements[0].filterOperandsSize = 1;
    contentFilter[0].elements[0].filterOperands = (UA_ExtensionObject*)
        UA_Array_new(contentFilter[0].elements[0].filterOperandsSize, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    if (!contentFilter[0].elements[0].filterOperands){
        UA_ContentFilter_clear(contentFilter);
        return NULL;
    }


    contentFilter[0].elements[0].filterOperands[0].content.decoded.type = &UA_TYPES[UA_TYPES_LITERALOPERAND];

    UA_LiteralOperand *pOperand;
    pOperand = UA_LiteralOperand_new();
    UA_LiteralOperand_init(pOperand);
    pOperand->value.type = &UA_TYPES[UA_TYPES_NODEID];
    pOperand->value.storageType = UA_VARIANT_DATA;


    UA_NodeId *baseEventTypeId;
    baseEventTypeId = UA_NodeId_new();
    UA_NodeId_init(baseEventTypeId);
    //*baseEventTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE);  // filtern nach BaseEventType
    *baseEventTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_AUDITEVENTTYPE);      // filtern nach AuditEventType


    pOperand->value.data = baseEventTypeId;



    contentFilter[0].elements[0].filterOperands[0].content.decoded.data = pOperand;
    contentFilter[0].elements[0].filterOperands[0].encoding = UA_EXTENSIONOBJECT_DECODED;

    return contentFilter;
}


static UA_SimpleAttributeOperand *
setupSelectClauses(void) {
    UA_SimpleAttributeOperand *selectClauses = (UA_SimpleAttributeOperand*)
        UA_Array_new(nSelectClauses, &UA_TYPES[UA_TYPES_SIMPLEATTRIBUTEOPERAND]);
    if(!selectClauses)
        return NULL;

    for(size_t i =0; i<nSelectClauses; ++i) {
        UA_SimpleAttributeOperand_init(&selectClauses[i]);
    }

    selectClauses[0].typeDefinitionId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE);
    selectClauses[0].browsePathSize = 1;
    selectClauses[0].browsePath = (UA_QualifiedName*)
        UA_Array_new(selectClauses[0].browsePathSize, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
    if(!selectClauses[0].browsePath) {
        UA_SimpleAttributeOperand_delete(selectClauses);
        return NULL;
    }
    selectClauses[0].attributeId = UA_ATTRIBUTEID_VALUE;
    selectClauses[0].browsePath[0] = UA_QUALIFIEDNAME_ALLOC(0, "Message");

    selectClauses[1].typeDefinitionId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE);
    selectClauses[1].browsePathSize = 1;
    selectClauses[1].browsePath = (UA_QualifiedName*)
        UA_Array_new(selectClauses[1].browsePathSize, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
    if(!selectClauses[1].browsePath) {
        UA_SimpleAttributeOperand_delete(selectClauses);
        return NULL;
    }
    selectClauses[1].attributeId = UA_ATTRIBUTEID_VALUE;
    selectClauses[1].browsePath[0] = UA_QUALIFIEDNAME_ALLOC(0, "Severity");

    return selectClauses;
}

#endif

int main(int argc, char *argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    if(argc < 1) {  // geändert, damit auf Cilon direkt mit dem Server verbinden zu können
        printf("Usage: tutorial_client_events <opc.tcp://server-url>\n");
        return EXIT_FAILURE;
    }

    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    /* opc.tcp://uademo.prosysopc.com:53530/OPCUA/SimulationServer */
    /* opc.tcp://opcua.demo-this.com:51210/UA/SampleServer */
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://ammar:4840/"); // geändert, damit auf Cilon direkt mit dem Server verbinden zu können
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return EXIT_FAILURE;
    }

#ifdef UA_ENABLE_SUBSCRIPTIONS
    /* Create a subscription */
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
                                                                            NULL, NULL, NULL);
    if(response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_Client_disconnect(client);
        UA_Client_delete(client);
        return EXIT_FAILURE;
    }
    UA_UInt32 subId = response.subscriptionId;
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create subscription succeeded, id %u", subId);

    /* Add a MonitoredItem */
    UA_MonitoredItemCreateRequest item;
    UA_MonitoredItemCreateRequest_init(&item);
    item.itemToMonitor.nodeId = UA_NODEID_NUMERIC(0, 2253); // Root->Objects->Server
    item.itemToMonitor.attributeId = UA_ATTRIBUTEID_EVENTNOTIFIER;
    item.monitoringMode = UA_MONITORINGMODE_REPORTING;

    UA_EventFilter filter;
    UA_EventFilter_init(&filter);
    filter.selectClauses = setupSelectClauses();
    filter.selectClausesSize = nSelectClauses;
    setupWhereClauses();
    filter.whereClause = *setupWhereClausesComplex();



    item.requestedParameters.filter.encoding = UA_EXTENSIONOBJECT_DECODED;
    item.requestedParameters.filter.content.decoded.data = &filter;
    item.requestedParameters.filter.content.decoded.type = &UA_TYPES[UA_TYPES_EVENTFILTER];

    UA_UInt32 monId = 0;

    UA_MonitoredItemCreateResult result =
        UA_Client_MonitoredItems_createEvent(client, subId,
                                             UA_TIMESTAMPSTORETURN_BOTH, item,
                                             &monId, handler_events, NULL);

    if(result.statusCode != UA_STATUSCODE_GOOD) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Could not add the MonitoredItem with %s", UA_StatusCode_name(retval));
        goto cleanup;
    } else {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Monitoring 'Root->Objects->Server', id %u", response.subscriptionId);
    }

    monId = result.monitoredItemId;

    while(running)
        retval = UA_Client_run_iterate(client, 100);

    /* Delete the subscription */
    cleanup:
    UA_MonitoredItemCreateResult_clear(&result);
    UA_Client_Subscriptions_deleteSingle(client, response.subscriptionId);
    UA_Array_delete(filter.selectClauses, nSelectClauses, &UA_TYPES[UA_TYPES_SIMPLEATTRIBUTEOPERAND]);
#endif

    UA_Client_disconnect(client);
    UA_Client_delete(client);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
