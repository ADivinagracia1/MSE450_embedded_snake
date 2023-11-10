Snake Game Release 1.0 Notes

Team Members: Andrei Divinagracia, Zainab Vajahath, Dexter Zamora

What went right with this release from a project, team, and time management perspectives:
- Before starting the project in C we decided to study a C++ version of it on our own to help prepare and understand the scope of work
- We made it a point as a group to discuss project management strategies early on. We used dedicated channels in our group discord server to ensure open communcation
- We also created a project and milestones using the github project management tools to help us keep on top of the sub tasks needed to create release 1.0
- Github was the most useful tool that was used throughout the development process. Being able to push/pull and branch made a huge difference in allowing each of us	
  to work on seperate tasks and combine them one at the time.


### Lessons learned from this release from a project, team, and time management perspectives:
1. Github is a necessity for any sort of programming/development project
2. Took us an extra week (thank you for the extension) to understand the scope of the work, and also have the background knowledge in programming theory to carry out tasks
3. Object oriented programming makes integration much easier


Compared to your project proposal original plans. list the state of all features implemented, changed, modified, unimplemented, moved to next release, etc:
In our project proposal document we outlined the following features:
Graphical model of snake and food (LCD)
- Ability for snake to “move”
- Ability for snake to “grow”
- User controls → Button Interrupts
- Collision with walls should end game
- Collision with itself should end game

All features above have been implemented into release 1.0

In the next release we plan to improve the quality of the game visually and from a user experience perspective. This includes
- Points/score system
- Restart/pause game
- Highscore tracker
- Physical Implementation


Additional Notes and Comments: 
- 2 of our team members also purchased the physical Tiva board to create the project physically as well
- We noticed the game slows down when using Port F controls for the game compared to ADC, we intend to investigate that for release 2.0
- We also intend to investigate edge cases and find bugs that could break that game (i.e leaving SW1 clicked and pressing another control)
- We would like to remove the latching capability of port F to make it behave like LEFT and RIGHT on ADC

 