## Peer Reflection of Final Design Document and UML Documentation

**RESPONDER: Jacob Elafandi**

**AUTHOR: Jack Stanek**

  1. In the first paragraph (or first section) of your design documentation, what is the most effective sentence with respect to orienting the reader to the project?
  * "New features include the use of multiple types of different sensors by robots which can exhibit autonomous behavior." This sentence signals the reader that the sensors and the robots are the things to pay attention to in the document, as they are the focus of this iteration.

  2. Identify a sentence in the first paragraph that needs to be reworked and state what you think is problematic about that sentence. (Do not edit it.)
  * "Thus it will primarily describe design decisions made during the second iteration." Beginning a sentence with "thus" feels a bit clunky. (Yes, I know this is a small thing; the introduction is pretty good.)

  3. Identify a sentence or two in any of the paragraphs that provides the "big picture" with respect to the software, design, or class structure, AND is accompanied by low-level details that help the reader better understand the "big picture."
  * "A _mobile entity_ is fairly self-explanatory; it is an entity which can move through the arena. It has a speed, heading angle, and maximum speed."

  4. Comment on the effectiveness of this technique in the example from (3). If it is effective, analyze why you think it works here. If you think there are other details that would be more elucidating, state those.
  * Noting the attributes of a mobile entity is a good idea, and helps provide the reader with a better understanding of what the program looks like. On the other hand, some crucial details are missing; would the user automatically assume that mobile entities move in a straight line (as opposed to an arc) and do not accelerate?

  5. Identify a topic in the writing that is either underspecified or is discussed too in-depth. If underspecified, what is the most important idea that is missing? If too in-depth, what can be removed?
  * The document gives no indication of what the entities look like in the Arena. The fact that all entities are circular is important in understanding how they interact, and conspicuously absent from the document.

  6. What do you think would be the single most impactful change to this document - in other words, what would you recommend to the author as the one area on which to focus? It could be related to the content (e.g. level of detail, more or less technical information, highlight more or fewer classes, etc.) or to the writing (e.g. reorganize paragraph or sentence order, condense text, improve sentence structure, etc.).
  * The paragraphs should be smaller; they frequently take up half a page or more. I'd recommend splitting them at natural break points, e.g. classes or methods.

  7. As a programmer new to this project, which class do you think the document is emphasizing as the place to begin to engage the code? This might be explicit or implicit. What part of the writing made you think you should start with that class?
  * The Robot is the only class that contains every sensor. Since the sensor section of the document is so long, sensor feedback must be important, so it makes sense to look at the Robot first in order to understand the effects of all the sensors.

  Now explore the documentation of the classes. Go to the class that you identified in (7).

  8. What do you consider to be the best and worst documented method in that class and why. OR, if you think they are all of equal quality, comment on the level of detail provided in the documentation. Is it sufficient, clear, and correct? If it is excellent, state what makes it excellent.
  * Not applicable; doxygen documentation is not included.

  9. Skim through all the brief comments on the main classes page. What strikes you as you look at the collection? Is there an effective pattern in the comments? Is there something consistently lacking?
  * Not applicable; doxygen documentation is not included.

  Now look at the UML - be conscious of your first reaction!

  10. Where did your eye go? What jumps out at you on the page? Is this an important element, thus warrants the attention? If not, offer a suggestion on how to make it less visually prominent.
  * This UML doesn't have an immediately clear hierarchy, so my eye first jumped to Arena, since it's the largest box and it's close to the center. Arena is the class that contains and updates all entities and sensors, so it's important.

  11. What did the author do in her/his UML diagram that you would like to incorporate into your UML? Why do you like that part of the UML and how does it differ from what you did?
  * This UML diagram includes comments along the arrows that briefly explain class relationships. For example, the arrow from arena\_entity\_params to ArenaEntity is labeled "(configures)", the arrow from RobotBattery to Player is labeled "allows movement", and the arrow from Sensor to ArenaMobileEntity is labeled "informs". The arrows in my UML were all simple composition, inheritance, or dependence labels, with the only labels being the quantities in composition. The labeling method conveys more information without requiring the user to go digging for it in the method names.

  12. Try to recall your sense of your first attempt to engage the base code, and think of how it is even more complex now. Keeping that in mind, what do you think was the most successful part of the author's writing (in doxygen and UML) with respect to helping a programmer get acclimated to the code? What do you think could be very helpful but needs some rework?
  * The thorough explanation of the classes in the design document, and the labeling of relationships in the UML diagram, both might help an initiate understand what's going on in this code. The UML is messy, though; what I did is put classes below the classes they depend on, so as to establish a clear hierarchy. As mentioned above, the paragraphs also should be split to increase readability and to keep new programmers from feeling overwhelmed.
