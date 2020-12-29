#Product-backlog

- Apply Filter:
    - select Events based on EventTypes specified by SimpleAttributeOperands in the selectClause
    - filter selected events by condition specified in the whereClause

- generate Result:
    - selectClauseResult
    - whereClauseResul
    
- validate selectClauses when the EventFilter is created or updated. 
    - general errors (true for all selected values) are returned in the selectClauseResults parameter
    - do not return errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
    - do not report errors that might occur depending on the state or the Server or Type of Event

- validate whereClauses when the EventFilter is created or updated. 
    - structural errors in the construction of the filter are returned in the whereClauseResult parameter
    - general errors (true fpr all specified values) are returned in the whereClauseResult parameter
    - do not report errors for unknown EventTypes or BrowsePaths if not all EventTypes are known to the Server yet
    - do not report errors that might occur depending on the state or the Server or Type of Event

-