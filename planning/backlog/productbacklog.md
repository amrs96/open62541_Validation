#Product-backlog

1. validate filter:
    - validate selectClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
        - general errors (true for all selected values) are returned in the selectClauseResults parameter
        - do not return errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
        - do not report errors that might occur depending on the state or the Server or Type of Event
    - validate whereClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
        - structural errors in the construction of the filter are returned in the whereClauseResult parameter (See permitted operands)
        - general errors (true fpr all specified values) are returned in the whereClauseResult parameter (See permitted operands)
        - do not report errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
        - do not report errors that might occur depending on the state or the Server or Type of Event

2. Apply Filter:
    - select Events based on EventTypes specified by SimpleAttributeOperands in the selectClause
    - filter selected events by condition specified in the whereClause (See permitted operands)
    - Server is required to expose all its EventTypes, so a client can usefully subscribe to Events.

3. generate Result:
    - selectClauseResult
      - should return null in the corresponding field, if in the response of the server there were no corresponding fields to the EventFilter
      - should return null in the corresponding field, if an error was returned
      - if a selected field is temporarily unavailable, there should be a status code representing that information
      - if this status code is bad (see table 144) or unknown, the status code itself should be returned instead of the value attribute
      - If the BaseEventType is specified in the SimpleAttributeOperand by the Client, the Server should evaluate the BrowsePath without considering the Type.
    - whereClauseResult
      - contains only those results that are described by a logical expression
      - the logical expression is formed using the SimpleAttributeOperand
      - a null value is used, if an error occures in the whereClause for a certain event
      - a null value is used, if the status code is unknown or bad


- selectClauseDiagnosticInfo 
    - A list of diagnostic information for individual elements in the select clause
    - his list is empty if diagnostics
      information was not requested in the request header or if no diagnostic
      information was encountered in processing of the select clauses
    - The size and order of the list matches the size and order of the elements
      in the selectClauses request parameter

- EventQueueOverflowEventType Events
    - to control information to the client
    - only get published to MonitoredItems that produced the event
    - bypass the whereClause
    
- Permitted operands:
    - Equals_0
    - IsNull_1
    - GreaterThan_2
    - LessThan_3
    - GreaterThanOrEqual_4
    - LessThanOrEqual_5
    - Like_6 (First operand matches defined by pattern in second operand (See Table 121 for definition of patterns))
    - Not_7
    - Between_8 (First operand between second and third)
    - InList_9 (2 to n operands (first in some of the others))
    - And_10
    - Or_11
    - Cast_12 (Converts first operand to type defined by second operand)
    - OfType_14
    - BitwiseAnd_16
    - BitwiseOr_17

- EventFilterResult structure
    - selectClauseResult (list of status codes)
    - selectClauseDiagnosticInfo
    - whereClauseResult
    
- EventFilterResult Result Codes
    - Bad_TypeDefinitionInvalid: The type definition node id does not reference an appropriate type node
    - Bad_NodeIdUnknown: the node id refers to a node that does not exist in the Server address space.
    - Bad_BrowseNameInvalid: The browse name is invalid
    - Bad_AttributeIdInvalid: The attribute is not supported for the specified node
    - Bad_IndexRangeInvalid:
    - Bad_TypeMismatch
- 