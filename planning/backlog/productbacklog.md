##Product-backlog

1. validate filter:
    - validate selectClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
        - **return general errors** (true for all selected values)  in the selectClauseResults parameter
        - **do not return** errors for unknown EventTypes or BrowsePaths, **if** not all EventTypes are known to the Server yet
        - **do not return** errors, **if** they might occur depending on the state or the Server or Type of Event
    - validate whereClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
        - **return structural errors** (errors in the construction of the filter) in the whereClauseResult parameter (See permitted operands)
        - **return general errors** (true for all specified values) in the whereClauseResult parameter (See permitted operands)
        - **do not return** errors for unknown EventTypes or BrowsePaths, **if** not all EventTypes are known to the Server yet
        - **do not return** errors, **if** they might occur depending on the state or the Server or Type of Event
        - *the logical expression is formed using the SimpleAttributeOperand*

2. Apply Filter:
    - select Events based on EventTypes specified by SimpleAttributeOperands in the selectClause
    - filter selected events by condition specified in the whereClause (See permitted operands)
    - *Server is required to expose all its EventTypes, so a client can subscribe to Events*

3. generate Result: (See EventFilterResult result codes)
    - selectClauseResult 
      - **return null** in the corresponding field, **if** in the response of the server there were no corresponding fields to the EventFilter
      - **return null** in the corresponding field, **if** an error was returned
      - **return a status code** representing the reason, **if** a selected field is temporarily unavailable
      - **return the status code** itself instead of  value attribute, **if** this status code is bad (see table 144) or unknown
      - Server should evaluate the BrowsePath without considering the Type, **if** the BaseEventType is specified in the SimpleAttributeOperand by the Client
      - *Size and order match size and order of elements in the request*
    - selectClauseDiagnosticInfo 
        - **return diagnostic information** for the individual elements in the select clause
        - **return empty list**, **if** diagnostic information was not requested in the request header 
        - **return empty list**, **if** there was no diagnostic information in processing of the select clauses
        - *Size and order match size and order of elements in the request
    - whereClauseResult
      - **return results** that are described by the logical expression
      - **return null**, if an error occures in the whereClause for a certain event
      - **return null**, if the status code is unknown or bad
#

-EventQueueOverflowEventType:
    - If Events are lost, an Event of the type
      EventQueueOverflowEventType is placed in the queue.
    - This Event is generated when the first Event
      has to be discarded on a MonitoredItem subscribing for Events
    - It is put into the Queue of the
      MonitoredItem in addition to the size of the Queue defined for this MonitoredItem without discarding
      any other Event
    - If discardOldest is set to TRUE it is put at the beginning of the queue and is never
      discarded, otherwise at the end. An aggregating Server shall not pass on such an Event
    - It shall be handled like other connection error scenarios
    
- EventQueueOverflowEventType Events
    - to control information to the client
    - only get published to MonitoredItems that produced the event
    - bypass the whereClause

##Information about Operations in WhereClause

- Operands: 
    - are SimpleAttributeOperands
    - are restricted to TypeDefinitionNode or subtypes by typeId (NodeId of a TypeDefinitionNode)
    - contain a browsePath which is a relative path to a Node (Nodes followed by the browsePath are NodeClass Objects or Variables)
        - If this list is empty the Node is the instance of the TypeDefinition
    - contain an attributeId (Id of the Attribute which is supported by all servers)
    - contain an indexRange parameter which identifies elements of the array
        - Is null (All values of an array are used) if not specified 
        - Is ignored if Node is not a Variable or the Value of the Variable is not an array

- Permitted Operations:
    - *Equals_0*
    - *IsNull_1*
    - *GreaterThan_2*
    - *LessThan_3*
    - *GreaterThanOrEqual_4*
    - *LessThanOrEqual_5*
    - *Like_6* (First operand matches defined by pattern in second operand (See Table 121 for definition of patterns))
    - *Not_7*
    - *Between_8* (First operand between second and third)
    - *InList_9* (2 to n operands (first in some of the others))
    - *And_10*
    - *Or_11*
    - *Cast_12* (Converts first operand to type defined by second operand)
    - *OfType_14*
    - *BitwiseAnd_16*
    - *BitwiseOr_17*

##Information about EventFilterResult

- EventFilterResult structure:
    - selectClauseResult (list of status codes)
    - selectClauseDiagnosticInfo
    - whereClauseResult

- EventFilterResult Result Codes:
    - *Bad_TypeDefinitionInvalid*: The type definition node id does not reference an appropriate type node
    - *Bad_NodeIdUnknown*: the node id refers to a node that does not exist in the Server address space.
    - *Bad_BrowseNameInvalid*: The browse name is invalid
    - *Bad_AttributeIdInvalid*: The attribute is not supported for the specified node
    - *Bad_IndexRangeInvalid*: The syntax of the index range parameter is invalid
    - *Bad_TypeMismatch*: The value supplied for the attribute is not of the same type as the attribute's value  