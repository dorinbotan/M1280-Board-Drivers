#ifndef BIT_MANIPULATION_
#define BIT_MANIPULATION_

#ifndef F_CPU
# define F_CPU 7372800L
#endif

#define SET_BIT(ADDRESS, BIT_POSITION) (ADDRESS |= _BV(BIT_POSITION))
#define CLEAR_BIT(ADDRESS, BIT_POSITION) (ADDRESS &= ~_BV(BIT_POSITION))
#define TOGGLE_BIT(ADDRESS, BIT_POSITION) (ADDRESS ^= _BV(BIT_POSITION))
#define WRITE_BIT(ADDRESS, BIT_POSITION, BIT_VALUE) (BIT_VALUE ? SET_BIT(ADDRESS, _BV(BIT_POSITION)) : CLEAR_BIT(ADDRESS, _BV(BIT_POSITION)))
#define CLOCK_PIN(ADDRESS, BIT_POSITION) (SET_BIT(ADDRESS,BIT_POSITION), CLEAR_BIT(ADDRESS,BIT_POSITION))

#endif /* BIT_MANIPULATION_ */
