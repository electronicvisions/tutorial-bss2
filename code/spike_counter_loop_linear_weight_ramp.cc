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
	vector uint8_t inhibitory_weights_l;
	vector uint8_t inhibitory_weights_r;

	uint8_t excitatory_row = 0;
	uint8_t inhibitory_row = 1;

	get_weights(&excitatory_weights_l, &excitatory_weights_r, excitatory_row);
	get_weights(&inhibitory_weights_l, &inhibitory_weights_r, inhibitory_row);

	// decrease the weight with the post-synaptic neuron id
	for (size_t i = 0; i < dls_num_columns / 2; ++i) {
		excitatory_weights_l[i] -= i;
		excitatory_weights_r[i] -= i + dls_num_columns/2;
		inhibitory_weights_l[i] -= i;
		inhibitory_weights_r[i] -= i + dls_num_columns/2;
	}

	set_weights(&excitatory_weights_l, &excitatory_weights_r, excitatory_row);
	set_weights(&inhibitory_weights_l, &inhibitory_weights_r, inhibitory_row);

	// Spike count accumulator storage
	uint32_t spike_counts[dls_num_columns];

	for (uint32_t& count: spike_counts) {
		count = 0;
	}

	// Timeframe of spike count measurement
	time_base_t constexpr timeframe = 10000000;

	// Sleep period [cycles] between consecutive readouts
	uint32_t constexpr sleep_period = 10000;

	// Reset counters to only measure spikes emitted during the loop below
	reset_all_neuron_counters();

	// Read spike count and accumulate
	while (get_time_base() < timeframe) {
		for (size_t nrn = 0; nrn < dls_num_columns; ++nrn) {
			uint32_t count = get_neuron_counter(nrn);
			spike_counts[nrn] += count;
		}
		sleep_cycles(sleep_period);
	}

	// Write results to end of RAM
	uint32_t* pos = reinterpret_cast<uint32_t*>(&ram_end) - dls_num_columns;
	for (size_t i = 0; i < dls_num_columns; ++i) {
		*pos = spike_counts[i];
		pos++;
	}

	return 0;
}
