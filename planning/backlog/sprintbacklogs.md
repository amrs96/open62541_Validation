#Sprint-backlogs

## Sprint 1: 01.01 - 07.01 
### Define Framework
- Planning
- Define Backlog Items
- Research 

## Sprint 2: 08.01 - 14.01
### Start Test implementation
- Start implementing Tests:
     - *Equals_0*
     - *GreaterThan_2*
     - *LessThan_3*
     - *GreaterThanOrEqual_4*
     - *LessThanOrEqual_5*
     - *IsNull_1*
     - *And_10*
     - *Or_11*## Sprint 3: 15.01 - 21.01 
## Sprint 3: 15.01 - 21.01
### Planning and Examples
- Creation of tutorial:
    - Server (can create multiple events)
    - Client (with Where-filter (filteroperator OfType)
- UML diagrams:
    - Function calls of UA_filterEvent
    - EventTypes (subtypes of BaseEventType)
- Gantt diagram
## Sprint 4: 22.01 - 28.01
### Start Development
- validate selectClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
     - **return general errors** (true for all selected values) in the selectClauseResults parameter
     - **do not return** errors for unknown EventTypes or BrowsePaths, **if** not all EventTypes are known to the Server yet
     - **do not return** errors, **if** they might occur depending on the state or the Server or Type of Event
- validate whereClauses when the EventFilter is created or updated. (See EventFilterResult result codes)
     - **return structural errors** (errors in the construction of the filter) in the whereClauseResult parameter (See permitted operands)
     - **return general errors** (true for all specified values) in the whereClauseResult parameter (See permitted operands)
     - **do not return** errors for unknown EventTypes or BrowsePaths, **if** not all EventTypes are known to the Server yet      - **do not return** errors, **if** they might occur depending on the state or the Server or Type of Event
     - *the logical expression is formed using the SimpleAttributeOperand*
- Presentation of planned approach 
- Add complex filter to tutorial
## Woche 5-7: 01.02 - 22.02
### **Klausurphase 1**

## Woche 8: 22.02 - 01.03 
### Überschrift 5
  
## Woche 7: 01.03 - 08.03 
### Überschrift 6

## Woche 8: 08.03 - 15.03
### Überschrift 7

  
## Woche 9, 10: 15.03 - 29.03 
### **Klausurphase 2**
  
## Woche 11: 29.03 - 31.03
### ENDE