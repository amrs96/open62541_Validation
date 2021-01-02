#Product-backlog

- Apply Filter:
    - select Events based on EventTypes specified by SimpleAttributeOperands in the selectClause
    - filter selected events by condition specified in the whereClause

- generate Result:
    - selectClauseResult
      - should return null in the corresponding field, if in the response of the server there were no corresponding fields to the EventFilter
      - should return null in the corresponding field, if an error was returned
      - if a selected field is temporarily unavailable, there should be a status code representing that information
    - whereClauseResult
      - contains only those results that are described by a logical expression
      - the logical expression is formed using the SimpleAttributeOperand

- validate selectClauses when the EventFilter is created or updated. 
    - general errors (true for all selected values) are returned in the selectClauseResults parameter
    - do not return errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
    - do not report errors that might occur depending on the state or the Server or Type of Event

- validate whereClauses when the EventFilter is created or updated. 
    - structural errors in the construction of the filter are returned in the whereClauseResult parameter
    - general errors (true fpr all specified values) are returned in the whereClauseResult parameter
    - do not report errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
    - do not report errors that might occur depending on the state or the Server or Type of Event

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
    
