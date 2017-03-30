#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

def plot_integer(parameter):

    results_file = "{}.out".format(parameter)
    fct = {}
    
    with open(results_file, "r") as infile:
        lines = infile.readlines()
        for line in lines:
            if line != '\n':
                flowsize = int(line.split()[0])
                if flowsize in fct:
                    fct[flowsize].append((int(line.split()[1]), float(line.split()[2]) / 1000.0))
                else:
                    fct[flowsize] = []
    		
    for flowsize in sorted(fct):
        fct_values = []
        param_labels = []
        for (param_value, fct_value) in fct[flowsize]:
            fct_values.append(fct_value)
            param_labels.append(param_value)
        plt.plot(param_labels, fct_values, label=flowsize)
    
    # Labels
    plt.xlabel("{} value".format(parameter))
    plt.ylabel('FCT (msec)')
    plt.title('Flow completion time for different flow sizes')
    
    # Axes and ticks
    plt.margins(0.2)
    plt.legend()
    plt.grid(True)
    plt.show()

def plot_flow_xaxis(parameter):
    results_file="{}.out".format(parameter)
    fct = {}
    
    with open(results_file, "r") as infile:
        lines = infile.readlines()
        for line in lines:
            if line != '\n':
                option = line.split()[1]
                if option in fct:
                    fct[option].append((int(line.split()[0]), float(line.split()[2]) / 1000.0))
                else:
                    fct[option] = []
			
    for option in sorted(fct):
        flowsizes = []
        fct_values = []
        for (flowsize, fct_value) in fct[option]:
            fct_values.append(fct_value)
            flowsizes.append(flowsize/1000)
        plt.plot(flowsizes, fct_values, label="{}={}".format(parameter, option))
    
    # Labels
    plt.xlabel('Flow size (KB)')
    plt.ylabel('FCT (msec)')
    plt.title("Flow completion time for {} settings".format(parameter))
    
    # Axes and ticks
    plt.margins(0.2)
    plt.gca().set_ylim([0, 12000])
    plt.legend()
    plt.grid(True)
    plt.show()

#plot_integer("tcp_rmem")
plot_flow_xaxis("tcp_timestamps")
plot_flow_xaxis("tcp_window_scaling")
plot_flow_xaxis("tcp_sack")
plot_flow_xaxis("tcp_dsack")
plot_flow_xaxis("tcp_syn_retries")
plot_flow_xaxis("tcp_retries1")
plot_flow_xaxis("tcp_retries2")
plot_flow_xaxis("tcp_syncookies")
plot_flow_xaxis("tcp_tw_recycle")
plot_flow_xaxis("tcp_fack")
plot_flow_xaxis("tcp_reordering")
plot_flow_xaxis("tcp_ecn")
plot_flow_xaxis("tcp_tw_reuse")
plot_flow_xaxis("tcp_slow_start_after_idle")
plot_flow_xaxis("tcp_frto")
plot_flow_xaxis("tcp_frto_response")
plot_flow_xaxis("tcp_no_metrics_save")
plot_flow_xaxis("tcp_low_latency")
plot_flow_xaxis("tcp_mtu_probing")
plot_flow_xaxis("tcp_slow_start_after_idle")
plot_flow_xaxis("tcp_adv_win_scale")
plot_flow_xaxis("tcp_app_win")
plot_flow_xaxis("tcp_autocorking")
plot_flow_xaxis("tcp_base_mss")
plot_flow_xaxis("tcp_early_retrans")

