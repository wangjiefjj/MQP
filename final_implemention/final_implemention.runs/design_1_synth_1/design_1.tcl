# 
# Synthesis run script generated by Vivado
# 

set_param xicom.use_bs_reader 1
set_msg_config -id {Common 17-41} -limit 10000000
set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7z020clg484-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.cache/wt [current_project]
set_property parent.project_path C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.xpr [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_MEMORY} [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property board_part em.avnet.com:zed:part0:1.3 [current_project]
set_property ip_repo_paths {
  c:/Users/georges/Documents/GitHub/MQP/ip_repo/nu_nu_rangefinder_vga_1.0
  c:/Users/georges/Documents/GitHub/MQP/ip_repo/nu_rangefinder_vga_1.0
  c:/Users/georges/Documents/GitHub/MQP/ip_repo/rangefinder_vga_1.0
  c:/Users/georges/Documents/GitHub/MQP/ip_repo/new_rangefinder_vga_1.0
  c:/Users/georges/Documents/GitHub/MQP/ip_repo/rangefinder_vga_1.0
} [current_project]
add_files C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1.bd
set_property is_enabled true [get_files -all C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/ip/design_1_nu_nu_rangefinder_vga_0_0/synth/design_1_nu_nu_rangefinder_vga_0_0.v]
set_property is_locked true [get_files C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1.bd]

foreach dcp [get_files -quiet -all *.dcp] {
  set_property used_in_implementation false $dcp
}
read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]

synth_design -top design_1 -part xc7z020clg484-1 -mode out_of_context

rename_ref -prefix_all design_1_

write_checkpoint -force -noxdef design_1.dcp

catch { report_utilization -file design_1_utilization_synth.rpt -pb design_1_utilization_synth.pb }

if { [catch {
  file copy -force C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.runs/design_1_synth_1/design_1.dcp C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1.dcp
} _RESULT ] } { 
  send_msg_id runtcl-3 error "ERROR: Unable to successfully create or copy the sub-design checkpoint file."
  error "ERROR: Unable to successfully create or copy the sub-design checkpoint file."
}

if { [catch {
  write_verilog -force -mode synth_stub C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_stub.v
} _RESULT ] } { 
  puts "CRITICAL WARNING: Unable to successfully create a Verilog synthesis stub for the sub-design. This may lead to errors in top level synthesis of the design. Error reported: $_RESULT"
}

if { [catch {
  write_vhdl -force -mode synth_stub C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_stub.vhdl
} _RESULT ] } { 
  puts "CRITICAL WARNING: Unable to successfully create a VHDL synthesis stub for the sub-design. This may lead to errors in top level synthesis of the design. Error reported: $_RESULT"
}

if { [catch {
  write_verilog -force -mode funcsim C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_sim_netlist.v
} _RESULT ] } { 
  puts "CRITICAL WARNING: Unable to successfully create the Verilog functional simulation sub-design file. Post-Synthesis Functional Simulation with this file may not be possible or may give incorrect results. Error reported: $_RESULT"
}

if { [catch {
  write_vhdl -force -mode funcsim C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_sim_netlist.vhdl
} _RESULT ] } { 
  puts "CRITICAL WARNING: Unable to successfully create the VHDL functional simulation sub-design file. Post-Synthesis Functional Simulation with this file may not be possible or may give incorrect results. Error reported: $_RESULT"
}

if {[file isdir C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.ip_user_files/bd/design_1]} {
  catch { 
    file copy -force C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_stub.v C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.ip_user_files/bd/design_1
  }
}

if {[file isdir C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.ip_user_files/bd/design_1]} {
  catch { 
    file copy -force C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.srcs/sources_1/bd/design_1/design_1_stub.vhdl C:/Users/georges/Documents/GitHub/MQP/final_implemention/final_implemention.ip_user_files/bd/design_1
  }
}
