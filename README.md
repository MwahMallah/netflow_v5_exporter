# p2nprobe - NetFlow v5 Exporter

**Author:** Maksim Dubrovin  
**Login:** xdubro01  
**Date Created:** 12.11.2024  

## Program Description
The `p2nprobe` program extracts network flow information from a PCAP file and exports it in NetFlow v5 format. This tool reads packets from a specified PCAP file and aggregates them into flows. Only TCP traffic flows are processed and exported; other types of traffic are ignored. The aggregated flows are sent via UDP to a NetFlow v5 collector, where they can be received and further analyzed.

### Features and Limitations
- **Features:** 
  - Aggregates TCP flows from PCAP files.
  - Exports flows in NetFlow v5 format over UDP.

## Compilation and Execution
in the project directory run `Makefile` and execute `p2nprobe`:
  ```
    make
    ./p2nprobe 127.0.0.1:12345 [pcap-example].pcap -a 5 -i 5 
  ```