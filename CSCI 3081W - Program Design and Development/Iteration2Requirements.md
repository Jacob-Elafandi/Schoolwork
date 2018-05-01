**NOTES ON ITERATION 2**
- (11/13)  Many edits, clarifications and additions in response to student questions from our class and the other section.  All are marked **EDIT** or **CLARIFICATION** or **ADDITION**.  Of note,
  - SUGGESTED design has been added to areas in which you can make design decisions.  Notw why you are making those design decisions, as you will have to defend your choice in your design document and stating that the professor or TA suggestion this design is not a sufficient reason.
  - Every method of a non-abstract class should have tests that cover a minimum 2 typical inputs and 1 edge case.
  - You may hard code the number of Robots in the game and the initial position - have at least 5 robots in thea arena.
  - Keep an active bug list.  Don't obsess about bugs when you have lots to still implement.
- (11/13)  The requirements state to create a SuperBot class.  You may instead implement a SuperBot within the class Robot.  However, please do not use a flag in the Robot class that "turns on" SuperBot and uses that flag to implment one of two different pieces of code.  If you want to vary behavior, consider the Strategy pattern or the Template pattern.  (For a review read the Head First Design Patterns chapters.)
- (11/09) Develop in your _devel_ branch - do not merge with Master branch until time of submission. We will use tags for the preliminary submission (see the Tagging section in the git usage document: https://github.umn.edu/umn-csci-3081F17/class-repo-public/blob/master/project/git-usage-f17.pdf). And don't forget to use issues and branching to develop your code.
- (11/09) The preliminary submission requirements are 1) refactoring as defined in the lab, 2) refactoring the initialization process, and 3) writing unit tests for the sensors.
- (11/09) The preliminary submission requirements for writing unit tests will require you to create the header file for each of the sensors. Tests will fail, but that is the nature of Test-Driven Development - write your tests first to better understand the requirements and needed interfaces, then fill in the functionality that will result in the passing of the tests. You might completely change the interface prior to submitting iteration 2 - that's okay.
- (11/09) There is a lot of functionality to implement for this iteration. We highly recommend that as you come across bugs in your code, spend a little time trying to track down the problem, but do not obsess about it. Create a bug report in your github issues, and return to it **_after_** you have completed the other requirements. The only exception to this would be if the bug prevents compilation - then you either comment it out or spend more time fixing it.


<hr>
>Your software is a reflection of your understanding of the requirements as specified in this document. If you do not understand any portion of the requirements or you think that the requirements are underspecified, it is your responsibility to get clarification from the instructor or a TA. Please read this document carefully and review it prior to turning in your iteration for assessment.

# Robot Simulator: Iteration 2

<span style="color:red">

| Date | Item | Description |
|:----:|:-----|:------------|
| Wed, Nov 15, 11:55pm | Preliminary code tests   | Unit Tests and Refactoring |
| Wed, Nov 22, 11:55pm | Design Document for Interfaces | Provide design documentation for Priority 1 Tasks Only |
| Wed, Nov 29, 11:55pm | Documentation and Code Implementation | As described below |

</span>

Your project should demonstrate thoughtful software development with good design quality, using good process. This
iteration will help you to establish those good habits. You will create design documents including a UML diagram. _doxygen_ will be used to automatically
generate code documentation. Code style will comply to the Google Style Guide. Intermediate deadlines will keep you on track in this iterative process.

#### Deliverables and Submission Process

Everything will be submitted via Github. We will pull your repository at the specified due dates and grade according to the contents at that time.

> Please verify that your submission compiles on a cselabs machine and it is complete. There are many components to be submitted - a checklist has been provided.

The schedule is very tight, and it is important that you keep up with the project. The project will continue to build
throughout the semester, and if you get behind, it will be very difficult to catch up. For this reason, **late
assignments will not be accepted**, but you will receive partial credit for partial completion of the requirements.

You can convert this requirements document to pdf using this tool: http://www.markdowntopdf.com/

<hr>

## Documentation and Code Implementation Requirements

### Overview

The goal of this software project is to develop a rudimentary robot simulator in which robot behavior is visualized
within a graphics window. The exact use of the application is yet to be determined. It might be that it is used more
like a video game in which users control the robots. Alternatively, it might be a test environment for experimenting
with autonomous robot control using sensor feedback, or it might be a visualization of real robots operating in a remote
location. Due to the ambiguity of future use, it is important that you plan for change through good design.

In this iteration, you will continue the software development of iteration 1, write unit tests with Google Test, use the Google style guide, and maintain your UML and doxygen
documentation. Feature enhancements for this iteration include the addition of various types of sensors, and autonomous robots (in addition to the user controlled robot).

In this iteration, the robot simulator is still like a video game, but moving towards autonomous, intelligent robot behavior. Users control a player with the arrow keys. The
objective of the game is for the player to freeze all robots at the same time before running out of energy. Energy is depleted constantly and is depleted even more when moving
or when it bumps into obstacles, but it can renew its energy by going to the charging station. Autonomous robots that use sensors to avoid objects will move around the environment interfering with play. If the user-controlled robot collides with an autonomous robot, it will freeze (i.e. stop moving), but if another autonomous robot collides with a _frozen_ robot, it will move again. A frozen robot will emit a distress call, which when detected by another autonomous robot, allows the moving robot to ignore its proximity sensor and bump into the frozen robot. Furthermore, SuperBots roam the arena and if they collide with the Player, the Player freezes for a fixed period of time. SuperBots become SuperBots by an orndinary Robot colliding with home base.

### Functional Requirements

It is  better to continue to develop your version, because your version is familiar to you, and you likely spent time documenting the code. If your code is truly dysfunctional, contact Dr. Watters and you will be supplied with a working version, of which you can use as is or take sections of. Functional requirements are detailed below.

#### Robot Arena

The robot arena has a user-controlled player, autonomous robots, obstacles, home base, and a charging station. The intent is for the player
to move around the space searching for robots to freeze. As it moves, its battery is depleted.
The player can refill its battery by touching the recharge station.

The primary addition to the environment is autonomous robots that have 2 proximity sensors, a distress call receiver (i.e a type of sensor), an entity type sensor, and a touch sensor. _Notes on sensors are at the bottom of the requirements._

#### Graphics Environment

The graphics environment consists primarily of a single window with robots, obstacles, home base, and charging
station. All objects in the environment will be drawn as circles, which greatly simplifies collision detection. You are
welcome to add graphics enhancements, such as color, text, or decorations provided it does not interfere with required
functionality.


#### GUI and User Input

The user should have the following control:

- With UI buttons: Restart and Pause
- ~~With UI buttons: Initialize game settings~~  Do not need to add a UI button to Initialize since we have Restart
- With left and right arrow keys, change the direction of the robot
- With up and down arrow keys, change the speed of the robot (no reverse)

<hr>

**FUNCTIONAL REQUIREMENTS you need to fulfill**

> This is just functionality. Along the way, please document the code using
Doxygen syntax and test for Google Style compliance using cpplint.

The iterative method identifies and prioritizes feature enhancements and code refactoring
over short intervals called sprints. Below is a prioritized list of feature
enhancements, fixes, and refactoring of the code. **Complete all functionality in
one priority level before moving on to the next.** In this way, if we run out
of time, the low priority items will be dropped from the requirements.

>CREATE DEVEL BRANCH and REFACTOR as described in lab08 before you do anything!

- **Refactoring**
  - As outlined in lab and in class, please refactor your code to standardize interfaces.
  - Refactor the initialization process.  You can use a file or get input from a user.  The refactoring should ensure that our initialization parameters are not hard coded in main.cc  You can decide what the file looks like if you decide to use a file to bring in initialization parameters.  

- **Priority Level 1 - Sensor, Event, and Robot Interfaces**

> **ADDITION**: At the bottom of the document are descriptions and resources of
robot sensors to help you better understand the application.

  - **Write unit tests for each Sensor class as discussed below. These tests and the refactoring will be your preliminary submission.** Unit tests do not have to be Google style compliant. These should be in the _student\_tests\_it2.cc_ file in the _tests_ folder.  (**EDIT**:  You may separate the tests of each class into a separate file or put them all in the one file _student\_tests\_it2.cc_.  All tests should be in the tests folder.  Eventually every method should have, as a minimum, tests for 2 typical values and 1 edge case.  You can remove the test files from iteration 1 - many are no longer relevant to the new code.)  **ADDED**:  For the first checkin, you need to have 5 unit tests per sensor (e.g. test constructor, destructor, other functionality).) (**ADDED RESOURCE**:  It might be beneficial to use Google Test Fixtures, which allows for a special Set-Up function to initialize elements to be used in a collect of tests -- see https://github.com/google/googletest/blob/master/googletest/docs/Primer.md.)
  - Define an enumerated type entity_type for representing the type of entities. Use kRobot, kSuperBot, kPlayer, kHomeBase, kRechargeStation, kObstacle, kWall.
  - (**EDIT** SUGGESTED design)  Define the interface for the abstract class Sensor modeled after the observer pattern (with a little Visitor mixed in), as well as its member variables. Each sensor should Accept an event (any type of event) from the Arena directly, not from the entity to which it belongs. The sensor determines if the event activates the sensor. It will likely be necessary for the sensor to hold a link to the arena entity to which it belongs.
  - Define the interface and member variables of the SensorProximity class, derived from the Sensor base class. A proximity sensor should have a range and _field of view_, such that it has a limited cone in which it can sense objects. A single cone emanating from the center of the robot, split in two, signifies the two fields of view for the two sensors. The range and field of view, expressed as an angle, are attributes of the sensor. Sensor output is the distance to an obstacle. If there is no obstacle, it should output -1. (_See the file proximity.py in class repo in Exercises folder._)
  - Define the interface and member variables of the SensorDistress class, derived from the Sensor base class. The distress signal can be sensed when it is within a defined range, but the direction of the signal cannot be determined. Sensor output is 1 for a sensed call and 0 for none.
  - Define the interface and member variables of the SensorEntityType class. The type of the entity emitting the signal can be sensed when it is within a defined range, but the direction of the signal cannot be determined. Sensor output is the enumerated entity type. (**CLARIFICATION**:  Think of this as a physical system.  Although type information is readily available within the Arena, the other sensors would not have access to this information.)
  - Revise the interface and member variables as appropriate for the SensorTouch. Sensor output is 1 for a sensed collision and 0 for none (this is a change from boolean so that all sensor readings are an integer value).
  - Define the interface and member variables of the following events (as appropriate): EventCollision, EventProximity, EventDistressCall, EventTypeEmit. (**EDIT**:  SUGGESTED design) Events should provide information regarding the entities involved and the location of the event.
  - Define an abstract MotionHandler class that is the base class for the various types of entities in the environment.
  - Define the interface and member variables as appropriate for derived motion handler classes for the various entities. Name these to match other nomenclature using base class then the derived class name, for example MotionHandlerRobot.
  - Rename the Robot class the Player class.
  - Define the interface and member variables for a Robot class that has 2 proximity sensors (**CLARIFICATION**:  The robot can be composed of a single sensor that has a left and right component, rather than 2 sensors), a distress call sensor, an entity type sensor, and a touch sensor. (_The term "robot" implies that it is autonomous, thus the user-controlled entity is not a robot._)
  - Define the interface and member variables for a SuperBot class that has 2 proximity sensors, a distress call sensor, an entity type sensor, and a touch sensor. SuperBots can unfreeze all Robots and freeze the Player. Create a visual distinction between Robot and SuperBot - color would suffice. (**EDIT**:  Alternatively, you can implement this inside of the Robot class - See Notes at the top of the file.)

(**EDIT**:  KEEP A LIST OF YOUR BUGS using either GitHub or Doxygen, as this is part of the requirements.  Do not spend too much time tracking down bugs - record them and move on.  When everything is implement, then return to your bug report and fix them.  You will be docked points if there are too many bugs in your code, but you will be docked more points for not fully implmeneting the functionality.  Remember that the goal of the class is to understand and be practiced in software development, not to implement a perfect robot simulator.  Yes, you can get full points even with some bugs.)

- **Priority Level 2 - Sensor Functionality**

  - (**EDIT**:  SUGGESTED Design)  Define an interface for Arena methods modeled after the observer pattern. The Arena should have a structure for registered sensors and the methods Arena::RegisterSensor(Sensor * s) and Arena::RemoveSensor(Sensor * s) for adding and deleting to the structure. For each update loop, the Arena checks for various events in the environment and sends a notice  of that event to every registered sensor.
  - (**EDIT**:  SUGGESTED Design)  Implement functionality in Arena that checks for the various events and sends those events to all registered sensors. Conceptually, this is similar to the current CheckEntityCollision method. **NOTE**: Calculating the distance to an object within range of the proximity sensor has been provided in Python. In the class repo, go to Exercises and look at the file _proximity.py_.
 

- **Priority Level 3 - Entity Functionality**
  - Implement functionality for the Accept method for each event of each sensor (**EDIT** moved from Priority 2)
  - Implement functionality for Player incorporating the new structure of sensors and events. The Player has a touch sensor. It's motion is controlled by the user. If Player collides with ...
    - Robot: the Player should bounce - the robot will freeze.
    - SuperBot: the Player should freeze for a fixed amount of time before allowed to carry on, ignoring user input while frozen.
    - Recharge Station: recharge and bounce.
    - Wall: bounce.
    - Home Base: bounce (not a win).
  - Implement functionality for Robot. (**EDIT**:  You may hard code the number of robots - minimum of 5 and their intitial position.)  The Robot moves at a fixed speed at a given heading, except under the following situations:
    - If it gets close to anything other than the Player and it is not sensing a distress call, then it should avoid it by adjusting its heading and slowing as some function to the distance to the obstacle, as indicated by the proximity sensor. If nothing is sensed by the proximity sensor, resume to default speed.
    - If it is sensing a distress call, then it's heading should stay fixed.
    - If it collide with the Player, then it should "freeze," meaning its speed goes to 0 until another Robot or SuperBot runs into it.
    - If it collides with a Robot and that robot is "frozen," that should unfreeze the robot.
    - If the Robot collides with home base, it becomes a SuperBot! - think about the design of this carefully - lot's of options.
    - It should bounce after colliding with an entity.
  - Implement functionality for the SuperBot. It is identical to a Robot with a few exceptions:
    - If it detects the Player, it should not avoid collision.
    - If it collides with the Player, it bounces and the Player freezes.

  - Implement functionality for the home base and recharge station. The home base should have a touch sensor so that it bounces off of other obstacles. The recharge station does not move.
  - Implement win / loss. If the Player freezes all regular robots, Player wins. If all robots turn in to SuperBots, Player loses.

<hr>

### Unit Testing with Google Test

https://github.com/google/googletest

Unit tests are essential in a large-scale project, because the entire code base can be tested regularly, automatically,
as it undergoes development. In the strictest application of Test-Driven Development (TDD), the tests are written
_before_ the code, which helps solidify requirements and write testable code.

Part of the requirements of this iteration are to write the tests for the Sensor classes. These tests should be in the students_tests_it2.cc file in the tests folder.

### File Structure

You must follow the file structure provided. It follows the conventions of code development, with some specific
requirements added as part of the Google style guide. Not being compliant with the structure will likely break
compilation and testing. Even if it doesn't, you will lose points for not maintaining the provided file structure.

### Google Style Guide Compliance

https://google.github.io/styleguide/cppguide.html

Consistency in code organization, naming conventions, file structure, and formatting makes code easier to read and
integrate. There are many options with various merits, so it is up to the development team to establish these
conventions, or in the words of Kevin Wendt "The only style guide that really matters is the one your boss uses." We
decided to use the Google style guide because it is published, documented, and has automated tests. We don't agree with
every decision, but we are complying so that we are all coding to a single standard.

<hr>

## Documentation

Code documentation comes in many forms for many audiences. For this project, your audience is other programmers, who
need to understand class interfaces, function parameters, function behavior, code organization, class organization, and
code logic. Self-documenting code (i.e. using good organization and naming conventions) is an efficient means of
communicating, since you have to write the code anyway. _Good_ comments - not too much, not too little - help guide
others through the logic and class interfaces. For the larger picture of code and class structure, use UML diagrams and
Doxygen-generated (automatic) web pages.

Doxygen automatically generates documentation of class and code structure when you follow some simple conventions for
commenting within your code (_see_ http://www.stack.nl/~dimitri/doxygen/).

All of these elements of documentation will be assessed, accounting for half of your grade for each iteration. Think
about these two extremes: 1) your code functions perfectly and you have almost no documentation - you will likely fail
on this iteration; 2) your code is not functional and will only compile when you comment most of it out, but you have
complete, great documentation - you will likely get a decent grade on this iteration. Get in the habit of creating the
documentation along with the code.

<hr>

## Assessment

Your software **must compile on a cselabs machine or it will not be graded**. We will not grade your project unless it
compiles. As long as you provide a version that compiles, we will evaluate those aspects that are partially functional
for partial credit. In other words, <u>comment it out if it breaks compilation</u>.

This is the breakdown for point distribution:

20% : Iteration \#2
- 05% : UML
- 15% : Refactoring Initialization and Unit Tests for Sensor
- 10% : Design Document Checkin on Wednesday, November 22
- 40% : Final Documentation
- 30% : Final Code

### Documentation Assessment ( 50% )

#### Design Document Checkin (10%)
For your checkin, we would like a 2-3 page writeup of your full design.  All interfaces should be designed and you should have a clear vision of how your code should work.  We need you to describe your design and you can provide diagrams to us at this time also (e.g.  UML, class diagrams).  This checkin is to ensure that you have done your design first before you actually start writing your code.  

#### Complete Iteration Documentation ( 40% )

#### Design Document and UML

In this iteration, a design document will be required, which discusses key design choices. In this document, you will outline a substantial design decision, describe the process of choosing the design, and justify your final choice. Design can be driven by personal preference, and very different designs might be equally justified. Therefore, the quality of your response greatly depends on the justification, not the _right_ or _wrong-ness_ of your design.

Documentation includes an overview of the design provided in mainpage.h in /src and the final UML diagram saved in /docs. The overview should be written in prose and be about 1 page of text (if it were on an 8.5x11 paper). It should highlight important design elements, such as the separation
of graphics from arena, and the composition of the robot. We will
look for a well written and well organized document that clearly articulates the design. It must stand alone in the
sense that anyone reading it should not have to look at code or other documents to understand the design.

#### Google Style Compliance

We will run the automated test using 'cpplint' to check compliance. There are a few additional elements for compliance
that _cpplint_ does not check for, thus TAs will inspect for those.

#### Doxygen Documentation

We will compile the code to generate _Doxygen_ web pages. All classes and major functions must be documented as
specified in the _Doxygen_ documentation.

#### BugReport.md

The github submission must include a _bugreport_ that communicates to the TAs. It should report any requirements that are either not attempted, not complete, and/or not functional, and any known bugs in the system. Also, if you have a sense of the problem or how to fix it, please provide a brief description, as this will help TA's give partial credit to that element.

#### Self-Documenting Code

TAs will inspect code for good naming conventions, good code organization, and internal comments to highlight logic.

### Code Assessment ( 40% )

#### Refactoring of Initialization and Unit Tests ( 10% )

In the week prior to the due date of the completed iteration, you will submit a subset of the requirements. You must pass
all tests to receive full points for the preliminary submission. There will be no inspection of the code at this point
-- it is strictly a test for completed functionality. There will be transparency in this process in that you will know
what functionality will be tested, and you will see the results. Points earned at this stage are independent of the 40%
points for the final submission.

#### Completed Iteration Code ( 30% )

Code functionality will be tested with unit tests and visual inspection of the game being played.

<hr>

## Getting Started

It is best if you use your code from iteration1. If you think your code is so dysfunctional that it cannot be recovered, code that can be used has been posted to GitHub.  

**CREATE DEVEL BRANCH** and **REFACTOR** as described in lab08 before you do anything!

<hr>

## Resources

Guides for Github:
- Tutorial: https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging

- Guides on best practices: https://www.atlassian.com/git/tutorials/comparing-workflows

- A workflow guide: http://nvie.com/posts/a-successful-git-branching-model.

- This workflow in particular is pretty popular, but far more than needed for working on your own project: https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow

Graphics Libraries :
- https://github.com/wjakob/nanogui
- https://nanogui.readthedocs.io/en/latest/
- https://github.com/memononen/NanoVG
- https://www.opengl.org/

Local System and Compilation:
- Repo: https://github.umn.edu/umn-csci-3081F17/class-repo-public
- Linking library: `/project/3081cf17/`
- Compilation: https://gcc.gnu.org/
- Linux quick reference: http://www-users.cs.umn.edu/~larson/repo-website-resources/website/examples/csresources/linux.html
- Makefile Resources: http://www-users.cs.umn.edu/~larson/repo-website-resources/website/examples/csresources/cpp.html

Testing:
- https://github.com/google/googletest
- Unit Testing: https://martinfowler.com/bliki/UnitTest.html

Style:
- https://google.github.io/styleguide/cppguide.html
- https://github.com/google/styleguide/tree/gh-pages/cpplint
- https://www.python.org/downloads/

Documentation:
- http://www.stack.nl/~dimitri/doxygen/
- https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet

IDEs:
- Xemacs: https://www.xemacs.org/
- vi: http://www.lagmonster.org/docs/vi.html
- Atom: https://atom.io/
- Sublime: https://www.sublimetext.com/
- Eclipse: https://eclipse.org/

<hr>

#### sensors

This software is attempting to simulate physical systems with various degrees of fidelity. Each sensor is representative of a physical device with limited sensing capability. Below, I have provided some resources for you to better understand the physical device you are trying to simulate.

- **__Proximity Sensor__**: There are a variety of ways to sense distance to an object. These sensors can output an analog signal (i.e. a voltage range that must be calibrated to relate a voltage to a distance) or a digital signal (i.e. a number that relates to the distance to the sensed object). The only information these sensors output are distance. If you use multiple sensors, you can get a sense of the direction of the object, but the individual sensor cannot provide that information. Here is a link to a sonar sensor -- notice the "cone" of the sensing region that has a range and width: https://www.pololu.com/product/723

- **__Contact Sensor__**: Contact sensors are on/off buttons that typically output high or low voltage for pressed and not pressed, respectively. Here is a link to a typical contact sensor: https://www.pololu.com/product/1403. And here is how you might use them on a round robot: http://robot50.net/2015/02/ - notice that any contact along the bumper will activate either one or both of the contact sensors. For our purposes, we are modeling a system that can tell where the contact is taking place along the perimeter of the robot. This would require something far more sophisticated than the image shown.

- **__Distress Call Sensor__**: This might be accomplished with a small microphone, such as this one: https://www.pololu.com/product/1618/resources. It is omnidirectional with sensitivity that we can describe with range (i.e. the furthest it can be from the source of the sound and still detect it). There is a lot more information that could be gained from analyzing the frequency and amplitude of the sound waves, but for our purposes, when a robot gets within a certain distance of the robot, it will sense the distress call. An alternative to this is using IR beacons: https://www.pololu.com/product/702, which give directionality of a signal (maybe for the next iteration!).

- **__Entity Type Sensor__**: There are a lot of sophisticated devices for transmitting and receiving complex information wirelessly, but for this simulation, you should think of cheap and small devices that can transmit and receive distinguishable signals. We could have a microphone for this as well, in which each type of entity emits a different sound. IR remote controls are one possibility: https://www.pololu.com/product/2777
