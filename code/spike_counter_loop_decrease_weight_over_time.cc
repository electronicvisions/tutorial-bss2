#include <stddef.h>
#include <stdint.h>
extern "C"
{
#include "libnux/counter.h"
#include "libnux/dls.h"
#include "libnux/spr.h"
#include "libnux/syn.h"
#include "libnux/time.h"
}

extern uint8_t ram_end;

// This program reads out neuron-local spike counters in a loop. The per-neuron accumulation is
// stored to a defined memory address range after reading for a defined timeframe for readout from
// the host.

// Program entry point
extern "C" int start()
{
	vector uint8_t excitatory_weights_l;
	vector uint8_t excitatory_weights_r;

	uint8_t excitatory_row = 0;

	// Timeframe of spike count measurement
	time_base_t constexpr timeframe = 2000000;

	// Sleep period [cycles] between consecutive readouts
	uint32_t constexpr sleep_period = 10000;
    
    // read in initial weights
	get_weights(&excitatory_weights_l, &excitatory_weights_r, excitatory_row);

	// decrease weights over time (w/ wrap around)
	while (get_time_base() < timeframe) {

		excitatory_weights_l = excitatory_weights_l - vec_splat_u8(1);
		excitatory_weights_r = excitatory_weights_r - vec_splat_u8(1);

		set_weights(&excitatory_weights_l, &excitatory_weights_r, excitatory_row);

		sleep_cycles(sleep_period);
	}

	return 0;
}
