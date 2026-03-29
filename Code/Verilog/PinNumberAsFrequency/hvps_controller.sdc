# Define the base clock on the 50 MHz oscillator pin
create_clock -name clk -period 20.000 [get_ports clk]

# Automatically derive clock uncertainty
derive_clock_uncertainty

# Tell the timing analyzer to assume no input delay (for now)
set_input_delay 0 -clock clk [all_inputs]

# And no output delay
set_output_delay 0 -clock clk [all_outputs]
