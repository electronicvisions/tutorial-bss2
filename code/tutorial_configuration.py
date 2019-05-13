from dlens.v2 import halco, hal
from dlens.v2.halco import CommonNeuronParameter, NeuronOnDLS, \
    NeuronParameter, SynapseDriverOnDLS, SynapseOnDLS

from calibration import UniformCapmemCalibration

class TutorialBoard(hal.Board):
    def __init__(self):
        super(TutorialBoard, self).__init__()
        # Static baseboard configuration
        self.set_parameter(self.Parameter.syn_v_bias, 1000)
        self.set_parameter(self.Parameter.capmem_i_buf_bias, 3000)
        self.set_parameter(self.Parameter.capmem_i_ref, 3913)

class TutorialChip(hal.Chip):
    def __init__(self):
        super(TutorialChip, self).__init__()
        # Apply neuron calibration
        capmem_calib = UniformCapmemCalibration(chip = 30)
        self.capmem = capmem_calib.get_calibration(refractory_time = 30e-6, leak_time = 1e-6, \
            excitatory_synapse_time = 30e-6, inhibitory_synapse_time = 30e-6)

        # Static configuration of capmem parameters (analog neuron parameters)
        self.capmem_config.enable_capmem = True
        # self.chip.capmem = capmem

        # Analog neuron parameter
        for nrn in halco.iter_all(NeuronOnDLS):
            self.capmem.set(nrn, NeuronParameter.v_treshold, 460)
            self.capmem.set(nrn, NeuronParameter.i_bias_exc_syn_input_sd, 1022)
            self.capmem.set(nrn, NeuronParameter.i_bias_inh_syn_input_sd, 1022)
            self.capmem.set(nrn, NeuronParameter.i_bias_exc_syn_input_main, 1022)
            self.capmem.set(nrn, NeuronParameter.i_bias_inh_syn_input_main, 1022)

        self.capmem.set(CommonNeuronParameter.e_reset, 280)

        # Shared digital neuron configuration
        self.common_neuron_config.enable_digital_out = True

        # Individual digital neuron configuration
        for nrn in halco.iter_all(NeuronOnDLS):
            neuron_config = self.get_neuron_digital_config(nrn)
            neuron_config.enable_synapse_input_excitatory = True
            neuron_config.enable_synapse_input_inhibitory = True
            neuron_config.enable_leak = True
            neuron_config.fire_out_mode = neuron_config.FireOutMode.enabled
            self.set_neuron_digital_config(nrn, neuron_config)

        # Set switches between synapses and neurons
        for nrn in halco.iter_all(NeuronOnDLS):
            switch = self.get_column_current_switch(nrn.toColumnCurrentSwitchOnDLS())
            switch.inh_config = switch.Config.internal
            switch.exc_config = switch.Config.internal
            self.set_column_current_switch(nrn.toColumnCurrentSwitchOnDLS(), switch)

        # Synapse driver to be used for applying excitatory noise
        self.excitatory_synapse_driver = SynapseDriverOnDLS(0)

        # Synapse driver to be used for applying inhibitory noise
        self.inhibitory_synapse_driver = SynapseDriverOnDLS(1)

        # Configure Synapse drivers
        self.synapse_drivers.pulse_length = 8  # Shared parameter
        self.synapse_drivers.set_mode(self.excitatory_synapse_driver,
                                        self.synapse_drivers.Mode.excitatory)
        self.synapse_drivers.set_mode(self.inhibitory_synapse_driver,
                                        self.synapse_drivers.Mode.inhibitory)

        # Configure per-neuron spike counters
        for nrn in halco.iter_all(NeuronOnDLS):
            self.rate_counter_config.set_enable_neuron(nrn, True)
            self.rate_counter_config.enable_clear_on_read = True

    def get_excitatory_synapse_driver(self):
        return self.excitatory_synapse_driver

    def get_inhibitory_synapse_driver(self):
        return self.inhibitory_synapse_driver