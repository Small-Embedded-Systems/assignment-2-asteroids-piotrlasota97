/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Ship struct
p = origin point of ship,
v = vector,
Heading: 0 = north,	1 = north-east, 2 = east, 3 = south-east, 4 = south, 5 = south-west, 6 = west, 7 = north-west */

struct ship {
	  int heading;
    coordinate_t p;
    vector_t     v;
	  int lostlife;
};

/* initial structs for building linked lists are: */

//struct rock {
//    coordinate_t p;
//    struct rock *next;
//};

//struct missile {
//    coordinate_t p;
//    struct missile *next;
//};

typedef struct missile {
    coordinate_t p;
	  vector_t v;
		int hit;
		float ttl;
    struct missile *next;
} miss_t;

// missiles
void minitialise(void);
miss_t *mallocnode(void);
void mfreenode(miss_t *n);
void mstrike(struct missile *l);
void missile_system(void);
extern struct missile *mactive;

typedef struct asteroid {
    coordinate_t p, p1, p2;
	  vector_t v, v1, v2;
		float ttl;
		int hit;
    struct asteroid *next;
} ast_t;

// asteroids
void ainitialise(void);
ast_t *aallocnode(void);
void afreenode(ast_t *n);
void astrike(struct asteroid *l);
void asteroid_system(void);
extern struct asteroid *aactive;

void physics(void);
void reset(void);
