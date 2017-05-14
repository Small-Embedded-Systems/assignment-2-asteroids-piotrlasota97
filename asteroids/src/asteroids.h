/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int 	 prevscore;		 /* score from previous game */
extern int   lives;        /* lives remaining */
extern bool paused;				 /* paused game state */

extern struct ship player;

extern struct asteroid *asteroids; /* array of rocks / pointer to linked-list */
extern struct missile *shots;  /* array of missiles / pointer to linked-list */

extern const float Dt; /* Time step for physics, needed for consistent motion */
