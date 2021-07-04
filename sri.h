//
// Based Primarily on This Work: 
// https://services.math.duke.edu/education/ccp/materials/diffcalc/sir/index.html
// 

#pragma once

// Represents an SRI set, (susceptible, infected, recovered),
// all ratios, over the total population, at a set time
typedef struct {
	float s_ratio, i_ratio, r_ratio;
} SRI_iteration, * pSRI_iteration;


// Represents an initial SRI set, as well as the current time
// the constant amount of contacts, and the recovery rate
typedef struct {
	SRI_iteration iteration;
	unsigned long long time; // it truly has been a long, long time
	float contacts, recover_rate;
} SRI_information, * pSRI_information;


// about '10.45' microseconds per call
// iterates the ratio values of the SRI set by '1' day
static void SRI_iterate(pSRI_information sri) {
	float precomp = sri->contacts * sri->iteration.s_ratio * sri->iteration.i_ratio;

	float ds = precomp;
	*(unsigned long*)&ds ^= 0x80000000l;
	// Some may call the above bithack overkill,
	// However I found on my machine, with no optimization flags
	// that it took about ~0.0025us per "* -1" operation, and
	// ~0.0012us per xor+ptr_cast operation, in this case, it may just be worth it
	float dr = sri->recover_rate * sri->iteration.i_ratio;
	float di = precomp - dr;

	sri->iteration.s_ratio += ds;
	sri->iteration.i_ratio += di;
	sri->iteration.r_ratio += dr;

	sri->time++;
}