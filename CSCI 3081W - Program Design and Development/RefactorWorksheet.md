# Refactoring Worksheet
#### Refactoring is defined by Martin Fowler as "a change made to the internal structure of the software to make it easier to understand and cheaper to modify without changing its observable behavior" (Fowler 1999)

***

Read Chapter 24:  Refactoring, Code Complete 2
The checklists are more fully discussed in the book.  Be sure to read carefully the examples.

 # Checklist:  Reasons to Refactor
  - [ ] Code is duplicated
  - [ ] A routine is too long
  - [ ] A loop is too long or too deeply nested
  - [ ] A class has poor cohesion
  - [ ] A class interface does not provide a consistent level of abstraction
  - [ ] A parameter list has too many parameters
  - [ ] Changes require parallel modifications
  - [ ] Changes require parallel modifications in multiple classes
  - [ ] Inheritance hierarchies have to be modified in parallel
  - [ ] Case statements have to be modified in parallel
  - [ ] Related data items that are used together are not organized into classes
  - [ ] A routine uses more features of another class than of its own
  - [ ] A primitive data type is overloaded
  - [ ] A class doesn't do very much
  - [ ] A chain of routines passes tramp data
  - [ ] A middleman object isn't doing anything
  - [ ] One class is overly intimate with another
  - [ ] A routine has a poor name
  - [ ] Data members are public
  - [ ] A subclass uses only a small percentage of its parents' routines
  - [ ] Comments are used to explain difficult code
  - [ ] Global variables are used
  - [ ] A routine uses setup code before a routine call
  - [ ] A routine uses takedown code after a routine call
  - [ ] A program contains code that seems like it might be needed someday

***
# Checklist:  Summary of Refactorings
## Data-Level Refactorings
- [ ] Replace a magic number with a named constant
- [ ] Rename a variable with a clearer or more informative name
- [ ] Move an expression inline
- [ ] Replace an expression with a routine
- [ ] Convert a multiuse variable to multiple single-use variables
- [ ] Convert a data primitive to a class
- [ ] Convert a set of type codes to a class or an enumeration
- [ ] Convert a set of type codes to a class with subclasses
- [ ] Change an array to an object
- [ ] Encapsulate a collection
- [ ] Replace a traditional record with a data class
***
## Statement-Level Refactoring
- [ ] Decompose a boolean expression
- [ ] Consolidate fragments that are duplicated within different parts of a conditional
- [ ] Use _break_ or _return_ instead of a loop control variable
- [ ] Return as soon as you know the answer instead of assigning a return value within nested _if-then-else_ statements
- [ ] Replace conditions (especially repeated _case_ statements) with polymorphism
- [ ] Create an use null objects instead of testing for null values
***
## Routine-Level Refactoring
- [ ] Extract a routine
- [ ] Move a routine's code inline
- [ ] Convert long routine to a class
- [ ] Substitute a simple algorithm for a complex one
- [ ] Add a parameter
- [ ] Remove a parameter
- [ ] Combine similar routines by parameterizing them
- [ ] Separate routines whose behavior depends on parameters passed in
- [ ] Pass a whole object rather than specific fields
- [ ] Pass specific fields rather than a whole object
- [ ] Encapsulate downcasting
***
## Class Implementation Refactorings
- [ ] Change value objects to reference objects
- [ ] Change reference objects to value objects
- [ ] Replace virtual routines with data initialization
- [ ] Change member routine or data placement
- [ ] Extract specialized code into a subclass
- [ ] Combine similar code into a superclass
***
## Class Interface Refactorings
- [ ] Move a routine to another class
- [ ] Convert one class to two
- [ ] Eliminate a class
- [ ] Hide a delegate
- [ ] Remove a middleman
- [ ] Replace inheritance with delegation
- [ ] Replace delegation with inheritance
- [ ] Introduce a foreign routine
- [ ] Introduce an extension class
- [ ] Encapsulate an exposed member variable
- [ ] Hide routines that are not intended to be used outside the class
- [ ] Encapsulate unused routines
- [ ] Remove _Set()_ routines for fields that cannot be changed
- [ ] Collapse a superclass and subclass if their implementations are very similar
***
## System-Level Refactorings
- [ ] Create a definitive reference source for data you can't control
- [ ] Change unidirectional class association to bidirectional class association
- [ ] Change bidirectional class association to unidirectional class association
- [ ] Provide a factory routine rather than a simple constructor
- [ ] Replace error codes with exceptions or vice versa
